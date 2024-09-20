/**
 * ******************************************************************
 * NTCMproj File: NTCM_G.c
 * ******************************************************************
 *  @author      Francesco Menzione
 *  @reviewer    Matteo Sgammini
 *  @copyright   Joint Research Centre (JRC), 2022
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 *  @ingroup NTCM_G
 * ******************************************************************
 * Code generated for Simulink model 'NTCM_G'.
 * Model version                  : 1.0
 * Simulink Coder version         : 9.5 (R2021a) 14-Nov-2020
 * C/C++ source code generated on : Tue Apr 26 10:03:01 2022
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

#include "NTCM_G.h"
#include "NTCM_G_private.h"

/**
 * ******************************************************************
 * Compute_AzPar_Eq_2
 * Output and update for atomic system: '<Root>/Compute_AzPar_Eq_2'
 * ******************************************************************
 * Arguments    : const real_T brdcIonoParam[3]
 * ******************************************************************
 * Return Type  : real_T
 * ******************************************************************
 * Block description for: '<Root>/Compute_AzPar_Eq_2'
 *      <strong> FUNCTION </strong>
 *
 *      Computation of the proxy measure of the solar activity level (azPar) from the broadcast Ionisation level  parameters ai0,ai1,ai2 (Eq.2)
 *
 *      @param[in] brdcIonoParam  => ai0,ai1,ai2 Effective Ionisation Level parameters broadcast in the Galileo Navigation Message
 *
 *      @param[out] azPar   => Proxy measure of the solar activity level
 *
 *   @ingroup NTCM_G
 * ******************************************************************
 */
real_T Compute_AzPar_Eq_2(const real_T brdcIonoParam[3])
{
  real_T azPar_0;
  azPar_0 = sqrt(((((brdcIonoParam[1] * brdcIonoParam[1]) * 1633.33) +
                   (brdcIonoParam[0] * brdcIonoParam[0])) + ((brdcIonoParam[2] *
    brdcIonoParam[2]) * 4.802E+6)) + ((3266.67 * brdcIonoParam[0]) *
    brdcIonoParam[2]));
  return azPar_0;
}

/**
 * ******************************************************************
 * Compute_Eq_29_30_31
 * Output and update for atomic system: '<Root>/Compute_Eq_29_30_31'
 * ******************************************************************
 * Arguments    : real_T Delta
 *                const real_T IppPosRad[2]
 *                const real_T Phys_Const[10]
 *                real_T *LatmRad
 *                real_T *SolzenithTerm1
 *                real_T *SolzenithTerm2
 * ******************************************************************
 * Return Type  : void
 * ******************************************************************
 * Block description for: '<Root>/Compute_Eq_29_30_31'
 *
 *      <strong> FUNCTION </strong>
 *
 *      This module computes the dependency of TEC with the solar zenith angle and geomagnetic latitude , see Sec.2.5.6 and 2.5.7 Eq.30,31,32
 *
 *      @param[in] Delta=> Sun declination Angle [rad]
 *      @param[in] IppCoord =>Pierce Point Coordinates in  Latitude[rad], Longitude[rad]
 *      @param[in] Phys_Const ==> Vector of Pysical Constant see [2]
 *
 *      @param[out] LatmRad==> geomagnetic latitude
 *      @param[out] SolzenithTerm1==> 1st solar zenith angle
 *      @param[out] SolzenithTerm2==> 2nd solar zenith angle
 *
 *      @ingroup NTCM_G
 * ******************************************************************
 */
void Compute_Eq_29_30_31(real_T Delta, const real_T IppPosRad[2], const real_T
  Phys_Const[10], real_T *LatmRad, real_T *SolzenithTerm1, real_T
  *SolzenithTerm2)
{
  real_T cos_lat_delta;
  real_T phiGNP;
  cos_lat_delta = cos(IppPosRad[0] - Delta);
  phiGNP = 0.017453292519943295 * Phys_Const[8];
  *SolzenithTerm1 = cos_lat_delta + Phys_Const[0];
  *SolzenithTerm2 = cos_lat_delta - ((IppPosRad[0] / 1.5707963267948966) * sin
    (Delta));
  *LatmRad = (cos(IppPosRad[1] - (0.017453292519943295 * Phys_Const[9])) * (cos
    (IppPosRad[0]) * cos(phiGNP))) + (sin(IppPosRad[0]) * sin(phiGNP));
  *LatmRad = asin(*LatmRad);
}

