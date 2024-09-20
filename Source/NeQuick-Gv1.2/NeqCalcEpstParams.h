/*****************************
(c) Copyright European Space Agency, 2017
This file is subject to the terms and conditions defined in file 'LICENCE.txt', 
which is part of this [source code/executable] package. No part of the package, 
including this file, may be copied, modified, propagated, or distributed except
according to the terms contained in the file 'LICENCE.txt'.
******************************************************************************/

#ifndef NEQCALCEPSTPARAMS_H
#define NEQCALCEPSTPARAMS_H

#include "NeqTypes.h"

/**
 *******************************************************************************
 * @par Name: NeqCalcEpstParams
 * @par Purpose:
 *   (corresponds to part of 'eldens', part of 'cciri', 'ef1' and 'prepmdgr' in
 *    fortran and 'preprepmdgr' in C++)
 *   This function calculates the values of ionospheric properties for the
 *   current latitude, longitude, time, etc.
 *
 *   The ionospheric properties calculated (in order) are:
 *     pdF0         ([F2])
 *     dM3000
 *     pdF0         (    [E][F1])
 *     pdPeakHeight ([F2][E][F1])
 *     pdBotThick   ([F2]    [F1]   [E]                )
 *     pdTopThick   (    [F1]    [E]              [F2] )
 *     dAmp         (                  [F2][F1][E]     )
 *
 * @par Argument I/O:
 *   @param pstNeQuickInputData (I) Structure containing readonly NeQuick inputs
 *   @param tPactual            (I) (dLat and dLng only) The current position
 *   @param dSinDelta           (I) Declination of sun
 *   @param dCosDelta           (I) Declination of sun
 *   @param ptCurrCCIR          (I/O) In: if already set, Out: if not set before
 *   @param pdNmax              (O) 
 *   @param ptLayers            (O) The current ionospheric properties
 *
 * @par I/O files:
 *   None
 *
 * @return void
 ******************************************************************************/
void NeqCalcEpstParams(NeQuickInputData_st *pstNeQuickInputData,
                       SPoint_st stPactual, double dSinDelta, double dCosDelta,
                       CurrentCCIR_st* pstCurrCCIR, double* pdNmax,
                       LayerProperties_st* pstLayers);


#endif /* NEQCALCEPSTPARAMS_H */
