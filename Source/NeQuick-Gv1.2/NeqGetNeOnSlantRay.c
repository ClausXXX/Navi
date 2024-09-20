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
#include "NeqGetNeOnSlantRay.h"
#include "NeqConstants.h"
#include "NeqCalcEpstParams.h"
#include "NeqCalcTopsideNe.h"
#include "NeqCalcBottomsideNe.h"
#include "IonoConstants.h"

/* Function prototypes */
static void NeqCalcLLHOnRay(
  double dS, SPoint_st* pstRay, SPoint_st* pstP1,
  double dSinSig, double dCosSig, SPoint_st* pstPactual);

/**
 *******************************************************************************
 * @par Name: NeqGetNeOnSlantRay
 * @par Purpose:
 *   (corresponds to 'eldens' in fortran and in C++)
 *   This function calculates electron content at the specified distance along a
 *   slanted ray.
 *
 *   It does this by calling NeqCalcLLHOnRay to update the current lat/lng/h,
 *   before calling NeqCalcEpstParams to update ionosphere properties at this
 *   new point and then calling NeqCalcTopsideNe or NeqCalcBottomsideNe
 *   (depending on position height relative to the F2 electron density peak
 *   point) to calculate the electron content.
 *
 * @par Argument I/O:
 *   @param pstNeQuickInputData (I) Structure containing readonly NeQuick inputs
 *   @param ptGeom->tRay        (I) dR and dLng only
 *   @param ptGeom->tP1         (I) dSinLat and dCosLat only
 *   @param ptGeom->dSinSig     (I) 
 *   @param ptGeom->dCosSig     (I) 
 *   @param ptGeom->dSinDelta   (I) 
 *   @param ptGeom->dCosDelta   (I) 
 *   @param dS                  (I) Distance of the point along the ray (km)
 *   @param ptCurrCCIR          (I/O) In: if already set, Out: if not set before
 *   @param ptPactual           (O) dLat/dLng/dH only updated for current
 *                                  position along slant
 *   @param pdNmax              (O) Electron content at F2 peak
 *   @param ptLayers            (O) Current ionospheric properties (see
 *                                  NeqCalcEpstParams)
 *
 * @par I/O files:
 *   None
 *
 * @return Electron content at specified distance along the slanted ray
 ******************************************************************************/
double NeqGetNeOnSlantRay(NeQuickInputData_st *pstNeQuickInputData,
                          GeometryData_st* pstGeom, double dS,
                          CurrentCCIR_st* pstCurrCCIR, 
                          SPoint_st* pstPactual, double* pdNmax, 
                          LayerProperties_st* pstLayers)
{
  double dReturn; /* Return value, electron content at point */


  /* Calculate the current position (new lat/lng/h) */
  NeqCalcLLHOnRay(dS, &(pstGeom->stRay), &(pstGeom->stP1),
                  pstGeom->dSinSig, pstGeom->dCosSig, pstPactual);

  /* Call NeqCalcEpstParams to calculate the ionosphere properties at new pt */
  NeqCalcEpstParams(pstNeQuickInputData, *pstPactual, pstGeom->dSinDelta, 
                    pstGeom->dCosDelta, pstCurrCCIR, pdNmax, pstLayers);

  if (pstPactual->dH > pstLayers->pdPeakHeight[LAYER_F2_SI]) {
    /* Point is above F2 peak, so call NeqCalcTopsideNe */
    dReturn = NeqCalcTopsideNe(pstPactual->dH, pstLayers, pdNmax);
  } else {
    /* Point is at or below F2 peak, so call NeqCalcBottomsideNe */
    dReturn = NeqCalcBottomsideNe(pstPactual->dH, pstLayers);
  } /* end if above F2 layer peak */

  /* Return the electron content */
  return(dReturn);
}


/**
 *******************************************************************************
 * @par Name: NeqCalcLLHOnRay
 * @par Purpose:
 *   (corresponds to part of 'eldens' in fortran and 'geogra' in C++)
 *   This function adjust tPactual to be the current position (lat/lng/h)
 *   corresponding to slant position at a specified distance (s) along the
 *   slanted ray.
 *
 * @par Argument I/O:
 *   @param dS        (I) Distance of the point along the ray (km)
 *   @param pstRay     (I) dR and dLng only
 *   @param pstP1      (I) dSinLat and dCosLat only
 *   @param dSinSig   (I) 
 *   @param dCosSig   (I) 
 *   @param ptPactual (O) dLat/dLng/dH updated for current position along slant
 *
 * @par I/O files:
 *   None
 *
 * @return void
 ******************************************************************************/
static void NeqCalcLLHOnRay(
  double dS, SPoint_st* pstRay, SPoint_st* pstP1,
  double dSinSig, double dCosSig, SPoint_st* pstPactual)
{
  double dCLong;  /* temporary variable */
  double dTanDel; /* temporary variable */
  double dCosDel; /* temporary variable */
  double dSinDel; /* temporary variable */
  double dArg;    /* temporary variable */


  /* Calculate schtuff */
  dTanDel = dS / pstRay->dR;
  dCosDel = 1.0 / sqrt(1.0 + (dTanDel * dTanDel));
  dSinDel = dTanDel * dCosDel;

  dArg = (pstP1->dSinLat * dCosDel) + (pstP1->dCosLat * dSinDel * dCosSig);

  pstPactual->dLat = atan2(dArg, sqrt(1.0 - (dArg * dArg))) * RADTODEG_D;
  /*ptPactual->dLat = asin(dArg) * RADTODEG_D;*/

  dCLong = atan2(dSinDel * dSinSig * pstP1->dCosLat,
                 dCosDel - (pstP1->dSinLat * dArg)) * RADTODEG_D;
  pstPactual->dLng = dCLong + pstRay->dLng;
  pstPactual->dH = sqrt((dS * dS) + (pstRay->dR * pstRay->dR)) - EARTH_RADIUS_D;
}

/* End of file: $RCSfile: NeqGetNeOnSlantRay.c,v $ */
