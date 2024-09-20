/**
 * ******************************************************************
 * NTCMproj File: rt_roundd.c
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
 * C/C++ source code generated on : Tue Apr 26 10:03:24 2022
 *
 * @file
 */

#include "rtwtypes.h"
#include <math.h>
#include "rt_roundd.h"

real_T rt_roundd(real_T u_0)
{
  real_T y_0;
  if (fabs(u_0) < 4.503599627370496E+15) {
    if (u_0 >= 0.5) {
      y_0 = floor(u_0 + 0.5);
    } else if (u_0 > -0.5) {
      y_0 = 0.0;
    } else {
      y_0 = ceil(u_0 - 0.5);
    }
  } else {
    y_0 = u_0;
  }

  return y_0;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
