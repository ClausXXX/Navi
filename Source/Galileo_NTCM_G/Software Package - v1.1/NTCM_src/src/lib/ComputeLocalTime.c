/**
 * ******************************************************************
 * NTCMproj File: ComputeLocalTime.c
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

#include "ComputeLocalTime.h"

/* Include model header file for global data */
#include "NTCM_Procedure.h"
#include "NTCM_Procedure_private.h"

/**
 * ******************************************************************
 * ComputeLocalTime
 * Output and update for atomic system: '<S4>/LocalTime'
 * ******************************************************************
 * Arguments    : const real_T IppPosRad[2]
 *                real_T UTC
 * ******************************************************************
 * Return Type  : real_T
 * ******************************************************************
 * Block description for: '<S4>/LocalTime'
 *      @ingroup NTCM_Procedure
 *      FUNCTION:
 *      ------
 *      This module computes the local time LT given the pierce point longitude and UT.
 *
 *      @param[in] IppPosRad   => Pierce Point Position in  Latitude[rad], Longitude[rad], height[m]
 *      @param[in] UTC  => utc time
 *      @param[out] LT  => Local Time
 *
 *   @file
 * ******************************************************************
 */
real_T ComputeLocalTime(const real_T IppPosRad[2], real_T UTC)
{
  return UTC - (-(((IppPosRad[1] * 180.0) / 3.1415926535897931) / 15.0));
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
