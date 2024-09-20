/**
 * ******************************************************************
 * NTCMproj File: set_NTCM_G.c
 * ******************************************************************
 *  @author      Francesco Menzione
 *  @reviewer    Matteo Sgammini
 *  @copyright   Joint Research Centre (JRC), 2022
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), Version 1.2 or later
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 *  @ingroup NTCM_G
 * ******************************************************************
 * Code generated for Simulink model 'NTCM_Procedure'.
 * Model version                  : 1.0
 * Simulink Coder version         : 9.5 (R2021a) 14-Nov-2020
 * C/C++ source code generated on : Tue Apr 26 10:11:39 2022
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

#include "set_NTCM_G.h"

/* Include model header file for global data */
#include "PhysicalConst.h"
#include "NTCM_G_Coeff.h"


void set_NTCM_G(real_T NTCM_Coeff_[12], real_T Physiscal_Const_[10])
{
/*
   * Block description for '<S4>/NTCM_Coeff_':
   *  Allocate NTCM Coeff as Input Vector
   *
   * Block description for '<S4>/NTCM_Coeff_':
   *  Allocate NTCM Coeff as Input Vector
   */
  NTCM_Coeff_[0] = k1;
  NTCM_Coeff_[1] = k2;
  NTCM_Coeff_[2] = k3;
  NTCM_Coeff_[3] = k4;
  NTCM_Coeff_[4] = k5;
  NTCM_Coeff_[5] = k6;
  NTCM_Coeff_[6] = k7;
  NTCM_Coeff_[7] = k8;
  NTCM_Coeff_[8] = k9;
  NTCM_Coeff_[9] = k10;
  NTCM_Coeff_[10] = k11;
  NTCM_Coeff_[11] = k12;

  /*
   * Block description for '<S4>/Physiscal_Const_':
   *  Allocate NTCM Physiscal Const as Input Vector
   *
   * Block description for '<S4>/Physiscal_Const_':
   *  Allocate NTCM Physiscal Const as Input Vector
   */
  Physiscal_Const_[0] = Pf1;
  Physiscal_Const_[1] = LTd;
  Physiscal_Const_[2] = doy_DAV;
  Physiscal_Const_[3] = doy_DSAV;
  Physiscal_Const_[4] = phi_c1;
  Physiscal_Const_[5] = phi_c2;
  Physiscal_Const_[6] = sigma_c1;
  Physiscal_Const_[7] = sigma_c2;
  Physiscal_Const_[8] = phiGNP_deg;
  Physiscal_Const_[9] = lamGNP_deg;

}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
