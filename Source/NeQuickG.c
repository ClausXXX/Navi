#include "NeQuickG.h"

#include <stdio.h>

MODIP_st* loadmodipgrid(char *pchModipFile, MODIP_st *pstModip)
{
  FILE *fpFile;
  int siLatIndex;
  int siLngIndex;
  fpFile = fopen(pchModipFile, "r");
  if (fpFile == NULL) {
    fprintf(stderr, "Unable to open file: %s\n", pchModipFile);
    return NULL;
  }
  for (siLatIndex = 0; siLatIndex < NUM_MODIP_DIM_1_SI; siLatIndex++) {
    for (siLngIndex = 0; siLngIndex < NUM_MODIP_DIM_2_SI; siLngIndex++) {
      if (fscanf(fpFile, "%lf", &(pstModip->pdModip[siLatIndex][siLngIndex])) == EOF) {
        fprintf(stderr, "Unexpected EOF in %s\n\n", pchModipFile);
        return NULL;
      }
    }
  }
  return pstModip;
}

CCIR_st* loadccirdata(char *pchCcirFilePattern, CCIR_st *pstCCIR)
{
  int siMonth;
  char fname[32];
  FILE* pAsc[MONTHS_SI];
  int siIndex1;
  int siIndex2;
  int siSolar;

  /* Open the input files */
  for (siMonth = 0; siMonth < MONTHS_SI; siMonth++) {
    /* Attempt to open the files ccir11.asc -> ccir22.asc */
    sprintf(fname, pchCcirFilePattern, siMonth + 11);
    pAsc[siMonth] = fopen(fname, "r");
    /* Check if file was opened correctly */
    if (pAsc[siMonth] == NULL) {
      fprintf(stderr, "Unable to open file: %s\n", fname);
      /* Open has failed so need to close all files already opened */
      for (siIndex1 = 0; siIndex1 < siMonth; siIndex1++) {
        fclose(pAsc[siIndex1]);
      }
      return NULL;
    }
  }
  /* Files are all open, so read in the data */
  for (siSolar = 0; siSolar < CCIR_F2_DIM_3_SI; siSolar++) {
    for (siIndex2 = 0; siIndex2 < CCIR_F2_DIM_2_SI; siIndex2++) {
      for (siIndex1 = 0; siIndex1 < CCIR_F2_DIM_1_SI; siIndex1++) {
        for (siMonth = 0; siMonth < MONTHS_SI; siMonth++) {
          fscanf(pAsc[siMonth], "%lf",
                 &(pstCCIR->pdF2[siMonth][siIndex1][siIndex2][siSolar]));
        }
      }
    }
  }
  for (siSolar = 0; siSolar < CCIR_M3000_DIM_3_SI; siSolar++) {
    for (siIndex2 = 0; siIndex2 < CCIR_M3000_DIM_2_SI; siIndex2++) {
      for (siIndex1 = 0; siIndex1 < CCIR_M3000_DIM_1_SI; siIndex1++) {
        for (siMonth = 0; siMonth < MONTHS_SI; siMonth++) {
          fscanf(pAsc[siMonth], "%lf",
                 &(pstCCIR->pdM3000[siMonth][siIndex1][siIndex2][siSolar]));
        }
      }
    }
  }
  /* Close the files */
  for (siMonth = 0; siMonth < MONTHS_SI; siMonth++) {
    fclose(pAsc[siMonth]);
  }
  return pstCCIR;
}

// pdRecvLLHdeg[3], °, °, m
// pdSatLLHdeg[3], °, °, m
void performtest(int siMonth, double dTime,
						double pdRecvLLHdeg[3], double pdSatLLHdeg[3],
                        double pdCoeffs[3], MODIP_st *pstModip, CCIR_st *pstCCIR,
                        double *pdModip, double *pdStec)
{
  static CurrentCCIR_st stCurrCCIR;
  static NeQuickInputData_st stNeQuickInputData;
  double pdKronrodTol[2] = {KRONROD_TOL_1, KRONROD_TOL_2};
  double pdRecvLLHrad[3];
  double pdSatLLHrad[3];
  pdRecvLLHrad[0] = pdRecvLLHdeg[0] * PI_D / 180.0;
  pdRecvLLHrad[1] = pdRecvLLHdeg[1] * PI_D / 180.0;
  pdRecvLLHrad[2] = pdRecvLLHdeg[2];
  pdSatLLHrad[0] = pdSatLLHdeg[0] * PI_D / 180.0;
  pdSatLLHrad[1] = pdSatLLHdeg[1] * PI_D / 180.0;
  pdSatLLHrad[2] = pdSatLLHdeg[2];
  stNeQuickInputData.pstModip = pstModip;
  stNeQuickInputData.pstCCIR = pstCCIR;
  stNeQuickInputData.pdGssPosLLH = pdRecvLLHrad;
  stNeQuickInputData.pdSatPosLLH = pdSatLLHrad;
  stNeQuickInputData.pdCoeff = pdCoeffs;
  stNeQuickInputData.pdKronrodTol = pdKronrodTol;
  stNeQuickInputData.siMaxRecurse = RECURSION_LIMIT;
  stNeQuickInputData.siMonth = siMonth;
  stNeQuickInputData.dUT = dTime;
  *pdModip = NeqCalcModip(pdRecvLLHdeg[0], pdRecvLLHdeg[1], pstModip);
  stNeQuickInputData.pdCoeff = pdCoeffs;
  stNeQuickInputData.siNumCoeff = 3;
  NeQuick(&stNeQuickInputData, &stCurrCCIR, pdStec);
  *pdStec = *pdStec / 1.0E16;
}
