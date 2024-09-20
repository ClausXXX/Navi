/**
 * ******************************************************************
 * NTCMproj File: ComputeDoA.c
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

#include "ComputeDoA.h"

/* Include model header file for global data */
#include "NTCM_Procedure.h"
#include "NTCM_Procedure_private.h"

/**
 * ******************************************************************
 * ComputeDoA
 * Output and update for atomic system: '<S4>/ComputeDoA'
 * ******************************************************************
 * Arguments    : const real_T losVec[3]
 *                const real_T llhUserRad[3]
 *                real_T *SvDoAEleRad
 *                real_T *SvDoAAzRad
 * ******************************************************************
 * Return Type  : void
 * ******************************************************************
 * Block description for: '<S4>/ComputeDoA'
 *      @ingroup NTCM_Procedure
 *      <strong> FUNCTION </strong>
 *
 *      This module computes the satellite azimuth and elevation angles (Described in Section 2.5.2)
 *
 *      @param[in] losVec   => Line of sight vector
 *      @param[in] llhUserRad   =>  Receiver position Latitude [rad], Longitude[rad], height[m]
 *
 *      @param[out] SvDoAEleRad   =>  Line of sight Elevation
 *      @param[out] SvDoAAzRad    =>  Line of sight Azimuth
 *
 *   @file
 * ******************************************************************
 */
void ComputeDoA(const real_T losVec[3], const real_T llhUserRad[3], real_T
                *SvDoAEleRad, real_T *SvDoAAzRad)
{
  real_T sinphi_0[9];
  real_T dV_rot[3];
  real_T coslam;
  real_T cosphi;
  real_T sinlam;
  real_T sinphi;
  int32_T i;
  sinphi = sin(llhUserRad[0]);
  cosphi = cos(llhUserRad[0]);
  sinlam = sin(llhUserRad[1]);
  coslam = cos(llhUserRad[1]);
  sinphi_0[0] = -(sinphi * coslam);
  sinphi_0[3] = -(sinphi * sinlam);
  sinphi_0[6] = cosphi;
  sinphi_0[1] = -sinlam;
  sinphi_0[4] = coslam;
  sinphi_0[7] = 0.0;
  sinphi_0[2] = cosphi * coslam;
  sinphi_0[5] = cosphi * sinlam;
  sinphi_0[8] = sinphi;
  for (i = 0; i < 3; i++) {
    dV_rot[i] = (sinphi_0[i + 6] * losVec[2]) + ((sinphi_0[i + 3] * losVec[1]) +
      (sinphi_0[i] * losVec[0]));
  }

  sinphi = atan2(dV_rot[1], dV_rot[0]);
  if (sinphi <= 0.0) {
    *SvDoAAzRad = sinphi + 6.2831853071795862;
  } else {
    *SvDoAAzRad = sinphi;
  }

  *SvDoAEleRad = (0.5 - (atan2(sqrt((dV_rot[0] * dV_rot[0]) + (dV_rot[1] *
    dV_rot[1])), dV_rot[2]) / 3.1415926535897931)) * 3.1415926535897931;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
