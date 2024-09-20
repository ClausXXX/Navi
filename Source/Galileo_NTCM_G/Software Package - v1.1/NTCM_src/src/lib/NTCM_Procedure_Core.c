/**
 * ******************************************************************
 * NTCMproj File: NTCM_Procedure_Core.c
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

#include "NTCM_Procedure_Core.h"

/* Include model header file for global data */
#include "NTCM_Procedure.h"
#include "NTCM_Procedure_private.h"

/**
 * ******************************************************************
 * NTCM_Procedure_Core
 * Output and update for atomic system: '<Root>/NTCM_Procedure_Core'
 * ******************************************************************
 * Arguments    : const real_T brdcIonoParam[3]
 *                real_T Doy
 *                real_T UTC
 *                const real_T llhRecDeg[3]
 *                const real_T llhSatDeg[3]
 *                real_T fcarr
 *                real_T *vTEC
 *                real_T *sTEC
 *                real_T *delay_m
 *                uint16_T *ErrorCode
 * ******************************************************************
 * Return Type  : void
 * ******************************************************************
 * Block description for: '<Root>/NTCM_Procedure_Core'
 *      <strong> FUNCTION </strong>
 *       The core function implements the step-by-step procedure as described in [1].
 *      The "core" suffix aim at sparating this pure algorithmic block from any consistency/control
 *      or I/O logic implemented externally by the user ( the check-in/checkout steps included in the
 *      overall procedure are provided just an example of possible wrapping. It is worthy to remind
 *      that in order to implement the ionospheric algorithm for Galileo single frequency receivers
 *      for each satellite-user line-of-sight the following steps shall be followed:
 *      \n 1) Obtain estimates of user receiver position (lat,long,h)_i, satellite position (lat,long,h)_j
 *      \n 2) Universal Time (UT), in terms of time of day and month
 *      \n 3) Obtain Effective Ionisation Level Azpar and broadcast coefficients (a_i0,a_i1,a_i2 )
 *      \n 4)Calculate satellite elevation (E) and azimuth (A) angles
 *      \n 5)Calculate ionospheric pierce point location for the user-to-satellite
 *      \n 6) Call NTCM_G to calculate VTEC at pierce point location and local time (LT)
 *      \n   (Section 2.5.4 in[1] provides a formula to convert UT to LT).
 *      \n 7) Calculate ionospheric Mapping Function (MF) using the equation provided in Section 2.5.8.
 *      \n 8) Convert VTEC to STEC using the ionospheric mapping function.
 *      \n 9) Convert STEC to time delay using Eq. 1 in[1] for the corresponding frequency to obtain
 *      \n the correction
 *      \n 10) Apply correction to selected link
 *
 *      @param[in] AzPara    => alpha0, alpha1, and alpha2 Iono Parameters broadcasted by Galileo
 *      @param[in] Doy    => Day of the year
 *      @param[in] UTC     => UTC Time in hours (0 for midnight)
 *      @param[in] llhRecDeg     => User position in Longitude[rad], Latitude[rad], height[m]
 *      @param[in] llhSatDeg    => Satellite position in Longitude[rad], Latitude[rad], height[m]
 *      @param[in] fcarr    => Carrier frequency
 *
 *      @param[out] vTEC    => Vertical TEC
 *      @param[out] sTEC    => Slant TEC
 *      @param[out] delay_m    => Signal propagation time delay in [m]
 *      @param[out] ErrorCode    => Error Code (optional)
 *
 *
 *    <strong> Tuning Constant </strong>
 *     <table>
 *    <tr><th> Label                      <th>Value
 *    <tr><td> ErrCodeEl <td> 2U
 *    <tr><td> ErrCodeEl    <td>2U
 *    <tr><td> ErrCodeIn    <td>1U
 *    <tr><td> ErrCodeOut   <td>3U
 *    <tr><td> IPP_RefAltitude <td> 450000.0
 *    <tr><td> LTd     <td>14.0
 *    <tr><td> NoErr     <td>0U
 *    <tr><td> Pf1     <td>0.4
 *    <tr><td> doy_DAV    <td>18.0
 *    <tr><td> doy_DSAV    <td>6.0
 *    <tr><td> lamGNP_deg   <td>-71.78
 *    <tr><td> phiGNP_deg   <td>79.74
 *    <tr><td> phi_c1    <td>16.0
 *    <tr><td> phi_c2    <td>-10.0
 *    <tr><td> sigma_c1    <td>12.0
 *    <tr><td> sigma_c2    <td>13.0
 *    <tr><td> k1     <td>0.92519
 *    <tr><td> k10     <td>1.20556
 *    <tr><td> k11     <td>1.41808
 *    <tr><td> k12     <td>0.13985
 *    <tr><td> k2     <td>0.16951
 *    <tr><td> k3     <td>0.00443
 *    <tr><td> k4     <td>0.06626
 *    <tr><td> k5     <td>0.00899
 *    <tr><td> k6     <td>0.21289
 *    <tr><td> k7     <td>-0.15414
 *    <tr><td> k8     <td>-0.38439
 *    <tr><td> k9     <td>1.14023
 *   </table>
 *   @ingroup NTCM_Procedure
 *   @file
 * ******************************************************************
 */
