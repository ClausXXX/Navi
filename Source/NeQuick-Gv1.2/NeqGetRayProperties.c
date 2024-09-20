/*****************************
(c) Copyright European Space Agency, 2017
This file is subject to the terms and conditions defined in file 'LICENCE.txt', 
which is part of this [source code/executable] package. No part of the package, 
including this file, may be copied, modified, propagated, or distributed except
according to the terms contained in the file 'LICENCE.txt'.
 ******************************************************************************/

/* System Includes */
#include <math.h>

/* NeQuick Includes */
#include "NeqGetRayProperties.h"
#include "NeqTypes.h"
#include "NeqConstants.h"

/* Function prototypes */
static void NeqCalcRayProperties1(SPoint_st* pstP1, SPoint_st* pstP2,
                                  SPoint_st* pstRay, double* pdZeta);
static void NeqCalcRayProperties2(SPoint_st* pstRay, SPoint_st* pstP2,
                                  SPoint_st* pstP1,
                                  double* pdSinSig, double* pdCosSig);

/**
 *******************************************************************************
 * @par Name: NeqGetRayProperties
 * @par Purpose:
 *   (corresponds to 'rays' in fortran and 'rays2' in C++)
 *   This function checks if the ray is vertical and set P2 if it is. It then
 *   calculates the radius of P1 and P2 before calling NeqCalcRayProperties1 to
 *   calculate the properties of the ray and checks if it is valid. If the ray
 *   is slanted it then calculates some other ray properties by calling
 *   NeqCalcRayProperties2.
 *
 * @par Argument I/O:
 *   @param ptP1     (I/O) In: dLat/dLng(deg) and dH(km) only
 *                         Out: dSinLat/dCosLat(slant) & dR(km) only
 *   @param ptP2     (I/O) In: dLat/dLng(deg) and dH(km) only
 *                         Out: dLng(deg)(vert), dSinLat/dCosLat(slant) & dR(km)
 *   @param ptRay    (O) Ray perigee (dLat/dLng(deg) and dR(km) only)
 *   @param pdZeta   (O) Ray zenith angle (deg)
 *   @param pdSinSig (O) Sin of azimuth(slant)
 *   @param pdCosSig (O) Cos of azimuth(slant)
 *
 * @par I/O files:
 *   None
 *
 * @return TRUE if error found, otherwise FALSE
 ******************************************************************************/
Boolean_b NeqGetRayProperties(SPoint_st* pstP1, 
                              SPoint_st* pstP2, 
                              SPoint_st* pstRay,
                              double* pdZeta, double* pdSinSig,
                              double* pdCosSig)
{
  Boolean_b bError = FALSE; /* Return value for function */


  /* If p2 is virtually above p1 then set p2 lng to the same as p1 lng */
  if ((fabs(pstP2->dLat - pstP1->dLat) < VERTTOLLATLNG_D) &&
      (fabs(pstP2->dLng - pstP1->dLng) < VERTTOLLATLNG_D)) {
    pstP2->dLng = pstP1->dLng;

  } /* End if p2 above p1 */

  /* Calculate the ranges of P1 and P2 from the centre of the earth */
  pstP1->dR = EARTH_RADIUS_D + pstP1->dH;
  pstP2->dR = EARTH_RADIUS_D + pstP2->dH;

  /* Call NeqCalcRayProperties1 to calculate the properties of the ray itself
   * and other stuff */
  NeqCalcRayProperties1(pstP1, pstP2, pstRay, pdZeta);

  /* Check pdZeta and ray radius are valid */
  if ((fabs(*pdZeta) > MAX_ZETA_D) && (pstRay->dR < EARTH_RADIUS_D)) {
    /* Values are invalid, so set return value to NOTOK */
    bError = TRUE;
  }

  /* If p2 not above p1, call NeqCalcRayProperties2 to calculate  */
  if (pstRay->dR >= VERTTOLRAYDR_D) {
    NeqCalcRayProperties2(pstRay, pstP2, pstP1, pdSinSig, pdCosSig);
  }

  /* Return error status */
  return bError;
}


/**
 *******************************************************************************
 * @par Name: NeqCalcRayProperties1
 * @par Purpose:
 *   (corresponds to 'naut' in fortran and in C++)
 *   This function calculates the properties of the ray. It does not calculate
 *   as many properties (e.g. angles) if the ray is vertical as they are not
 *   needed.
 *
 * @par Argument I/O:
 *   @param ptP1   (I/O) In: dLat/dLng(deg) and dR(km) only
 *                       Out: dSinLat/dCosLat(slant) only
 *   @param ptP2   (I/O) In: dLat/dLng(deg) and dR(km) only
 *                       Out: dSinLat/dCosLat(slant) only
 *   @param ptRay  (O) Ray perigee (dLat/dLng(deg) and dR(km) only)
 *   @param pdZeta (O) Ray zenith angle (deg)
 *
 * @par I/O files:
 *   None
 *
 * @return void
 ******************************************************************************/
static void NeqCalcRayProperties1(SPoint_st* pstP1, SPoint_st* pstP2,
                                  SPoint_st* pstRay, double* pdZeta)
{
  double dCosDl12; /* cos zenith angle */
  double dSinDl12; /* sin zenith angle */
  double dCosDel;  /* cos(angle between P1 and P2 at earth centre) */
  double dSinDel;  /* sin(angle between P1 and P2 at earth centre) */ 
  double dSinSigp; /* sine of azimuth */
  double dCosSigp; /* cosine of azimuth */
  double dDelp;    /* temporary variable */
  double dSinDelp; /* sin(dDelp) */
  double dCosDelp; /* cos(dDelp) */
  double dSinPhp;  /* ray perigee latitude */
  double dCosPhp;  /* ray perigee latitude */
  double dSinLamp; /* ray perigee longitude */
  double dCosLamp; /* ray perigee longitude */


  /* nb. ptRay stores only lat,lng,r (other attributes not used) */

  /* Check if p2 is directly above p1 */
  if ((fabs(pstP2->dLat - pstP1->dLat) < VERTTOLLATLNG_D) &&
      (fabs(pstP2->dLng - pstP1->dLng) < VERTTOLLATLNG_D)) {
    /* p2 is directly above p1 */
    /* Set ray lat/lng to p1 lat/lng */
    pstRay->dLat = pstP1->dLat;
    pstRay->dLng = pstP1->dLng;

    /* Set ray to have no slant (r=0 and zeta=0) */
    pstRay->dR = 0.0;
    *pdZeta = 0.0;
  } else {
    /* p2 is not directly above p1 */
    /* Set sin/cos of p1 and p2 lats */
    pstP1->dSinLat = sin(pstP1->dLat * DEG_TO_RAD_D);
    pstP2->dSinLat = sin(pstP2->dLat * DEG_TO_RAD_D);
    pstP1->dCosLat = cos(pstP1->dLat * DEG_TO_RAD_D);
    pstP2->dCosLat = cos(pstP2->dLat * DEG_TO_RAD_D);

    /* Calculate zeta (the zenith angle of p2 at p1) using the great circle
     * cosine formula.
     * nb. zeta is in radians at this point!!! */
    dCosDl12 = cos((pstP2->dLng - pstP1->dLng) * DEG_TO_RAD_D);
    dSinDl12 = sin((pstP2->dLng - pstP1->dLng) * DEG_TO_RAD_D);
    dCosDel = (pstP1->dSinLat * pstP2->dSinLat) +
              (pstP1->dCosLat * pstP2->dCosLat * dCosDl12);
    dSinDel = sqrt(1.0 - (dCosDel * dCosDel));
    *pdZeta = atan2(dSinDel, dCosDel - (pstP1->dR / pstP2->dR));

    /* Calculate temporary variables dSinDelp, dCosDelp, dSinPhp and dCosPhp */
    dSinSigp = (dSinDl12 * pstP2->dCosLat) / dSinDel;
    dCosSigp = ((pstP2->dSinLat -
                 (dCosDel * pstP1->dSinLat)) / dSinDel) / pstP1->dCosLat;
    dDelp = (PI_D / TWO_D) - *pdZeta;
    dSinDelp = sin(dDelp);
    dCosDelp = cos(dDelp);
    dSinPhp = (pstP1->dSinLat * dCosDelp) -
              (pstP1->dCosLat * dSinDelp * dCosSigp);
    dCosPhp = sqrt(1.0 - (dSinPhp * dSinPhp));

    /* Set ray latitude */
    pstRay->dLat = atan2(dSinPhp, dCosPhp) * RADTODEG_D;

    /* Set ray longitude */
    dSinLamp = (-dSinSigp * dSinDelp) / dCosPhp;
    dCosLamp = ((dCosDelp -
                 (pstP1->dSinLat * dSinPhp)) / pstP1->dCosLat) / dCosPhp;
    pstRay->dLng = (atan2(dSinLamp, dCosLamp) * RADTODEG_D) + pstP1->dLng;

    /* Set ray radius */
    pstRay->dR = pstP1->dR * sin(*pdZeta);

    /* Convert zeta to degrees */
    *pdZeta = *pdZeta * RADTODEG_D;
  } /* End if p2 above p1 */
}


