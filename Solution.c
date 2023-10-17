#include "Solution.h"
#include "Common.h"

#include <math.h>
#include <stdlib.h>
//#include <alloc.h>

void SattelitesInitialRange(struct Epoch *Epoch, struct Sattelite *Sattelites,
				  			char *FrequencyMode, struct CurSolution *CurSolution)
{
	int i, j;
    double tau;
	for(i = 0; i < Epoch->NOfSattelites; i++)
	{
		//printf("\n%c%d %d\n", Sattelites[i].Number[0], Sattelites[i].Number[1], Sattelites[i].Valid);
		//getch();
		if(Sattelites[i].Number[0] == 'G' && Sattelites[i].Valid)
		{
			if(strstr(FrequencyMode, "SC") || strstr(FrequencyMode, "SP"))
			{
				Sattelites[i].rho = Sattelites[i].P[0];
			}

			if(strstr(FrequencyMode, "DC") ||
			   strstr(FrequencyMode, "DP") ||
			   strstr(FrequencyMode, "DA"))
			{
				Sattelites[i].rho = Sattelites[i].P1[0];
			}
			tau = Sattelites[i].rho / c;
			Sattelites[i].tk -= tau;
			CurSolution->NOfValidSattelites++;
		}

		if(Sattelites[i].Number[0] == 'R' && Sattelites[i].Valid)
		{
			if(strstr(FrequencyMode, "SC") || strstr(FrequencyMode, "SP"))
			{
				Sattelites[i].rho = Sattelites[i].P[0];
			}

			if(strstr(FrequencyMode, "DC") ||
			   strstr(FrequencyMode, "DP") ||
			   strstr(FrequencyMode, "DA"))
			{
				Sattelites[i].rho = Sattelites[i].P1[0];
			}
			tau = Sattelites[i].rho / c;
			Sattelites[i].tk -= tau;
			for(j = 0; j < 1; j++)
			{
				Sattelites[i].dt = -Sattelites[i].TauN + Sattelites[i].GammaN * Sattelites[i].tk;
				Sattelites[i].tk = Epoch->t - tau - Sattelites[i].dt - Sattelites[i].toc;
			}
			CurSolution->NOfValidSattelites++;
		}

		if(Sattelites[i].Number[0] == 'E' && Sattelites[i].Valid)
		{
			if(strstr(FrequencyMode, "SC1"))
			{
				if(Sattelites[i].GALNavMsgType == F_NAV)
				{
					Sattelites[i].Valid = 0;
					CurSolution->NOfValidSattelites--;
				}
			}

			if(strstr(FrequencyMode, "SC2"))
			{
				if(Sattelites[i].GALNavMsgType == F_NAV)
				{
					Sattelites[i].P[0] = Sattelites[i].PC5[0];
					Sattelites[i].P[1] = Sattelites[i].PC5[1];
				}

                if(Sattelites[i].GALNavMsgType == I_NAV)
				{
					Sattelites[i].P[0] = Sattelites[i].PC7[0];
                    Sattelites[i].P[1] = Sattelites[i].PC7[1];
				}
			}

			if(strstr(FrequencyMode, "DC") || strstr(FrequencyMode, "DA"))
			{
				if(Sattelites[i].GALNavMsgType == F_NAV)
				{
					Sattelites[i].P2[0] = Sattelites[i].PC5[0];
					Sattelites[i].P2[1] = Sattelites[i].PC5[1];
				}

				if(Sattelites[i].GALNavMsgType == I_NAV)
				{
					Sattelites[i].P2[0] = Sattelites[i].PC7[0];
                    Sattelites[i].P2[1] = Sattelites[i].PC7[1];
				}
			}

			if(strstr(FrequencyMode, "SC"))
			{
				Sattelites[i].rho = Sattelites[i].P[0];
			}

			if(strstr(FrequencyMode, "DC") || strstr(FrequencyMode, "DA"))
			{
				Sattelites[i].rho = Sattelites[i].P1[0];
			}
			tau = Sattelites[i].rho / c;
			Sattelites[i].tk -= tau;
			CurSolution->NOfValidSattelites++;
		}

		if(Sattelites[i].Number[0] == 'C' && Sattelites[i].Valid)
		{
			if(strstr(FrequencyMode, "SC"))
			{
				Sattelites[i].rho = Sattelites[i].P[0];
			}

			if(strstr(FrequencyMode, "DC") || strstr(FrequencyMode, "DA"))
			{
				Sattelites[i].rho = Sattelites[i].P1[0];
			}
			tau = Sattelites[i].rho / c;
			Sattelites[i].tk -= tau;
			CurSolution->NOfValidSattelites++;
		}
    }
}

