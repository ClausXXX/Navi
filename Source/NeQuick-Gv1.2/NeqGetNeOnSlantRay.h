/*****************************
(c) Copyright European Space Agency, 2017
This file is subject to the terms and conditions defined in file 'LICENCE.txt', 
which is part of this [source code/executable] package. No part of the package, 
including this file, may be copied, modified, propagated, or distributed except
according to the terms contained in the file 'LICENCE.txt'.
 ******************************************************************************/

#ifndef NEQGETNEONSLANTRAY_H
#define NEQGETNEONSLANTRAY_H

/* System Includes */

/* NeQuick Includes */
#include "NeqTypes.h"


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
                          LayerProperties_st* pstLayers);

#endif /* NEQGETNEONSLANTRAY_H */
