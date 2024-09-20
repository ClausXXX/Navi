/**
 * ******************************************************************
 * NTCMproj File: ErrorMnG.h
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

#ifndef RTW_HEADER_ErrorMnG_h_
#define RTW_HEADER_ErrorMnG_h_
#ifndef NTCM_Procedure_COMMON_INCLUDES_
#define NTCM_Procedure_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* NTCM_Procedure_COMMON_INCLUDES_ */

#include "NTCM_Procedure_types.h"

/* Parameters for system: '<Root>/ErrorMnG' */
struct P_ErrorMnG_T_ {

#if !CheckFlag

  uint16_T Constant_Value[16];         /* Computed Parameter: Constant_Value
                                        * Referenced by: '<S19>/Constant'
                                        */

#define P_ERRORMNG_T_VARIANT_EXISTS
#endif

#ifndef P_ERRORMNG_T_VARIANT_EXISTS

  char _rt_unused;

#endif

};

extern void ErrorMnG(const uint16_T ErrorCodeCheckOut[3], uint16_T ErrorCodeCore,
                     const uint16_T ErrorCodeCheckIn[12], uint16_T ErrorCode[16],
                     P_ErrorMnG_T *localP);

#endif                                 /* RTW_HEADER_ErrorMnG_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