void NTCM_Procedure_Core(const real_T brdcIonoParam[3], real_T Doy, real_T UTC,
  const real_T llhRecDeg[3], const real_T llhSatDeg[3], real_T fcarr, real_T
  *vTEC, real_T *sTEC, real_T *delay_m, uint16_T *ErrorCode)
{
  real_T lOut_NTCM_Coeff_[12];
  real_T lOut_Physiscal_Const_[10];
  real_T lOut_llhRad[3];
  real_T lOut_llhRad_a[3];
  real_T lOut_losVec[3];
  real_T lOut_xyz[3];
  real_T lOut_xyz_i[3];
  real_T lOut_IppCoord_[2];
  real_T lOut_El_check_out_;
  real_T lOut_LT;
  real_T lOut_SvDoAAzRad_;
  real_T lOut_SvDoAEleRad_;
  lOut_llhRad_a[2] = llhRecDeg[2];
  lOut_llhRad_a[0] = 0.017453292519943295 * llhRecDeg[0];
  lOut_llhRad_a[1] = 0.017453292519943295 * llhRecDeg[1];

  /*
   * Block description for '<S4>/llh2xyz1':
   *     @ingroup NTCM_Procedure
   *
   *     <strong> FUNCTION </strong>
   *     This module converts the receiver geodetic WGS84 coordinates to ECEF (Described in Sec.2.5.1)
   *
   *     @param[in] llh 		=> WGS84 coordinates in Latitude[rad], Longitude[rad], height[m]
   *     @param[out] xyz 		=> ECEF coordinates in x[m], y[m], z[m]
   *
   *     @file
   */
  llh2xyz_Rec(lOut_llhRad_a, lOut_xyz_i);
  lOut_llhRad[2] = llhSatDeg[2];
  lOut_llhRad[0] = 0.017453292519943295 * llhSatDeg[0];
  lOut_llhRad[1] = 0.017453292519943295 * llhSatDeg[1];

  /*
   * Block description for '<S4>/llh2xyz2':
   *    @ingroup NTCM_Procedure
   *
   *     <strong> FUNCTION </strong>
   *     This module converts the SV geodetic WGS84 coordinates to ECEF (Described in Sec.2.5.1)
   *     @param[in] llh 		=> WGS84 coordinates in Latitude[rad], Longitude[rad], height[m]
   *     @param[out] xyz 		=> ECEF coordinates in x[m], y[m], z[m]
   *
   *     @file
   */
  llh2xyz_Sat(lOut_llhRad, lOut_xyz);
  lOut_losVec[0] = lOut_xyz[0] - lOut_xyz_i[0];
  lOut_losVec[1] = lOut_xyz[1] - lOut_xyz_i[1];
  lOut_losVec[2] = lOut_xyz[2] - lOut_xyz_i[2];

  /*
   * Block description for '<S4>/ComputeDoA':
   *     @ingroup NTCM_Procedure
   *     <strong> FUNCTION </strong>
   *
   *     This module computes the satellite azimuth and elevation angles (Described in Section 2.5.2)
   *
   *     @param[in] losVec 		=> Line of sight vector
   *     @param[in] llhUserRad 		=>  Receiver position Latitude [rad], Longitude[rad], height[m]
   *
   *     @param[out] SvDoAEleRad 		=>  Line of sight Elevation
   *     @param[out] SvDoAAzRad  		=>  Line of sight Azimuth
   *
   *  @file
   */
  ComputeDoA(lOut_losVec, lOut_llhRad_a, &lOut_SvDoAEleRad_, &lOut_SvDoAAzRad_);

  /*
   * Block description for '<S4>/ElevationCheck':
   *     <strong> FUNCTION </strong>
   *     Positivity elevetaion check
   *
   *     @param[in] El_check_in/t/t Satellite Elevation [rad]
   *
   *     @param[out] El_check_out/t/t Limited Elevation
   *     @param[out] El_ErrCOde /t/t Elevation out of range error code
   *
   *

   */
  ElevetionCheck(lOut_SvDoAEleRad_, &lOut_El_check_out_, ErrorCode);

  /*
   * Block description for '<S4>/getIonoPiercePoint':
   *        @ingroup NTCM_Procedure
   *
   *  <strong> FUNCTION </strong>
   *     This module computes the geographic latitude and longitude of the Ionospheric Pierce Point (IPP) (Described in Section 2.5.2).
   *     IPP is the point where the Line of Sight (LoS) intersects the reference ionospheric layer, this latter set at an altitude of 450Km.
   *
   *     @param[in] llhUser 		=> Receiver position in  Latitude[rad], Longitude[rad], height[m]
   *     @param[in] ElRad 		=> Elevation (DoA) [rad]
   *     @param[in] AzRad 		=> Azimuth (DoA) [rad]
   *
   *     @param [out] IppCoord 		=>Pierce Point coordinates in  Latitude[rad], Longitude[rad]
   *
   *  @file
   */
  getIonoPiercePoint(lOut_llhRad_a, lOut_El_check_out_, lOut_SvDoAAzRad_,
                     lOut_IppCoord_);

  /*
   * Block description for '<S4>/LocalTime':
   *     @ingroup NTCM_Procedure
   *     FUNCTION:
   *     ------
   *     This module computes the local time LT given the pierce point longitude and UT.
   *
   *     @param[in] IppPosRad 		=> Pierce Point Position in  Latitude[rad], Longitude[rad], height[m]
   *     @param[in] UTC		=> utc time
   *     @param[out] LT		=> Local Time
   *
   *  @file
   */
  lOut_LT = ComputeLocalTime(lOut_IppCoord_, UTC);

  /*
   * Block description for '<S4>/NTCM_Coeff_':
   *  Allocate NTCM Coeff as Input Vector
   *
   * Block description for '<S4>/NTCM_Coeff_':
   *  Allocate NTCM Coeff as Input Vector
   */
  lOut_NTCM_Coeff_[0] = k1;
  lOut_NTCM_Coeff_[1] = k2;
  lOut_NTCM_Coeff_[2] = k3;
  lOut_NTCM_Coeff_[3] = k4;
  lOut_NTCM_Coeff_[4] = k5;
  lOut_NTCM_Coeff_[5] = k6;
  lOut_NTCM_Coeff_[6] = k7;
  lOut_NTCM_Coeff_[7] = k8;
  lOut_NTCM_Coeff_[8] = k9;
  lOut_NTCM_Coeff_[9] = k10;
  lOut_NTCM_Coeff_[10] = k11;
  lOut_NTCM_Coeff_[11] = k12;

  /*
   * Block description for '<S4>/Physiscal_Const_':
   *  Allocate NTCM Physiscal Const as Input Vector
   *
   * Block description for '<S4>/Physiscal_Const_':
   *  Allocate NTCM Physiscal Const as Input Vector
   */
  lOut_Physiscal_Const_[0] = Pf1;
  lOut_Physiscal_Const_[1] = LTd;
  lOut_Physiscal_Const_[2] = doy_DAV;
  lOut_Physiscal_Const_[3] = doy_DSAV;
  lOut_Physiscal_Const_[4] = phi_c1;
  lOut_Physiscal_Const_[5] = phi_c2;
  lOut_Physiscal_Const_[6] = sigma_c1;
  lOut_Physiscal_Const_[7] = sigma_c2;
  lOut_Physiscal_Const_[8] = phiGNP_deg;
  lOut_Physiscal_Const_[9] = lamGNP_deg;

  /*
   * Block description for '<S4>/NTCM_G':
   *     <strong> FUNCTION </strong>
   *     The NTCM G is an empirical model that allows easy determination of global TEC.
   *     The model describes the main ionospheric features with good quality using a small
   *     number of model coefficients and parameters. The non-linear approach needs only 12
   *     coefficients and a few empirically fixed parameters for describing the broad spectrum
   *     of TEC variation for all levels of solar activity
   *
   *     @param[in] brdcIonoParam		=> ai0,ai1,ai2 Effective Ionisation Level parameters broadcast in the Galileo Navigation Message
   *     @param[in] doy 		=> Day of the Year
   *     @param[in] LT 		=> Local Time [hours]
   *     @param[in] IppCoord 		=> Pierce Point Coordinates in  Latitude[rad], Longitude[rad]
   *     @param[in] NTCM_Coeff_Vect 		=> NTCM model coefficients k1,k2,..,k12 (Described in Section 2.4.3)
   *     @param[in] NTCM_Phys_Cons 		=> Vector of Pysical Constant
   *
   *     @param[out] vTEC_0 		=> Total Electron Content [TECU]
   *
   *  @ingroup NTCM_Procedure
   *  @ingroup NTCM_G
   *  @file
   */
  NTCM_G(&brdcIonoParam[0], &Doy, &lOut_LT, &lOut_IppCoord_[0],
         &lOut_NTCM_Coeff_[0], &lOut_Physiscal_Const_[0], vTEC);

  /*
   * Block description for '<S4>/Compute_sTEC':
   *     @ingroup NTCM_Procedure
   *
   *     <strong> FUNCTION </strong>
   *     This module computes the Mapping Function and converts the estimated VTEC to STEC (Described in Section 2.5.8).
   *
   *     @param[in] ele_rad 		=> Satellite elevation
   *     @param[in] vTEC 		=>  Vertical Total Electron Content [TECU]
   *
   *     @param[out] sTEC 		=>  Slant Total Electron Content [TECU]
   *
   *  @file
   */
  *sTEC = Compute_sTEC(lOut_El_check_out_, *vTEC);

  /*
   * Block description for '<S4>/Compute_IonoDelay':
   *        @ingroup NTCM_Procedure
   *
   *     <strong> FUNCTION </strong>
   *     Compute ionospheric Delay
   *
   *     @param[in] sTEC		=> Slant TEC
   *     @param[in] fCarr		=> Carrier Frequency
   *     @param[out] delay_m		=>ionospheric Delay[m]
   *
   *  @file

   */
  *delay_m = Compute_IonoDelay(*sTEC, fcarr);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
