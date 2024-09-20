/*****************************
(c) Copyright European Space Agency, 2017
This file is subject to the terms and conditions defined in file 'LICENCE.txt', 
which is part of this [source code/executable] package. No part of the package, 
including this file, may be copied, modified, propagated, or distributed except
according to the terms contained in the file 'LICENCE.txt'.
 ******************************************************************************/

/* System Includes */
#include <math.h>

#include "NeqCalcModipAz.h"
#include "NeqConstants.h"
#include "IonoConstants.h"

/* Function prototypes */
static double NeqInterpolate(const double pdZ[NUM_INTER_PTS_SI],
                             const double dDeltaX);
 
/**
 *******************************************************************************
 * @par Name: NeqCalcModip
 * @par Purpose:
 *   (corresponds to 'philam' in fortran and in C++)
 *   This function uses the current lat/lng to calculate the corresponding
 *   modified dip latitude (MODIP) value.
 *
 * @par Argument I/O:
 *   @param dLat     (I) The current latitude (deg)
 *   @param dLng     (I) The current longitude (deg)
 *   @param pstModip (I) Modip grid from config
 *
 * @par I/O files:
 *   None
 *
 * @return Interpolated Modip for position lat/lng (deg)
 ******************************************************************************/
double NeqCalcModip(double dLat, double dLng, MODIP_st* pstModip)
{
  int    siLatBaseInd;  /* Lat base index (ie. -1pt index in pdPDip) */
  int    siLngBaseInd;  /* Lng base index (ie. -1pt index in pdPDip) */
  int    siLatInterInd; /* Loop variable for setting lat -1,0,1,2 pts */
  int    siLngInterInd; /* Loop variable for setting lng -1,0,1,2 pts */
  double dDeltaLat;     /* Interpolation offset from latitude 0 pt */
  double dDeltaLng;     /* Interpolation offset from longitude 0 pt */
  double pdDipN[NUM_INTER_PTS_SI];
                        /* Point values for longitude interpolation */
  double pdDipE[NUM_INTER_PTS_SI];
                        /* Point values for latitude interpolation */
  double dReturn;       /* Interpolated Modip value at lat/lng */


  /* Ensure latitude is within -90 -> +90 range.
   * This will prevent an array out of bounds error for values outside range. */
  if (dLat <= -RANGE_LATDEG_MAX_D) {
    /* Latitude is less than -90, so return Modip of -90 */
    dReturn = -RANGE_LATDEG_MAX_D;
  } else if (dLat >= RANGE_LATDEG_MAX_D) {
    /* Latitude is greater than +90, so return Modip of +90 */
    dReturn = RANGE_LATDEG_MAX_D;
  } else {
    /* Calculate dDeltaLng and indices into the Modip grid */
    dDeltaLng = (dLng + RANGE_LONDEG_MAX_D) / (double)LON_STEP_SI;
    siLngBaseInd = (int)floor(dDeltaLng);
    dDeltaLng = dDeltaLng - (double)siLngBaseInd;

    /* Ensure siLngBaseInd is within limits 0-35.
     * This will not be needed if dLng is within the range +-180. */
    while (siLngBaseInd < 0) {
      siLngBaseInd = siLngBaseInd + NUM_LNGS_SI;
    }
    while (siLngBaseInd > NUM_LNGS_SI - 1) {
      siLngBaseInd = siLngBaseInd - NUM_LNGS_SI;
    }

    /* Obtain lat grid square and position in that square.
     * The original calculation for siLatBaseInd (which subtracted 1e-6 from
     * dDeltaLat before truncation) has been modified as this subtraction serves
     * no useful purpose (except for potentially fixing the north pole array
     * index out of range bug, whilst increasing the risk of a south pole
     * problem. */
    dDeltaLat = (dLat + RANGE_LATDEG_MAX_D) / (double)LAT_STEP_SI;
    siLatBaseInd = (int)floor(dDeltaLat);
    dDeltaLat = (dDeltaLat - (double)siLatBaseInd);

    /* Now perform 2D interpolation */
    for (siLngInterInd = 0; siLngInterInd < NUM_INTER_PTS_SI;
         siLngInterInd = siLngInterInd + 1) {
      /* Interpolate the PDip values against latitude */
      for (siLatInterInd = 0; siLatInterInd < NUM_INTER_PTS_SI;
           siLatInterInd = siLatInterInd + 1) {
        pdDipN[siLatInterInd] = pstModip->pdModip[siLatBaseInd + siLatInterInd]
                                                 [siLngBaseInd + siLngInterInd];
      }

      /* Set the longitude point values by interpolating latitude values */
      pdDipE[siLngInterInd] = NeqInterpolate(pdDipN, dDeltaLat);
    }

    /* Now interpolate the PDip values against longitude and return it */
    dReturn = NeqInterpolate(pdDipE, dDeltaLng);
  } /* end if dLat out of range */

  return(dReturn);
}


