/*****************************
(c) Copyright European Space Agency, 2017
This file is subject to the terms and conditions defined in file 'LICENCE.txt', 
which is part of this [source code/executable] package. No part of the package, 
including this file, may be copied, modified, propagated, or distributed except
according to the terms contained in the file 'LICENCE.txt'.
 ******************************************************************************/


/* System Includes */
#include <math.h>
#include <stdio.h>

/* NeQuick Includes */
#include "NeqConstants.h"
#include "IonoConstants.h"
#include "NeqIntegrate.h"
#include "NeqGetNeOnVertRay.h"
#include "NeqGetNeOnSlantRay.h"

/**
 *******************************************************************************
 * @par Name: NeqIntegrate
 * @par Purpose:
 *   (replaces Gaussian integration implemented in 'gint' and 'gintv' in fortran
 *    and gintg7k15ad in C++)
 *
 *   This function performs intgeration to calculate the TEC along rays using
 *   the Kronrod G7-K15 adaptive quadrature method (see Knezevich and Radicella
 *   paper).
 *
 *   This method involves sampling values at 15 points and calculating the
 *   integration from them. At the same time it misses out half of the points to
 *   see what difference it makes and therefore the likely error contained in
 *   the result, before deciding whether to accept the result, or to split the
 *   portion into two and try again in order to improve accuracy.
 *
 *   This function is recursive and an appropriate safeguards has been
 *   implemented to prevent infinite looping.
 *
 *   Interestingly, when an integration is split into two equal parts the
 *   tolerance isn't halved. So, the accuracy of the result is +- (2*dTolerance)
 *   and this error soon propagates down the recursive tree
 *
 * @par Argument I/O:
 *   @param pstIntegrateData  (I) Structure containing readonly NeQuick inputs
 *   @param dH1               (I) Height of point 1
 *   @param dH2               (I) Height of point 2
 *   @param siCurrLevel       (I) Current level of integration recursion
 *   @param ptPactual         (O) dLat/dLng/dH updated for current position
 *                                along slant
 *   @param pdNmax            (O) Electron content at F2 peak
 *   @param ptLayers          (O) Current ionospheric properties (see
 *                                NeqCalcEpstParams)
 *
 * @par I/O files:
 *   None
 *
 * @return Result of numerical integration
 ******************************************************************************/
