/**
 * ******************************************************************
 * NTCMproj File: ChekInput.c
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

#include "ChekInput.h"

/* Include model header file for global data */
#include "NTCM_Procedure.h"
#include "NTCM_Procedure_private.h"

/**
 * ******************************************************************
 * ChekInput
 * Output and update for atomic system: '<Root>/ChekInput'
 * ******************************************************************
 * Arguments    : const real_T AzParam[3]
 *                const real_T *Doy
 *                const real_T *UTC
 *                const real_T llhRecDeg[3]
 *                const real_T llhSatDeg[3]
 *                const real_T *fcarr
 *                real_T brdcIonoParam[3]
 *                real_T *Doy1
 *                real_T *UTC1
 *                real_T llhRecDeg1[3]
 *                real_T llhSatDeg1[3]
 *                real_T *fcarr1
 *                uint16_T ErrorCode[12]
 * ******************************************************************
 * Return Type  : void
 * ******************************************************************
 * Block description for: '<Root>/ChekInput'
 *      FUNCTION:
 *      ------
 *      The function check that all input lie within the expected range and perform
 *      correspondent saturation when needed
 *
 *      INPUT:
 *      ------
 *      AzPara => Az0, Az1 and Az2 Iono Parameters broadcasted by Galileo
 *   \n Doy => Day of the year
 *   \n UTC => UTC Time in hours (0 for midnight)
 *   \n llhRecDeg => User position in Longitude[rad], Latitude[rad], height[m]
 *   \n llhSatDeg => Satellite position in Longitude[rad], Latitude[rad], height[m]
 *   \n fcarr => Carrier frequency
 *
 *      OUTPUT:
 *   -  -----
 *      lOutCheck_Input => Validated Input vector [1,...12]
 *   \n lOut_ErrorCheckIn =>Validity Flags [1,...12]
 * ******************************************************************
 */