/**
 * ******************************************************************
 * Compute_Eq_5_6_7_9_10_13_14
 * Output and update for atomic system: '<Root>/Compute_Eq_5_6_7_9_10_13_14'
 * ******************************************************************
 * Arguments    : const real_T *doy
 *                const real_T *LT
 *                real_T latm_rad
 *                const real_T Phys_Const[10]
 *                real_T *Va
 *                real_T *Vsa
 *                real_T *cosVd
 *                real_T *cosVsd
 *                real_T *sinVsd
 *                real_T *cosVtd
 *                real_T *sinVtd
 *                real_T *exp_EC1
 *                real_T *exp_EC2
 * ******************************************************************
 * Return Type  : void
 * ******************************************************************
 * Block description for: '<Root>/Compute_Eq_5_6_7_9_10_13_14'
 *      <strong> FUNCTION </strong>
 *
 *      This module computes Local time dependency and Seasonal dependency , see Sec.2.4.1 in [1] : Eq.5,6,7,9,10,13,14
 *
 *      @param[in] Doy => Day of year
 *      @param[in] LT  => Local Time [h]
 *      @param[in] latm_rad => geomagnetic latitude [rad]
 *
 *      @param[out] Va , Vsa => annual  and semiannual component
 *      @param[out] cosVd, cosVsd, sinVsd, cosVtd, sinVtd, => diurnal, semidiurnal and ter-diurnal  components
 *      @param[out] exp_EC1 => ionisation crests 1st component
 *      @param[out] exp_EC2 => ionisation crests 2nd component
 *
 *   @ingroup NTCM_G
 * ******************************************************************
 */
void Compute_Eq_5_6_7_9_10_13_14(const real_T *doy, const real_T *LT, real_T
  latm_rad, const real_T Phys_Const[10], real_T *Va, real_T *Vsa, real_T *cosVd,
  real_T *cosVsd, real_T *sinVsd, real_T *cosVtd, real_T *sinVtd, real_T
  *exp_EC1, real_T *exp_EC2)
{
  real_T Vsd;
  real_T Vtd;
  Vsd = (6.2831853071795862 * (*LT)) / 12.0;
  Vtd = (6.2831853071795862 * (*LT)) / 8.0;
  *Va = (((*doy) - Phys_Const[2]) * 6.2831853071795862) / 365.25;
  *Vsa = (((*doy) - Phys_Const[3]) * 12.566370614359172) / 365.25;
  *cosVd = (((*LT) - Phys_Const[1]) * 6.2831853071795862) / 24.0;
  *cosVd = cos(*cosVd);
  *cosVsd = Vsd;
  *cosVsd = cos(*cosVsd);
  *sinVsd = Vsd;
  *sinVsd = sin(*sinVsd);
  *cosVtd = Vtd;
  *cosVtd = cos(*cosVtd);
  *sinVtd = Vtd;
  *sinVtd = sin(*sinVtd);
  Vsd = 57.295779513082323 * latm_rad;
  Vtd = Vsd - Phys_Const[4];
  Vsd -= Phys_Const[5];
  *exp_EC1 = ((-(Vtd * Vtd)) / 2.0) / (Phys_Const[6] * Phys_Const[6]);
  *exp_EC1 = exp(*exp_EC1);
  *exp_EC2 = ((-(Vsd * Vsd)) / 2.0) / (Phys_Const[7] * Phys_Const[7]);
  *exp_EC2 = exp(*exp_EC2);
}

/**
 * ******************************************************************
 * Compute_F_factors
 * Output and update for atomic system: '<Root>/Compute_F_factors'
 * ******************************************************************
 * Arguments    : real_T azPar
 *                real_T LatmRad
 *                real_T SolzenithTerm1
 *                real_T SolzenithTerm2
 *                real_T Va
 *                real_T Vsa
 *                real_T cosVd
 *                real_T cosVsd
 *                real_T sinVsd
 *                real_T cosVtd
 *                real_T sinVtd
 *                real_T exp_EC1
 *                real_T exp_EC2
 *                const real_T NTCM_Coeff_Vect[12]
 *                real_T *F1
 *                real_T *F2
 *                real_T *F3
 *                real_T *F4
 *                real_T *F5
 * ******************************************************************
 * Return Type  : void
 * ******************************************************************
 * Block description for: '<Root>/Compute_F_factors'
 *      <strong> FUNCTION </strong>
 *
 *      Compute all dependency contributions ( F factors) and their combination, see Sec.2.4.1
 *
 *      @param[in] azPar => Proxy measure of the solar activity level
 *      @param[in] LatmRad => Geomagnetic latitude [rad]
 *      @param[in] SolzenithTerm1 => 1st solar zenith angle
 *      @param[in] SolzenithTerm2 => 2nd solar zenith angle
 *      @param[in] Va , Vsa => Annual  and semiannual component
 *      @param[in] cosVd, cosVsd, sinVsd, cosVtd, sinVtd, => diurnal, semidiurnal and ter-diurnal  components
 *      @param[in] exp_EC1 => Ionisation crests 1st component
 *      @param[in] exp_EC2 => Ionisation crests 2nd component
 *      @param[in] NTCM_Coeff_Vect ==> NTCM model coefficients k1,k2,..,k12 (Described in Section 2.4.3)
 *
 *      @param[out] F1,F2,F3,F4,F5 => Model dependency factors
 *
 *      @ingroup NTCM_G
 * ******************************************************************
 */