double NeqIntegrate( IntegrateData_st * pstIntegrateData, double dH1, 
  double dH2, int siCurrLevel, SPoint_st* pstPactual, double* pdNmax,
  LayerProperties_st* pstLayers)
{
  const double XI_D[KRONROD_K15_PTS_SI] = {
                    /* Dictates at what points samples are used in integration
                     * process (-1 => dH1, 1 => dH2). */
    -KRONROD_POINT_7_D, -KRONROD_POINT_6_D, -KRONROD_POINT_5_D,
    -KRONROD_POINT_4_D, -KRONROD_POINT_3_D, -KRONROD_POINT_2_D,
    -KRONROD_POINT_1_D,  0,                  KRONROD_POINT_1_D,
     KRONROD_POINT_2_D,  KRONROD_POINT_3_D,  KRONROD_POINT_4_D,
     KRONROD_POINT_5_D,  KRONROD_POINT_6_D,  KRONROD_POINT_7_D
  };
  const double K15WEIGHTS_D[KRONROD_K15_PTS_SI] = {
                    /* Weights for K15 sample points */
    KRONROD_K15_1_D,    KRONROD_K15_2_D,    KRONROD_K15_3_D,
    KRONROD_K15_4_D,    KRONROD_K15_5_D,    KRONROD_K15_6_D,
    KRONROD_K15_7_D,    KRONROD_K15_8_D,    KRONROD_K15_7_D,
    KRONROD_K15_6_D,    KRONROD_K15_5_D,    KRONROD_K15_4_D,
    KRONROD_K15_3_D,    KRONROD_K15_2_D,    KRONROD_K15_1_D
  };
  const double G7WEIGHTS_D[KRONROD_G7_PTS_SI] = {
                    /* Weights for G7 sample points */
    KRONROD_G7_1_D,     KRONROD_G7_2_D,     KRONROD_G7_3_D,
    KRONROD_G7_4_D,     KRONROD_G7_3_D,     KRONROD_G7_2_D,
    KRONROD_G7_1_D
  };
  double dMidpoint; /* Midpoint value between dH1 and dH2 */
  double dHalfDiff; /* Half of the difference between dH1 and dH2 */
  double dIntK15;   /* Integration result using K15WEIGHTS_D coefficients */
  double dIntG7;    /* Integration result using G7WEIGHTS_D coefficients */
  int siGWiInd;     /* Index into the G7WEIGHTS_D array */
  int siIndex;      /* Loop variable */
  double dSampleX;  /* Value of X (height) at which TEC sample is taken */
  double dSampleY;  /* Value of Y (TEC) at sample point */
  double dReturn;   /* Return value, integration result */
  /*unsigned char uszInfo[EVENT_ADD_INF_SIZE_UI+1];*/
                    /* Used for info strings in GenerateEvent calls */

  /* Calculate the midpoint and the difference  */
  dMidpoint = (dH2 + dH1) / TWO_D;
  dHalfDiff = (dH2 - dH1) / TWO_D;

  /* Initialise the G7 and K15 integration results and the G7 array index */
  dIntK15 = 0.0;
  dIntG7 = 0.0;
  siGWiInd = 0;

  /* Loop through the G7/K15 points */
  for (siIndex = 0; siIndex < KRONROD_K15_PTS_SI; siIndex = siIndex + 1) {
    /* Calculate where the sample point is */
    dSampleX = dMidpoint + (dHalfDiff * XI_D[siIndex]);

    /* Obtain the TEC at sample point */
    if (pstIntegrateData->bVert == TRUE) {
      dSampleY = NeqGetNeOnVertRay(dSampleX, pstLayers, pdNmax);
    } else {
      dSampleY = NeqGetNeOnSlantRay(pstIntegrateData->pstNeQuickInputData, 
                                    pstIntegrateData->pstGeom, dSampleX,
                                    pstIntegrateData->pstCurrCCIR, pstPactual, 
                                    pdNmax, pstLayers);
    } /* end if vertical */

    /* Accumulate onto the K15 integration total */
    dIntK15 = dIntK15 + (dSampleY * K15WEIGHTS_D[siIndex]);

    /* Check if this is a G7 point, ie. every other point */
    if (siIndex % TWO_SI == 1) {
      /* This is a G7 point, so accumulate it onto the G7 integration total */
      dIntG7 = dIntG7 + (dSampleY * G7WEIGHTS_D[siGWiInd]);

      /* Increment the G7 array index ready for the next G7 point */
      siGWiInd = siGWiInd + 1;
    } /* end if a G7 point */
  } /* for each G7/K15 point */

  /* Complete the calculation of the integration results */
  dIntK15 = dIntK15 * dHalfDiff;
  dIntG7 = dIntG7 * dHalfDiff;

  /* Check if the error is within the tolerance */
  if ((fabs((dIntK15 - dIntG7) / dIntK15) <= pstIntegrateData->dTolerance) ||
      (fabs(dIntK15 - dIntG7) <= pstIntegrateData->dTolerance)) {
    /* Error is acceptable so set return value */
    dReturn = dIntK15;
  } else if (siCurrLevel ==
               pstIntegrateData->pstNeQuickInputData->siMaxRecurse) {
    /* Error is not acceptable so would normally split into two equal parts to
     * improve the accuracy, but we've reached the recursion limit set by
     * config, so log an informational message and return the best guess */
     
/*    IonoSprintf(uszInfo, 
      (unsigned char *)"Integration Level:%.0f T:%f  G7:%.3e  K15:%.3e", 
      (double) siCurrLevel, pstIntegrateData->dTolerance, dIntG7, dIntK15, 
      FOUR_SI);
                        
    IonoError(pstIntegrateData->pstNeQuickInputData->eModuleID, __LINE__, 
              E_EC_VERBOSE_I,  uszInfo,(unsigned char *) __FUNCTION__, 
              (double)siCurrLevel,
              (double)pstIntegrateData->pstNeQuickInputData->siMaxRecurse);*/
                  
    dReturn = dIntK15;
  } else {
    /* Error is not acceptable so split into two equal parts to improve
     * accuracy and try again.
     * nb. There is recursion here!!! */
    dReturn = NeqIntegrate(pstIntegrateData, dH1, dH1 + dHalfDiff,
                           siCurrLevel + 1, pstPactual, pdNmax, pstLayers) +
              NeqIntegrate(pstIntegrateData, dH1 + dHalfDiff, dH2, 
                           siCurrLevel + 1, pstPactual, pdNmax, pstLayers);
  } /* End if within tolerance */

  /* Return the integration result */
  return(dReturn);
}

/* End of file: $RCSfile: NeqIntegrate.c,v $ */
