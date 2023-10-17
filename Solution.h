#ifndef SOLUTION_H
#define SOLUTION_H

#include "Constants.h"
#include "DataFiles.h"

struct CurSolution
{
	char Valid;
    int NOfValidSattelites;
	double Q[MAX_OF_UNKNOWNS], dQp[3], dQm[3],
		   B, L, H,
		   dEp, dNp, dUp, dEm, dNm, dUm,
		   RMSEHp, RMSEVp, RMSE3Dp,
		   RMSEHm, RMSEVm, RMSE3Dm,
		   HDOP, VDOP, PDOP, TDOP;
};

struct ComSolution
{
	int N;
	double *Q[3], Qp[3], Qm[3], *dQp[3], *dQm[3],
		   dQpm[3], dQmm[3],
           dEpm, dNpm, dUpm, dEmm, dNmm, dUmm,
		   SdEp2, SdNp2, SdUp2, SdEm2, SdNm2, SdUm2,
		   SRMSEHp, SRMSEVp, SRMSE3Dp,
		   SRMSEHm, SRMSEVm, SRMSE3Dm;
};

void SattelitesInitialRange(struct Epoch*, struct Sattelite*,
							char *, struct CurSolution*);
void NaviSolve(struct Sattelite*, char, char, char*, struct RINEXObs*,
			   double*, double*, double (*)[MAX_OF_UNKNOWNS], double (*)[MAX_OF_UNKNOWNS]);
void CalcRMSE(struct CurSolution*, struct ComSolution*);
void CalcDOPs(struct CurSolution*, double (*)[MAX_OF_UNKNOWNS]);
void AddStatData(struct CurSolution*, struct ComSolution*);
void CalcStatistics(struct CurSolution*, struct ComSolution*, int);

#endif
