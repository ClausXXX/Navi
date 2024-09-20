/**
 * ******************************************************************
 * NTCMproj File: NTCM_Procedure.c
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

#include "NTCM_Procedure.h"
#include "NTCM_Procedure_private.h"

P_NTCM_Procedure_T NTCM_Procedure_P = {
  /* Start of '<Root>/ErrorMnG' */
  {

#if !CheckFlag

    /* Computed Parameter: Constant_Value
     * Referenced by: '<S19>/Constant'
     */
    { 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U }
#define P_ERRORMNG_T_VARIANT_EXISTS
#endif
#ifndef P_ERRORMNG_T_VARIANT_EXISTS
    0
#endif                               /* P_ERRORMNG_T_VARIANT_EXISTS undefined */
  }
  /* End of '<Root>/ErrorMnG' */
};

/**
 * ******************************************************************
 * NTCM_Procedure
 * Output and update for referenced model: 'NTCM_Procedure'
 * ******************************************************************
 * Arguments    : const real_T brdcIonoParam[3]
 *                const real_T *Doy
 *                const real_T *UTC
 *                const real_T llhRecDeg[3]
 *                const real_T llhSatDeg[3]
 *                const real_T *fcarr
 *                real_T *vTEC
 *                real_T *sTEC
 *                real_T *delay_m
 *                uint16_T ErrorCode[16]
 * ******************************************************************
 * Return Type  : void
 * ******************************************************************
 *
 * ******************************************************************
 */
