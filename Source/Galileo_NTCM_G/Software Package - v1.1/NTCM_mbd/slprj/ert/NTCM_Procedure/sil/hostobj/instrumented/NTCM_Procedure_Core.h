/**
 * ******************************************************************
 * NTCMproj File: NTCM_Procedure_Core.h
 * ******************************************************************
 *  @author      Francesco Menzione
 *  @reviewer    Matteo Sgammini
 *  @copyright   Joint Research Centre (JRC), 2022
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 *  @ingroup NTCM_Procedure
 * ******************************************************************
 * Code generated for Simulink model 'NTCM_Procedure'.
 * Model version                  : 1.0
 * Simulink Coder version         : 9.5 (R2021a) 14-Nov-2020
 * C/C++ source code generated on : Tue Apr 26 16:40:10 2022
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Custom Processor->Custom
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 *    3. MISRA C:2012 guidelines
 * Validation result: Passed (21), Warning (1), Error (0)
 *
 *
 * @file
 */

#ifndef RTW_HEADER_NTCM_Procedure_Core_h_
#define RTW_HEADER_NTCM_Procedure_Core_h_
#ifndef NTCM_Procedure_COMMON_INCLUDES_
#define NTCM_Procedure_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* NTCM_Procedure_COMMON_INCLUDES_ */

#include "NTCM_Procedure_types.h"

/* Child system includes */
#include "ComputeDoA.h"
#include "ComputeLocalTime.h"
#include "Compute_IonoDelay.h"
#include "Compute_sTEC.h"
#include "ElevetionCheck.h"
#include "getIonoPiercePoint.h"
#include "llh2xyz_Rec.h"
#include "llh2xyz_Sat.h"
#include "NTCM_G.h"

extern void NTCM_Procedure_Core(const real_T brdcIonoParam[3], real_T Doy,
  real_T UTC, const real_T llhRecDeg[3], const real_T llhSatDeg[3], real_T fcarr,
  real_T *vTEC, real_T *sTEC, real_T *delay_m, uint16_T *ErrorCode);

#endif                                 /* RTW_HEADER_NTCM_Procedure_Core_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
