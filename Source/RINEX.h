#ifndef RINEX_H
#define RINEX_H

#include <stdio.h>
#include <dir.h>
#include <string.h>

#include "Common.h"

struct RINEXGPSEphemeris
{
	short Year;
	char Number[2], Month, Day, Hours, Minutes;
	float Seconds;
	double toc, a0, a1, a2,
		   Crs, dn, M0,
		   Cuc, e, Cus, sqrta,
		   toe, Cic, OMEGA0, Cis,
		   i0, Crc, omega, OMEGAi,
		   ii,
		   Health, TGD;
};

struct RINEXGLOEphemeris
{
	short Year;
	char Number[2], Month, Day, Hours, Minutes;
	float Seconds;
	double toc, TauN, GammaN,
		   x0, vx0, ax, Bn,
		   y0, vy0, ay, k,
		   z0, vz0, az,
		            dTauN;
};

struct RINEXGALEphemeris
{
    enum GALNavMsgType GALNavMsgType;
	short Year;
	char Number[2], Month, Day, Hours, Minutes;
	float Seconds;
	double toc, a0, a1, a2,
		   Crs, dn, M0,
		   Cuc, e, Cus, sqrta,
		   toe, Cic, OMEGA0, Cis,
		   i0, Crc, omega, OMEGAi,
		   ii, Ds,
		   Health, BGD_E1_E5a, BGD_E1_E5b;
};

struct RINEXBDSEphemeris
{
	short Year;
	char Number[2], Month, Day, Hours, Minutes;
	float Seconds;
	double toc, a0, a1, a2,
		   Crs, dn, M0,
		   Cuc, e, Cus, sqrta,
		   toe, Cic, OMEGA0, Cis,
		   i0, Crc, omega, OMEGAi,
		   ii,
		   SatH1, TGD1_B1_B3, TGD2_B2_B3;
};

struct RINEXNav
{
	char Type, SatteliteMask;
	double AlphaGPS[4], BetaGPS[4], AlphaBDSK[4], BetaBDSK[4], AzGAL[3],
		   AlphaBDS[12][9],
		   a0, A1, T, W,
		   TauC;
	float Version, LeapSeconds;
	int NOfGPSEphemeris, NOfGLOEphemeris, NOfGALEphemeris, NOfBDSEphemeris;
	struct RINEXGPSEphemeris *GPSEphemeris;
	struct RINEXGLOEphemeris *GLOEphemeris;
	struct RINEXGALEphemeris *GALEphemeris;
    struct RINEXBDSEphemeris *BDSEphemeris;
};

struct Epoch
{
	short Year;
	char Month, Day, Hours, Minutes, NOfSattelites, **Sattelites;
	float TimeInHours, Seconds;
	double TimeInHours1, t, ***Values;
};

struct RINEXObs
{
	char GPS, GLO, GAL, BDS,
		 NOfTypesOfObservInString, Systems[TYPES_OF_SYSTEMS],
		 NOfTypesOfObserv[TYPES_OF_SYSTEMS],
		 TypesOfObserv[64][3];
	int IndexOfC1C[TYPES_OF_SYSTEMS], IndexOfC2C[TYPES_OF_SYSTEMS],
		IndexOfC1P[TYPES_OF_SYSTEMS], IndexOfC2P[TYPES_OF_SYSTEMS],
		IndexOfC5Q[TYPES_OF_SYSTEMS], IndexOfC7Q[TYPES_OF_SYSTEMS],
		IndexOfC2I[TYPES_OF_SYSTEMS], IndexOfC7I[TYPES_OF_SYSTEMS],
		NOfEpochs, CurrentEpoch;
	float Version, Interval,
		  LeapSeconds;
	double x, y, z;
	struct Epoch *Epochs;
};

int ReadRINEXNavIonData(char*, struct RINEXNav*);
int ReadRINEXEphemeris(char*, struct RINEXNav*);
int ReadRINEXObsHeader(char*, struct RINEXObs*);
int ReadRINEXObs(char*, struct RINEXObs*);

#endif
