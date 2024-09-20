/**
 * ******************************************************************
 * NTCMproj File: NTCM_Procedure.h
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

#ifndef RTW_HEADER_NTCM_Procedure_h_
#define RTW_HEADER_NTCM_Procedure_h_
#ifndef NTCM_Procedure_COMMON_INCLUDES_
#define NTCM_Procedure_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* NTCM_Procedure_COMMON_INCLUDES_ */

#include "NTCM_Procedure_types.h"

/* Child system includes */
#include "CheckOutput.h"
#include "ChekInput.h"
#include "ErrorMnG.h"
#include "NTCM_Procedure_Core.h"

/* Includes for objects with custom storage classes. */
#if CheckFlag
#include "ErrorCode.h"
#endif

#include "PhysicalConst.h"
#include "NTCM_G_Coeff.h"
#include "Check.h"

/* Parameters (default storage) */
struct P_NTCM_Procedure_T_ {
  P_ErrorMnG_T ErrorMnG_i;             /* '<Root>/ErrorMnG' */
};



/* Declare global externs for instrumentation */
extern void profileStart_NTCM_Procedure(uint32_T);
extern void profileEnd_NTCM_Procedure(uint32_T);
/* Code_Instrumentation_Declarations_Placeholder */
extern void NTCM_Procedure(const real_T brdcIonoParam[3], const real_T *Doy,
  const real_T *UTC, const real_T llhRecDeg[3], const real_T llhSatDeg[3], const
  real_T *fcarr, real_T *vTEC, real_T *sTEC, real_T *delay_m, uint16_T
  ErrorCode[16]);

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
 * '<Root>' : 'NTCM_Procedure'
 * '<S1>'   : 'NTCM_Procedure/CheckOutput'
 * '<S2>'   : 'NTCM_Procedure/ChekInput'
 * '<S3>'   : 'NTCM_Procedure/ErrorMnG'
 * '<S4>'   : 'NTCM_Procedure/NTCM_Procedure_Core'
 * '<S5>'   : 'NTCM_Procedure/CheckOutput/Variant Subsystem'
 * '<S6>'   : 'NTCM_Procedure/CheckOutput/Variant Subsystem/CheckOutput'
 * '<S7>'   : 'NTCM_Procedure/CheckOutput/Variant Subsystem/CheckOutput_Bypass'
 * '<S8>'   : 'NTCM_Procedure/CheckOutput/Variant Subsystem/CheckOutput/ErrorCodeGen'
 * '<S9>'   : 'NTCM_Procedure/CheckOutput/Variant Subsystem/CheckOutput/Saturation3'
 * '<S10>'  : 'NTCM_Procedure/CheckOutput/Variant Subsystem/CheckOutput/Saturation3/Interval Test'
 * '<S11>'  : 'NTCM_Procedure/ChekInput/Variant'
 * '<S12>'  : 'NTCM_Procedure/ChekInput/Variant/CheckInput'
 * '<S13>'  : 'NTCM_Procedure/ChekInput/Variant/CheckInput_Bypass'
 * '<S14>'  : 'NTCM_Procedure/ChekInput/Variant/CheckInput/ErrorCodeGen'
 * '<S15>'  : 'NTCM_Procedure/ChekInput/Variant/CheckInput/Saturation3'
 * '<S16>'  : 'NTCM_Procedure/ChekInput/Variant/CheckInput/Saturation3/Interval Test'
 * '<S17>'  : 'NTCM_Procedure/ErrorMnG/Variant Subsystem'
 * '<S18>'  : 'NTCM_Procedure/ErrorMnG/Variant Subsystem/ErrorMnG '
 * '<S19>'  : 'NTCM_Procedure/ErrorMnG/Variant Subsystem/Subsystem1'
 * '<S20>'  : 'NTCM_Procedure/NTCM_Procedure_Core/ComputeDoA'
 * '<S21>'  : 'NTCM_Procedure/NTCM_Procedure_Core/Compute_IonoDelay'
 * '<S22>'  : 'NTCM_Procedure/NTCM_Procedure_Core/Compute_sTEC'
 * '<S23>'  : 'NTCM_Procedure/NTCM_Procedure_Core/Deg2Radllh1'
 * '<S24>'  : 'NTCM_Procedure/NTCM_Procedure_Core/Deg2Radllh2'
 * '<S25>'  : 'NTCM_Procedure/NTCM_Procedure_Core/ElevationCheck'
 * '<S26>'  : 'NTCM_Procedure/NTCM_Procedure_Core/LocalTime'
 * '<S27>'  : 'NTCM_Procedure/NTCM_Procedure_Core/NTCM_Coeff'
 * '<S28>'  : 'NTCM_Procedure/NTCM_Procedure_Core/Physical_Const'
 * '<S29>'  : 'NTCM_Procedure/NTCM_Procedure_Core/getIonoPiercePoint'
 * '<S30>'  : 'NTCM_Procedure/NTCM_Procedure_Core/llh2xyz1'
 * '<S31>'  : 'NTCM_Procedure/NTCM_Procedure_Core/llh2xyz2'
 * '<S32>'  : 'NTCM_Procedure/NTCM_Procedure_Core/ComputeDoA/computeDoA'
 * '<S33>'  : 'NTCM_Procedure/NTCM_Procedure_Core/Compute_IonoDelay/Compute_Delay'
 * '<S34>'  : 'NTCM_Procedure/NTCM_Procedure_Core/Compute_sTEC/Mapping_fun'
 * '<S35>'  : 'NTCM_Procedure/NTCM_Procedure_Core/ElevationCheck/Variant Model'
 * '<S36>'  : 'NTCM_Procedure/NTCM_Procedure_Core/ElevationCheck/Variant Model/ElevationCheck'
 * '<S37>'  : 'NTCM_Procedure/NTCM_Procedure_Core/ElevationCheck/Variant Model/ElevationCheck_ByPass'
 * '<S38>'  : 'NTCM_Procedure/NTCM_Procedure_Core/ElevationCheck/Variant Model/ElevationCheck/ErrorCodeGen'
 * '<S39>'  : 'NTCM_Procedure/NTCM_Procedure_Core/ElevationCheck/Variant Model/ElevationCheck/Saturation6'
 * '<S40>'  : 'NTCM_Procedure/NTCM_Procedure_Core/ElevationCheck/Variant Model/ElevationCheck/Saturation6/Interval Test'
 * '<S41>'  : 'NTCM_Procedure/NTCM_Procedure_Core/LocalTime/LocalTime'
 * '<S42>'  : 'NTCM_Procedure/NTCM_Procedure_Core/getIonoPiercePoint/Ipp_Const'
 * '<S43>'  : 'NTCM_Procedure/NTCM_Procedure_Core/getIonoPiercePoint/MATLAB Function'
 * '<S44>'  : 'NTCM_Procedure/NTCM_Procedure_Core/llh2xyz1/llh2xyz'
 * '<S45>'  : 'NTCM_Procedure/NTCM_Procedure_Core/llh2xyz1/llh2xyz/llh2xyz'
 * '<S46>'  : 'NTCM_Procedure/NTCM_Procedure_Core/llh2xyz2/llh2xyz'
 * '<S47>'  : 'NTCM_Procedure/NTCM_Procedure_Core/llh2xyz2/llh2xyz/llh2xyz'
 */
#endif                                 /* RTW_HEADER_NTCM_Procedure_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
