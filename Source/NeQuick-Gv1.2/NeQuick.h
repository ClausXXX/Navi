/*****************************
(c) Copyright European Space Agency, 2017
This file is subject to the terms and conditions defined in file 'LICENCE.txt', 
which is part of this [source code/executable] package. No part of the package, 
including this file, may be copied, modified, propagated, or distributed except
according to the terms contained in the file 'LICENCE.txt'.
 ******************************************************************************/

#ifndef NEQUICK_H
#define NEQUICK_H

#include "NeqTypes.h"
#include "NeqCalcModipAz.h"
#include "NeqGetRayProperties.h"
#include "NeqCalcEpstParams.h"
#include "NeqIntegrate.h"
#include "IonoConstants.h"

/*******************************************************************************
* @par Name: 
* @par Purpose:
*   (corresponds to 'slQu' in fortran and 'TecEval' and 'NeQuickMain2' in C++)
*   This function calculates the total electron content (TEC) between two
*   points. It calculates STEC between the points or VTEC if point 2 is almost
*   directly above point 1. Point 1 is the sensor station and point 2 is the
*   satellite. The value is calulated from the month, time and Az value, as 
*   well as the two positions.
*
* @par Argument I/O:
* @param pstNeQuickInputData[in] Input data for NeQuick function
* @param pstCurrCCIR[out]        Current CCIR data (CCIR map for current month
*                                and R12 values)- used internally.
* @param pdSTEC[out]             Calculated TEC between points p1 and p2
*
* @par I/O files:
*   None
*
* @return E_ERROR if error found, otherwise E_OK
*******************************************************************************/
ReturnValue_e NeQuick(NeQuickInputData_st *pstNeQuickInputData,
                      CurrentCCIR_st *pstCurrCCIR, double *pdSTEC);

#endif /* NEQUICK_H */
