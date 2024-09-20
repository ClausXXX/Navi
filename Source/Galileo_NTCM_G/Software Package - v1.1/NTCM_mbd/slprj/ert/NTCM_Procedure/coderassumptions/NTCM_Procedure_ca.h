/*
 * File: NTCM_Procedure_ca.h
 *
 * Abstract: Tests assumptions in the generated code.
 */

#ifndef NTCM_PROCEDURE_CA_H
#define NTCM_PROCEDURE_CA_H

/* preprocessor validation checks */
#include "NTCM_Procedure_ca_preproc.h"
#include "coder_assumptions_hwimpl.h"

/* variables holding test results */
extern CA_HWImpl_TestResults CA_NTCM_Procedure_HWRes;
extern CA_PWS_TestResults CA_NTCM_Procedure_PWSRes;

/* variables holding "expected" and "actual" hardware implementation */
extern const CA_HWImpl CA_NTCM_Procedure_ExpHW;
extern CA_HWImpl CA_NTCM_Procedure_ActHW;

/* entry point function to run tests */
void NTCM_Procedure_caRunTests(void);

#endif                                 /* NTCM_PROCEDURE_CA_H */