// Решение системы нелинейных уравнений методом нименьших квадратов
// NumberOfSattelites - число уравнений в системе
// SattelitesThreshold - число неизвестных
// **A - матрица направляющих косинусов
// *L - вектор разностей рассчётных и известных величин, м
// **E - ковариационная матрица, рассчитываемая в процессе решения системы
// *Q - вектор неизвестных (априорно заданных) величин (координат и задержек
//      в часах приёмника), м
void LSq(char NOfSattelites, char SattelitesThreshold,
		 double (*A)[MAX_OF_UNKNOWNS],
		 double *L, double (*E)[MAX_OF_UNKNOWNS], double *Q)
{
	int i, j, k;
	double b;
	double AT[MAX_OF_UNKNOWNS][MAX_OF_VISIBLE_SATS],
		   ATA[MAX_OF_UNKNOWNS][MAX_OF_UNKNOWNS],
		   ATL[MAX_OF_UNKNOWNS];
	for(i = 0; i < SattelitesThreshold; i++)
	{
		for(j = 0; j < NOfSattelites; j++)
		{
			AT[i][j] = A[j][i];
		}
	}

	for(i = 0; i < SattelitesThreshold; i++)
	{
		for(j = 0; j < SattelitesThreshold; j++)
		{
			ATA[i][j] = 0.0;
			for(k = 0; k < NOfSattelites; k++)
			{
				ATA[i][j] += AT[i][k] * A[k][j];
			}
		}
	}

	for(i = 0; i < SattelitesThreshold; i++)
	{
		ATL[i] = 0.0;
		for(j = 0; j < NOfSattelites; j++)
		{
			ATL[i] += AT[i][j] * L[j];
		}
	}

	for(i = 0; i < SattelitesThreshold; i++)
	{
		for(j = 0; j < SattelitesThreshold; j++)
		{
			E[i][j] = (i == j ? 1.0 : 0.0);
		}
	}

	for(i = 0; i < SattelitesThreshold; i++)
	{
		b = ATA[i][i];
		for(j = SattelitesThreshold - 1; j >= 0; j--)
		{
			E[i][j] /= b;
			ATA[i][j] /= b;
		}

		for(j = 0; j < SattelitesThreshold; j++)
		{
			if (j != i)
			{
				b = ATA[j][i];
				for(k = SattelitesThreshold - 1; k >= 0; k--)
				{
					E[j][k] -= E[i][k] * b;
					ATA[j][k] -= ATA[i][k] * b;
				}
			}
		}
	}

	for(i = 0; i < SattelitesThreshold; i++)
	{
		for(j = 0; j < SattelitesThreshold; j++)
		{
			Q[i] -= E[i][j] * ATL[j];
		}
	}
}

// Подготовка исходных матрмц и решение навигационной задачи методом наименьших квадратов
// Sattelites* - массив структур данных спутников, участвующих в решении
// NOfSattelites - число наблюдаемых спутников
// SattelitesThreshold - число неизвестных
// *System - массив литер навигационных систем, для которых осуществляется решение
// *RINEXObs - указатель на структуру данных наблюдений
// **A - матрица направляющих косинусов
// *L - вектор разностей рассчётных и известных величин, м
// **E - ковариационная матрица, рассчитываемая в процессе решения системы
// *Q - вектор неизвестных (априорно заданных) величин (координат и задержек
//      в часах приёмника для каждой системы), м
void NaviSolve(struct Sattelite *Sattelites, char NOfSattelites,
			   char SattelitesThreshold, char *System, struct RINEXObs *RINEXObs,
			   double *Q, double *L, double (*A)[MAX_OF_UNKNOWNS], double (*E)[MAX_OF_UNKNOWNS])
{
	int i, j;
	for(i = 0; i < NOfSattelites; i++)
	{
        for(j = 0; j < MAX_OF_UNKNOWNS; j++)
		{
			A[i][j] = 0.0;
			L[i] = 0.0;
		}

		if(Sattelites[i].Valid)
		{
			L[i] = Sattelites[i].rho - Sattelites[i].P[0] - c * Sattelites[i].dt + Sattelites[i].I + Sattelites[i].T;
			for(j = 3; j < MAX_OF_UNKNOWNS; j++)
			{
				L[i] += Q[j];
			}
			A[i][0] = (Q[0] - Sattelites[i].x) / Sattelites[i].rho;
			A[i][1] = (Q[1] - Sattelites[i].y) / Sattelites[i].rho;
			A[i][2] = (Q[2] - Sattelites[i].z) / Sattelites[i].rho;
			if(strstr(System, "G") && RINEXObs->GPS && (Sattelites[i].Number[0] == 'G'))
			{
				A[i][3 + GetIndex(System, 'G')] = 1.0;
			}

			if(strstr(System, "R") && RINEXObs->GLO && (Sattelites[i].Number[0] == 'R'))
			{
				A[i][3 + GetIndex(System, 'R')] = 1.0;
			}

			if(strstr(System, "E") && RINEXObs->GAL && (Sattelites[i].Number[0] == 'E'))
			{
				A[i][3 + GetIndex(System, 'E')] = 1.0;
			}

			if(strstr(System, "C") && RINEXObs->BDS && (Sattelites[i].Number[0] == 'C'))
			{
				A[i][3 + GetIndex(System, 'C')] = 1.0;
			}
		}
	}
	LSq(NOfSattelites, SattelitesThreshold, A, L, E, Q);
}

void CalcRMSE(struct CurSolution *CurSolution, struct ComSolution *ComSolution)
{
	CurSolution->dQp[0] = CurSolution->Q[0] - ComSolution->Qp[0];
	CurSolution->dQp[1] = CurSolution->Q[1] - ComSolution->Qp[1];
	CurSolution->dQp[2] = CurSolution->Q[2] - ComSolution->Qp[2];
	ECEFToLocalENU(CurSolution->dQp[0], CurSolution->dQp[1], CurSolution->dQp[2],
				   &CurSolution->dEp, &CurSolution->dNp, &CurSolution->dUp);
	CurSolution->RMSEHp = sqrt(sqr(CurSolution->dEp) + sqr(CurSolution->dNp));
	CurSolution->RMSEVp = sqrt(sqr(CurSolution->dUp));
	CurSolution->RMSE3Dp = sqrt(sqr(CurSolution->RMSEHp) + sqr(CurSolution->RMSEVp));
}

void CalcDOPs(struct CurSolution *CurSolution, double (*E)[MAX_OF_UNKNOWNS])
{
	int i, j, k;
	double R[4][4], RT[4][4], RE[4][4], Qxi[4][4];
	DecartToGeo(CurSolution->Q[0], CurSolution->Q[1], CurSolution->Q[2], &CurSolution->B, &CurSolution->L, &CurSolution->H);
	CurSolution->B *= DegToRad;
	CurSolution->L *= DegToRad;
	R[0][0] = -sin(CurSolution->L);
	R[0][1] = cos(CurSolution->L);
	R[0][2] = 0.0;
	R[1][0] = -sin(CurSolution->B) * cos(CurSolution->L);
	R[1][1] = -sin(CurSolution->B) * sin(CurSolution->L);
	R[1][2] = cos(CurSolution->B);
	R[2][0] = cos(CurSolution->B) * cos(CurSolution->L);
	R[2][1] = cos(CurSolution->B) * sin(CurSolution->L);
	R[2][2] = sin(CurSolution->B);
	CurSolution->B *= RadToDeg;
	CurSolution->L *= RadToDeg;
	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 3; j++)
		{
			RT[i][j] = R[j][i];
		}
	}

	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 3; j++)
		{
			RE[i][j] = 0.0;
			for(k = 0; k < 3; k++)
			{
				RE[i][j] += R[i][k] * E[k][j];
			}
		}
	}

	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 3; j++)
		{
			Qxi[i][j] = 0.0;
			for(k = 0; k < 3; k++)
			{
				Qxi[i][j] += RE[i][k] * RT[k][j];
			}
		}
	}
	Qxi[3][3] = E[3][3];
	CurSolution->HDOP = sqrt(Qxi[0][0] + Qxi[1][1]);
	CurSolution->VDOP = sqrt(Qxi[2][2]);
	CurSolution->PDOP = sqrt(sqr(CurSolution->HDOP) + sqr(CurSolution->VDOP));
	CurSolution->TDOP = sqrt(Qxi[3][3]);
}

void AddStatData(struct CurSolution *CurSolution,
				 struct ComSolution *ComSolution)
{
	ComSolution->Q[0][ComSolution->N] = CurSolution->Q[0];
	ComSolution->Q[1][ComSolution->N] = CurSolution->Q[1];
	ComSolution->Q[2][ComSolution->N] = CurSolution->Q[2];
	ComSolution->dQp[0][ComSolution->N] = CurSolution->dQp[0];
	ComSolution->dQp[1][ComSolution->N] = CurSolution->dQp[1];
	ComSolution->dQp[2][ComSolution->N] = CurSolution->dQp[2];
	ComSolution->SdEp2 += sqr(CurSolution->dEp);
	ComSolution->SdNp2 += sqr(CurSolution->dNp);
	ComSolution->SdUp2 += sqr(CurSolution->dUp);
	ComSolution->N++;
}

