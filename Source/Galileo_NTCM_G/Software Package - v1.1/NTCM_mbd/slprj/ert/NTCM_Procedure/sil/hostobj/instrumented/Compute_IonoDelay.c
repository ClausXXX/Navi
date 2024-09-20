/**
 * ******************************************************************
 * NTCMproj File: Compute_IonoDelay.c
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

#include "Compute_IonoDelay.h"

/* Include model header file for global data */
#include "NTCM_Procedure.h"
#include "NTCM_Procedure_private.h"
#include "rt_roundd.h"

/**
 * ******************************************************************
 * Compute_IonoDelay
 * Output and update for atomic system: '<S4>/Compute_IonoDelay'
 * ******************************************************************
 * Arguments    : real_T sTEC
 *                real_T fCarr
 * ******************************************************************
 * Return Type  : real_T
 * ******************************************************************
 * Block description for: '<S4>/Compute_IonoDelay'
 *         @ingroup NTCM_Procedure
 *
 *      <strong> FUNCTION </strong>
 *      Compute ionospheric Delay
 *
 *      @param[in] sTEC  => Slant TEC
 *      @param[in] fCarr  => Carrier Frequency
 *      @param[out] delay_m  =>ionospheric Delay[m]
 *
 *   @file
 * ******************************************************************
 */
real_T Compute_IonoDelay(real_T sTEC, real_T fCarr)
{
  real_T delay_m1_0;
  real_T tmp;
  uint32_T tmp_0;
  profileStart_NTCM_Procedure(8U); /* original_line:65 */tmp = rt_roundd(fCarr);profileEnd_NTCM_Procedure(8U); /* original_line:65 */
  if (tmp < 4.294967296E+9) {
    if (tmp >= 0.0) {
      tmp_0 = (uint32_T)tmp;
    } else {
      tmp_0 = 0U;
    }
  } else {
    tmp_0 = MAX_uint32_T;
  }

  if (tmp_0 == 0U) {
    delay_m1_0 = 0.0;
  } else {
    delay_m1_0 = (sTEC * 4.03E+17) / (fCarr * fCarr);
  }

  return delay_m1_0;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
