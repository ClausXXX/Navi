/**
 * ******************************************************************
 * NTCMproj File: getIonoPiercePoint.c
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

#include "getIonoPiercePoint.h"

/* Include model header file for global data */
#include "NTCM_Procedure.h"
#include "NTCM_Procedure_private.h"

/**
 * ******************************************************************
 * getIonoPiercePoint
 * Output and update for atomic system: '<S4>/getIonoPiercePoint'
 * ******************************************************************
 * Arguments    : const real_T llhUserRad[3]
 *                real_T ElRad
 *                real_T AzRad
 *                real_T IppCoord[2]
 * ******************************************************************
 * Return Type  : void
 * ******************************************************************
 * Block description for: '<S4>/getIonoPiercePoint'
 *         @ingroup NTCM_Procedure
 *
 *   <strong> FUNCTION </strong>
 *      This module computes the geographic latitude and longitude of the Ionospheric Pierce Point (IPP) (Described in Section 2.5.2).
 *      IPP is the point where the Line of Sight (LoS) intersects the reference ionospheric layer, this latter set at an altitude of 450Km.
 *
 *      @param[in] llhUser   => Receiver position in  Latitude[rad], Longitude[rad], height[m]
 *      @param[in] ElRad   => Elevation (DoA) [rad]
 *      @param[in] AzRad   => Azimuth (DoA) [rad]
 *
 *      @param [out] IppCoord   =>Pierce Point coordinates in  Latitude[rad], Longitude[rad]
 *
 *   @file
 * ******************************************************************
 */
void getIonoPiercePoint(const real_T llhUserRad[3], real_T ElRad, real_T AzRad,
  real_T IppCoord[2])
{
  real_T Psi_pp;
  real_T sinPsi_pp;
  Psi_pp = (1.5707963267948966 - ElRad) - asin((Re / (IPP_RefAltitude + Re)) *
    cos(ElRad));
  sinPsi_pp = sin(Psi_pp);
  Psi_pp = asin(((cos(llhUserRad[0]) * sinPsi_pp) * cos(AzRad)) + (sin
    (llhUserRad[0]) * cos(Psi_pp)));
  IppCoord[0] = Psi_pp;
  IppCoord[1] = asin((sinPsi_pp * sin(AzRad)) / cos(Psi_pp)) + llhUserRad[1];
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