void Compute_F_factors(real_T azPar, real_T LatmRad, real_T SolzenithTerm1,
  real_T SolzenithTerm2, real_T Va, real_T Vsa, real_T cosVd, real_T cosVsd,
  real_T sinVsd, real_T cosVtd, real_T sinVtd, real_T exp_EC1, real_T exp_EC2,
  const real_T NTCM_Coeff_Vect[12], real_T *F1, real_T *F2, real_T *F3, real_T
  *F4, real_T *F5)
{
  *F1 = ((((((NTCM_Coeff_Vect[0] * cosVd) + (NTCM_Coeff_Vect[1] * cosVsd)) +
            (NTCM_Coeff_Vect[2] * sinVsd)) + (NTCM_Coeff_Vect[3] * cosVtd)) +
          (NTCM_Coeff_Vect[4] * sinVtd)) * SolzenithTerm2) + SolzenithTerm1;
  *F2 = ((NTCM_Coeff_Vect[5] * cos(Va)) + 1.0) + (NTCM_Coeff_Vect[6] * cos(Vsa));
  *F3 = (NTCM_Coeff_Vect[7] * cos(LatmRad)) + 1.0;
  *F4 = ((NTCM_Coeff_Vect[8] * exp_EC1) + 1.0) + (NTCM_Coeff_Vect[9] * exp_EC2);
  *F5 = (NTCM_Coeff_Vect[11] * azPar) + NTCM_Coeff_Vect[10];
}

/**
 * ******************************************************************
 * Sun_declination_Eq_28
 * Output and update for atomic system: '<Root>/Sun_declination_Eq_28'
 * ******************************************************************
 * Arguments    : const real_T *doy
 * ******************************************************************
 * Return Type  : real_T
 * ******************************************************************
 * Block description for: '<Root>/Sun_declination_Eq_28'
 *      <strong> FUNCTION </strong>
 *
 *      This module computes the Sun’s declination for the applicable day (Described in 2.5.5)
 *
 *      @param[in] doy   => Day of the Year
 *
 *      @param[out] delta_0   => Sun's declination [radians]
 *
 *   @ingroup NTCM_G
 * ******************************************************************
 */
real_T Sun_declination_Eq_28(const real_T *doy)
{
  real_T delta_0;
  delta_0 = ((sin(((((*doy) - 80.7) * 0.9856) * 3.1415926535897931) / 180.0) *
              23.44) * 3.1415926535897931) / 180.0;
  return delta_0;
}

/**
 * ******************************************************************
 * NTCM_G
 * Output and update for referenced model: 'NTCM_G'
 * ******************************************************************
 * Arguments    : const real_T brdcIonoParam[3]
 *                const real_T *doy
 *                const real_T *LT
 *                const real_T IppCoord[2]
 *                const real_T NTCM_Coeff_Vect[12]
 *                const real_T NTCM_Phys_Cons[10]
 *                real_T *vTEC
 * ******************************************************************
 * Return Type  : void
 * ******************************************************************
 *
 * ******************************************************************
 */
void NTCM_G(const real_T brdcIonoParam[3], const real_T *doy, const real_T *LT,
            const real_T IppCoord[2], const real_T NTCM_Coeff_Vect[12], const
            real_T NTCM_Phys_Cons[10], real_T *vTEC)
{
  real_T lOut_F1;
  real_T lOut_F2;
  real_T lOut_F3;
  real_T lOut_F4;
  real_T lOut_F5;
  real_T lOut_LatmRad;
  real_T lOut_SolzenithTerm1;
  real_T lOut_SolzenithTerm2;
  real_T lOut_Va;
  real_T lOut_Vsa;
  real_T lOut_azPar;
  real_T lOut_cosVd;
  real_T lOut_cosVsd;
  real_T lOut_cosVtd;
  real_T lOut_delta;
  real_T lOut_exp_EC1;
  real_T lOut_exp_EC2;
  real_T lOut_sinVsd;
  real_T lOut_sinVtd;

  /*
   * Block description for '<Root>/Compute_AzPar_Eq_2':
   *     <strong> FUNCTION </strong>
   *
   *     Computation of the proxy measure of the solar activity level (azPar) from the broadcast Ionisation level  parameters ai0,ai1,ai2 (Eq.2)
   *
   *     @param[in] brdcIonoParam		=> ai0,ai1,ai2 Effective Ionisation Level parameters broadcast in the Galileo Navigation Message
   *
   *     @param[out] azPar 		=> Proxy measure of the solar activity level
   *
   *  @ingroup NTCM_G

   */
  lOut_azPar = Compute_AzPar_Eq_2(brdcIonoParam);

  /*
   * Block description for '<Root>/Sun_declination_Eq_28':
   *     <strong> FUNCTION </strong>
   *
   *     This module computes the Sun’s declination for the applicable day (Described in 2.5.5)
   *
   *     @param[in] doy 		=> Day of the Year
   *
   *     @param[out] delta_0 		=> Sun's declination [radians]
   *
   *  @ingroup NTCM_G
   *

   */
  lOut_delta = Sun_declination_Eq_28(doy);

  /*
   * Block description for '<Root>/Compute_Eq_29_30_31':
   *
   *     <strong> FUNCTION </strong>
   *
   *     This module computes the dependency of TEC with the solar zenith angle and geomagnetic latitude , see Sec.2.5.6 and 2.5.7 Eq.30,31,32
   *
   *     @param[in] Delta=> Sun declination Angle [rad]
   *     @param[in] IppCoord =>Pierce Point Coordinates in  Latitude[rad], Longitude[rad]
   *     @param[in] Phys_Const ==> Vector of Pysical Constant see [2]
   *
   *     @param[out] LatmRad==> geomagnetic latitude
   *     @param[out] SolzenithTerm1==> 1st solar zenith angle
   *     @param[out] SolzenithTerm2==> 2nd solar zenith angle
   *
   *     @ingroup NTCM_G
   */
  Compute_Eq_29_30_31(lOut_delta, IppCoord, NTCM_Phys_Cons, &lOut_LatmRad,
                      &lOut_SolzenithTerm1, &lOut_SolzenithTerm2);

  /*
   * Block description for '<Root>/Compute_Eq_5_6_7_9_10_13_14':
   *     <strong> FUNCTION </strong>
   *
   *     This module computes Local time dependency and Seasonal dependency , see Sec.2.4.1 in [1] : Eq.5,6,7,9,10,13,14
   *
   *     @param[in] Doy => Day of year
   *     @param[in] LT  => Local Time [h]
   *     @param[in] latm_rad => geomagnetic latitude [rad]
   *
   *     @param[out] Va , Vsa => annual  and semiannual component
   *     @param[out] cosVd, cosVsd, sinVsd, cosVtd, sinVtd, => diurnal, semidiurnal and ter-diurnal  components
   *     @param[out] exp_EC1 => ionisation crests 1st component
   *     @param[out] exp_EC2 => ionisation crests 2nd component
   *
   *  @ingroup NTCM_G

   */
  Compute_Eq_5_6_7_9_10_13_14(doy, LT, lOut_LatmRad, NTCM_Phys_Cons, &lOut_Va,
    &lOut_Vsa, &lOut_cosVd, &lOut_cosVsd, &lOut_sinVsd, &lOut_cosVtd,
    &lOut_sinVtd, &lOut_exp_EC1, &lOut_exp_EC2);

  /*
   * Block description for '<Root>/Compute_F_factors':
   *     <strong> FUNCTION </strong>
   *
   *     Compute all dependency contributions ( F factors) and their combination, see Sec.2.4.1
   *
   *     @param[in] azPar => Proxy measure of the solar activity level
   *     @param[in] LatmRad => Geomagnetic latitude [rad]
   *     @param[in] SolzenithTerm1 => 1st solar zenith angle
   *     @param[in] SolzenithTerm2 => 2nd solar zenith angle
   *     @param[in] Va , Vsa => Annual  and semiannual component
   *     @param[in] cosVd, cosVsd, sinVsd, cosVtd, sinVtd, => diurnal, semidiurnal and ter-diurnal  components
   *     @param[in] exp_EC1 => Ionisation crests 1st component
   *     @param[in] exp_EC2 => Ionisation crests 2nd component
   *     @param[in] NTCM_Coeff_Vect ==> NTCM model coefficients k1,k2,..,k12 (Described in Section 2.4.3)
   *
   *     @param[out] F1,F2,F3,F4,F5 => Model dependency factors
   *
   *     @ingroup NTCM_G

   */
  Compute_F_factors(lOut_azPar, lOut_LatmRad, lOut_SolzenithTerm1,
                    lOut_SolzenithTerm2, lOut_Va, lOut_Vsa, lOut_cosVd,
                    lOut_cosVsd, lOut_sinVsd, lOut_cosVtd, lOut_sinVtd,
                    lOut_exp_EC1, lOut_exp_EC2, NTCM_Coeff_Vect, &lOut_F1,
                    &lOut_F2, &lOut_F3, &lOut_F4, &lOut_F5);
  *vTEC = (((lOut_F1 * lOut_F2) * lOut_F3) * lOut_F4) * lOut_F5;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