/**
 *******************************************************************************
 * @par Name: NeqCalcRayProperties2
 * @par Purpose:
 *   (corresponds to 'gcirc' in fortran and in C++)
 *   This function calculates the sine and cosine of end point latitudes and
 *   azimuth. It is only called for slanted rays.
 *
 * @par Argument I/O:
 *   @param ptRay    (I) Ray perigee (dLat/dLng(deg) only)
 *   @param ptP2     (I/O) In: dLat/dLng(deg) only
 *                         Out: dSinLat/dCosLat only
 *   @param ptP1     (O) dSinLat/dCosLat only
 *   @param pdSinSig (O) Sin of azimuth
 *   @param pdCosSig (O) Cos of azimuth
 *
 * @par I/O files:
 *   None
 *
 * @return void
 ******************************************************************************/
static void NeqCalcRayProperties2(SPoint_st* pstRay, SPoint_st* pstP2,
                                  SPoint_st* pstP1,
                                  double* pdSinSig, double* pdCosSig)
{
  double dDeltaLng; /* Difference between longitudes of p2 and ray (deg) */
  double dCosPsi;   /* cos(angular distance between ends of ray) */
  double dSinPsi;   /* sin(angular distance between ends of ray) */


  /* Calculate sin and cos of end point lats (using ray latitude for p1) */
  pstP1->dSinLat = sin(pstRay->dLat * DEG_TO_RAD_D); 
  pstP1->dCosLat = cos(pstRay->dLat * DEG_TO_RAD_D); 
  pstP2->dSinLat = sin(pstP2->dLat * DEG_TO_RAD_D);
  pstP2->dCosLat = cos(pstP2->dLat * DEG_TO_RAD_D);

  /* Calculate the difference in longitude of ray end points */
  dDeltaLng = (pstP2->dLng - pstRay->dLng) * DEG_TO_RAD_D;

  /* Check if lat of lower end point is +-90 degrees. If so, this would cause a
   * divide by zero error later as p1 coslat would be zero. */
  if (fabs(fabs(pstRay->dLat) - RANGE_LATDEG_MAX_D) < RAYPOLETOL_D) {
    /* Set sin and cos sig appropriately */
    *pdSinSig = 0.0;
    if (pstRay->dLat > 0.0) {
      /* ray lat is +90 */
      *pdCosSig = -1.0;
    } else {
      /* ray lat is -90 */
      *pdCosSig = 1.0;
    } /* End if ray lat > 0.0 */
  } else {
    /* Calculate sin and cos of angular distance between ends of ray */
    dCosPsi = (pstP1->dSinLat * pstP2->dSinLat) +
              (pstP1->dCosLat * pstP2->dCosLat * cos(dDeltaLng));
    dSinPsi = sqrt(1.0 - (dCosPsi * dCosPsi));

    /* Calculate sin and cos of azimuth */
    *pdSinSig = (pstP2->dCosLat * sin(dDeltaLng)) / dSinPsi;
    *pdCosSig = (pstP2->dSinLat - (pstP1->dSinLat * dCosPsi)) /
                (dSinPsi * pstP1->dCosLat);
  } /* End if ray lat is +-90 degrees */
}

/* End of file: $RCSfile: NeqGetRayProperties.c,v $ */