/**
 *******************************************************************************
 * @par Name: NeqInterpolate
 * @par Purpose:
 *   (corresponds to 'finter3' in fortran and in C++)
 *   This function performs a third order interpolation. It is used when
 *   calculating the modified dip latitude (MODIP) value.
 *
 * @par Argument I/O:
 *   @param dZ      (I) Array containing the -1, 0, 1 and 2 point values
 *   @param dDeltaX (I) Position to interpolate to (offset from 0pt to 1pt)
 *
 * @par I/O files:
 *   None
 *
 * @return The interpolated value at the point
 ******************************************************************************/
static double NeqInterpolate(const double pdZ[NUM_INTER_PTS_SI],
                             const double dDeltaX)
{
  double dIntZ;                 /* Return value for this function */
  double dDX;                   /* Working variable used in interpolation */
  double dSumPt0Pt1;            /* Sum of point 0 and 1 */
  double dGradPt0Pt1;           /* Gradient between point 0 and 1 */
  double dSumPtM1Pt2;           /* Sum of point -1 and 2 */
  double dGradPtM1Pt2;          /* Gradient between point -1 and 2 */
  double pdA[NUM_INTER_PTS_SI]; /* Working variable used in interpolation */
  int    siIndex;               /* Loop variable */


  /* Check if dDeltaX is small */
  if (fabs(dDeltaX) < INTER_THRESH_D) {

    /* dDeltaX is small, so return the zero point */
    dIntZ = pdZ[INT_PT1_SI];
  } else {

    /* Interpolate */
    dSumPt0Pt1 = pdZ[INT_PT2_SI] + pdZ[INT_PT1_SI];
    dGradPt0Pt1 = pdZ[INT_PT2_SI] - pdZ[INT_PT1_SI];
    dSumPtM1Pt2 = pdZ[INT_PT3_SI] + pdZ[INT_PT0_SI];
    dGradPtM1Pt2 = (pdZ[INT_PT3_SI] - pdZ[INT_PT0_SI]) /
                   (double)(NUM_INTER_PTS_SI - 1);

    pdA[INTER_COEFF1_SI] = (INTERPOL_CONST1_D * dSumPt0Pt1) - dSumPtM1Pt2;
    pdA[INTER_COEFF2_SI] = (INTERPOL_CONST1_D * dGradPt0Pt1) - dGradPtM1Pt2;
    pdA[INTER_COEFF3_SI] = dSumPtM1Pt2 - dSumPt0Pt1;
    pdA[INTER_COEFF4_SI] = dGradPtM1Pt2 - dGradPt0Pt1;

    dDX = (TWO_D * dDeltaX) - 1.0;

    /* Now calculate dIntZ = 1/16(dA[3]dDX^3 + dA[2]dDX^2 + dA[1]dDX + dA[0])
     * where dDX3 means 'dDX cubed' and dDX2 means 'dDX squared'.*/
    dIntZ = 0;
    for (siIndex = NUM_INTER_PTS_SI - 1; siIndex >= 0; siIndex = siIndex - 1) {
      dIntZ = (dIntZ * dDX) + pdA[siIndex];
    }
    dIntZ = dIntZ / INTERPOL_CONST2_D;
  } /* end if dDeltaX is small */

  /* Return the interpolated value */
  return(dIntZ);
}


/**
 *******************************************************************************
 * @par Name: NeqModipToAz
 * @par Purpose:
 *   (corresponds to 'gflx' in fortran and in C++)
 *   This function calculates Az from the provided coefficients and modified dip
 *   latitude value (dModip). If only one coefficient (A0) is supplied then that
 *   is Az (ie. it doesn't vary with Modip).
 *
 * @par Argument I/O:
 *   @param dModip     (I) The modified dip latitude of current position
 *   @param siNumCoeff (I) The number of items in the pdCoeff array
 *   @param pdCoeff    (I) Array containing values of A0, A1, A2, etc.
 *
 * @par I/O files:
 *   None
 *
 * @return The calculated Az for given Modip
 ******************************************************************************/
double NeqModipToAz(double dModip, int siNumCoeff, double* pdCoeff)
{
  double dFlx;  /* The flux (Az) value */
  double dTerm; /* The value of the next term of polynomial before any coeff
                 * is applied (ie. dMoDIP to the powers of 0,1,2,etc.) */
  int siIndex;  /* Loop variable */


  /* Initialise dFlx to zero */
  dFlx = 0.0;

  /* Now calculate Az (= A0 + A1*dModip + A2*dModip^2 +...) by iteration */
  dTerm = 1.0;
  for (siIndex = 0; siIndex < siNumCoeff; siIndex = siIndex + 1) {
    dFlx = dFlx + (pdCoeff[siIndex] * dTerm);
    dTerm = dTerm * dModip;
  }

  /* Calculation is now over, so return flux */
  return(dFlx);
}


