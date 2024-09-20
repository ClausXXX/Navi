/**
 * ******************************************************************
 * NTCMproj File: ErrorMnG.c
 * ******************************************************************
 *  @author      Francesco Menzione
 *  @reviewer    Matteo Sgammini
 *  @copyright   Joint Research Centre (JRC), 2022
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), Version 1.2 or later
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

#include "ErrorMnG.h"

/* Include model header file for global data */
#include "NTCM_Procedure.h"
#include "NTCM_Procedure_private.h"

/**
 * ******************************************************************
 * ErrorMnG
 * Output and update for atomic system: '<Root>/ErrorMnG'
 * ******************************************************************
 * Arguments    : const uint16_T ErrorCodeCheckOut[3]
 *                uint16_T ErrorCodeCore
 *                const uint16_T ErrorCodeCheckIn[12]
 *                uint16_T ErrorCode[16]
 *                P_ErrorMnG_T *localP
 * ******************************************************************
 * Return Type  : void
 * ******************************************************************
 * Block description for: '<Root>/ErrorMnG'
 *      ChekOutput
 *      FUNCTION:
 *      ------
 *      The function ccollect all errors log in a unique vector of unsigned integers
 *
 *      INPUT:
 *      ------
 *      ErrorCodeCheckOut => Error Code for Output Interface
 *   /n ErrorCodeCore => Error Code for Core
 *   /n ErrorCodeCheckIn =>Error Code for Input  Interface
 *
 *      OUTPUT:
 *      ------
 *   /n ErrorCode =>Final Err Code Vector [ ErrorCodeCheckOut,ErrorCodeCore,ErrorCodeCheckIn
 * ******************************************************************
 */
void ErrorMnG(const uint16_T ErrorCodeCheckOut[3], uint16_T ErrorCodeCore, const
              uint16_T ErrorCodeCheckIn[12], uint16_T ErrorCode[16],
              P_ErrorMnG_T *localP)
{
  int32_T i;
  uint16_T lOut_VariantMergeForOutportEr_e[16];

#if !CheckFlag

  (void) (ErrorCodeCheckOut);

#endif

#if !CheckFlag

  (void) (ErrorCodeCore);

#endif

#if !CheckFlag

  (void) (ErrorCodeCheckIn);

#endif

#if !(!CheckFlag)

  (void) (localP);

#endif

#if CheckFlag

  for (i = 0; i < 12; i++) {
    lOut_VariantMergeForOutportEr_e[i + 4] = ErrorCodeCheckIn[i];
  }

  lOut_VariantMergeForOutportEr_e[0] = ErrorCodeCheckOut[0];
  lOut_VariantMergeForOutportEr_e[1] = ErrorCodeCheckOut[1];
  lOut_VariantMergeForOutportEr_e[2] = ErrorCodeCheckOut[2];
  lOut_VariantMergeForOutportEr_e[3] = ErrorCodeCore;

#endif

  for (i = 0; i < 16; i++) {

#if !CheckFlag

    lOut_VariantMergeForOutportEr_e[i] = localP->Constant_Value[i];

#endif

    ErrorCode[i] = lOut_VariantMergeForOutportEr_e[i];
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