void NTCM_Procedure(const real_T brdcIonoParam[3], const real_T *Doy, const
                    real_T *UTC, const real_T llhRecDeg[3], const real_T
                    llhSatDeg[3], const real_T *fcarr, real_T *vTEC, real_T
                    *sTEC, real_T *delay_m, uint16_T ErrorCode[16])
{
  /* local block i/o variables */
  real_T lOut_NTCM_G;
  real_T lOut_brdcIonoParam_[3];
  real_T lOut_llhRecDeg_[3];
  real_T lOut_llhSatDeg_[3];
  real_T lOut_Doy_;
  real_T lOut_UTC_;
  real_T lOut_delay_m1__d;
  real_T lOut_fcarr_;
  real_T lOut_sTEC_;
  uint16_T lOut_ErrorCodeIn_[12];
  uint16_T lOut_ErrorCodeOut_[3];
  uint16_T lOut_El_ErrCOde_;

  /*
   * Block description for '<Root>/ChekInput':
   *     FUNCTION:
   *     ------
   *     The function check that all input lie within the expected range and perform
   *     correspondent saturation when needed
   *
   *     INPUT:
   *     ------
   *     AzPara => Az0, Az1 and Az2 Iono Parameters broadcasted by Galileo
   *  \n Doy => Day of the year
   *  \n UTC => UTC Time in hours (0 for midnight)
   *  \n llhRecDeg => User position in Longitude[rad], Latitude[rad], height[m]
   *  \n llhSatDeg => Satellite position in Longitude[rad], Latitude[rad], height[m]
   *  \n fcarr => Carrier frequency
   *
   *     OUTPUT:
   *  -  -----
   *     lOutCheck_Input => Validated Input vector [1,...12]
   *  \n lOut_ErrorCheckIn =>Validity Flags [1,...12]
   *
   *
   *
   *
   */
  ChekInput(brdcIonoParam, Doy, UTC, llhRecDeg, llhSatDeg, fcarr,
            lOut_brdcIonoParam_, &lOut_Doy_, &lOut_UTC_, lOut_llhRecDeg_,
            lOut_llhSatDeg_, &lOut_fcarr_, lOut_ErrorCodeIn_);

  /*
   * Block description for '<Root>/NTCM_Procedure_Core':
   *     <strong> FUNCTION </strong>
   *      The core function implements the step-by-step procedure as described in [1].
   *     The "core" suffix aim at sparating this pure algorithmic block from any consistency/control
   *     or I/O logic implemented externally by the user ( the check-in/checkout steps included in the
   *     overall procedure are provided just an example of possible wrapping. It is worthy to remind
   *     that in order to implement the ionospheric algorithm for Galileo single frequency receivers
   *     for each satellite-user line-of-sight the following steps shall be followed:
   *     \n 1) Obtain estimates of user receiver position (lat,long,h)_i, satellite position (lat,long,h)_j
   *     \n 2) Universal Time (UT), in terms of time of day and month
   *     \n 3) Obtain Effective Ionisation Level Azpar and broadcast coefficients (a_i0,a_i1,a_i2 )
   *     \n 4)Calculate satellite elevation (E) and azimuth (A) angles
   *     \n 5)Calculate ionospheric pierce point location for the user-to-satellite
   *     \n 6) Call NTCM_G to calculate VTEC at pierce point location and local time (LT)
   *     \n   (Section 2.5.4 in[1] provides a formula to convert UT to LT).
   *     \n 7) Calculate ionospheric Mapping Function (MF) using the equation provided in Section 2.5.8.
   *     \n 8) Convert VTEC to STEC using the ionospheric mapping function.
   *     \n 9) Convert STEC to time delay using Eq. 1 in[1] for the corresponding frequency to obtain
   *     \n the correction
   *     \n 10) Apply correction to selected link
   *
   *     @param[in] AzPara  		=> alpha0, alpha1, and alpha2 Iono Parameters broadcasted by Galileo
   *     @param[in] Doy  		=> Day of the year
   *     @param[in] UTC   		=> UTC Time in hours (0 for midnight)
   *     @param[in] llhRecDeg   		=> User position in Longitude[rad], Latitude[rad], height[m]
   *     @param[in] llhSatDeg  		=> Satellite position in Longitude[rad], Latitude[rad], height[m]
   *     @param[in] fcarr  		=> Carrier frequency
   *
   *     @param[out] vTEC  		=> Vertical TEC
   *     @param[out] sTEC  		=> Slant TEC
   *     @param[out] delay_m  		=> Signal propagation time delay in [m]
   *     @param[out] ErrorCode  		=> Error Code (optional)
   *
   *
   *   <strong> Tuning Constant </strong>
   *    <table>
   *   <tr><th> Label                      <th>Value
   *   <tr><td> ErrCodeEl <td> 2U
   *   <tr><td> ErrCodeEl  		<td>2U
   *   <tr><td> ErrCodeIn  		<td>1U
   *   <tr><td> ErrCodeOut  	<td>3U
   *   <tr><td> IPP_RefAltitude <td> 450000.0
   *   <tr><td> LTd  			<td>14.0
   *   <tr><td> NoErr  			<td>0U
   *   <tr><td> Pf1  			<td>0.4
   *   <tr><td> doy_DAV  		<td>18.0
   *   <tr><td> doy_DSAV  		<td>6.0
   *   <tr><td> lamGNP_deg  	<td>-71.78
   *   <tr><td> phiGNP_deg  	<td>79.74
   *   <tr><td> phi_c1  		<td>16.0
   *   <tr><td> phi_c2  		<td>-10.0
   *   <tr><td> sigma_c1  		<td>12.0
   *   <tr><td> sigma_c2  		<td>13.0
   *   <tr><td> k1  			<td>0.92519
   *   <tr><td> k10  			<td>1.20556
   *   <tr><td> k11  			<td>1.41808
   *   <tr><td> k12  			<td>0.13985
   *   <tr><td> k2  			<td>0.16951
   *   <tr><td> k3  			<td>0.00443
   *   <tr><td> k4  			<td>0.06626
   *   <tr><td> k5  			<td>0.00899
   *   <tr><td> k6  			<td>0.21289
   *   <tr><td> k7  			<td>-0.15414
   *   <tr><td> k8  			<td>-0.38439
   *   <tr><td> k9  			<td>1.14023
   *  </table>
   *  @ingroup NTCM_Procedure
   *  @file
   */
  NTCM_Procedure_Core(lOut_brdcIonoParam_, lOut_Doy_, lOut_UTC_, lOut_llhRecDeg_,
                      lOut_llhSatDeg_, lOut_fcarr_, &lOut_NTCM_G, &lOut_sTEC_,
                      &lOut_delay_m1__d, &lOut_El_ErrCOde_);

  /*
   * Block description for '<Root>/CheckOutput':
   *     FUNCTION:
   *     ------
   *     The function check that all output lie within the expected range and perform
   *     correspondent saturation when needed
   *
   *     INPUT:
   *     ------
   *  /n vTEC => Vertical TEC [TECU]
   *  /n sTEC => Slant TEC [TECU]
   *  /n delay_m=> Ionospheric Delay [m]
   *  /n llhRecDeg => User position in Longitude[rad], Latitude[rad], height[m]
   *  /n llhSatDeg => Satellite position in Longitude[rad], Latitude[rad], height[m]
   *  /n fcarr => Carrier frequency
   *
   *     OUTPUT:
   *     ------
   *  /n lOutCheck_Input => Validated  Output vector [1,2,3]
   *  /n lOut_ErrorCheckIn =>Validity  Output flag [1,2,3]
   *

   */
  CheckOutput(lOut_NTCM_G, lOut_sTEC_, lOut_delay_m1__d, vTEC, sTEC, delay_m,
              lOut_ErrorCodeOut_);

  /*
   * Block description for '<Root>/ErrorMnG':
   *     ChekOutput
   *     FUNCTION:
   *     ------
   *     The function ccollect all errors log in a unique vector of unsigned integers
   *
   *     INPUT:
   *     ------
   *     ErrorCodeCheckOut => Error Code for Output Interface
   *  /n ErrorCodeCore => Error Code for Core
   *  /n ErrorCodeCheckIn =>Error Code for Input  Interface
   *
   *     OUTPUT:
   *     ------
   *  /n ErrorCode =>Final Err Code Vector [ ErrorCodeCheckOut,ErrorCodeCore,ErrorCodeCheckIn

   */
  ErrorMnG(lOut_ErrorCodeOut_, lOut_El_ErrCOde_, lOut_ErrorCodeIn_, ErrorCode,
           &NTCM_Procedure_P.ErrorMnG_i);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
