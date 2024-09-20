/*
 * File: xil_instrumentation.c
 *
 * Code generated for instrumentation.
 *
 */

#include "xil_instrumentation.h"

/* Code instrumentation offset(s) for model NTCM_Procedure */
#define profileStart_NTCM_Procedure_offset 1
#define profileEnd_NTCM_Procedure_offset 1

/* Code instrumentation offset(s) for model NTCM_G */
#define profileStart_NTCM_G_offset     36
#define profileEnd_NTCM_G_offset       36

/* Code instrumentation offset(s) for model NTCM_Procedure */
#define taskTimeStart_NTCM_Procedure_offset 0
#define taskTimeEnd_NTCM_Procedure_offset 0

/* A function parameter may be intentionally unused */
#ifndef UNUSED_PARAMETER
# if defined(__LCC__)
#   define UNUSED_PARAMETER(x)
# else
#   define UNUSED_PARAMETER(x)         (void) (x)
# endif
#endif

#define SIZEOF_SECTION_ID_CONTAINER    sizeof(uint64_T)
#define SIZEOF_TIMER_TYPE              sizeof(uint64_T)

static uint64_T xsd_xil_timer_corrected = 0;
static uint64_T xsd_xil_timer_unfreeze = 0;
static uint32_T xsd_xil_freezing_busy = 0;
void xilUploadProfilingData(uint32_T sectionId)
{
  xilUploadCodeInstrData((void *)(&xsd_xil_timer_corrected), (uint32_T)
    (SIZEOF_TIMER_TYPE), sectionId);
}

/* The internal freeze and unfreeze methods cannot be nested. The customer-visible implementation avoids nesting problems */
void xilProfilingTimerFreezeInternal(void)
{
  /* Update the value of the corrected timer to exclude time spent in the
   * instrumentation code.
   *
   * Using a timer that increments on each tick.
   */
  xsd_xil_timer_corrected = xsd_xil_timer_corrected + (((uint64_T)(timestamp_x86
    ())) - xsd_xil_timer_unfreeze);
}

void xilProfilingTimerUnFreezeInternal(void)
{
  xsd_xil_timer_unfreeze = ( uint64_T ) (timestamp_x86());
}

void xilProfilingTimerFreeze(void)
{
  if (xsd_xil_freezing_busy == 0) {
    xilProfilingTimerFreezeInternal();
  }                                    /* if */
}

void xilProfilingTimerUnFreeze(void)
{
  if (xsd_xil_freezing_busy == 0) {
    xilProfilingTimerUnFreezeInternal();
  }                                    /* if */
}

void taskTimeStart(uint32_T sectionId)
{
  /* Send execution profiling data to host */
  xilUploadProfilingData(sectionId);
  xilProfilingTimerUnFreezeInternal();
}

void taskTimeEnd(uint32_T sectionId)
{
  uint32_T sectionIdNeg = ~sectionId;
  xilProfilingTimerFreezeInternal();

  /* Send execution profiling data to host */
  xilUploadProfilingData(sectionIdNeg);
}

void profileStart(uint32_T sectionId)
{
  xilProfilingTimerFreezeInternal();

  /* Send execution profiling data to host */
  xilUploadProfilingData(sectionId);
  xilProfilingTimerUnFreezeInternal();
}

void profileEnd(uint32_T sectionId)
{
  uint32_T sectionIdNeg = ~sectionId;
  xilProfilingTimerFreezeInternal();

  /* Send execution profiling data to host */
  xilUploadProfilingData(sectionIdNeg);
  xilProfilingTimerUnFreezeInternal();
}

/* Code instrumentation method(s) for model NTCM_Procedure */
void profileStart_NTCM_Procedure(uint32_T sectionId)
{
  profileStart(profileStart_NTCM_Procedure_offset + sectionId);
}

void profileEnd_NTCM_Procedure(uint32_T sectionId)
{
  profileEnd(profileEnd_NTCM_Procedure_offset + sectionId);
}

/* Code instrumentation method(s) for model NTCM_G */
void profileStart_NTCM_G(uint32_T sectionId)
{
  profileStart(profileStart_NTCM_G_offset + sectionId);
}

void profileEnd_NTCM_G(uint32_T sectionId)
{
  profileEnd(profileEnd_NTCM_G_offset + sectionId);
}

/* Code instrumentation method(s) for model NTCM_Procedure */
void taskTimeStart_NTCM_Procedure(uint32_T sectionId)
{
  taskTimeStart(taskTimeStart_NTCM_Procedure_offset + sectionId);
}

void taskTimeEnd_NTCM_Procedure(uint32_T sectionId)
{
  taskTimeEnd(taskTimeEnd_NTCM_Procedure_offset + sectionId);
}
