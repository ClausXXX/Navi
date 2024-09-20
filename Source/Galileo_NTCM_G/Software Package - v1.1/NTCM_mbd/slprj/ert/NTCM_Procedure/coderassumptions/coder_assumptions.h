/*
 * File: coder_assumptions.h
 *
 * Abstract: Coder assumptions header file
 */

#ifndef CODER_ASSUMPTIONS_H
#define CODER_ASSUMPTIONS_H

/* include model specific checks */
#include "NTCM_Procedure_ca.h"

/* global results variable mapping for static code */
#define CA_Expected_HWImpl             CA_NTCM_Procedure_ExpHW
#define CA_Actual_HWImpl               CA_NTCM_Procedure_ActHW
#define CA_HWImpl_Results              CA_NTCM_Procedure_HWRes
#define CA_PortableWordSizes_Results   CA_NTCM_Procedure_PWSRes

/* entry point function mapping for static code */
#define CA_Run_Tests                   NTCM_Procedure_caRunTests
#endif                                 /* CODER_ASSUMPTIONS_H */
