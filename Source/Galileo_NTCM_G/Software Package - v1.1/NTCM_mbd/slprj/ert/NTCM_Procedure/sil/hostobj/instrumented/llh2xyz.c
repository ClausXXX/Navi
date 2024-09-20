/**
 * ******************************************************************
 * NTCMproj File: llh2xyz.c
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

#include "llh2xyz.h"

/* Include model header file for global data */
#include "NTCM_Procedure.h"
#include "NTCM_Procedure_private.h"

/**
 * ******************************************************************
 * llh2xyz
 * Output and update for atomic system:
 *    '<S30>/llh2xyz'
 *    '<S31>/llh2xyz'
 * ******************************************************************
 * Arguments    : const real_T llh[3]
 *                real_T xyz[3]
 * ******************************************************************
 * Return Type  : void
 * ******************************************************************
 * Common block description:
 *      <strong> FUNCTION </strong>
 *      This module converts the geodetic WGS84 coordinates to ECEF (Described in Sec.2.5.1)
 *
 *      @param[in] llh => WGS84 coordinates in Latitude[rad], Longitude[rad], height[m]
 *
 *      @param[out] xyz => ECEF coordinates in x[m], y[m], z[m]
 *
 *      <strong>  Constant </strong>
 *      <table>
 *      <tr><th> Label                      <th>Value <th>Description
 *      <tr><td> a    <td>6378137.0   <td>Semi-
 *      <tr><td> b    <td>6356752.3142  <td>Se
 *      <tr><td> e2   <td>0.006694380004261 <td>Eccentricity (e) of the ellipsoid squared (e2=1-b^2/a^2)
 *
 *   @file
 * ******************************************************************
 */
void llh2xyz(const real_T llh[3], real_T xyz[3])
{
  real_T a_tmp;
  real_T tmp;
  real_T v;
  profileStart_NTCM_Procedure(29U); /* original_line:71 */a_tmp = sin(llh[0]);profileEnd_NTCM_Procedure(29U); /* original_line:71 */
  profileStart_NTCM_Procedure(30U); /* original_line:72 */v = 6.378137E+6 / sqrt(1.0 - ((a_tmp * a_tmp) * 0.0066943799901413165));profileEnd_NTCM_Procedure(30U); /* original_line:72 */
  profileStart_NTCM_Procedure(31U); /* original_line:73 */tmp = (v + llh[2]) * cos(llh[0]);profileEnd_NTCM_Procedure(31U); /* original_line:73 */
  profileStart_NTCM_Procedure(32U); /* original_line:74 */xyz[0] = tmp * cos(llh[1]);profileEnd_NTCM_Procedure(32U); /* original_line:74 */
  profileStart_NTCM_Procedure(33U); /* original_line:75 */xyz[1] = tmp * sin(llh[1]);profileEnd_NTCM_Procedure(33U); /* original_line:75 */
  xyz[2] = ((v * 0.99330562000985867) + llh[2]) * a_tmp;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */