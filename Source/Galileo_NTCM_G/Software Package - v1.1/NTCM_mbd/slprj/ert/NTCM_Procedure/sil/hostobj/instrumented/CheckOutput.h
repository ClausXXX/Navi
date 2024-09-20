/**
 * ******************************************************************
 * NTCMproj File: CheckOutput.h
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

#ifndef RTW_HEADER_CheckOutput_h_
#define RTW_HEADER_CheckOutput_h_
#ifndef NTCM_Procedure_COMMON_INCLUDES_
#define NTCM_Procedure_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* NTCM_Procedure_COMMON_INCLUDES_ */

#include "NTCM_Procedure_types.h"
#if CheckFlag

extern void NTCM_Procedure_n(const real_T In1[3], real_T Output[3], boolean_T
  SaturFlag[3]);

#endif

extern void CheckOutput(real_T vTEC, real_T sTEC, real_T delay_m, real_T *vTEC1,
  real_T *sTEC1, real_T *delay_m1, uint16_T ErrorCode[3]);

#endif                                 /* RTW_HEADER_CheckOutput_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */