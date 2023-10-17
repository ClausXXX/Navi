/*
	Программа решения навигационной задачи ГНСС
	Navi
	© Затолокин Д.А., 2015-2021
*/
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#undef abs
#include <math.h>
#include <string.h>
//#include <alloc.h>
#include <dir.h>
#include <time.h>
#include <dirent.h>

#include "Constants.h"
#include "Common.h"
#include "DataFiles.h"
#include "RINEX.h"
#include "SP3.h"
#include "Ephemeris.h"
#include "Solution.h"
#include "Ionosphere.h"
#include "NeQuickG.h"
#include "Ionex.h"
#include "Troposphere.h"

//#ifndef MAXPATH
//#define MAXPATH 260
//#endif

//#define DEBUG

//void print_dir(char* dir_name)
//{
//  /* Open the directory and check for success */
//  DIR* dir = opendir(dir_name);
//  struct dirent* ent;
//
//  if(!dir)
//	return;
//
//  /* Navigate in the directory stream */
//  while(ent = readdir(dir))
//  {
//	/* print the name of the entry */
//	if(strcmp(ent->d_name, ".") && strcmp(ent->d_name, "..") && strstr(ent->d_name, ".dat"))
//	{
//		printf("%s\n", ent->d_name);
//	}
//  }
//  closedir(dir);
//}

int main(int argc, char **argv)
{
	char SattelitesThreshold;
	int i, j, k, l, m, Cycles, Index1, Index2, DOY, NOfTayAbsTECFiles = 0,
		IndexOfC1, IndexOfC2,
        IndexOfC5, IndexOfC7,
		IndexOfP1, IndexOfP2;
	register int Done;
	double tau, theta, deltat, TimeInSeconds, tec, f1, f2;
	clock_t t1, t2;
	size_t SizeOfChar = sizeof(char);
	size_t SizeOfCharPtr = sizeof(char*);
	size_t SizeOfDouble = sizeof(double);
	//size_t SizeOfDoublePtr = sizeof(double*);
	size_t SizeOfRINEXObs = sizeof(struct RINEXObs);
    //size_t SizeOfEpoch = sizeof(struct Epoch);
	size_t SizeOfRINEXNav = sizeof(struct RINEXNav);
	size_t SizeOfSP3 = sizeof(struct SP3);
	size_t SizeOfCurSolution = sizeof(struct CurSolution);
	size_t SizeOfTayAbsTEC = sizeof(struct TayAbsTEC);
	char *Pch, CurrentString[BYTES_OF_STRING], OutputFileName[MAXPATH],
		 OutputFileString[BYTES_OF_STRING], OutputFileParams[BYTES_OF_STRING],
		 RMSFileName[MAXPATH], RMSString[BYTES_OF_STRING],
		 CommandString[MAXPATH], CurrentPath[MAXPATH];
	double L[MAX_OF_VISIBLE_SATS],
		   A[MAX_OF_VISIBLE_SATS][MAX_OF_UNKNOWNS],
		   E[MAX_OF_UNKNOWNS][MAX_OF_UNKNOWNS],
		   R[3][3], RT[3][3], RE[3][3], Qxi[4][4];
	FILE *BDGIMFile = NULL, *GEMTECfile = NULL,
		 *IonexFile = NULL, *TECfile = NULL,
		 *outputfile = NULL,
		 *RMSfile = NULL; //*DTMfile = NULL;
	struct Settings Settings;
	struct RINEXObs *RINEXObs = NULL;
	struct RINEXNav *RINEXNav = NULL;
	struct SP3 *SP3 = NULL;
	struct Sattelite Sattelites[MAX_OF_VISIBLE_SATS];
	struct InterpolationPoints InterpolationPoints[MAX_OF_VISIBLE_SATS];
	struct GLOIon GLOIonData;
	struct GEMTECData GEMTECData;
	struct Weather Weather;
	struct CurSolution *CurSolution = NULL;
	struct ComSolution ComSolution;
	struct TayAbsTEC *TayAbsTEC = NULL;
	MODIP_st stModip;
	CCIR_st stCCIR;
	double pdRecvLLHdeg[3];
	double pdSatLLHdeg[3];
	double dModip;
	double ep[6];
	nav_t nav;
	gtime_t time;
	double pos[3];
	double azel[2];
	int opt;
	double delay;
	double var;
	//struct ffblk f;
	enum Ephemeris Ephemeris;
	DIR* dir;
	struct dirent* ent;

//	short Year;
//	char Month;
//	char Day;
//	char Hours;
//	char Minutes;
//	float Seconds;

	RINEXObs = (struct RINEXObs*)calloc(1, SizeOfRINEXObs);
	RINEXNav = (struct RINEXNav*)calloc(1, SizeOfRINEXNav);
	SP3 = (struct SP3*)calloc(1, SizeOfSP3);
	TayAbsTEC = (struct TayAbsTEC*)calloc(1, SizeOfTayAbsTEC);
	t1 = clock();
	SetDefaultSettings(&Settings);
   	if(!ReadSettings("navi.ini", &Settings))
	{
		WriteSettings("navi.ini", &Settings);
	}

//	tau = DateTimeToUNIXTime(2020, 01, 07,
//						  01, 59, 44.0);
//	printf("\n%lf\n", tau);
//	getch();
//
//	UNIXTimeToDateTime(tau, &Year, &Month, &Day,
//						&Hours, &Minutes, &Seconds);
//	printf("\n%hd %hhd %hhd %hhd %hhd %f\n", Year, Month, Day,
//						Hours, Minutes, Seconds);
//	getch();

#ifdef DEBUG
	printf("RINEXObsFilename=%s\n", Settings.RINEXObsFile);
	printf("EphemerisFilename=%s %s %s %s %s\n", Settings.EphemerisFiles[0], Settings.EphemerisFiles[1], Settings.EphemerisFiles[2], Settings.EphemerisFiles[3], Settings.EphemerisFiles[4]);
	printf("System=%s\n", Settings.System);
	printf("InitialTime=%hhd %hhd %.1f\n", Settings.InitialHours, Settings.InitialMinutes, Settings.InitialSeconds);
	printf("FinalTime=%hhd %hhd %.1f\n", Settings.FinalHours, Settings.FinalMinutes, Settings.FinalSeconds);
	printf("Step=%.1f\n", Settings.Step);
	printf("TruePosition=%.3lf %.3lf %.3lf\n", Settings.x, Settings.y, Settings.z);
	printf("ApproximatePosition=%.3lf %.3lf %.3lf\n", Settings.x0, Settings.y0, Settings.z0);
	printf("FrequencyMode=%s\n", Settings.FrequencyMode);
	printf("ExcludeSattelites=%s\n", Settings.ExcludeSattelitesString);
    printf("SpecSatFilename=%s\n", Settings->SpecSatFile);
	printf("ElevationMask=%.1f\n", Settings.ElevationMask);
    printf("SNRMask=%hhd\n", Settings.SNRMask);
	printf("CorrectIonosphereDelay=%c\n", Settings.Ionosphere);
	printf("Alpha=%E %E %E %E\n", Settings.Alpha[0], Settings.Alpha[1], Settings.Alpha[2], Settings.Alpha[3]);
	printf("Beta=%E %E %E %E\n", Settings.Beta[0], Settings.Beta[1], Settings.Beta[2], Settings.Beta[3]);
	printf("A=%.1f\n", Settings.A);
	printf("F10.7=%.1f\n", Settings.F107);
	printf("Ap=%.1f\n", Settings.Ap);
	printf("Az=%.3lf %.3lf %.3lf\n", Settings.Az[0], Settings.Az[1], Settings.Az[2]);
	printf("BDGIMFilename=%s\n", Settings.BDGIMFile);
	printf("IONEXFilename=%s\n", Settings.IONEXFile);
	printf("TECFile=%s\n", Settings.TECFile);
	printf("CorrectTroposphereDelay=%c\n", Settings.Trophosphere);
	printf("Weather=%.1f %.1f %.1f\n", Settings.p, Settings.t, Settings.r);
	printf("NumberOfIterations=%hhd\n", Settings.Iterations);
	printf("RMSThreshold=%.3lf\n", Settings.RMSThreshold);
	printf("GeodethicCoordinates=%hhd\n", Settings.Geodethic);
	printf("OutputType=%hhd\n", Settings.OutputType);
	printf("OutputPath=%s\n", Settings.OutputPath);
	printf("AccumulateOutputData=%hhd\n", Settings.AccumulateOutputData);
	printf("DifferentialTimeMode=%hhd\n", Settings.DTM);
#endif

	if(argc >= 3)
	{
		for(i = 1; i < argc; i++)
		{
			if(argv[i][strlen(argv[i]) - 1] == 'o' ||
			   strstr(argv[i], "O.rnx"))
			{
				strcpy(Settings.RINEXObsFile, argv[i]);
			}
            strcpy(Settings.EphemerisFiles[i - 1], "");
			if(argv[i][strlen(argv[i]) - 1] == 'n' ||
			   argv[i][strlen(argv[i]) - 1] == 'g' ||
			   argv[i][strlen(argv[i]) - 1] == 'l' ||
			   argv[i][strlen(argv[i]) - 1] == 'p' ||
			   strstr(argv[i], "N.rnx") ||
			   strstr(argv[i], ".sp3"))
			{
				strcpy(Settings.EphemerisFiles[i - 1], argv[i]);
			}
		}
	}
    RINEXObs->x = 1.0;
	RINEXObs->y = 1.0;
	RINEXObs->z = 1.0;
	if(!ReadRINEXObsHeader(Settings.RINEXObsFile, RINEXObs))
	{
        printf("Unable to open observation file %s", Settings.RINEXObsFile);
		if(Settings.OutputType == TO_SCREEN || Settings.OutputType == TO_SCREEN_FILE)
		{
			getch();
		}
		return 1;
	}
	ReadRINEXObs(Settings.RINEXObsFile, RINEXObs);
    Ephemeris = BOARD;
	for(i = 0; i < MAX_OF_SYSTEMS + 1; i++)
	{
	    if(strcmp(Settings.EphemerisFiles[i], ""))
		{
            if(strstr(Settings.EphemerisFiles[i], ".sp3"))
			{
				Ephemeris = PRECISE;
			}
			else
			{
				if(!ReadRINEXNavIonData(Settings.EphemerisFiles[i], RINEXNav))
				{
					printf("Unable to open ephemeris file %s", Settings.EphemerisFiles[i]);
					if(Settings.OutputType == TO_SCREEN || Settings.OutputType == TO_SCREEN_FILE)
					{
						getch();
					}
					return 1;
				}
			}
		}
    }

#ifdef DEBUG
	printf("Kolbuchar\n%E %E %E %E\n%E %E %E %E", RINEXNav->AlphaGPS[0],
												  RINEXNav->AlphaGPS[1],
												  RINEXNav->AlphaGPS[2],
												  RINEXNav->AlphaGPS[3],
												  RINEXNav->BetaGPS[0],
												  RINEXNav->BetaGPS[1],
												  RINEXNav->BetaGPS[2],
												  RINEXNav->BetaGPS[3]);
	printf("\nNeQuickG\n%E %E %E",
		   RINEXNav->AzGAL[0], RINEXNav->AzGAL[1], RINEXNav->AzGAL[2]);
	getch();
#endif

	if(Ephemeris == BOARD)
	{
		for(i = 0; i < MAX_OF_SYSTEMS + 1; i++)
		{
			if(strcmp(Settings.EphemerisFiles[i], "") &&
			   !strstr(Settings.EphemerisFiles[i], ".sp3"))
			{
				if(!ReadRINEXEphemeris(Settings.EphemerisFiles[i], RINEXNav))
				{
					printf("Unable to open ephemeris file %s", Settings.EphemerisFiles[i]);
					if(Settings.OutputType == TO_SCREEN || Settings.OutputType == TO_SCREEN_FILE)
					{
						getch();
					}
					return 1;
				}
//                printf("\n%s\n", Settings.EphemerisFiles[i]);
//				getch();
			}
		}

		if(RINEXNav->NOfGPSEphemeris +
		   RINEXNav->NOfGLOEphemeris +
		   RINEXNav->NOfGALEphemeris +
		   RINEXNav->NOfBDSEphemeris == 0)
		{
			printf("Broadcast ephemeris not loaded");
			if(Settings.OutputType == TO_SCREEN || Settings.OutputType == TO_SCREEN_FILE)
			{
				getch();
			}
			return 1;
		}
	}

	if(Ephemeris == PRECISE)
	{
		for(i = 0; i < MAX_OF_SYSTEMS + 1; i++)
		{
			if(strstr(Settings.EphemerisFiles[i], ".sp3"))
			{
				if(!ReadSP3Ephemeris(Settings.EphemerisFiles[i], SP3))
				{
                    printf("Unable to open ephemeris file %s", Settings.EphemerisFiles[i]);
					if(Settings.OutputType == TO_SCREEN || Settings.OutputType == TO_SCREEN_FILE)
					{
						getch();
					}
					return 1;
                }
			}
		}

		if(SP3->NOfGPSEphemeris +
		   SP3->NOfGLOEphemeris +
		   SP3->NOfGALEphemeris +
		   SP3->NOfBDSEphemeris == 0)
		{
			printf("Precise ephemeris not loaded");
			if(Settings.OutputType == TO_SCREEN || Settings.OutputType == TO_SCREEN_FILE)
			{
				getch();
			}
			return 1;
		}
	}

	// Выбираем предварительно число неизвестных, соответствующее желаемому набору систем в решении,
	// учитывая  только то, эфемериды каких спутников есть в наличии, если эфемерид нет, убираем соответствующую
	//  букву системы из списка
	SattelitesThreshold = 3;
	for(i = 0; i < (int)strlen(Settings.System); i++)
	{
		if(Settings.System[i] == 'G')
		{
			if(RINEXNav->NOfGPSEphemeris || SP3->NOfGPSEphemeris)
			{
				SattelitesThreshold++;
			}
			else
			{
				Settings.System[i] = ' ';
			}
		}

		if(Settings.System[i] == 'R')
		{
			if(RINEXNav->NOfGLOEphemeris || SP3->NOfGLOEphemeris)
			{
				SattelitesThreshold++;
			}
			else
			{
				Settings.System[i] = ' ';
			}
		}

		if(Settings.System[i] == 'E')
		{
			if(RINEXNav->NOfGALEphemeris || SP3->NOfGALEphemeris)
			{
				SattelitesThreshold++;
			}
			else
			{
				Settings.System[i] = ' ';
			}
		}

		if(Settings.System[i] == 'C')
		{
			if(RINEXNav->NOfBDSEphemeris || SP3->NOfBDSEphemeris)
			{
				SattelitesThreshold++;
			}
			else
			{
				Settings.System[i] = ' ';
			}
		}

        if(Settings.System[i] == ' ')
		{
			for(j = i; j < (int)strlen(Settings.System); ++j)
			{
				Settings.System[j] = Settings.System[j + 1];
			}
		}
	}

	if(!Settings.InitialHours &&
	   !Settings.InitialMinutes &&
	   !Settings.InitialSeconds &&
	   !Settings.FinalHours &&
	   !Settings.FinalMinutes &&
	   !Settings.FinalSeconds)
	{
		printf("Enter initial time (h m s) ");
		scanf("%hhd %hhd %f", &Settings.InitialHours,
							  &Settings.InitialMinutes,
							  &Settings.InitialSeconds);
		printf("Enter final time (h m s) ");
		scanf("%hhd %hhd %f", &Settings.FinalHours,
							  &Settings.FinalMinutes,
							  &Settings.FinalSeconds);
	}

	if(RINEXObs->Interval)
	{
		if(!Settings.Step)
		{
			Settings.Step = RINEXObs->Interval;
		}
	}
	else
	{
        if(Settings.Step)
		{
			RINEXObs->Interval = Settings.Step;
		}
		else
		{
			RINEXObs->Interval = Settings.Step = 30.0;
		}
	}
	CurSolution = (struct CurSolution*)calloc(RINEXObs->NOfEpochs, SizeOfCurSolution);
	if(Settings.x || Settings.y || Settings.z)
	{
		ComSolution.Qp[0] = Settings.x;
		ComSolution.Qp[1] = Settings.y;
		ComSolution.Qp[2] = Settings.z;
	}
	else
	{
		ComSolution.Qp[0] = RINEXObs->x;
		ComSolution.Qp[1] = RINEXObs->y;
		ComSolution.Qp[2] = RINEXObs->z;
	}

	if(Settings.x0 || Settings.y0 || Settings.z0)
	{
		CurSolution[0].Q[0] = Settings.x0;
		CurSolution[0].Q[1] = Settings.y0;
		CurSolution[0].Q[2] = Settings.z0;
	}
	else
	{
		CurSolution[0].Q[0] = RINEXObs->x;
		CurSolution[0].Q[1] = RINEXObs->y;
		CurSolution[0].Q[2] = RINEXObs->z;
	}
   //	printf("\n%lf %lf %lf\n", CurSolution[RINEXObs->CurrentEpoch].Q[0], CurSolution[RINEXObs->CurrentEpoch].Q[1], CurSolution[RINEXObs->CurrentEpoch].Q[2]);
	if(fabs(CurSolution[0].Q[0]) >= 1.0E+7 ||
	   fabs(CurSolution[0].Q[1]) >= 1.0E+7 ||
	   fabs(CurSolution[0].Q[2]) >= 1.0E+7)
	{
		printf("Incorrect initial coordinate values");
		if(Settings.OutputType == TO_SCREEN || Settings.OutputType == TO_SCREEN_FILE)
		{
			getch();
		}
		return 1;
	}

    if(RINEXObs->x == 1 && RINEXObs->y == 1 && RINEXObs->z == 1)
	{
		Settings.RMSThreshold = 0.0;
	}

	Settings.ExcludeSattelites = (char**)calloc(1, SizeOfCharPtr);
	i = 0;
	Pch = strpbrk(Settings.ExcludeSattelitesString, "GRESC");
	while(Pch)
	{
		Settings.ExcludeSattelites =
		(char**)realloc(Settings.ExcludeSattelites, (i + 1) * SizeOfCharPtr);
		Settings.ExcludeSattelites[i] = (char*)calloc(2, SizeOfChar);
		Settings.ExcludeSattelites[i][0] = Pch[0];
		Pch = strpbrk(Pch + 1, "GRESC");
		i++;
	}
	Settings.NOfExcludeSattelites = i;
	i = 0;
	Pch = strtok(Settings.ExcludeSattelitesString, " GRESC");
	while(Pch)
	{
		Settings.ExcludeSattelites[i][1] = atoi(Pch);
		Pch = strtok(NULL, " GRESC");
		i++;
	}

	if(strstr(Settings.FrequencyMode, "DC") ||
	   strstr(Settings.FrequencyMode, "DP") ||
	   strstr(Settings.FrequencyMode, "DA"))
	{
        Settings.Ionosphere = 0;
	}

	if(Settings.Ionosphere == 'K')
	{
		if(Settings.Alpha[0] || Settings.Alpha[1] ||
		   Settings.Alpha[2] || Settings.Alpha[3] ||
		   Settings.Beta[0] || Settings.Beta[1] ||
		   Settings.Beta[2] || Settings.Beta[3])
		{
            RINEXNav->AlphaGPS[0] = Settings.Alpha[0];
			RINEXNav->AlphaGPS[1] = Settings.Alpha[1];
			RINEXNav->AlphaGPS[2] = Settings.Alpha[2];
			RINEXNav->AlphaGPS[3] = Settings.Alpha[3];
			RINEXNav->BetaGPS[0] = Settings.Beta[0];
			RINEXNav->BetaGPS[1] = Settings.Beta[1];
			RINEXNav->BetaGPS[2] = Settings.Beta[2];
			RINEXNav->BetaGPS[3] = Settings.Beta[3];
		}
		else
		{
			if(!RINEXNav->AlphaGPS[0] && !RINEXNav->AlphaGPS[1] &&
			   !RINEXNav->AlphaGPS[2] && !RINEXNav->AlphaGPS[3] &&
			   !RINEXNav->BetaGPS[0] && !RINEXNav->BetaGPS[1] &&
			   !RINEXNav->BetaGPS[2] && !RINEXNav->BetaGPS[3])
		   {
				printf("There are no coefficients of the Klobuchar ionosphere model");
				if(Settings.OutputType == TO_SCREEN || Settings.OutputType == TO_SCREEN_FILE)
				{
					getch();
				}
				return 1;
		   }
		}
	}

	if(Settings.Ionosphere == 'N')
	{
		if(Settings.Az[0] || Settings.Az[1] || Settings.Az[2])
		{
            RINEXNav->AzGAL[0] = Settings.Az[0];
			RINEXNav->AzGAL[1] = Settings.Az[1];
			RINEXNav->AzGAL[2] = Settings.Az[2];
		}
		else
		{
			if(!RINEXNav->AzGAL[0] && !RINEXNav->AzGAL[1] && !RINEXNav->AzGAL[2])
			{
				printf("There are no coefficients of the NeQuickG ionosphere model");
				if(Settings.OutputType == TO_SCREEN || Settings.OutputType == TO_SCREEN_FILE)
				{
					getch();
				}
				return 1;
			}
        }
		loadmodipgrid(MODIPFILE, &stModip);
		loadccirdata(CCIRFILEPATTERN, &stCCIR);
	}

	if(Settings.Ionosphere == 'B')
	{
		BDGIMFile = fopen(Settings.BDGIMFile, "r");
        if(!BDGIMFile)
		{
			printf("Unable to open BDGIM file %s", Settings.BDGIMFile);
			if(Settings.OutputType == TO_SCREEN || Settings.OutputType == TO_SCREEN_FILE)
			{
				getch();
			}
			return 1;
		}
		Pch = FindStringWithData(BDGIMFile, "IONOSPHERIC CORR");
		if(Pch)
		{
			strcpy(CurrentString, Pch);
			for(i = 0; i < 12; i++)
			{
				sscanf(CurrentString, "%*s%lf%lf%lf", &RINEXNav->AlphaBDS[i][0], &RINEXNav->AlphaBDS[i][1], &RINEXNav->AlphaBDS[i][2]);
				fgets(CurrentString, BYTES_OF_STRING, BDGIMFile);
				sscanf(CurrentString, "%*s%lf%lf%lf", &RINEXNav->AlphaBDS[i][3], &RINEXNav->AlphaBDS[i][4], &RINEXNav->AlphaBDS[i][5]);
				fgets(CurrentString, BYTES_OF_STRING, BDGIMFile);
				sscanf(CurrentString, "%*s%lf%lf%lf", &RINEXNav->AlphaBDS[i][6], &RINEXNav->AlphaBDS[i][7], &RINEXNav->AlphaBDS[i][8]);
				fgets(CurrentString, BYTES_OF_STRING, BDGIMFile);
				//printf("\n%lf %lf %lf", RINEXNav->AlphaBDS[i][0], RINEXNav->AlphaBDS[i][1], RINEXNav->AlphaBDS[i][2]);
			}
		}
		fclose(BDGIMFile);
	}

#ifdef DEBUG
    printf("\nBDGIM\n");
	for(i = 0; i < 12; i++)
	{
		for(j = 0; j < 3; j++)
		{
			printf("%E %E %E\n", RINEXNav->AlphaBDS[i][j],
								 RINEXNav->AlphaBDS[i][j + 1],
								 RINEXNav->AlphaBDS[i][j + 2]);
		}
		printf("\n");
	}
	getch();
#endif

	if(Settings.Ionosphere == 'G')
	{
		GEMTECfile = fopen("GEMTEC\\aio.bin", "rb");
		if(!GEMTECfile)
		{
			printf("Unable to open GEMTEC file aio.bin");
			if(Settings.OutputType == TO_SCREEN || Settings.OutputType == TO_SCREEN_FILE)
			{
				getch();
			}
			return 1;
		}
		fread(GEMTECData.A, 4160, 1, GEMTECfile);
		fread(GEMTECData.evt, 104, 1, GEMTECfile);
		fread(GEMTECData.evm, 416, 1, GEMTECfile);
		fread(GEMTECData.evl, 2400, 1, GEMTECfile);
		fread(GEMTECData.evf, 640, 1, GEMTECfile);
		//printf("%lf",GEMTEC(50.0,13.75,45.0,125.3,3,4, evt, evm, evl, evf, A_G));
		fclose(GEMTECfile);
	}

	if(Settings.Ionosphere == 'I')
	{
		IonexFile = fopen(Settings.IONEXFile, "r");
		if(!IonexFile)
		{
			printf("Unable to open Ionex file %s", Settings.IONEXFile);
			if(Settings.OutputType == TO_SCREEN || Settings.OutputType == TO_SCREEN_FILE)
			{
				getch();
			}
			return 1;
		}
        fclose(IonexFile);
		readtec(Settings.IONEXFile, &nav, 0);
	}

	if(Settings.Ionosphere == 'M')
	{
		TECfile = fopen(Settings.TECFile, "r");
		if(!TECfile)
		{
			printf("Unable to open TEC map file %s", Settings.TECFile);
			if(Settings.OutputType == TO_SCREEN || Settings.OutputType == TO_SCREEN_FILE)
			{
				getch();
			}
			return 1;
		}
		fclose(TECfile);
		ConvertTECToIONEX(Settings.TECFile, Settings.IONEXFile);
		readtec(Settings.IONEXFile, &nav, 0);
	}

	if(Settings.Ionosphere == 'v')
	{
		if(!ReadTayAbsTECData(Settings.TECFile, TayAbsTEC))
		{
			printf("Unable to open Taylor Absolute TEC file %s", Settings.TECFile);
			if(Settings.OutputType == TO_SCREEN || Settings.OutputType == TO_SCREEN_FILE)
			{
				getch();
			}
			return 1;
		}
	}

	if(Settings.Ionosphere == 's')
	{
	   //	print_dir(Settings.TayAbsTECPath);
	   //	getch();

		getcwd(CurrentPath, MAXPATH);
		dir = opendir(Settings.TayAbsTECPath);
		if(chdir(Settings.TayAbsTECPath) == -1)
		{
			printf("Unable to open Taylor Absolute TEC directory %s", Settings.TayAbsTECPath);
			if(Settings.OutputType == TO_SCREEN || Settings.OutputType == TO_SCREEN_FILE)
			{
				getch();
			}
			return 1;
		}

		if(!dir)
		{
			printf("Unable to open Taylor Absolute TEC directory %s", Settings.TayAbsTECPath);
			if(Settings.OutputType == TO_SCREEN || Settings.OutputType == TO_SCREEN_FILE)
			{
				getch();
			}
			return 1;
		}

		//Done = findfirst("*.dat", &f, 0);
		//while(!Done)
		while(ent = readdir(dir))
		{
			if(strcmp(ent->d_name, ".") && strcmp(ent->d_name, "..") && strstr(ent->d_name, ".dat"))
			{
				sscanf(ent->d_name, "%*[^_]_%c%hhd", &TayAbsTEC[NOfTayAbsTECFiles].SatNumber[0],
												 &TayAbsTEC[NOfTayAbsTECFiles].SatNumber[1]);
				TayAbsTEC[NOfTayAbsTECFiles].SatNumber[1] -= (TayAbsTEC[NOfTayAbsTECFiles].SatNumber[1] / 37) * 37;
	//			printf("\n%c%d\n", TayAbsTEC[NOfTayAbsTECFiles].SatNumber[0],
	//							   TayAbsTEC[NOfTayAbsTECFiles].SatNumber[1]);
				k = 0;
				for(i = 0; i < NOfTayAbsTECFiles - 1; i++)
				{
					if(TayAbsTEC[i].SatNumber[0] == TayAbsTEC[NOfTayAbsTECFiles].SatNumber[0] &&
					   TayAbsTEC[i].SatNumber[1] == TayAbsTEC[NOfTayAbsTECFiles].SatNumber[1])
					{
						if(!ReadTayAbsTECData(ent->d_name, &TayAbsTEC[i]))
						{
							printf("Unable to open Taylor Absolute TEC file %s", ent->d_name);
							if(Settings.OutputType == TO_SCREEN || Settings.OutputType == TO_SCREEN_FILE)
							{
								getch();
							}
							return 1;
						}
						k = 1;
					}
				}

				if(!k)
				{
					if(!ReadTayAbsTECData(ent->d_name, &TayAbsTEC[NOfTayAbsTECFiles]))
					{
						printf("Unable to open Taylor Absolute TEC file %s", ent->d_name);
						if(Settings.OutputType == TO_SCREEN || Settings.OutputType == TO_SCREEN_FILE)
						{
							getch();
						}
						return 1;
					}
					else
					{
						TayAbsTEC = (struct TayAbsTEC*)realloc(TayAbsTEC, SizeOfTayAbsTEC * (++NOfTayAbsTECFiles + 1));
						TayAbsTEC[NOfTayAbsTECFiles].NOfRecords = 0;
					}
				}
			}
			//Done = findnext(&f);
		}
        closedir(dir);
		chdir(CurrentPath);
		for(i = 0; i < NOfTayAbsTECFiles; i++)
		{
			qsort(TayAbsTEC[i].TECRecords, TayAbsTEC[i].NOfRecords, sizeof(struct TECRecord), SortFunctionTEC);
		}
	}

//	for(i = 0; i < NOfTayAbsTECFiles; i++)
//	{
//		for(j = 0; j < TayAbsTEC[i].NOfRecords; j++)
//		{
//			printf("\n%c%d %f %f", TayAbsTEC[i].SatNumber[0], TayAbsTEC[i].SatNumber[1],
//							   TayAbsTEC[i].TECRecords[j].UT, TayAbsTEC[i].TECRecords[j].TEC);
//		}
//        getch();
//	}

	if(Settings.Trophosphere == 'S' || Settings.Trophosphere == 'D')
	{
		if(Settings.p)
		{
			Weather.p = Settings.p;
			Weather.t = Settings.t;
			Weather.T = Settings.t + 273.15;
			Weather.r = Settings.r;
			Weather.e = 6.108 * Weather.r / 100.0 *
						exp((17.15 * Weather.T - 4684.0) / (Weather.T - 38.45));
		}
	}
	ReadSpecSatData(Settings.SpecSatFile, RINEXObs);

	if(!RINEXObs->LeapSeconds)
	{
		RINEXObs->LeapSeconds = RINEXNav->LeapSeconds;
	}

	if(Settings.OutputType == TO_SCREEN || Settings.OutputType == TO_SCREEN_FILE)
	{
		printf(" RINEXObsFilename = %s\n", Settings.RINEXObsFile);
		printf(" EphemerisFilename = %s %s %s %s %s\n", Settings.EphemerisFiles[0], Settings.EphemerisFiles[1], Settings.EphemerisFiles[2], Settings.EphemerisFiles[3], Settings.EphemerisFiles[4]);
		printf(" System = %s\n", Settings.System);
		printf(" InitialTime = %hhd %hhd %.1f\n", Settings.InitialHours, Settings.InitialMinutes, Settings.InitialSeconds);
		printf(" FinalTime = %hhd %hhd %.1f\n", Settings.FinalHours, Settings.FinalMinutes, Settings.FinalSeconds);
		printf(" Step = %.1f\n", Settings.Step);
		printf(" FrequencyMode = %s\n", Settings.FrequencyMode);
        printf(" GalileoDataType = %c\n", Settings.GalileoDataType);
		printf(" ExcludeSattelites = %s\n", Settings.ExcludeSattelitesString);
        printf(" SpecSatFilename = %s\n", Settings.SpecSatFile);
		printf(" ElevationMask = %.1f\n", Settings.ElevationMask);
        printf(" SNRMask = %hhd\n", Settings.SNRMask);
		printf(" CorrectIonosphereDelay = %c\n", Settings.Ionosphere);
		printf(" Alpha = %E %E %E %E\n", Settings.Alpha[0], Settings.Alpha[1], Settings.Alpha[2], Settings.Alpha[3]);
		printf(" Beta = %E %E %E %E\n", Settings.Beta[0], Settings.Beta[1], Settings.Beta[2], Settings.Beta[3]);
		printf(" A = %.1f\n", Settings.A);
		printf(" F10.7 = %.1f\n", Settings.F107);
		printf(" Ap = %.1f\n", Settings.Ap);
		printf(" Az = %.3lf %.3lf %.3lf\n", Settings.Az[0], Settings.Az[1], Settings.Az[2]);
		printf(" BDGIMFilename = %s\n", Settings.BDGIMFile);
		printf(" IONEXFilename = %s\n", Settings.IONEXFile);
		printf(" TECFilename = %s\n", Settings.TECFile);
        printf(" TayAbsTECPath = %s\n", Settings.TayAbsTECPath);
		printf(" CorrectTroposphereDelay = %c\n", Settings.Trophosphere);
		printf(" Weather = %.1f %.1f %.1f\n", Settings.p, Settings.t, Settings.r);
		printf(" NumberOfIterations = %hhd\n", Settings.Iterations);
		printf(" RMSThreshold = %.3lf\n", Settings.RMSThreshold);
		printf(" DifferentialTimeMode = %hhd\n", Settings.DTM);
		DecartToGeo(ComSolution.Qp[0], ComSolution.Qp[1], ComSolution.Qp[2],
					&Settings.B, &Settings.L, &Settings.H);
		DecartToGeo(CurSolution[0].Q[0], CurSolution[0].Q[1], CurSolution[0].Q[2],
					&CurSolution[0].B, &CurSolution[0].L, &CurSolution[0].H);
		printf("\n True receiver position:");
		printf("\n  x = %12.3lf m   B = %13.8lf \xF8", ComSolution.Qp[0], Settings.B);
		printf("\n  y = %12.3lf m   L = %13.8lf \xF8", ComSolution.Qp[1], Settings.L);
		printf("\n  z = %12.3lf m   H = %13.3lf m",  ComSolution.Qp[2], Settings.H);
        printf("\n\n Approximate receiver position:");
		printf("\n x0 = %12.3lf m  B0 = %13.8lf \xF8", CurSolution[0].Q[0], CurSolution[0].B);
		printf("\n y0 = %12.3lf m  L0 = %13.8lf \xF8", CurSolution[0].Q[1], CurSolution[0].L);
		printf("\n z0 = %12.3lf m  H0 = %13.3lf m", CurSolution[0].Q[2], CurSolution[0].H);
		printf("\n\n --------------------------------------------------------------------------");
		printf("\n    y  m  d  h  m  s       x(m)          y(m)         z(m)        cdt(m)");
		if(Settings.Geodethic)
		{
			printf("\n                           B(\xF8)          L(\xF8)         H(m)");
		}
		printf("\n                            SdEN(m)       SdU(m)       SdENU(m)");
		printf("\n --------------------------------------------------------------------------");
	}

	if(Settings.OutputType == TO_FILE || Settings.OutputType == TO_SCREEN_FILE)
	{
		if(Settings.OutputType == TO_FILE)
		{
			printf("\nProcessing...\n\n");
		}
		mkdir(Settings.OutputPath);
		chdir(Settings.OutputPath);
		strcpy(CurrentString, "");
		strcpy(CurrentString, Settings.RINEXObsFile);
		Pch = strtok(CurrentString, "\\");
		while(Pch && !strstr(Pch, "."))
		{
			Pch = strtok(NULL, "\\");
		}

		for(i = 0; i < (int)strlen(Pch); i++)
		{
			if(Pch[i] == '.')
			{
				Pch[i] = '_';
				if(Pch[i + 3] == 'o')
				{
					Pch[i + 3] = '\0';
				}
			}
		}
		strcpy(OutputFileString, Pch);
		strncpy(RMSString, Pch, 4);
		RMSString[4] = '\0';

		if(Ephemeris == BOARD)
		{
			strcpy(OutputFileParams, "_RNX");
		}

		if(Ephemeris == PRECISE)
		{
			strcpy(OutputFileParams, "_SP3");
		}

		strcat(OutputFileParams, "_");
		for(i = 0; i < (int)strlen(Settings.System); i++)
		{
			strcpy(CurrentString, OutputFileParams);
			if(strchr("GREC", Settings.System[i]))
			{
				sprintf(OutputFileParams, "%s%c", CurrentString, Settings.System[i]);
                if(i + 1 < (int)strlen(Settings.System))
				{
					strcat(OutputFileParams, "+");
				}
			}
		}

		if(strstr(Settings.FrequencyMode, "SC") ||
		   strstr(Settings.FrequencyMode, "SP"))
		{
			if(strstr(Settings.FrequencyMode, "SC1"))
			{
				strcat(OutputFileParams, "_C1");
			}

			if(strstr(Settings.FrequencyMode, "SC2"))
			{
				strcat(OutputFileParams, "_C2");
			}

			if(strstr(Settings.FrequencyMode, "SP1"))
			{
				strcat(OutputFileParams, "_P1");
			}

			if(strstr(Settings.FrequencyMode, "SP2"))
			{
				strcat(OutputFileParams, "_P2");
			}

			if(Settings.Ionosphere)
			{
				strcpy(CurrentString, OutputFileParams);
				sprintf(OutputFileParams, "%s_I%c", CurrentString, Settings.Ionosphere);
			}
		}

		if(strstr(Settings.FrequencyMode, "DC"))
		{
			strcat(OutputFileParams, "_C1+C2");
		}

		if(strstr(Settings.FrequencyMode, "DP"))
		{
			strcat(OutputFileParams, "_P1+P2");
		}

		if(strstr(Settings.FrequencyMode, "DA"))
		{
			strcat(OutputFileParams, "_X1+X2");
		}

		if(Settings.Trophosphere)
		{
			strcpy(CurrentString, OutputFileParams);
			sprintf(OutputFileParams, "%s_T%c", CurrentString, Settings.Trophosphere);
		}

		if(Settings.AccumulateOutputData)
		{
			strcpy(OutputFileString, RMSString);
		}
		sprintf(OutputFileName, "%s%s.txt", OutputFileString, OutputFileParams);
		sprintf(RMSFileName, "RMS_%s%s.txt", RMSString, OutputFileParams);
		if(Settings.AccumulateOutputData)
		{
			outputfile = fopen(OutputFileName, "r");
			if(!outputfile)
			{
				outputfile = fopen(OutputFileName, "w");
                if(!outputfile)
				{
					printf("Unable to create or open output file %s", OutputFileName);
					if(Settings.OutputType == TO_SCREEN || Settings.OutputType == TO_SCREEN_FILE)
					{
						getch();
					}
					return 1;
				}
				WriteOutputFileHeader(outputfile, &Settings, CurSolution, &ComSolution);
			}
			else
			{
				fclose(outputfile);
				outputfile = fopen(OutputFileName, "a");
			}
		}
		else
		{
			outputfile = fopen(OutputFileName, "w");
            if(!outputfile)
			{
				printf("Unable to create or open output file %s", OutputFileName);
				if(Settings.OutputType == TO_SCREEN || Settings.OutputType == TO_SCREEN_FILE)
				{
					getch();
				}
				return 1;
			}
			WriteOutputFileHeader(outputfile, &Settings, CurSolution, &ComSolution);
		}
	}
	RINEXObs->CurrentEpoch = 0;
	Settings.InitialTime = RINEXObs->Epochs[RINEXObs->CurrentEpoch].t +
							Settings.InitialHours * 3600 +
							Settings.InitialMinutes * 60 +
							Settings.InitialSeconds;
	Settings.FinalTime = RINEXObs->Epochs[RINEXObs->CurrentEpoch].t +
						  Settings.FinalHours * 3600 +
						  Settings.FinalMinutes * 60 +
						  Settings.FinalSeconds;
	while(RINEXObs->Epochs[RINEXObs->CurrentEpoch].t < Settings.InitialTime)
	{
		RINEXObs->CurrentEpoch++;
	}
	ComSolution.SdEp2 = 0.0;
	ComSolution.SdNp2 = 0.0;
	ComSolution.SdUp2 = 0.0;
	ComSolution.SdEm2 = 0.0;
	ComSolution.SdNm2 = 0.0;
	ComSolution.SdUm2 = 0.0;
	ComSolution.N = 0;
	for(i = 0; i < 3; i++)
	{
		ComSolution.Q[i] = (double*)calloc(RINEXObs->NOfEpochs, SizeOfDouble);
		ComSolution.dQp[i] = (double*)calloc(RINEXObs->NOfEpochs, SizeOfDouble);
		ComSolution.dQm[i] = (double*)calloc(RINEXObs->NOfEpochs, SizeOfDouble);
	}
	IndexOfC1 = -1;
	IndexOfC2 = -1;
	IndexOfP1 = -1;
	IndexOfP2 = -1;
	IndexOfC5 = -1;
	IndexOfC7 = -1;
	while(RINEXObs->Epochs[RINEXObs->CurrentEpoch].t <=
		  Settings.FinalTime && RINEXObs->CurrentEpoch < RINEXObs->NOfEpochs)
	{
		CurSolution[RINEXObs->CurrentEpoch].NOfValidSattelites = 0;
		Cycles = 0;
		CurSolution[RINEXObs->CurrentEpoch].Q[0] = CurSolution[0].Q[0];
		CurSolution[RINEXObs->CurrentEpoch].Q[1] = CurSolution[0].Q[1];
		CurSolution[RINEXObs->CurrentEpoch].Q[2] = CurSolution[0].Q[2];
		for(i = 0; i < RINEXObs->Epochs[RINEXObs->CurrentEpoch].NOfSattelites; i++)
		{
			if(RINEXObs->Version < 3.0)
			{
                j = 0;
			}
			else
			{
				j = GetIndex(RINEXObs->Systems, RINEXObs->Epochs[RINEXObs->CurrentEpoch].Sattelites[i][0]);
            }

			if(RINEXObs->Epochs[RINEXObs->CurrentEpoch].Sattelites[i][0] == 'G')
			{
                IndexOfC1 = RINEXObs->IndexOfC1C[j];
				IndexOfC2 = RINEXObs->IndexOfC2C[j];
				IndexOfP1 = RINEXObs->IndexOfC1P[j];
				IndexOfP2 = RINEXObs->IndexOfC2P[j];
			}

			if(RINEXObs->Epochs[RINEXObs->CurrentEpoch].Sattelites[i][0] == 'R')
			{
                IndexOfC1 = RINEXObs->IndexOfC1C[j];
				IndexOfC2 = RINEXObs->IndexOfC2C[j];
                IndexOfP1 = RINEXObs->IndexOfC1P[j];
				IndexOfP2 = RINEXObs->IndexOfC2P[j];
			}

			if(RINEXObs->Epochs[RINEXObs->CurrentEpoch].Sattelites[i][0] == 'E')
			{
				IndexOfC1 = RINEXObs->IndexOfC1C[j];
				IndexOfC5 = RINEXObs->IndexOfC5Q[j];
				IndexOfC7 = RINEXObs->IndexOfC7Q[j];
			}

			if(RINEXObs->Epochs[RINEXObs->CurrentEpoch].Sattelites[i][0] == 'C')
			{
				IndexOfC1 = RINEXObs->IndexOfC2I[j];
				IndexOfC2 = RINEXObs->IndexOfC7I[j];
			}

			if(strstr(Settings.FrequencyMode, "SC1"))
			{
				if(IndexOfC1 >= 0)
				{
					Sattelites[i].P[0] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfC1][0];
                    Sattelites[i].P[1] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfC1][1];
				}
				else
				{
					Sattelites[i].P[0] = 0.0;
				}

				if(Sattelites[i].P[0] <= 0.0)
				{
					Sattelites[i].Valid = 0;
				}
				else
				{
					Sattelites[i].Valid = 1;
					if(Sattelites[i].P[1] > 0.0 && Sattelites[i].P[1] < Settings.SNRMask)
					{
						Sattelites[i].Valid = 0;
					}
				}
			}

			if(strstr(Settings.FrequencyMode, "SC2"))
			{
				if(RINEXObs->Epochs[RINEXObs->CurrentEpoch].Sattelites[i][0] != 'E')
				{
					if(IndexOfC2 >= 0)
					{
						Sattelites[i].P[0] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfC2][0];
						Sattelites[i].P[1] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfC2][1];
					}
					else
					{
						Sattelites[i].P[0] = 0.0;
					}

					if(Sattelites[i].P[0] <= 0.0)
					{
						Sattelites[i].Valid = 0;
					}
					else
					{
						Sattelites[i].Valid = 1;
						if(Sattelites[i].P[1] > 0.0 && Sattelites[i].P[1] < Settings.SNRMask)
						{
							Sattelites[i].Valid = 0;
						}
					}
				}
				else
				{
					if(Settings.GalileoDataType == 'F')
					{
						if(IndexOfC5 >= 0)
						{
							Sattelites[i].PC5[0] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfC5][0];
                            Sattelites[i].PC5[1] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfC5][1];
						}
						else
						{
							Sattelites[i].PC5[0] = 0.0;
						}

						if(Sattelites[i].PC5[0] <= 0.0)
						{
							Sattelites[i].Valid = 0;
						}
						else
						{
							Sattelites[i].Valid = 1;
							if(Sattelites[i].PC5[1] > 0.0 && Sattelites[i].PC5[1] < Settings.SNRMask)
							{
								Sattelites[i].Valid = 0;
							}
						}
					}

					if(Settings.GalileoDataType == 'I')
					{
						if(IndexOfC7 >= 0)
						{
							Sattelites[i].PC7[0] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfC7][0];
                            Sattelites[i].PC7[1] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfC7][1];
						}
						else
						{
							Sattelites[i].PC7[0] = 0.0;
						}

						if(Sattelites[i].PC7[0] <= 0.0)
						{
							Sattelites[i].Valid = 0;
						}
						else
						{
							Sattelites[i].Valid = 1;
                            if(Sattelites[i].PC7[1] > 0.0 && Sattelites[i].PC7[1] < Settings.SNRMask)
							{
								Sattelites[i].Valid = 0;
							}
						}
					}
				}
			}

			if(strstr(Settings.FrequencyMode, "SP1"))
			{
				if(IndexOfP1 >= 0)
				{
					Sattelites[i].P[0] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfP1][0];
                    Sattelites[i].P[1] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfP1][1];
				}
				else
				{
					Sattelites[i].P[0] = 0.0;
				}

				if(Sattelites[i].P[0] <= 0.0)
				{
					Sattelites[i].Valid = 0;
				}
				else
				{
					Sattelites[i].Valid = 1;
					if(Sattelites[i].P[1] > 0.0 && Sattelites[i].P[1] < Settings.SNRMask)
					{
						Sattelites[i].Valid = 0;
					}
				}
			}

			if(strstr(Settings.FrequencyMode, "SP2"))
			{
				if(IndexOfP2 >= 0)
				{
					Sattelites[i].P[0] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfP2][0];
					Sattelites[i].P[1] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfP2][1];
				}
				else
				{
					Sattelites[i].P[0] = 0.0;
				}

				if(Sattelites[i].P[0] == 0.0)
				{
					Sattelites[i].Valid = 0;
				}
				else
				{
					Sattelites[i].Valid = 1;
                    if(Sattelites[i].P[1] > 0.0 && Sattelites[i].P[1] < Settings.SNRMask)
					{
						Sattelites[i].Valid = 0;
					}
				}
			}

			if(strstr(Settings.FrequencyMode, "DC"))
			{
				if(RINEXObs->Epochs[RINEXObs->CurrentEpoch].Sattelites[i][0] != 'E')
				{
					if(IndexOfC1 >= 0 && IndexOfC2 >= 0)
					{
						Sattelites[i].P1[0] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfC1][0];
						Sattelites[i].P2[0] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfC2][0];
						Sattelites[i].P1[1] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfC1][1];
						Sattelites[i].P2[1] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfC2][1];
					}
					else
					{
						Sattelites[i].P1[0] = 0.0;
						Sattelites[i].P2[0] = 0.0;
					}

					if(Sattelites[i].P1[0] <= 0.0 || Sattelites[i].P2[0] <= 0.0)
					{
						Sattelites[i].Valid = 0;
					}
					else
					{
						Sattelites[i].Valid = 1;
						if((Sattelites[i].P1[1] > 0.0 && Sattelites[i].P1[1] < Settings.SNRMask) ||
						   (Sattelites[i].P2[1] > 0.0 && Sattelites[i].P2[1] < Settings.SNRMask))
						{
							Sattelites[i].Valid = 0;
						}
					}
				}
				else
				{
					if(IndexOfC1 >= 0 && (IndexOfC5 >= 0 || IndexOfC7 >= 0))
					{
						Sattelites[i].P1[0] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfC1][0];
						Sattelites[i].PC5[0] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfC5][0];
						Sattelites[i].PC7[0] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfC7][0];
						Sattelites[i].P1[1] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfC1][1];
						Sattelites[i].PC5[1] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfC5][1];
						Sattelites[i].PC7[1] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfC7][1];
					}
					else
					{
						Sattelites[i].P1[0] = 0.0;
						Sattelites[i].P2[0] = 0.0;
					}

					if(Sattelites[i].P1[0] <= 0.0 &&
					  (Sattelites[i].PC5[0] <= 0.0 || Sattelites[i].PC7[0] <= 0.0))
					{
						Sattelites[i].Valid = 0;
					}
					else
					{
						Sattelites[i].Valid = 1;
						if((Sattelites[i].P1[1] > 0.0 && Sattelites[i].P1[1] < Settings.SNRMask) ||
						   (Sattelites[i].PC5[1] > 0.0 && Sattelites[i].PC5[1] < Settings.SNRMask) ||
						   (Sattelites[i].PC7[1] > 0.0 && Sattelites[i].PC7[1] < Settings.SNRMask))
						{
							Sattelites[i].Valid = 0;
						}
					}
				}
			}

			if(strstr(Settings.FrequencyMode, "DP"))
			{
				if(IndexOfP1 >= 0 && IndexOfP2 >= 0)
				{
					Sattelites[i].P1[0] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfP1][0];
					Sattelites[i].P2[0] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfP2][0];
					Sattelites[i].P1[1] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfP1][1];
					Sattelites[i].P2[1] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfP2][1];
				}
				else
				{
					Sattelites[i].P1[0] = 0.0;
					Sattelites[i].P2[0] = 0.0;
				}

				if(Sattelites[i].P1[0] <= 0.0 || Sattelites[i].P2[0] <= 0.0)
				{
					Sattelites[i].Valid = 0;
				}
				else
				{
					Sattelites[i].Valid = 1;
                    if((Sattelites[i].P1[1] > 0.0 && Sattelites[i].P1[1] < Settings.SNRMask) ||
					   (Sattelites[i].P2[1] > 0.0 && Sattelites[i].P2[1] < Settings.SNRMask))
					{
						Sattelites[i].Valid = 0;
					}
				}
			}

			if(strstr(Settings.FrequencyMode, "DA"))
			{
				if(IndexOfP1 >= 0 && IndexOfP2 >= 0)
				{
					Sattelites[i].P1[0] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfP1][0];
					Sattelites[i].P2[0] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfP2][0];
					Sattelites[i].P1[1] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfP1][1];
					Sattelites[i].P2[1] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfP2][1];
				}
				else
				{
					Sattelites[i].P1[0] = 0.0;
					Sattelites[i].P2[0] = 0.0;
				}

				if(Sattelites[i].P1[0] <= 0.0 || Sattelites[i].P2[0] <= 0.0)
				{
                    Sattelites[i].Valid = 0;
					if(RINEXObs->Epochs[RINEXObs->CurrentEpoch].Sattelites[i][0] != 'E')
					{
						if(IndexOfC1 >= 0 && IndexOfC2 >= 0)
						{
							Sattelites[i].P1[0] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfC1][0];
							Sattelites[i].P2[0] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfC2][0];
							Sattelites[i].P1[1] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfC1][1];
							Sattelites[i].P2[1] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfC2][1];
						}
						else
						{
							Sattelites[i].P1[0] = 0.0;
							Sattelites[i].P2[0] = 0.0;
						}

						if(Sattelites[i].P1[0] <= 0.0 || Sattelites[i].P2[0] <= 0.0)
						{
							Sattelites[i].Valid = 0;
							if(IndexOfP1 >= 0 && IndexOfC2 >= 0)
							{
								Sattelites[i].P1[0] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfP1][0];
								Sattelites[i].P2[0] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfC2][0];
								Sattelites[i].P1[1] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfP1][1];
								Sattelites[i].P2[1] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfC2][1];
							}
							else
							{
								Sattelites[i].P1[0] = 0.0;
								Sattelites[i].P2[0] = 0.0;
							}

							if(Sattelites[i].P1[0] <= 0.0 || Sattelites[i].P2[0] <= 0.0)
							{
								Sattelites[i].Valid = 0;
								if(IndexOfC1 >= 0 && IndexOfP2 >= 0)
								{
									Sattelites[i].P1[0] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfC1][0];
									Sattelites[i].P2[0] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfP2][0];
									Sattelites[i].P1[1] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfC1][1];
									Sattelites[i].P2[1] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfP2][1];
								}
								else
								{
									Sattelites[i].P1[0] = 0.0;
									Sattelites[i].P2[0] = 0.0;
								}

								if(Sattelites[i].P1[0] <= 0.0 || Sattelites[i].P2[0] <= 0.0)
								{
                                    Sattelites[i].Valid = 0;
								}
								else
								{
									Sattelites[i].Valid = 1;
                                    if((Sattelites[i].P1[1] > 0.0 && Sattelites[i].P1[1] < Settings.SNRMask) ||
									   (Sattelites[i].P2[1] > 0.0 && Sattelites[i].P2[1] < Settings.SNRMask))
									{
										Sattelites[i].Valid = 0;
									}
								}
							}
							else
							{
								Sattelites[i].Valid = 1;
                                if((Sattelites[i].P1[1] > 0.0 && Sattelites[i].P1[1] < Settings.SNRMask) ||
								   (Sattelites[i].P2[1] > 0.0 && Sattelites[i].P2[1] < Settings.SNRMask))
								{
									Sattelites[i].Valid = 0;
								}
							}
						}
						else
						{
							Sattelites[i].Valid = 1;
							if((Sattelites[i].P1[1] > 0.0 && Sattelites[i].P1[1] < Settings.SNRMask) ||
							   (Sattelites[i].P2[1] > 0.0 && Sattelites[i].P2[1] < Settings.SNRMask))
							{
								Sattelites[i].Valid = 0;
							}
						}
					}
					else
					{
						if(IndexOfC1 >= 0 && (IndexOfC5 >= 0 || IndexOfC7 >= 0))
						{
							Sattelites[i].P1[0] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfC1][0];
							Sattelites[i].PC5[0] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfC5][0];
							Sattelites[i].PC7[0] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfC7][0];
							Sattelites[i].P1[1] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfC1][1];
							Sattelites[i].PC5[1] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfC5][1];
							Sattelites[i].PC7[1] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Values[i][IndexOfC7][1];
						}
						else
						{
							Sattelites[i].P1[0] = 0.0;
							Sattelites[i].P2[0] = 0.0;
						}

						if(Sattelites[i].P1[0] <= 0.0 && (Sattelites[i].PC5[0] <= 0.0 || Sattelites[i].PC7[0] <= 0.0))
						{
							Sattelites[i].Valid = 0;
						}
						else
						{
							Sattelites[i].Valid = 1;
                            if((Sattelites[i].P1[1] > 0.0 && Sattelites[i].P1[1] < Settings.SNRMask) ||
							   (Sattelites[i].PC5[1] > 0.0 && Sattelites[i].PC5[1] < Settings.SNRMask) ||
							   (Sattelites[i].PC7[1] > 0.0 && Sattelites[i].PC7[1] < Settings.SNRMask))
							{
								Sattelites[i].Valid = 0;
							}
						}
					}
				}
				else
				{
					Sattelites[i].Valid = 1;
                    if((Sattelites[i].P1[1] > 0.0 && Sattelites[i].P1[1] < Settings.SNRMask) ||
					   (Sattelites[i].P2[1] > 0.0 && Sattelites[i].P2[1] < Settings.SNRMask))
					{
						Sattelites[i].Valid = 0;
					}
				}
			}

			//printf("\n  %c%d %.3lf %.3lf %.3lf %.3lf %.3lf\n", RINEXObs->Epochs[RINEXObs->CurrentEpoch].Sattelites[i][0], RINEXObs->Epochs[RINEXObs->CurrentEpoch].Sattelites[i][1], Sattelites[i].P, Sattelites[i].P1, Sattelites[i].P2, Sattelites[i].PC5, Sattelites[i].PC7);
			if(Sattelites[i].Valid)
			{
			    //printf("\n  %c%d %.3lf %.3lf %.3lf %.3lf %.3lf\n", RINEXObs->Epochs[RINEXObs->CurrentEpoch].Sattelites[i][0], RINEXObs->Epochs[RINEXObs->CurrentEpoch].Sattelites[i][1], Sattelites[i].P[0], Sattelites[i].P1[0], Sattelites[i].P2[0], Sattelites[i].PC5[0], Sattelites[i].PC7[0]);
				CurSolution[RINEXObs->CurrentEpoch].NOfValidSattelites++;
			}
		}

		// Если спутник есть в таблице исключённых спутников, его как бы "не существует"
		for(i = 0; i < RINEXObs->Epochs[RINEXObs->CurrentEpoch].NOfSattelites; i++)
		{
			for(j = 0; j < Settings.NOfExcludeSattelites; j++)
			{
				if(RINEXObs->Epochs[RINEXObs->CurrentEpoch].Sattelites[i][0] == Settings.ExcludeSattelites[j][0] &&
				   RINEXObs->Epochs[RINEXObs->CurrentEpoch].Sattelites[i][1] == Settings.ExcludeSattelites[j][1])
				{
					Sattelites[i].Valid = 0;
					CurSolution[RINEXObs->CurrentEpoch].NOfValidSattelites--;
				}
			}
		}

		if(CurSolution[RINEXObs->CurrentEpoch].NOfValidSattelites >= SattelitesThreshold)
		{
             CurSolution[RINEXObs->CurrentEpoch].NOfValidSattelites = 0; // А может у нас не наберётся в nav-файле столько спутников
			if(Ephemeris == BOARD)
			{
				FindRINEXEphemeris(&Settings, RINEXObs, RINEXNav,
								   Sattelites);
			}

			if(Ephemeris == PRECISE)
			{
				FindSP3Ephemeris(&Settings, RINEXObs, SP3,
				                 Sattelites, InterpolationPoints);
			}
			SattelitesInitialRange(&RINEXObs->Epochs[RINEXObs->CurrentEpoch], Sattelites,
						 Settings.FrequencyMode,
						 &CurSolution[RINEXObs->CurrentEpoch]);
		}

		if(Settings.OutputType == TO_SCREEN || Settings.OutputType == TO_SCREEN_FILE)
		{
			printf("\n\n %4hd %2hhd %2hhd %2hhd %2hhd %4.1f",
				   RINEXObs->Epochs[RINEXObs->CurrentEpoch].Year,
				   RINEXObs->Epochs[RINEXObs->CurrentEpoch].Month,
				   RINEXObs->Epochs[RINEXObs->CurrentEpoch].Day,
				   RINEXObs->Epochs[RINEXObs->CurrentEpoch].Hours,
				   RINEXObs->Epochs[RINEXObs->CurrentEpoch].Minutes,
				   RINEXObs->Epochs[RINEXObs->CurrentEpoch].Seconds);
		}

		 //Уточняем количество участвующих систем и число неизвестных согласно наличию псевдодальностей
//		switch(RINEXObs->GPS && strstr(Settings.System, "G") +
//			   RINEXObs->GLO && strstr(Settings.System, "R") +
//			   RINEXObs->GAL && strstr(Settings.System, "E") +
//			   RINEXObs->BDS && strstr(Settings.System, "C"))
//		{
//			case 1:
//				SattelitesThreshold = 4;
//			break;
//			case 2:
//				SattelitesThreshold = 5;
//			break;
//			case 3:
//				SattelitesThreshold = 6;
//			break;
//			case 4:
//				SattelitesThreshold = 7;
//			break;
//			default:
//				SattelitesThreshold = 4;
//		}

	 //	printf("\n%hhd %hhd\n", CurSolution[RINEXObs->CurrentEpoch].NOfValidSattelites, SattelitesThreshold);
//		getch();

		CurSolution[RINEXObs->CurrentEpoch].Valid = 1;
		if(CurSolution[RINEXObs->CurrentEpoch].NOfValidSattelites >= SattelitesThreshold)
		{
			do
			{
				for(i = 0; i < RINEXObs->Epochs[RINEXObs->CurrentEpoch].NOfSattelites; i++)
				{
					if(Sattelites[i].Valid)
					{
						if(Ephemeris == BOARD)
						{
							if(Sattelites[i].Number[0] == 'G')
							{
								GPSSatteliteXV(&Sattelites[i], 0);
								Sattelites[i].dt = Sattelites[i].a0 +
											   Sattelites[i].a1 * Sattelites[i].tk +
											   Sattelites[i].a2 * sqr(Sattelites[i].tk)
											   + C * Sattelites[i].e * Sattelites[i].sqrta * sin(Sattelites[i].E);
											   //- 2.0 * (Sattelites[i].x * Sattelites[i].vx + Sattelites[i].y * Sattelites[i].vy + Sattelites[i].z * Sattelites[i].vz) / sqr(c);
								//printf("\n%d %lf %lf %lf", Sattelites[i].Number[1], Sattelites[i].vx, Sattelites[i].vy, Sattelites[i].vz);
								if(strstr(Settings.FrequencyMode, "SC1") || strstr(Settings.FrequencyMode, "SP1"))
								{
									Sattelites[i].dt -= Sattelites[i].TGD;
								}

								if(strstr(Settings.FrequencyMode, "SC2") || strstr(Settings.FrequencyMode, "SP2"))
								{
									Sattelites[i].dt -= gamma_GPS * Sattelites[i].TGD;
								}
							}

							if(Sattelites[i].Number[0] == 'R')
							{
								WGS84ToPZ90_02(Sattelites[i].x0, Sattelites[i].y0, Sattelites[i].z0,
												  &Sattelites[i].x, &Sattelites[i].y, &Sattelites[i].z);
								GLOSatteliteXV(&Sattelites[i]);
								PZ90_02ToWGS84(Sattelites[i].xi, Sattelites[i].yi, Sattelites[i].zi,
											  &Sattelites[i].x, &Sattelites[i].y, &Sattelites[i].z);
								Sattelites[i].dt = -Sattelites[i].TauN + Sattelites[i].GammaN * Sattelites[i].tk;
							   //	Sattelites[i].tk = RINEXObs->Epochs[RINEXObs->CurrentEpoch].t - tau - Sattelites[i].dt - Sattelites[i].toc;
							}

							if(Sattelites[i].Number[0] == 'E')
							{
								GPSSatteliteXV(&Sattelites[i], 0);
								Sattelites[i].dt = Sattelites[i].a0 +
											   Sattelites[i].a1 * Sattelites[i].tk +
											   Sattelites[i].a2 * sqr(Sattelites[i].tk)
											   + C * Sattelites[i].e * Sattelites[i].sqrta * sin(Sattelites[i].E);
											   //- 2.0 * (Sattelites[i].x * Sattelites[i].vx + Sattelites[i].y * Sattelites[i].vy + Sattelites[i].z * Sattelites[i].vz) / sqr(c);
								//printf("\n%d %lf %lf %lf", Sattelites[i].Number[1], Sattelites[i].vx, Sattelites[i].vy, Sattelites[i].vz);
								if(strstr(Settings.FrequencyMode, "SC1"))
								{
									 //printf("\n%d\n", (int)Sattelites[i].Ds & 1 << 9);
									if(Settings.GalileoDataType == 'F')
									{
										Sattelites[i].dt -= Sattelites[i].BGD_E1_E5a;
									}

										if(Settings.GalileoDataType == 'I')
									{
										Sattelites[i].dt -= Sattelites[i].BGD_E1_E5b;
									}
								}

								if(strstr(Settings.FrequencyMode, "SC2"))
								{
									if(Settings.GalileoDataType == 'F')
									{
										Sattelites[i].dt -= gamma_E1_E5a_GAL * Sattelites[i].BGD_E1_E5a;
									}

									if(Settings.GalileoDataType == 'I')
									{
										Sattelites[i].dt -= gamma_E1_E5b_GAL * Sattelites[i].BGD_E1_E5b;
									}
								}
							}

							if(Sattelites[i].Number[0] == 'C')
							{
								GPSSatteliteXV(&Sattelites[i], 0);
                                Sattelites[i].dt = Sattelites[i].a0 +
												   Sattelites[i].a1 * Sattelites[i].tk +
												   Sattelites[i].a2 * sqr(Sattelites[i].tk)
												   + C * Sattelites[i].e * Sattelites[i].sqrta * sin(Sattelites[i].E);

								if(strstr(Settings.FrequencyMode, "SC1"))
								{
									Sattelites[i].dt -= Sattelites[i].TGD1_B1_B3;
								}

								if(strstr(Settings.FrequencyMode, "SC2"))
								{
									Sattelites[i].dt -= Sattelites[i].TGD2_B2_B3;
								}
							}

                        }

						if(Ephemeris == PRECISE)
						{
//							for(k = 0; k < (INTERPOLATION_ORDER + 1) && Cycles == 0; k++)
//							{
//								printf("\n%lf %lf %lf", InterpolationPoints[i].x[k], InterpolationPoints[i].toc[k], InterpolationPoints[i].dt[k]);
//							}
                            Sattelites[i].dt = Neville(InterpolationPoints[i].dt,
													   InterpolationPoints[i].toc,
													   RINEXObs->Epochs[RINEXObs->CurrentEpoch].t - tau - Sattelites[i].dt,
													   INTERPOLATION_ORDER + 1);
							Sattelites[i].x = Neville(InterpolationPoints[i].x,
													  InterpolationPoints[i].toc,
													  RINEXObs->Epochs[RINEXObs->CurrentEpoch].t - tau - Sattelites[i].dt,
													  INTERPOLATION_ORDER + 1);
							Sattelites[i].y = Neville(InterpolationPoints[i].y,
													  InterpolationPoints[i].toc,
													  RINEXObs->Epochs[RINEXObs->CurrentEpoch].t - tau - Sattelites[i].dt,
													  INTERPOLATION_ORDER + 1);
							Sattelites[i].z = Neville(InterpolationPoints[i].z,
													  InterpolationPoints[i].toc,
													  RINEXObs->Epochs[RINEXObs->CurrentEpoch].t - tau - Sattelites[i].dt,
													  INTERPOLATION_ORDER + 1);
						}

						//printf("\n%d\n", CurSolution[RINEXObs->CurrentEpoch].NOfValidSattelites);
						   //	printf("\n%d %lf %lf %lf", Sattelites[i].Number[1], Sattelites[i].x, Sattelites[i].y, Sattelites[i].z);
						  //  getch();
						if(fabs(Sattelites[i].x) >= 1.0E+9 ||
						   fabs(Sattelites[i].y) >= 1.0E+9 ||
						   fabs(Sattelites[i].z) >= 1.0E+9)
						{
							Sattelites[i].Valid = 0;
							CurSolution[RINEXObs->CurrentEpoch].NOfValidSattelites--;
						}

						for(j = 0; j < 2; j++)
						{
							Sattelites[i].rho = sqrt(sqr(CurSolution[RINEXObs->CurrentEpoch].Q[0] - Sattelites[i].x) + sqr(CurSolution[RINEXObs->CurrentEpoch].Q[1] - Sattelites[i].y) + sqr(CurSolution[RINEXObs->CurrentEpoch].Q[2] - Sattelites[i].z));
							Sattelites[i].S = (Sattelites[i].x * CurSolution[RINEXObs->CurrentEpoch].Q[1] - Sattelites[i].y * CurSolution[RINEXObs->CurrentEpoch].Q[0]) * OMEGAi_WGS84 / c;
							tau = (Sattelites[i].rho + Sattelites[i].S) / c;
							theta = -OMEGAi_WGS84 * tau;
							Sattelites[i].xi = Sattelites[i].x * cos(theta) - Sattelites[i].y * sin(theta);
							Sattelites[i].yi = Sattelites[i].x * sin(theta) + Sattelites[i].y * cos(theta);
							Sattelites[i].x = Sattelites[i].xi;
							Sattelites[i].y = Sattelites[i].yi;
							Sattelites[i].xi = Sattelites[i].vx * cos(theta) + Sattelites[i].vy * sin(theta) + OMEGAi_WGS84 * Sattelites[i].y;
							Sattelites[i].yi = -Sattelites[i].vx * sin(theta) + Sattelites[i].vy * cos(theta) - OMEGAi_WGS84 * Sattelites[i].x;
							Sattelites[i].vx = Sattelites[i].xi;
							Sattelites[i].vy = Sattelites[i].yi;
						}

						if(Ephemeris == BOARD)
						{
							Sattelites[i].tk = RINEXObs->Epochs[RINEXObs->CurrentEpoch].t - tau - Sattelites[i].dt - Sattelites[i].toc;
						}

						//printf("\n%lf %lf %lf\n", CurSolution[RINEXObs->CurrentEpoch].Q[0], CurSolution[RINEXObs->CurrentEpoch].Q[1], CurSolution[RINEXObs->CurrentEpoch].Q[2]);
						Sattelites[i].Az = Azimuth(CurSolution[RINEXObs->CurrentEpoch].Q[0], CurSolution[RINEXObs->CurrentEpoch].Q[1], CurSolution[RINEXObs->CurrentEpoch].Q[2], Sattelites[i].x, Sattelites[i].y, Sattelites[i].z);
						Sattelites[i].El = Elevation(CurSolution[RINEXObs->CurrentEpoch].Q[0], CurSolution[RINEXObs->CurrentEpoch].Q[1], CurSolution[RINEXObs->CurrentEpoch].Q[2], Sattelites[i].x, Sattelites[i].y, Sattelites[i].z);
						if(Settings.ElevationMask)
						{
							if(Cycles >= Settings.Iterations - 2)
							{
								if(Sattelites[i].El * RadToDeg < Settings.ElevationMask)
								{
									Sattelites[i].Valid = 0;
									CurSolution[RINEXObs->CurrentEpoch].NOfValidSattelites--;
								}
							}
						}

						if(Settings.Ionosphere)
						{
							if(Settings.Ionosphere == 'K')
							{
								DecartToGeo(CurSolution[RINEXObs->CurrentEpoch].Q[0], CurSolution[RINEXObs->CurrentEpoch].Q[1], CurSolution[RINEXObs->CurrentEpoch].Q[2], &CurSolution[RINEXObs->CurrentEpoch].B, &CurSolution[RINEXObs->CurrentEpoch].L, &CurSolution[RINEXObs->CurrentEpoch].H);
								TimeInSeconds = RINEXObs->Epochs[RINEXObs->CurrentEpoch].Hours * 3600 +
										  RINEXObs->Epochs[RINEXObs->CurrentEpoch].Minutes * 60 +
										  RINEXObs->Epochs[RINEXObs->CurrentEpoch].Seconds;
								Sattelites[i].I = Klobuchar(CurSolution[RINEXObs->CurrentEpoch].B, CurSolution[RINEXObs->CurrentEpoch].L, Sattelites[i].Az, Sattelites[i].El, TimeInSeconds, RINEXNav->AlphaGPS, RINEXNav->BetaGPS) *
								                  KlobMapping(Sattelites[i].El);
								//printf("\n%lf", Sattelites[i].I);
							}

							if(Settings.Ionosphere == 'A')
							{
								DecartToGeo(CurSolution[RINEXObs->CurrentEpoch].Q[0], CurSolution[RINEXObs->CurrentEpoch].Q[1], CurSolution[RINEXObs->CurrentEpoch].Q[2], &CurSolution[RINEXObs->CurrentEpoch].B, &CurSolution[RINEXObs->CurrentEpoch].L, &CurSolution[RINEXObs->CurrentEpoch].H);
								RINEXObs->Epochs[RINEXObs->CurrentEpoch].TimeInHours = (float)RINEXObs->Epochs[RINEXObs->CurrentEpoch].Hours + (float)RINEXObs->Epochs[RINEXObs->CurrentEpoch].Minutes / 60.0 + RINEXObs->Epochs[RINEXObs->CurrentEpoch].Seconds / 3600.0;
								DecartToGeo(Sattelites[i].x, Sattelites[i].y, Sattelites[i].z, &Sattelites[i].B, &Sattelites[i].L, &Sattelites[i].H);
								if(GLOIonosphere(RINEXObs->Epochs[RINEXObs->CurrentEpoch].TimeInHours, RINEXObs->Epochs[RINEXObs->CurrentEpoch].Month, CurSolution[RINEXObs->CurrentEpoch].B, CurSolution[RINEXObs->CurrentEpoch].L, Settings.A, Settings.F107, Settings.Ap,
											  &GLOIonData.hmax, &GLOIonData.A, &GLOIonData.Bbot, &GLOIonData.Btop))
								{
									Sattelites[i].I = IntegralTECv(Ne, CurSolution[RINEXObs->CurrentEpoch].H * 1.0E-3, Sattelites[i].H * 1.0E-3, 5.0,
																   GLOIonData.hmax, GLOIonData.A, GLOIonData.Bbot, GLOIonData.Btop) *
												                   GLOMapping(CurSolution[RINEXObs->CurrentEpoch].B, CurSolution[RINEXObs->CurrentEpoch].L, Sattelites[i].B, Sattelites[i].L, Sattelites[i].H * 1.0E-3);
								}
								else
								{
									Sattelites[i].I = 0.0;

								}
								//printf("\n%lf", Sattelites[i].I);
							}

							if(Settings.Ionosphere == 'S')
							{
								DecartToGeo(CurSolution[RINEXObs->CurrentEpoch].Q[0], CurSolution[RINEXObs->CurrentEpoch].Q[1], CurSolution[RINEXObs->CurrentEpoch].Q[2], &CurSolution[RINEXObs->CurrentEpoch].B, &CurSolution[RINEXObs->CurrentEpoch].L, &CurSolution[RINEXObs->CurrentEpoch].H);
								RINEXObs->Epochs[RINEXObs->CurrentEpoch].TimeInHours = (float)RINEXObs->Epochs[RINEXObs->CurrentEpoch].Hours + (float)RINEXObs->Epochs[RINEXObs->CurrentEpoch].Minutes / 60.0 + RINEXObs->Epochs[RINEXObs->CurrentEpoch].Seconds / 3600.0;
								DecartToGeo(Sattelites[i].x, Sattelites[i].y, Sattelites[i].z, &Sattelites[i].B, &Sattelites[i].L, &Sattelites[i].H);
								if(GLOIonosphere(RINEXObs->Epochs[RINEXObs->CurrentEpoch].TimeInHours, RINEXObs->Epochs[RINEXObs->CurrentEpoch].Month, CurSolution[RINEXObs->CurrentEpoch].B, CurSolution[RINEXObs->CurrentEpoch].L, Settings.A, Settings.F107, Settings.Ap,
											  &GLOIonData.hmax, &GLOIonData.A, &GLOIonData.Bbot, &GLOIonData.Btop))
								{
									Sattelites[i].I = SimpleTECv(GLOIonData.A, GLOIonData.Bbot, GLOIonData.Btop) *
													  GLOMapping(CurSolution[RINEXObs->CurrentEpoch].B, CurSolution[RINEXObs->CurrentEpoch].L, Sattelites[i].B, Sattelites[i].L, Sattelites[i].H * 1.0E-3);
								}
								else
								{
									Sattelites[i].I = 0.0;
								}
								//printf("\n%lf", Sattelites[i].B);
							}

							if(Settings.Ionosphere == 'N')
							{
								DecartToGeo(CurSolution[RINEXObs->CurrentEpoch].Q[0], CurSolution[RINEXObs->CurrentEpoch].Q[1], CurSolution[RINEXObs->CurrentEpoch].Q[2], &CurSolution[RINEXObs->CurrentEpoch].B, &CurSolution[RINEXObs->CurrentEpoch].L, &CurSolution[RINEXObs->CurrentEpoch].H);
								RINEXObs->Epochs[RINEXObs->CurrentEpoch].TimeInHours = (float)RINEXObs->Epochs[RINEXObs->CurrentEpoch].Hours + (float)RINEXObs->Epochs[RINEXObs->CurrentEpoch].Minutes / 60.0 + RINEXObs->Epochs[RINEXObs->CurrentEpoch].Seconds / 3600.0;
								DecartToGeo(Sattelites[i].x, Sattelites[i].y, Sattelites[i].z, &Sattelites[i].B, &Sattelites[i].L, &Sattelites[i].H);
								pdRecvLLHdeg[0] = CurSolution[RINEXObs->CurrentEpoch].B;
								pdRecvLLHdeg[1] = CurSolution[RINEXObs->CurrentEpoch].L;
								pdRecvLLHdeg[2] = CurSolution[RINEXObs->CurrentEpoch].H;
								pdSatLLHdeg[0] = Sattelites[i].B;
								pdSatLLHdeg[1] = Sattelites[i].L;
								pdSatLLHdeg[2] = Sattelites[i].H;
								performtest(RINEXObs->Epochs[RINEXObs->CurrentEpoch].Month, RINEXObs->Epochs[RINEXObs->CurrentEpoch].TimeInHours, pdRecvLLHdeg, pdSatLLHdeg,
											RINEXNav->AzGAL, &stModip, &stCCIR,
											&dModip, &Sattelites[i].I);
								//printf("\n%lf", Sattelites[i].I);
							}

							if(Settings.Ionosphere == 'B')
							{
								DecartToGeo(CurSolution[RINEXObs->CurrentEpoch].Q[0], CurSolution[RINEXObs->CurrentEpoch].Q[1], CurSolution[RINEXObs->CurrentEpoch].Q[2],
											&CurSolution[RINEXObs->CurrentEpoch].B, &CurSolution[RINEXObs->CurrentEpoch].L, &CurSolution[RINEXObs->CurrentEpoch].H);
								Sattelites[i].I =
								BDGIM(RINEXObs->Epochs[RINEXObs->CurrentEpoch].Year,
									  RINEXObs->Epochs[RINEXObs->CurrentEpoch].Month,
									  RINEXObs->Epochs[RINEXObs->CurrentEpoch].Day,
									  RINEXObs->Epochs[RINEXObs->CurrentEpoch].Hours,
									  RINEXObs->Epochs[RINEXObs->CurrentEpoch].Minutes,
									  RINEXObs->Epochs[RINEXObs->CurrentEpoch].Seconds,
									  CurSolution[RINEXObs->CurrentEpoch].B, CurSolution[RINEXObs->CurrentEpoch].L, Sattelites[i].Az, Sattelites[i].El, RINEXNav->AlphaBDS) *
									  IonoMapping(Sattelites[i].El, Re_BDS, hi_BDGIM);
							//printf("\n%lf", Sattelites[i].I);
							}

							if(Settings.Ionosphere == 'G')
							{
								DecartToGeo(CurSolution[RINEXObs->CurrentEpoch].Q[0], CurSolution[RINEXObs->CurrentEpoch].Q[1], CurSolution[RINEXObs->CurrentEpoch].Q[2], &CurSolution[RINEXObs->CurrentEpoch].B, &CurSolution[RINEXObs->CurrentEpoch].L, &CurSolution[RINEXObs->CurrentEpoch].H);
								RINEXObs->Epochs[RINEXObs->CurrentEpoch].TimeInHours = (float)RINEXObs->Epochs[RINEXObs->CurrentEpoch].Hours + (float)RINEXObs->Epochs[RINEXObs->CurrentEpoch].Minutes / 60.0 + RINEXObs->Epochs[RINEXObs->CurrentEpoch].Seconds / 3600.0;
								Sattelites[i].I = GEMTEC(CurSolution[RINEXObs->CurrentEpoch].L, RINEXObs->Epochs[RINEXObs->CurrentEpoch].TimeInHours, CurSolution[RINEXObs->CurrentEpoch].B, Settings.F107, RINEXObs->Epochs[RINEXObs->CurrentEpoch].Month, RINEXObs->Epochs[RINEXObs->CurrentEpoch].Day, GEMTECData.evt, GEMTECData.evm, GEMTECData.evl, GEMTECData.evf, GEMTECData.A) *
												  IonoMapping(Sattelites[i].El, Re_WGS84, hi_GEMTEC);
								//printf("\n%lf", Sattelites[i].I);
							}

							if(Settings.Ionosphere == 'I' || Settings.Ionosphere == 'M')
							{
								DecartToGeo(CurSolution[RINEXObs->CurrentEpoch].Q[0], CurSolution[RINEXObs->CurrentEpoch].Q[1], CurSolution[RINEXObs->CurrentEpoch].Q[2], &CurSolution[RINEXObs->CurrentEpoch].B, &CurSolution[RINEXObs->CurrentEpoch].L, &CurSolution[RINEXObs->CurrentEpoch].H);
								ep[0] = (double)RINEXObs->Epochs[RINEXObs->CurrentEpoch].Year;
								if(ep[0] < 100.0)
								{
									ep[0] += ep[0] < 80.0 ? 2000.0 : 1900.0;
								}
								ep[1] = (double)RINEXObs->Epochs[RINEXObs->CurrentEpoch].Month;
								ep[2] = (double)RINEXObs->Epochs[RINEXObs->CurrentEpoch].Day;
								ep[3] = (double)RINEXObs->Epochs[RINEXObs->CurrentEpoch].Hours;
								ep[4] = (double)RINEXObs->Epochs[RINEXObs->CurrentEpoch].Minutes;
								ep[5] = (double)RINEXObs->Epochs[RINEXObs->CurrentEpoch].Seconds;
								pos[0] = CurSolution[RINEXObs->CurrentEpoch].B * DegToRad;
								pos[1] = CurSolution[RINEXObs->CurrentEpoch].L * DegToRad;
								pos[2] = CurSolution[RINEXObs->CurrentEpoch].H;
								azel[0] = Sattelites[i].Az;
								azel[1] = Sattelites[i].El;
								opt = 0x00;
								time = epoch2time(ep);
								iontec(time, &nav, pos, azel, opt, &delay, &var);
								Sattelites[i].I = delay * sqr(fL1_GPS) / 40.308E+4;
								//printf("\n%lf", ep[0]);
							   //printf("\n%lf", Sattelites[i].I);
							}

							if(Settings.Ionosphere == 'v')
							{
								RINEXObs->Epochs[RINEXObs->CurrentEpoch].TimeInHours = (float)RINEXObs->Epochs[RINEXObs->CurrentEpoch].Hours + (float)RINEXObs->Epochs[RINEXObs->CurrentEpoch].Minutes / 60.0 + RINEXObs->Epochs[RINEXObs->CurrentEpoch].Seconds / 3600.0;
								if(RINEXObs->Epochs[RINEXObs->CurrentEpoch].TimeInHours < TayAbsTEC->TECRecords[TayAbsTEC->NOfRecords - 1].UT)
								{
									k = RINEXObs->CurrentEpoch * TayAbsTEC->NOfRecords  / RINEXObs->NOfEpochs;
									if(k + (INTERPOLATION_ORDER + 1) < TayAbsTEC->NOfRecords)
									{
										if(k < (INTERPOLATION_ORDER + 1) / 2)
										{
											k = 0;
										}
										else
										{
											k -= (INTERPOLATION_ORDER + 1) / 2;
										}
									}
									else
									{
										k = TayAbsTEC->NOfRecords - (INTERPOLATION_ORDER + 1);
									}

									for(j = 0; j < INTERPOLATION_ORDER + 1; j++)
									{
										TayAbsTEC->TECInt[j] = TayAbsTEC->TECRecords[j + k].TEC;
										TayAbsTEC->UTInt[j] = TayAbsTEC->TECRecords[j + k].UT;
									}
									Sattelites[i].I = Neville(TayAbsTEC->TECInt, TayAbsTEC->UTInt, RINEXObs->Epochs[RINEXObs->CurrentEpoch].TimeInHours, INTERPOLATION_ORDER + 1);
								}
								else
								{
									Sattelites[i].I = TayAbsTEC->TECRecords[TayAbsTEC->NOfRecords - 1].TEC;
								}
								//printf("\n%d %lf", k,  Sattelites[i].I);
								Sattelites[i].I *= IonoMappingT(CurSolution[RINEXObs->CurrentEpoch].B, Sattelites[i].El, Re_WGS84, hi_T);
							   //	Sattelites[i].I *= IonoMapping(Sattelites[i].El, Re_WGS84, hi_T);
							}

							if(Settings.Ionosphere == 's')
							{
								RINEXObs->Epochs[RINEXObs->CurrentEpoch].TimeInHours = (float)RINEXObs->Epochs[RINEXObs->CurrentEpoch].Hours + (float)RINEXObs->Epochs[RINEXObs->CurrentEpoch].Minutes / 60.0 + RINEXObs->Epochs[RINEXObs->CurrentEpoch].Seconds / 3600.0;
								for(j = 0; j < NOfTayAbsTECFiles; j++)
								{
									//if(!strcmp(Sattelites[i].Number, TayAbsTEC[j].SatNumber))
									if(Sattelites[i].Number[0] == TayAbsTEC[j].SatNumber[0] &&
									   Sattelites[i].Number[1] == TayAbsTEC[j].SatNumber[1])
									{
										//printf("\n%c%hhd %s", Sattelites[i].Number[0], TayAbsTEC[j].SatNumber);
//										if(RINEXObs->Epochs[RINEXObs->CurrentEpoch].TimeInHours < TayAbsTEC[j].TECRecords[TayAbsTEC[j].NOfRecords - 1].UT)
//										{
//											k = RINEXObs->CurrentEpoch * TayAbsTEC[j].NOfRecords  / RINEXObs->NOfEpochs;
//											if(k + (INTERPOLATION_ORDER + 1) < TayAbsTEC[j].NOfRecords)
//											{
//												if(k < (INTERPOLATION_ORDER + 1) / 2)
//												{
//													k = 0;
//												}
//												else
//												{
//													k -= (INTERPOLATION_ORDER + 1) / 2;
//												}
//											}
//											else
//											{
//												k = TayAbsTEC[j].NOfRecords - (INTERPOLATION_ORDER + 1);
//											}
//
//											for(l = 0; l < INTERPOLATION_ORDER + 1; l++)
//											{
//												TayAbsTEC[j].TECInt[l] = TayAbsTEC[j].TECRecords[l + k].TEC;
//												TayAbsTEC[j].UTInt[l] = TayAbsTEC[j].TECRecords[l + k].UT;
//											   //	printf("\n%lf %lf", TayAbsTEC[j].TECInt[l], TayAbsTEC[j].UTInt[l]);
//												//getch();
//											}
//											Sattelites[i].I = Neville(TayAbsTEC[j].TECInt, TayAbsTEC[j].UTInt, RINEXObs->Epochs[RINEXObs->CurrentEpoch].TimeInHours, INTERPOLATION_ORDER + 1);
//										}
//										else
//										{
//											Sattelites[i].I = TayAbsTEC[j].TECRecords[TayAbsTEC[j].NOfRecords - 1].TEC;
//										}

										for(k = 0; k < TayAbsTEC[j].NOfRecords; k++)
										{
											if(RINEXObs->Epochs[RINEXObs->CurrentEpoch].TimeInHours == TayAbsTEC[j].TECRecords[k].UT)
											{
												Sattelites[i].I = TayAbsTEC[j].TECRecords[k].TEC;
											}
										}
									}
								}
//								if(Cycles == 5)
//								{
//								printf("\n%lf", Sattelites[i].I);
//                                }
							   	//printf("\n%f %f", RINEXObs->Epochs[RINEXObs->CurrentEpoch].TimeInHours, TayAbsTEC[j].TECRecords[k].UT);
							   // getch();
							}

							if(Sattelites[i].Number[0] == 'G')
							{
								if(strstr(Settings.FrequencyMode, "SC1") || strstr(Settings.FrequencyMode, "SP1"))
								{
									Sattelites[i].I *= 40.308E+4 / sqr(fL1_GPS);
								}

								if(strstr(Settings.FrequencyMode, "SC2") || strstr(Settings.FrequencyMode, "SP2"))
								{
									Sattelites[i].I *= 40.308E+4 / sqr(fL2_GPS);
								}
							}

							if(Sattelites[i].Number[0] == 'R')
							{
								if(strstr(Settings.FrequencyMode, "SC1") || strstr(Settings.FrequencyMode, "SP1"))
								{
									Sattelites[i].I *= 40.308E+4 / sqr(fG1_GLO + Sattelites[i].k * dfG1_GLO);
								}

								if(strstr(Settings.FrequencyMode, "SC2") || strstr(Settings.FrequencyMode, "SP2"))
								{
									Sattelites[i].I *= 40.308E+4 / sqr(fG2_GLO + Sattelites[i].k * dfG2_GLO);
								}
							}

							if(Sattelites[i].Number[0] == 'E')
							{
								if(strstr(Settings.FrequencyMode, "SC1"))
								{
									Sattelites[i].I *= 40.308E+4 / sqr(fE1_GAL);
								}

								if(strstr(Settings.FrequencyMode, "SC2"))
								{
									if(Settings.GalileoDataType == 'F')
									{
										Sattelites[i].I *= 40.308E+4 / sqr(fE5a_GAL);
									}

									if(Settings.GalileoDataType == 'I')
									{
										Sattelites[i].I *= 40.308E+4 / sqr(fE5b_GAL);
									}
								}
							}

							if(Sattelites[i].Number[0] == 'C')
							{
								if(strstr(Settings.FrequencyMode, "SC1"))
								{
									Sattelites[i].I *= 40.308E+4 / sqr(fB1_BDS);
								}

								if(strstr(Settings.FrequencyMode, "SC2"))
								{
									Sattelites[i].I *= 40.308E+4 / sqr(fB2_BDS);
								}
							}
						}
						else
						{
							Sattelites[i].I = 0.0;
						}

						if(strstr(Settings.FrequencyMode, "DC") || strstr(Settings.FrequencyMode, "DP") || strstr(Settings.FrequencyMode, "DA"))
						{
							if(RINEXObs->Epochs[RINEXObs->CurrentEpoch].Sattelites[i][0] == 'G')
							{
								Sattelites[i].P[0] = (Sattelites[i].P2[0] - gamma_GPS * Sattelites[i].P1[0]) / (1.0 - gamma_GPS);
							}

							if(RINEXObs->Epochs[RINEXObs->CurrentEpoch].Sattelites[i][0] == 'R')
							{
								Sattelites[i].P[0] = (Sattelites[i].P2[0] - gamma_GLO * Sattelites[i].P1[0]) / (1.0 - gamma_GLO);
							}

							if(RINEXObs->Epochs[RINEXObs->CurrentEpoch].Sattelites[i][0] == 'E')
							{
								if(Settings.GalileoDataType == 'F')
								{
									Sattelites[i].P[0] = (Sattelites[i].P2[0] - gamma_E1_E5a_GAL * Sattelites[i].P1[0]) / (1.0 - gamma_E1_E5a_GAL);
								}

								if(Settings.GalileoDataType == 'I')
								{
									Sattelites[i].P[0] = (Sattelites[i].P2[0] - gamma_E1_E5b_GAL * Sattelites[i].P1[0]) / (1.0 - gamma_E1_E5b_GAL);
								}
							}

                            if(RINEXObs->Epochs[RINEXObs->CurrentEpoch].Sattelites[i][0] == 'C')
							{
								Sattelites[i].P[0] = ((Sattelites[i].P2[0] - gamma_B1_B2_BDS * Sattelites[i].P1[0]) + c * gamma_B1_B2_BDS * Sattelites[i].TGD1_B1_B3) / (1.0 - gamma_B1_B2_BDS);
							}
						}

						if(Settings.Trophosphere)
						{
							if(Settings.Trophosphere == 'G')
							{
								DecartToGeo(CurSolution[RINEXObs->CurrentEpoch].Q[0], CurSolution[RINEXObs->CurrentEpoch].Q[1], CurSolution[RINEXObs->CurrentEpoch].Q[2], &CurSolution[RINEXObs->CurrentEpoch].B, &CurSolution[RINEXObs->CurrentEpoch].L, &CurSolution[RINEXObs->CurrentEpoch].H);
								Sattelites[i].T = GCAT(CurSolution[RINEXObs->CurrentEpoch].H * 1.0E-3, Sattelites[i].El);
							}

							if(Settings.Trophosphere == 'S')
							{
								DecartToGeo(CurSolution[RINEXObs->CurrentEpoch].Q[0], CurSolution[RINEXObs->CurrentEpoch].Q[1], CurSolution[RINEXObs->CurrentEpoch].Q[2], &CurSolution[RINEXObs->CurrentEpoch].B, &CurSolution[RINEXObs->CurrentEpoch].L, &CurSolution[RINEXObs->CurrentEpoch].H);
								if(!Settings.p)
								{
									StandartAtmosphere(CurSolution[RINEXObs->CurrentEpoch].H, &Weather);
								}
								Sattelites[i].T = Saastamoinen(CurSolution[RINEXObs->CurrentEpoch].H * 1.0E-3, Sattelites[i].El, &Weather);
                           		//Sattelites[i].T = 0.0;
							}

							if(Settings.Trophosphere == 'D')
							{
								DecartToGeo(CurSolution[RINEXObs->CurrentEpoch].Q[0], CurSolution[RINEXObs->CurrentEpoch].Q[1], CurSolution[RINEXObs->CurrentEpoch].Q[2], &CurSolution[RINEXObs->CurrentEpoch].B, &CurSolution[RINEXObs->CurrentEpoch].L, &CurSolution[RINEXObs->CurrentEpoch].H);
								if(!Settings.p)
								{
									StandartAtmosphere(CurSolution[RINEXObs->CurrentEpoch].H, &Weather);
								}
								Sattelites[i].T = SaastamoinenDavis(CurSolution[RINEXObs->CurrentEpoch].B, CurSolution[RINEXObs->CurrentEpoch].H * 1.0E-3, Sattelites[i].El, &Weather);
							}
						}
						else
						{
							Sattelites[i].T = 0.0;
						}
					}
				}

				if(CurSolution[RINEXObs->CurrentEpoch].NOfValidSattelites >= SattelitesThreshold)
				{
					NaviSolve(Sattelites,
							  RINEXObs->Epochs[RINEXObs->CurrentEpoch].NOfSattelites,
							  SattelitesThreshold,
							  Settings.System, RINEXObs,
							  CurSolution[RINEXObs->CurrentEpoch].Q, L, A, E);
				}
				else
				{
                   CurSolution[RINEXObs->CurrentEpoch].Q[0] = 1.0;
				   CurSolution[RINEXObs->CurrentEpoch].Q[1] = 1.0;
				   CurSolution[RINEXObs->CurrentEpoch].Q[2] = 1.0;
				   CurSolution[RINEXObs->CurrentEpoch].Valid = 0;
                }

				if(fabs(CurSolution[RINEXObs->CurrentEpoch].Q[0]) >= 1.0E+8 ||
				   fabs(CurSolution[RINEXObs->CurrentEpoch].Q[1]) >= 1.0E+8 ||
				   fabs(CurSolution[RINEXObs->CurrentEpoch].Q[2]) >= 1.0E+8)
				{
						CurSolution[RINEXObs->CurrentEpoch].Q[0] = 1.0;
						CurSolution[RINEXObs->CurrentEpoch].Q[1] = 1.0;
						CurSolution[RINEXObs->CurrentEpoch].Q[2] = 1.0;
						CurSolution[RINEXObs->CurrentEpoch].Valid = 0;
				}
			}
			while(Cycles++ < Settings.Iterations && CurSolution[RINEXObs->CurrentEpoch].Valid);
			if(CurSolution[RINEXObs->CurrentEpoch].Valid)
			{
				CalcRMSE(&CurSolution[RINEXObs->CurrentEpoch], &ComSolution);
			    if(!Settings.RMSThreshold || CurSolution[RINEXObs->CurrentEpoch].RMSE3Dp <= Settings.RMSThreshold)
				{
                    CalcDOPs(&CurSolution[RINEXObs->CurrentEpoch], E);
					AddStatData(&CurSolution[RINEXObs->CurrentEpoch], &ComSolution);
					if(Settings.OutputType == TO_SCREEN || Settings.OutputType == TO_SCREEN_FILE)
					{
						printf(" %12.3lf  %12.3lf %12.3lf %12.3lf", CurSolution[RINEXObs->CurrentEpoch].Q[0], CurSolution[RINEXObs->CurrentEpoch].Q[1], CurSolution[RINEXObs->CurrentEpoch].Q[2], CurSolution[RINEXObs->CurrentEpoch].Q[3]);
						if(Settings.Geodethic)
						{
							printf("\n                      %13.8lf %13.8lf %12.3lf", CurSolution[RINEXObs->CurrentEpoch].B, CurSolution[RINEXObs->CurrentEpoch].L, CurSolution[RINEXObs->CurrentEpoch].H);
							printf(" %12.3lf", CurSolution[RINEXObs->CurrentEpoch].Q[4]);
							printf("\n                       %12.3lf  %12.3lf %12.3lf", CurSolution[RINEXObs->CurrentEpoch].RMSEHp, CurSolution[RINEXObs->CurrentEpoch].RMSEVp, CurSolution[RINEXObs->CurrentEpoch].RMSE3Dp);
							printf(" %12.3lf", CurSolution[RINEXObs->CurrentEpoch].Q[5]);
						   	printf("\n                                                               %12.3lf", CurSolution[RINEXObs->CurrentEpoch].Q[6]);
						}
						else
						{
							printf("\n                       %12.3lf  %12.3lf %12.3lf", CurSolution[RINEXObs->CurrentEpoch].RMSEHp, CurSolution[RINEXObs->CurrentEpoch].RMSEVp, CurSolution[RINEXObs->CurrentEpoch].RMSE3Dp);
							printf(" %12.3lf", CurSolution[RINEXObs->CurrentEpoch].Q[4]);
							printf("\n                                                               %12.3lf", CurSolution[RINEXObs->CurrentEpoch].Q[5]);
                            printf("\n                                                               %12.3lf", CurSolution[RINEXObs->CurrentEpoch].Q[6]);
						}
					}
				}
				else
				{
					CurSolution[RINEXObs->CurrentEpoch].Valid = 0;
				}
			}
		}
		else
		{
			CurSolution[RINEXObs->CurrentEpoch].Valid = 0;
		}
		k = Settings.Step;
		while(k > RINEXObs->Interval)
		{
            RINEXObs->CurrentEpoch++;
			k -= RINEXObs->Interval;
		}

		if(kbhit() && getch() == 27)
		{
			RINEXObs->CurrentEpoch = RINEXObs->NOfEpochs;
		}
		RINEXObs->CurrentEpoch++;
	}
	CalcStatistics(CurSolution, &ComSolution, RINEXObs->NOfEpochs);
	if((Settings.OutputType == TO_SCREEN || Settings.OutputType == TO_SCREEN_FILE) && !Settings.AccumulateOutputData)
	{
		printf("\n --------------------------------------------------------------------------");
		printf("\n median xm ym zm:      %12.3lf  %12.3lf %12.3lf", ComSolution.Qm[0], ComSolution.Qm[1], ComSolution.Qm[2]);
		printf("\n median dxm dym dzm:   %12.3E  %12.3E %12.3E", ComSolution.dQpm[0], ComSolution.dQpm[1], ComSolution.dQpm[2]);
		printf("\n                       %12.3E  %12.3E %12.3E",  ComSolution.dQmm[0], ComSolution.dQmm[1], ComSolution.dQmm[2]);
		printf("\n RMSE SEN SU SENU:     %12.3lf  %12.3lf %12.3lf", ComSolution.SRMSEHp, ComSolution.SRMSEVp, ComSolution.SRMSE3Dp);
		printf("\n                       %12.3lf  %12.3lf %12.3lf", ComSolution.SRMSEHm, ComSolution.SRMSEVm, ComSolution.SRMSE3Dm);
	}
	t2 = clock();
	if(Settings.OutputType == TO_FILE || Settings.OutputType == TO_SCREEN_FILE)
	{
        WriteSolutionToFile(outputfile, RINEXObs->Epochs,
						CurSolution, RINEXObs->NOfEpochs, &Settings);

		RMSfile = fopen(RMSFileName, "r");
		if(!RMSfile)
		{
			RMSfile = fopen(RMSFileName, "w");
			if(!RMSfile)
			{
				printf("Unable to create or open RMS file %s", RMSFileName);
				if(Settings.OutputType == TO_SCREEN || Settings.OutputType == TO_SCREEN_FILE)
				{
					getch();
				}
				return 1;
			}

			fprintf(RMSfile, "\n -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
			fprintf(RMSfile, "\n Year DOY                  SEN(m)       SU(m)       SENU(m)                                            dx(m)         dy(m)        dz(m)                                               xm(m)        ym(m)        zm(m)        t");
			fprintf(RMSfile, "\n -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
		}
		fclose(RMSfile);
		RMSfile = fopen(RMSFileName, "a");
        if(!RMSfile)
		{
			printf("Unable to create or open RMS file %s", RMSFileName);
			if(Settings.OutputType == TO_SCREEN || Settings.OutputType == TO_SCREEN_FILE)
			{
				getch();
			}
			return 1;
		}
		DOY = DateToDOY(RINEXObs->Epochs[RINEXObs->NOfEpochs / 2].Year,
						RINEXObs->Epochs[RINEXObs->NOfEpochs / 2].Month,
						RINEXObs->Epochs[RINEXObs->NOfEpochs / 2].Day);
		strcat(RMSString, OutputFileParams);
		fprintf(RMSfile, "\n %4d %03d %9.4f", RINEXObs->Epochs[RINEXObs->NOfEpochs / 2].Year, DOY,
											  RINEXObs->Epochs[RINEXObs->NOfEpochs / 2].Year + DOY / 365.25);
		//fprintf(RMSfile, " %-25s %12.3lf %12.3lf %12.3lf", RMSString, Solution.SRMSErrorHp, Solution.SRMSErrorVp, Solution.SRMSError3Dp);
		if(RINEXObs->x == 1 &&  RINEXObs->y == 1 && RINEXObs->z == 1)
		{
			ComSolution.SRMSEHp = 99999999.999;
			ComSolution.SRMSEVp = 99999999.999;
			ComSolution.SRMSE3Dp = 99999999.999;
		}
		fprintf(RMSfile, " %12.3lf %12.3lf %12.3lf", ComSolution.SRMSEHp, ComSolution.SRMSEVp, ComSolution.SRMSE3Dp);
		fprintf(RMSfile, " %12.3lf %12.3lf %12.3lf", ComSolution.SRMSEHm, ComSolution.SRMSEVm, ComSolution.SRMSE3Dm);
		fprintf(RMSfile, " %12.3E  %12.3E %12.3E", ComSolution.dQpm[0], ComSolution.dQpm[1], ComSolution.dQpm[2]);
		fprintf(RMSfile, " %12.3E  %12.3E %12.3E", ComSolution.dQmm[0], ComSolution.dQmm[1], ComSolution.dQmm[2]);
		fprintf(RMSfile, " %12.3lf %12.3lf %12.3lf", ComSolution.Qm[0], ComSolution.Qm[1], ComSolution.Qm[2]);
		fprintf(RMSfile, " %7.3f", (float)(t2 - t1) / CLK_TCK);
		fclose(RMSfile);
	}
	fclose(outputfile);
	for(i = 0; i < RINEXObs->NOfEpochs; i++)
	{
		for(j = 0; j < RINEXObs->Epochs[i].NOfSattelites; j++)
		{
			for(k = 0; k < MAX_TYPES_OF_OBSERV; k++)
			{
				free(RINEXObs->Epochs[i].Values[j][k]);
			}
			free(RINEXObs->Epochs[i].Sattelites[j]);
			free(RINEXObs->Epochs[i].Values[j]);
		}
		free(RINEXObs->Epochs[i].Sattelites);
		free(RINEXObs->Epochs[i].Values);
	}
	free(RINEXObs->Epochs);
	free(RINEXObs);
	free(RINEXNav->GPSEphemeris);
	free(RINEXNav->GLOEphemeris);
	free(RINEXNav->GALEphemeris);
	free(RINEXNav->BDSEphemeris);
	free(RINEXNav);
	free(SP3->GPSEphemeris);
	free(SP3->GLOEphemeris);
	free(SP3->GALEphemeris);
    free(SP3->BDSEphemeris);
	free(SP3);
	free(CurSolution);
	for(i = 0; i < 3; i++)
	{
		free(ComSolution.Q[i]);
		free(ComSolution.dQp[i]);
		free(ComSolution.dQm[i]);
	}

	for(i = 0; i < NOfTayAbsTECFiles; i++)
	{
		free(TayAbsTEC[i].TECRecords);
	}
	free(TayAbsTEC);
	for(i = 0; i < Settings.NOfExcludeSattelites; i++)
	{
		free(Settings.ExcludeSattelites[i]);
	}
	free(Settings.ExcludeSattelites);
    if(Settings.OutputType == TO_SCREEN || Settings.OutputType == TO_SCREEN_FILE)
	{
		printf("\nPress any key to exit...");
		getch();
	}
    return 0;
}
