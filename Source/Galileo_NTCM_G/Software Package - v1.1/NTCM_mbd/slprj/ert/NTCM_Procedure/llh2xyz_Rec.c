/**
 * ******************************************************************
 * NTCMproj File: llh2xyz_Rec.c
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

#include "llh2xyz_Rec.h"

/* Include model header file for global data */
#include "NTCM_Procedure.h"
#include "NTCM_Procedure_private.h"

/**
 * ******************************************************************
 * llh2xyz_Rec
 * Output and update for atomic system: '<S4>/llh2xyz1'
 * ******************************************************************
 * Arguments    : const real_T llh[3]
 *                real_T xyz[3]
 * ******************************************************************
 * Return Type  : void
 * ******************************************************************
 * Block description for: '<S4>/llh2xyz1'
 *      @ingroup NTCM_Procedure
 *
 *      <strong> FUNCTION </strong>
 *      This module converts the receiver geodetic WGS84 coordinates to ECEF (Described in Sec.2.5.1)
 *
 *      @param[in] llh   => WGS84 coordinates in Latitude[rad], Longitude[rad], height[m]
 *      @param[out] xyz   => ECEF coordinates in x[m], y[m], z[m]
 *
 *      @file
 * ******************************************************************
 */
void llh2xyz_Rec(const real_T llh[3], real_T xyz[3])
{
  /*
   * Block description for '<S30>/llh2xyz':
   *     <strong> FUNCTION </strong>
   *     This module converts the geodetic WGS84 coordinates to ECEF (Described in Sec.2.5.1)
   *
   *     @param[in] llh => WGS84 coordinates in Latitude[rad], Longitude[rad], height[m]
   *
   *     @param[out] xyz => ECEF coordinates in x[m], y[m], z[m]
   *
   *     <strong>  Constant </strong>
   *     <table>
   *     <tr><th> Label                      <th>Value <th>Description
   *     <tr><td> a    <td>6378137.0			<td>Semi-
   *     <tr><td> b    <td>6356752.3142		<td>Se
   *     <tr><td> e2   <td>0.006694380004261	<td>Eccentricity (e) of the ellipsoid squared (e2=1-b^2/a^2)
   *
   *  @file
   */
  llh2xyz(llh, xyz);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
