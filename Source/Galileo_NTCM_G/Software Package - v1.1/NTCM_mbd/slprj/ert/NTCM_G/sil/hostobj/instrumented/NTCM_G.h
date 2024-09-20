/**
 * ******************************************************************
 * NTCMproj File: NTCM_G.h
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

#ifndef RTW_HEADER_NTCM_G_h_
#define RTW_HEADER_NTCM_G_h_
#include <math.h>
#ifndef NTCM_G_COMMON_INCLUDES_
#define NTCM_G_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* NTCM_G_COMMON_INCLUDES_ */

#include "NTCM_G_types.h"



/* Declare global externs for instrumentation */
extern void profileStart_NTCM_G(uint32_T);
extern void profileEnd_NTCM_G(uint32_T);
/* Code_Instrumentation_Declarations_Placeholder */
extern real_T Compute_AzPar_Eq_2(const real_T brdcIonoParam[3]);
extern void Compute_Eq_29_30_31(real_T Delta, const real_T IppPosRad[2], const
  real_T Phys_Const[10], real_T *LatmRad, real_T *SolzenithTerm1, real_T
  *SolzenithTerm2);
extern void Compute_Eq_5_6_7_9_10_13_14(const real_T *doy, const real_T *LT,
  real_T latm_rad, const real_T Phys_Const[10], real_T *Va, real_T *Vsa, real_T *
  cosVd, real_T *cosVsd, real_T *sinVsd, real_T *cosVtd, real_T *sinVtd, real_T *
  exp_EC1, real_T *exp_EC2);
extern void Compute_F_factors(real_T azPar, real_T LatmRad, real_T
  SolzenithTerm1, real_T SolzenithTerm2, real_T Va, real_T Vsa, real_T cosVd,
  real_T cosVsd, real_T sinVsd, real_T cosVtd, real_T sinVtd, real_T exp_EC1,
  real_T exp_EC2, const real_T NTCM_Coeff_Vect[12], real_T *F1, real_T *F2,
  real_T *F3, real_T *F4, real_T *F5);
extern real_T Sun_declination_Eq_28(const real_T *doy);
extern void NTCM_G(const real_T brdcIonoParam[3], const real_T *doy, const
                   real_T *LT, const real_T IppCoord[2], const real_T
                   NTCM_Coeff_Vect[12], const real_T NTCM_Phys_Cons[10], real_T *
                   vTEC);

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'NTCM_G'
 * '<S1>'   : 'NTCM_G/Compute_AzPar_Eq_2'
 * '<S2>'   : 'NTCM_G/Compute_Eq_29_30_31'
 * '<S3>'   : 'NTCM_G/Compute_Eq_5_6_7_9_10_13_14'
 * '<S4>'   : 'NTCM_G/Compute_F_factors'
 * '<S5>'   : 'NTCM_G/Sun_declination_Eq_28'
 * '<S6>'   : 'NTCM_G/Compute_AzPar_Eq_2/Compute_AzPar_Eq_2'
 * '<S7>'   : 'NTCM_G/Compute_Eq_29_30_31/Compute_Eq_29_30_31'
 * '<S8>'   : 'NTCM_G/Compute_Eq_5_6_7_9_10_13_14/Compute_Eq_5_6_7_9_10_13_14'
 * '<S9>'   : 'NTCM_G/Compute_F_factors/Compute_F_factors'
 * '<S10>'  : 'NTCM_G/Sun_declination_Eq_28/Sun_declination_Eq_28'
 */
#endif                                 /* RTW_HEADER_NTCM_G_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
