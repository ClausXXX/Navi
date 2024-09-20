/**
 * ******************************************************************
 * NTCMproj File: Compute_sTEC.c
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

#include "Compute_sTEC.h"

/* Include model header file for global data */
#include "NTCM_Procedure.h"
#include "NTCM_Procedure_private.h"

/**
 * ******************************************************************
 * Compute_sTEC
 * Output and update for atomic system: '<S4>/Compute_sTEC'
 * ******************************************************************
 * Arguments    : real_T ele_rad
 *                real_T vTEC
 * ******************************************************************
 * Return Type  : real_T
 * ******************************************************************
 * Block description for: '<S4>/Compute_sTEC'
 *      @ingroup NTCM_Procedure
 *
 *      <strong> FUNCTION </strong>
 *      This module computes the Mapping Function and converts the estimated VTEC to STEC (Described in Section 2.5.8).
 *
 *      @param[in] ele_rad   => Satellite elevation
 *      @param[in] vTEC   =>  Vertical Total Electron Content [TECU]
 *
 *      @param[out] sTEC   =>  Slant Total Electron Content [TECU]
 *
 *   @file
 * ******************************************************************
 */
real_T Compute_sTEC(real_T ele_rad, real_T vTEC)
{
  real_T sTEC_0;
  real_T a;
  a = sin((1.5707963267948966 - ele_rad) * 0.9782) * 0.934027268728925;
  sTEC_0 = (1.0 / sqrt(1.0 - (a * a))) * vTEC;
  return sTEC_0;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
