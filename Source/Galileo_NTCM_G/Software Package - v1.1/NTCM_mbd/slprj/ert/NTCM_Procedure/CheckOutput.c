/**
 * ******************************************************************
 * NTCMproj File: CheckOutput.c
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

#include "CheckOutput.h"

/* Include model header file for global data */
#include "NTCM_Procedure.h"
#include "NTCM_Procedure_private.h"

/**
 * ******************************************************************
 * NTCM_Procedure_n
 * Output and update for atomic system: '<S6>/Saturation3'
 * ******************************************************************
 * Arguments    : const real_T In1[3]
 *                real_T Output[3]
 *                boolean_T SaturFlag[3]
 * ******************************************************************
 * Return Type  : void
 * ******************************************************************
 *
 * ******************************************************************
 */
#if CheckFlag

void NTCM_Procedure_n(const real_T In1[3], real_T Output[3], boolean_T
                      SaturFlag[3])
{
  SaturFlag[0] = ((RangeLwOut[0] <= In1[0]) && (In1[0] <= RangeUpOut[0]));
  if (SaturFlag[0]) {
    Output[0] = In1[0];
  } else if (In1[0] > RangeUpOut[0]) {
    Output[0] = RangeUpOut[0];
  } else if (In1[0] < RangeLwOut[0]) {
    Output[0] = RangeLwOut[0];
  } else {
    Output[0] = In1[0];
  }

  SaturFlag[1] = ((RangeLwOut[1] <= In1[1]) && (In1[1] <= RangeUpOut[1]));
  if (SaturFlag[1]) {
    Output[1] = In1[1];
  } else if (In1[1] > RangeUpOut[1]) {
    Output[1] = RangeUpOut[1];
  } else if (In1[1] < RangeLwOut[1]) {
    Output[1] = RangeLwOut[1];
  } else {
    Output[1] = In1[1];
  }

  SaturFlag[2] = ((RangeLwOut[2] <= In1[2]) && (In1[2] <= RangeUpOut[2]));
  if (SaturFlag[2]) {
    Output[2] = In1[2];
  } else if (In1[2] > RangeUpOut[2]) {
    Output[2] = RangeUpOut[2];
  } else if (In1[2] < RangeLwOut[2]) {
    Output[2] = RangeLwOut[2];
  } else {
    Output[2] = In1[2];
  }
}

#endif

/**
 * ******************************************************************
 * CheckOutput
 * Output and update for atomic system: '<Root>/CheckOutput'
 * ******************************************************************
 * Arguments    : real_T vTEC
 *                real_T sTEC
 *                real_T delay_m
 *                real_T *vTEC1
 *                real_T *sTEC1
 *                real_T *delay_m1
 *                uint16_T ErrorCode[3]
 * ******************************************************************
 * Return Type  : void
 * ******************************************************************
 * Block description for: '<Root>/CheckOutput'
 *      FUNCTION:
 *      ------
 *      The function check that all output lie within the expected range and perform
 *      correspondent saturation when needed
 *
 *      INPUT:
 *      ------
 *   /n vTEC => Vertical TEC [TECU]
 *   /n sTEC => Slant TEC [TECU]
 *   /n delay_m=> Ionospheric Delay [m]
 *   /n llhRecDeg => User position in Longitude[rad], Latitude[rad], height[m]
 *   /n llhSatDeg => Satellite position in Longitude[rad], Latitude[rad], height[m]
 *   /n fcarr => Carrier frequency
 *
 *      OUTPUT:
 *      ------
 *   /n lOutCheck_Input => Validated  Output vector [1,2,3]
 *   /n lOut_ErrorCheckIn =>Validity  Output flag [1,2,3]
 * ******************************************************************
 */
void CheckOutput(real_T vTEC, real_T sTEC, real_T delay_m, real_T *vTEC1, real_T
                 *sTEC1, real_T *delay_m1, uint16_T ErrorCode[3])
{

#if CheckFlag

  real_T lOut_CheckOut[3];
  real_T lOut_TmpSignalConversionAtSatur[3];
  boolean_T lOut_AND[3];

#endif

  real_T lOut_VariantMergeForOutportdela;
  real_T lOut_VariantMergeForOutportsTEC;
  real_T lOut_VariantMergeForOutportvTEC;
  uint16_T lOut_VariantMergeForOutportEr_0;
  uint16_T lOut_VariantMergeForOutportEr_1;
  uint16_T lOut_VariantMergeForOutportEr_2;

#if CheckFlag

  /*
   * Block description for '<S5>/CheckOutput':
   *   ChekIn
   *   FUNCTION:
   *   ------
   *   The function check that all output lie within the expected range and perform
   *   correspondent saturation when needed
   *
   *   INPUT:
   *   ------
   *   vTEC => Vertical TEC [TECU]
   *   sTEC => Slant TEC [TECU]
   *   delay_m=> Ionospheric Delay [m]
   *   llhRecDeg => User position in Longitude[rad], Latitude[rad], height[m]
   *   llhSatDeg => Satellite position in Longitude[rad], Latitude[rad], height[m]
   *   fcarr => Carrier frequency
   *
   *   OUTPUT:
   *  ------
   *   lOutCheck_Input => Validated  Output vector [1,2,3]
   *   lOut_ErrorCheckIn =>Validity  Output flag [1,...12]
   *
   *
   */
  lOut_TmpSignalConversionAtSatur[0] = vTEC;
  lOut_TmpSignalConversionAtSatur[1] = sTEC;
  lOut_TmpSignalConversionAtSatur[2] = delay_m;
  NTCM_Procedure_n(lOut_TmpSignalConversionAtSatur, lOut_CheckOut, lOut_AND);
  lOut_VariantMergeForOutportvTEC = lOut_CheckOut[0];
  lOut_VariantMergeForOutportsTEC = lOut_CheckOut[1];
  lOut_VariantMergeForOutportdela = lOut_CheckOut[2];
  if (lOut_AND[0]) {
    lOut_VariantMergeForOutportEr_0 = ((uint16_T)NoErr);
  } else {
    lOut_VariantMergeForOutportEr_0 = ((uint16_T)ErrCodeOut);
  }

  if (lOut_AND[1]) {
    lOut_VariantMergeForOutportEr_1 = ((uint16_T)NoErr);
  } else {
    lOut_VariantMergeForOutportEr_1 = ((uint16_T)ErrCodeOut);
  }

  if (lOut_AND[2]) {
    lOut_VariantMergeForOutportEr_2 = ((uint16_T)NoErr);
  } else {
    lOut_VariantMergeForOutportEr_2 = ((uint16_T)ErrCodeOut);
  }

#else

  lOut_VariantMergeForOutportvTEC = vTEC;
  lOut_VariantMergeForOutportsTEC = sTEC;
  lOut_VariantMergeForOutportdela = delay_m;
  lOut_VariantMergeForOutportEr_0 = ErrVarCheckOut[0];
  lOut_VariantMergeForOutportEr_1 = ErrVarCheckOut[1];
  lOut_VariantMergeForOutportEr_2 = ErrVarCheckOut[2];

#endif

  ErrorCode[0] = lOut_VariantMergeForOutportEr_0;
  ErrorCode[1] = lOut_VariantMergeForOutportEr_1;
  ErrorCode[2] = lOut_VariantMergeForOutportEr_2;
  *delay_m1 = lOut_VariantMergeForOutportdela;
  *sTEC1 = lOut_VariantMergeForOutportsTEC;
  *vTEC1 = lOut_VariantMergeForOutportvTEC;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
