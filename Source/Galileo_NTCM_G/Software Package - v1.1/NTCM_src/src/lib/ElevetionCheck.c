/**
 * ******************************************************************
 * NTCMproj File: ElevetionCheck.c
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

#include "ElevetionCheck.h"

/* Include model header file for global data */
#include "NTCM_Procedure.h"
#include "NTCM_Procedure_private.h"

/**
 * ******************************************************************
 * NTCM_Procedure_e
 * Output and update for atomic system: '<S36>/Saturation6'
 * ******************************************************************
 * Arguments    : real_T u
 *                real_T *y
 *                boolean_T *SaturFlag
 * ******************************************************************
 * Return Type  : void
 * ******************************************************************
 *
 * ******************************************************************
 */
#if CheckFlag

void NTCM_Procedure_e(real_T u, real_T *y, boolean_T *SaturFlag)
{
  *SaturFlag = ((RangeLwEl <= u) && (u <= RangeUpEl));
  if (*SaturFlag) {
    *y = u;
  } else if (u > RangeUpEl) {
    *y = RangeUpEl;
  } else if (u < RangeLwEl) {
    *y = RangeLwEl;
  } else {
    *y = u;
  }
}

#endif

/**
 * ******************************************************************
 * ElevetionCheck
 * Output and update for atomic system: '<S4>/ElevationCheck'
 * ******************************************************************
 * Arguments    : real_T El_check_in
 *                real_T *El_check_out
 *                uint16_T *El_ErrCOde
 * ******************************************************************
 * Return Type  : void
 * ******************************************************************
 * Block description for: '<S4>/ElevationCheck'
 *      <strong> FUNCTION </strong>
 *      Positivity elevetaion check
 *
 *      @param[in] El_check_in/t/t Satellite Elevation [rad]
 *
 *      @param[out] El_check_out/t/t Limited Elevation
 *      @param[out] El_ErrCOde /t/t Elevation out of range error code
 * ******************************************************************
 */
void ElevetionCheck(real_T El_check_in, real_T *El_check_out, uint16_T
                    *El_ErrCOde)
{

#if CheckFlag

  boolean_T lOut_AND;

#endif

  real_T lOut_VariantMergeForOutportEl_c;
  uint16_T lOut_VariantMergeForOutportEl_E;

#if CheckFlag

  /*
   * Block description for '<S35>/ElevationCheck':
   *  # Test Point
   *   FUNCTION:
   *   ------
   *   Positivity elevetaion check
   *
   *   INPUT:
   *   ------
   *   El_check_in=> Satellite Elevation [rad]
   *
   *   OUTPUT:
   *  ------
   *   El_check_out=> Limited Elevation
   *   El_ErrCOde => Elevation out of range error code
   *
   *

   */
  NTCM_Procedure_e(El_check_in, &lOut_VariantMergeForOutportEl_c, &lOut_AND);
  if (lOut_AND) {
    lOut_VariantMergeForOutportEl_E = ((uint16_T)NoErr);
  } else {
    lOut_VariantMergeForOutportEl_E = ((uint16_T)ErrCodeEl);
  }

#else

  lOut_VariantMergeForOutportEl_c = El_check_in;
  lOut_VariantMergeForOutportEl_E = ErrVarEl;

#endif

  *El_ErrCOde = lOut_VariantMergeForOutportEl_E;
  *El_check_out = lOut_VariantMergeForOutportEl_c;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
