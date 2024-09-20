#ifndef DATAFILES_H
#define DATAFILES_H

#include <stdio.h>
#include <dir.h>

#include "Constants.h"
#include "RINEX.h"
#include "Common.h"
#include "Solution.h"

struct Settings
{
	char SettingName[64],
		 RINEXObsFile[MAXPATH],
		 EphemerisFiles[MAX_OF_SYSTEMS + 1][MAXPATH],
		 BDGIMFile[MAXPATH],
		 IONEXFile[MAXPATH],
		 TECFile[MAXPATH],
		 TayAbsTECPath[MAXPATH],
		 OutputPath[MAXPATH],
         SpecSatFile[MAXPATH],
		 ExcludeSattelitesString[64],
		 System[MAX_OF_SYSTEMS + 1],
		 FrequencyMode[3 + 1],
         GalileoDataType,
		 InitialHours, InitialMinutes,
		 FinalHours, FinalMinutes,
		 NOfExcludeSattelites,
         SNRMask,
		 Ionosphere,
		 IonoMapping,
		 Troposphere,
         TropoMapping,
		 Iterations,
		 Geodethic,
		 OutputType,
		 AccumulateOutputData,
		 DTM,
         **ExcludeSattelites;
	float InitialSeconds, FinalSeconds, ElevationMask, Step, A, F107, Ap, RE, hI,
		  p, t, r, RMSThreshold;
	double InitialTime, FinalTime,
		   x, y, z, B, L, H,
		   x0, y0, z0,
		   Alpha[4], Beta[4], Az[3];
};

struct TECRecord
{
	float UT;
	double TEC;
};

struct TayAbsTEC
{
	char SatNumber[2];
   	int NOfRecords;
	double UTInt[INTERPOLATION_ORDER + 1],
		   TECInt[INTERPOLATION_ORDER + 1];
	struct TECRecord *TECRecords;
};

short DateToDOY(short, char, char);
void SetDefaultSettings(struct Settings*);
int ReadSettings(char*, struct Settings*);
int WriteSettings(char*, struct Settings*);
void WriteOutputFileHeader(FILE*, struct Settings*, struct CurSolution*,
						   struct ComSolution*);
void WriteSolutionToFile(FILE*, struct Epoch*, struct CurSolution*, int,
						 struct Settings*);
void ConvertTECToIONEX(char*, char*);
int ReadTayAbsTECData(char*, struct TayAbsTEC*);
int SortFunctionTEC(const void*, const void*);
int ReadSpecSatData(char*, struct RINEXObs*);

#endif