int SortFunction(const void *j, const void *i)
{
	double r = fabs(*(double*)j) - fabs(*(double*)i);
	if(r == 0.0)
	{
		return 0;
	}
	return r < 0.0 ? -1 : 1;
}

double Median(double *Q, int N)
{
	int i;
	double Median;
	qsort(Q, N, sizeof(double), SortFunction);
	i = N / 2;
	if(N % 2 == 0)
	{
		Median = (Q[i - 1] + Q[i]) / 2.0;
	}
	else
	{
		Median = Q[i];
	}
	return Median;
}

void CalcStatistics(struct CurSolution *CurSolution,
					struct ComSolution *ComSolution, int NOfEpochs)
{
	int i, j = 0;
	if(ComSolution->N)
	{
		ComSolution->Qm[0] = Median(ComSolution->Q[0], ComSolution->N);
		ComSolution->Qm[1] = Median(ComSolution->Q[1], ComSolution->N);
		ComSolution->Qm[2] = Median(ComSolution->Q[2], ComSolution->N);
		for(i = 0; i < NOfEpochs; i++)
		{
			if(CurSolution[i].Valid)
			{
				CurSolution[i].dQm[0] = CurSolution[i].Q[0] - ComSolution->Qm[0];
				CurSolution[i].dQm[1] = CurSolution[i].Q[1] - ComSolution->Qm[1];
				CurSolution[i].dQm[2] = CurSolution[i].Q[2] - ComSolution->Qm[2];
				ComSolution->dQm[0][j] = CurSolution[i].dQm[0];
				ComSolution->dQm[1][j] = CurSolution[i].dQm[1];
				ComSolution->dQm[2][j] = CurSolution[i].dQm[2];
				ECEFToLocalENU(CurSolution[i].dQm[0], CurSolution[i].dQm[1], CurSolution[i].dQm[2],
							   &CurSolution[i].dEm, &CurSolution[i].dNm, &CurSolution[i].dUm);
				CurSolution[i].RMSEHm = sqrt(sqr(CurSolution[i].dEm) + sqr(CurSolution[i].dNm));
				CurSolution[i].RMSEVm = sqrt(sqr(CurSolution[i].dUm));
				CurSolution[i].RMSE3Dm = sqrt(sqr(CurSolution[i].RMSEHm) + sqr(CurSolution[i].RMSEVm));
				ComSolution->SdEm2 += sqr(CurSolution[i].dEm);
				ComSolution->SdNm2 += sqr(CurSolution[i].dNm);
				ComSolution->SdUm2 += sqr(CurSolution[i].dUm);
				j++;
			}
		}
		ComSolution->dQpm[0] = Median(ComSolution->dQp[0], ComSolution->N);
		ComSolution->dQpm[1] = Median(ComSolution->dQp[1], ComSolution->N);
		ComSolution->dQpm[2] = Median(ComSolution->dQp[2], ComSolution->N);
//		ECEFToLocalENU(ComSolution->dQpm[0], ComSolution->dQpm[1], ComSolution->dQpm[2],
//						   &ComSolution->dEpm, &ComSolution->dNpm, &ComSolution->dUpm);
		ComSolution->dQmm[0] = Median(ComSolution->dQm[0], ComSolution->N);
		ComSolution->dQmm[1] = Median(ComSolution->dQm[1], ComSolution->N);
		ComSolution->dQmm[2] = Median(ComSolution->dQm[2], ComSolution->N);
//		ECEFToLocalENU(ComSolution->dQmm[0], ComSolution->dQmm[1], ComSolution->dQmm[2],
//					   &ComSolution->dEmm, &ComSolution->dNmm, &ComSolution->dUmm);
		ComSolution->SRMSEHp = sqrt((ComSolution->SdEp2 + ComSolution->SdNp2) / ComSolution->N);
		ComSolution->SRMSEVp = sqrt(ComSolution->SdUp2 / ComSolution->N);
		ComSolution->SRMSE3Dp = sqrt(sqr(ComSolution->SRMSEHp) + sqr(ComSolution->SRMSEVp));
		ComSolution->SRMSEHm = sqrt((ComSolution->SdEm2 + ComSolution->SdNm2) / ComSolution->N);
		ComSolution->SRMSEVm = sqrt(ComSolution->SdUm2 / ComSolution->N);
		ComSolution->SRMSE3Dm = sqrt(sqr(ComSolution->SRMSEHm) + sqr(ComSolution->SRMSEVm));
	}
}


