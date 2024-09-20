/**
 * ******************************************************************
 * NTCMproj File: Check.c
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

#include "rtwtypes.h"
#include "NTCM_Procedure_types.h"

/* Exported data definition */

/* Definition for custom storage class: ExportToFile */
#if !CheckFlag

uint16_T ErrVarCheckIn[12] = { 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U } ;

#endif

#if !CheckFlag

uint16_T ErrVarCheckOut[3] = { 0U, 0U, 0U } ;

#endif

#if !CheckFlag

uint16_T ErrVarEl = 0U;

#endif

#if CheckFlag

real_T RangeLwEl = 0.0;

#endif

#if CheckFlag

real_T RangeLwIn[12] = { 0.0, -4.0, -0.25, 1.0, 0.0, -90.0, -180.0, -4000.0,
  -90.0, -180.0, 450000.0, 0.0 } ;

#endif

#if CheckFlag

real_T RangeLwOut[3] = { 0.0, 0.0, 0.0 } ;

#endif

#if CheckFlag

real_T RangeUpEl = 1.5707963267948966;

#endif

#if CheckFlag

real_T RangeUpIn[12] = { 512.0, 4.0, 0.25, 366.0, 24.0, 90.0, 180.0, 400000.0,
  90.0, 180.0, 6.0E+8, 2.0E+10 } ;

#endif

#if CheckFlag

real_T RangeUpOut[3] = { 500.0, 1500.0, 1.0E+12 } ;

#endif

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