void ChekInput(const real_T AzParam[3], const real_T *Doy, const real_T *UTC,
               const real_T llhRecDeg[3], const real_T llhSatDeg[3], const
               real_T *fcarr, real_T brdcIonoParam[3], real_T *Doy1, real_T
               *UTC1, real_T llhRecDeg1[3], real_T llhSatDeg1[3], real_T *fcarr1,
               uint16_T ErrorCode[12])
{

#if CheckFlag

  boolean_T lOut_AND[12];
  boolean_T lOut_LowerTest[12];
  boolean_T lOut_UpperTest[12];

#endif

  real_T lOut_VariantMergeForOutportDoy1;
  real_T lOut_VariantMergeForOutportUTC1;
  real_T lOut_VariantMergeForOutportfcar;
  int32_T i;
  uint16_T lOut_VariantMergeForOutportEr_p[12];

#if CheckFlag

  real_T lOut_Saturation1_idx_3;
  real_T lOut_Saturation1_idx_4;
  boolean_T lOut_AND_j;

#endif

  real_T lOut_Saturation1_idx_11;
  real_T lOut_VariantMergeForOutportbr_0;
  real_T lOut_VariantMergeForOutportbr_1;
  real_T lOut_VariantMergeForOutportll_0;
  real_T lOut_VariantMergeForOutportll_1;
  real_T lOut_VariantMergeForOutportll_2;
  real_T lOut_VariantMergeForOutportll_3;
  real_T lOut_VariantMergeForOutportll_4;
  real_T lOut_VariantMergeForOutportll_5;

#if CheckFlag

  /*
   * Block description for '<S11>/CheckInput':
   *   ChekInput
   *   FUNCTION:
   *   ------
   *   The function check that all input lie within the expected range and perform
   *   correspondent saturation when needed
   *
   *   INPUT:
   *   ------
   *   AzPara => Az0, Az1 and Az2 Iono Parameters broadcasted by Galileo
   *   Doy => Day of the year
   *   UTC => UTC Time in hours (0 for midnight)
   *   llhRecDeg => User position in Longitude[rad], Latitude[rad], height[m]
   *   llhSatDeg => Satellite position in Longitude[rad], Latitude[rad], height[m]
   *   fcarr => Carrier frequency
   *
   *   OUTPUT:
   *  ------
   *   lOutCheck_Input => Validated Input vector [1,...12]
   *   lOut_ErrorCheckIn =>Validity Flags [1,...12]
   *
   *
   */
  lOut_LowerTest[3] = (RangeLwIn[3] <= (*Doy));
  lOut_LowerTest[4] = (RangeLwIn[4] <= (*UTC));
  lOut_LowerTest[11] = (RangeLwIn[11] <= (*fcarr));
  lOut_UpperTest[3] = ((*Doy) <= RangeUpIn[3]);
  lOut_UpperTest[4] = ((*UTC) <= RangeUpIn[4]);
  lOut_LowerTest[0] = (RangeLwIn[0] <= AzParam[0]);
  lOut_LowerTest[5] = (RangeLwIn[5] <= llhRecDeg[0]);
  lOut_LowerTest[8] = (RangeLwIn[8] <= llhSatDeg[0]);
  lOut_UpperTest[0] = (AzParam[0] <= RangeUpIn[0]);
  lOut_UpperTest[5] = (llhRecDeg[0] <= RangeUpIn[5]);
  lOut_UpperTest[8] = (llhSatDeg[0] <= RangeUpIn[8]);
  lOut_LowerTest[1] = (RangeLwIn[1] <= AzParam[1]);
  lOut_LowerTest[6] = (RangeLwIn[6] <= llhRecDeg[1]);
  lOut_LowerTest[9] = (RangeLwIn[9] <= llhSatDeg[1]);
  lOut_UpperTest[1] = (AzParam[1] <= RangeUpIn[1]);
  lOut_UpperTest[6] = (llhRecDeg[1] <= RangeUpIn[6]);
  lOut_UpperTest[9] = (llhSatDeg[1] <= RangeUpIn[9]);
  lOut_LowerTest[2] = (RangeLwIn[2] <= AzParam[2]);
  lOut_LowerTest[7] = (RangeLwIn[7] <= llhRecDeg[2]);
  lOut_LowerTest[10] = (RangeLwIn[10] <= llhSatDeg[2]);
  lOut_UpperTest[2] = (AzParam[2] <= RangeUpIn[2]);
  lOut_UpperTest[7] = (llhRecDeg[2] <= RangeUpIn[7]);
  lOut_UpperTest[10] = (llhSatDeg[2] <= RangeUpIn[10]);
  lOut_UpperTest[11] = ((*fcarr) <= RangeUpIn[11]);
  for (i = 0; i < 12; i++) {
    lOut_AND_j = ((lOut_LowerTest[i]) && (lOut_UpperTest[i]));
    if (lOut_AND_j) {
      lOut_VariantMergeForOutportEr_p[i] = ((uint16_T)NoErr);
    } else {
      lOut_VariantMergeForOutportEr_p[i] = ((uint16_T)ErrCodeIn);
    }

    lOut_AND[i] = lOut_AND_j;
  }

  if ((*Doy) > RangeUpIn[3]) {
    lOut_Saturation1_idx_3 = RangeUpIn[3];
  } else if ((*Doy) < RangeLwIn[3]) {
    lOut_Saturation1_idx_3 = RangeLwIn[3];
  } else {
    lOut_Saturation1_idx_3 = *Doy;
  }

  if ((*UTC) > RangeUpIn[4]) {
    lOut_Saturation1_idx_4 = RangeUpIn[4];
  } else if ((*UTC) < RangeLwIn[4]) {
    lOut_Saturation1_idx_4 = RangeLwIn[4];
  } else {
    lOut_Saturation1_idx_4 = *UTC;
  }

  if ((*fcarr) > RangeUpIn[11]) {
    lOut_Saturation1_idx_11 = RangeUpIn[11];
  } else if ((*fcarr) < RangeLwIn[11]) {
    lOut_Saturation1_idx_11 = RangeLwIn[11];
  } else {
    lOut_Saturation1_idx_11 = *fcarr;
  }

  if (lOut_AND[11]) {
    lOut_VariantMergeForOutportfcar = *fcarr;
  } else {
    lOut_VariantMergeForOutportfcar = lOut_Saturation1_idx_11;
  }

  if (lOut_AND[0]) {
    lOut_Saturation1_idx_11 = AzParam[0];
  } else if (AzParam[0] > RangeUpIn[0]) {
    lOut_Saturation1_idx_11 = RangeUpIn[0];
  } else if (AzParam[0] < RangeLwIn[0]) {
    lOut_Saturation1_idx_11 = RangeLwIn[0];
  } else {
    lOut_Saturation1_idx_11 = AzParam[0];
  }

  if (lOut_AND[5]) {
    lOut_VariantMergeForOutportll_0 = llhRecDeg[0];
  } else if (llhRecDeg[0] > RangeUpIn[5]) {
    lOut_VariantMergeForOutportll_0 = RangeUpIn[5];
  } else if (llhRecDeg[0] < RangeLwIn[5]) {
    lOut_VariantMergeForOutportll_0 = RangeLwIn[5];
  } else {
    lOut_VariantMergeForOutportll_0 = llhRecDeg[0];
  }

  if (lOut_AND[8]) {
    lOut_VariantMergeForOutportll_1 = llhSatDeg[0];
  } else if (llhSatDeg[0] > RangeUpIn[8]) {
    lOut_VariantMergeForOutportll_1 = RangeUpIn[8];
  } else if (llhSatDeg[0] < RangeLwIn[8]) {
    lOut_VariantMergeForOutportll_1 = RangeLwIn[8];
  } else {
    lOut_VariantMergeForOutportll_1 = llhSatDeg[0];
  }

  if (lOut_AND[1]) {
    lOut_VariantMergeForOutportbr_0 = AzParam[1];
  } else if (AzParam[1] > RangeUpIn[1]) {
    lOut_VariantMergeForOutportbr_0 = RangeUpIn[1];
  } else if (AzParam[1] < RangeLwIn[1]) {
    lOut_VariantMergeForOutportbr_0 = RangeLwIn[1];
  } else {
    lOut_VariantMergeForOutportbr_0 = AzParam[1];
  }

  if (lOut_AND[6]) {
    lOut_VariantMergeForOutportll_2 = llhRecDeg[1];
  } else if (llhRecDeg[1] > RangeUpIn[6]) {
    lOut_VariantMergeForOutportll_2 = RangeUpIn[6];
  } else if (llhRecDeg[1] < RangeLwIn[6]) {
    lOut_VariantMergeForOutportll_2 = RangeLwIn[6];
  } else {
    lOut_VariantMergeForOutportll_2 = llhRecDeg[1];
  }

  if (lOut_AND[9]) {
    lOut_VariantMergeForOutportll_3 = llhSatDeg[1];
  } else if (llhSatDeg[1] > RangeUpIn[9]) {
    lOut_VariantMergeForOutportll_3 = RangeUpIn[9];
  } else if (llhSatDeg[1] < RangeLwIn[9]) {
    lOut_VariantMergeForOutportll_3 = RangeLwIn[9];
  } else {
    lOut_VariantMergeForOutportll_3 = llhSatDeg[1];
  }

  if (lOut_AND[2]) {
    lOut_VariantMergeForOutportbr_1 = AzParam[2];
  } else if (AzParam[2] > RangeUpIn[2]) {
    lOut_VariantMergeForOutportbr_1 = RangeUpIn[2];
  } else if (AzParam[2] < RangeLwIn[2]) {
    lOut_VariantMergeForOutportbr_1 = RangeLwIn[2];
  } else {
    lOut_VariantMergeForOutportbr_1 = AzParam[2];
  }

  if (lOut_AND[7]) {
    lOut_VariantMergeForOutportll_4 = llhRecDeg[2];
  } else if (llhRecDeg[2] > RangeUpIn[7]) {
    lOut_VariantMergeForOutportll_4 = RangeUpIn[7];
  } else if (llhRecDeg[2] < RangeLwIn[7]) {
    lOut_VariantMergeForOutportll_4 = RangeLwIn[7];
  } else {
    lOut_VariantMergeForOutportll_4 = llhRecDeg[2];
  }

  if (lOut_AND[10]) {
    lOut_VariantMergeForOutportll_5 = llhSatDeg[2];
  } else if (llhSatDeg[2] > RangeUpIn[10]) {
    lOut_VariantMergeForOutportll_5 = RangeUpIn[10];
  } else if (llhSatDeg[2] < RangeLwIn[10]) {
    lOut_VariantMergeForOutportll_5 = RangeLwIn[10];
  } else {
    lOut_VariantMergeForOutportll_5 = llhSatDeg[2];
  }

  if (lOut_AND[3]) {
    lOut_VariantMergeForOutportDoy1 = *Doy;
  } else {
    lOut_VariantMergeForOutportDoy1 = lOut_Saturation1_idx_3;
  }

  if (lOut_AND[4]) {
    lOut_VariantMergeForOutportUTC1 = *UTC;
  } else {
    lOut_VariantMergeForOutportUTC1 = lOut_Saturation1_idx_4;
  }

#else

  lOut_VariantMergeForOutportDoy1 = *Doy;
  lOut_VariantMergeForOutportUTC1 = *UTC;
  lOut_Saturation1_idx_11 = AzParam[0];

  /*
   * Block description for '<S13>/llhRecDeg':
   *  dxdxxdxhx
   */
  lOut_VariantMergeForOutportll_0 = llhRecDeg[0];
  lOut_VariantMergeForOutportll_1 = llhSatDeg[0];
  lOut_VariantMergeForOutportbr_0 = AzParam[1];

  /*
   * Block description for '<S13>/llhRecDeg':
   *  dxdxxdxhx
   */
  lOut_VariantMergeForOutportll_2 = llhRecDeg[1];
  lOut_VariantMergeForOutportll_3 = llhSatDeg[1];
  lOut_VariantMergeForOutportbr_1 = AzParam[2];

  /*
   * Block description for '<S13>/llhRecDeg':
   *  dxdxxdxhx
   */
  lOut_VariantMergeForOutportll_4 = llhRecDeg[2];
  lOut_VariantMergeForOutportll_5 = llhSatDeg[2];
  lOut_VariantMergeForOutportfcar = *fcarr;
  for (i = 0; i < 12; i++) {
    lOut_VariantMergeForOutportEr_p[i] = ErrVarCheckIn[(i)];
  }

#endif

  *Doy1 = lOut_VariantMergeForOutportDoy1;
  for (i = 0; i < 12; i++) {
    ErrorCode[i] = lOut_VariantMergeForOutportEr_p[i];
  }

  *UTC1 = lOut_VariantMergeForOutportUTC1;
  *fcarr1 = lOut_VariantMergeForOutportfcar;
  brdcIonoParam[0] = lOut_Saturation1_idx_11;
  llhRecDeg1[0] = lOut_VariantMergeForOutportll_0;
  llhSatDeg1[0] = lOut_VariantMergeForOutportll_1;
  brdcIonoParam[1] = lOut_VariantMergeForOutportbr_0;
  llhRecDeg1[1] = lOut_VariantMergeForOutportll_2;
  llhSatDeg1[1] = lOut_VariantMergeForOutportll_3;
  brdcIonoParam[2] = lOut_VariantMergeForOutportbr_1;
  llhRecDeg1[2] = lOut_VariantMergeForOutportll_4;
  llhSatDeg1[2] = lOut_VariantMergeForOutportll_5;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
