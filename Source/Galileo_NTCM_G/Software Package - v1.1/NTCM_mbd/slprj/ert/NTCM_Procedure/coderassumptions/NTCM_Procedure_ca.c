/*
 * File: NTCM_Procedure_ca.c
 *
 * Abstract: Tests assumptions in the generated code.
 */

#include "NTCM_Procedure_ca.h"

CA_HWImpl_TestResults CA_NTCM_Procedure_HWRes;
CA_PWS_TestResults CA_NTCM_Procedure_PWSRes;
const CA_HWImpl CA_NTCM_Procedure_ExpHW = {

#ifdef PORTABLE_WORDSIZES

  8,                                   /* BitPerChar */
  16,                                  /* BitPerShort */
  32,                                  /* BitPerInt */
  32,                                  /* BitPerLong */
  64,                                  /* BitPerLongLong */
  32,                                  /* BitPerFloat */
  64,                                  /* BitPerDouble */
  64,                                  /* BitPerPointer */
  64,                                  /* BitPerSizeT */
  64,                                  /* BitPerPtrDiffT */
  CA_LITTLE_ENDIAN,                    /* Endianess */
  CA_ZERO,                             /* IntDivRoundTo */
  1,                                   /* ShiftRightIntArith */

#else

  8,                                   /* BitPerChar */
  16,                                  /* BitPerShort */
  32,                                  /* BitPerInt */
  32,                                  /* BitPerLong */
  64,                                  /* BitPerLongLong */
  32,                                  /* BitPerFloat */
  64,                                  /* BitPerDouble */
  32,                                  /* BitPerPointer */
  32,                                  /* BitPerSizeT */
  32,                                  /* BitPerPtrDiffT */
  CA_BIG_ENDIAN,                       /* Endianess */
  CA_ZERO,                             /* IntDivRoundTo */
  1,                                   /* ShiftRightIntArith */

#endif

  1,                                   /* LongLongMode */
  1,                                   /* PortableWordSizes */
  "Custom Processor->Custom",          /* HWDeviceType */
  0,                                   /* MemoryAtStartup */
  0,                                   /* DynamicMemoryAtStartup */
  0,                                   /* DenormalFlushToZero */
  0                                    /* DenormalAsZero */
};

CA_HWImpl CA_NTCM_Procedure_ActHW;
void NTCM_Procedure_caRunTests(void)
{
  /* verify hardware implementation */
  caVerifyPortableWordSizes(&CA_NTCM_Procedure_ActHW, &CA_NTCM_Procedure_ExpHW,
    &CA_NTCM_Procedure_PWSRes);
  caVerifyHWImpl(&CA_NTCM_Procedure_ActHW, &CA_NTCM_Procedure_ExpHW,
                 &CA_NTCM_Procedure_HWRes);
}
