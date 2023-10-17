#include "RINEX.h"

#include <stdlib.h>
#undef abs
#include <math.h>
//#include <alloc.h>
#include <string.h>
#include <time.h>

int ReadRINEXNavIonData(char *EphemerisFileName, struct RINEXNav *RINEXNav)
{
	int i;
	char *Pch, CurrentString[BYTES_OF_STRING];
	FILE *EphemerisFile = fopen(EphemerisFileName, "r");
	if(!EphemerisFile)
	{
		return 0;
	}
	Pch = FindStringWithData(EphemerisFile, "RINEX VERSION / TYPE");
	if(Pch)
	{
		strcpy(CurrentString, Pch);
		sscanf(CurrentString, "%f", &RINEXNav->Version);
	}

    if(RINEXNav->Version < 3.0)
	{
		if(EphemerisFileName[strlen(EphemerisFileName) - 1] == 'n')
		{
			Pch = FindStringWithData(EphemerisFile, "ION ALPHA");
			if(Pch)
			{
				strcpy(CurrentString, Pch);
				for(i = 0; i < BYTES_OF_STRING; i++)
				{
					if(CurrentString[i] == 'D')
					{
						CurrentString[i] = 'E';
					}
				}
				sscanf(CurrentString, "%lf%lf%lf%lf",
					   &RINEXNav->AlphaGPS[0],
					   &RINEXNav->AlphaGPS[1],
					   &RINEXNav->AlphaGPS[2],
					   &RINEXNav->AlphaGPS[3]);
			}
			Pch = FindStringWithData(EphemerisFile, "ION BETA");
			if(Pch)
			{
				strcpy(CurrentString, Pch);
				for(i = 0; i < BYTES_OF_STRING; i++)
				{
					if(CurrentString[i] == 'D')
					{
						CurrentString[i] = 'E';
					}
				}
				sscanf(CurrentString, "%lf%lf%lf%lf",
					   &RINEXNav->BetaGPS[0],
					   &RINEXNav->BetaGPS[1],
					   &RINEXNav->BetaGPS[2],
					   &RINEXNav->BetaGPS[3]);
			}
		}

		if(EphemerisFileName[strlen(EphemerisFileName) - 1] == 'l')
		{
			Pch = FindStringWithData(EphemerisFile, "ION ALPHA");
			if(Pch)
			{
				strcpy(CurrentString, Pch);
				for(i = 0; i < BYTES_OF_STRING; i++)
				{
					if(CurrentString[i] == 'D')
					{
						CurrentString[i] = 'E';
					}
				}
				sscanf(CurrentString, "%lf%lf%lf",
					   &RINEXNav->AzGAL[0],
					   &RINEXNav->AzGAL[1],
					   &RINEXNav->AzGAL[2]);
			}
			Pch = FindStringWithData(EphemerisFile, "GAL ");
			if(Pch)
			{
				strcpy(CurrentString, Pch);
				for(i = 0; i < BYTES_OF_STRING; i++)
				{
					if(CurrentString[i] == 'D')
					{
						CurrentString[i] = 'E';
					}
				}
				sscanf(CurrentString, "%*s%lf%lf%lf",
					   &RINEXNav->AzGAL[0],
					   &RINEXNav->AzGAL[1],
					   &RINEXNav->AzGAL[2]);
			}
		}
	}
	else
	{
		Pch = FindStringWithData(EphemerisFile, "GPSA");
		if(Pch)
		{
			strcpy(CurrentString, Pch);
			for(i = 0; i < BYTES_OF_STRING; i++)
			{
				if(CurrentString[i] == 'D')
				{
					CurrentString[i] = 'E';
				}
			}
			sscanf(CurrentString, "%*s%lf%lf%lf%lf",
				   &RINEXNav->AlphaGPS[0],
				   &RINEXNav->AlphaGPS[1],
				   &RINEXNav->AlphaGPS[2],
				   &RINEXNav->AlphaGPS[3]);
		}
		Pch = FindStringWithData(EphemerisFile, "GPSB");
		if(Pch)
		{
			strcpy(CurrentString, Pch);
			for(i = 0; i < BYTES_OF_STRING; i++)
			{
				if(CurrentString[i] == 'D')
				{
					CurrentString[i] = 'E';
				}
			}
			sscanf(CurrentString, "%*s%lf%lf%lf%lf",
				   &RINEXNav->BetaGPS[0],
				   &RINEXNav->BetaGPS[1],
				   &RINEXNav->BetaGPS[2],
				   &RINEXNav->BetaGPS[3]);
		}
		Pch = FindStringWithData(EphemerisFile, "GAL ");
		if(Pch)
		{
			strcpy(CurrentString, Pch);
			for(i = 0; i < BYTES_OF_STRING; i++)
			{
				if(CurrentString[i] == 'D')
				{
					CurrentString[i] = 'E';
				}
			}
			sscanf(CurrentString, "%*s%lf%lf%lf",
				   &RINEXNav->AzGAL[0],
				   &RINEXNav->AzGAL[1],
				   &RINEXNav->AzGAL[2]);
		}
	}
    fclose(EphemerisFile);
    return 1;
}

int ReadRINEXEphemeris(char *EphemerisFileName, struct RINEXNav *RINEXNav)
{
	int i, j;
	char *Pch, CurrentString[BYTES_OF_STRING], NavMessage[8][BYTES_OF_STRING];
	size_t SizeOfRINEXGPSEphemeris = sizeof(struct RINEXGPSEphemeris);
	size_t SizeOfRINEXGLOEphemeris = sizeof(struct RINEXGLOEphemeris);
	size_t SizeOfRINEXGALEphemeris = sizeof(struct RINEXGALEphemeris);
	size_t SizeOfRINEXBDSEphemeris = sizeof(struct RINEXBDSEphemeris);
	FILE *EphemerisFile = fopen(EphemerisFileName, "r");
	if(!EphemerisFile)
	{
		return 0;
	}
	Pch = FindStringWithData(EphemerisFile, "RINEX VERSION / TYPE");
	if(Pch)
	{
		strcpy(CurrentString, Pch);
		sscanf(CurrentString, "%f", &RINEXNav->Version);
	}
	Pch = FindStringWithData(EphemerisFile, "CORR TO SYSTEM TIME");
	if(Pch)
	{
		strcpy(CurrentString, Pch);
		for(i = 0; i < BYTES_OF_STRING; i++)
		{
			if(CurrentString[i] == 'D')
			{
				CurrentString[i] = 'E';
			}
		}
		sscanf(CurrentString, "%*d%*d%*d%lf", &RINEXNav->TauC);
	}
	Pch = FindStringWithData(EphemerisFile, "GLUT");
	if(Pch)
	{
		strcpy(CurrentString, Pch);
		for(i = 0; i < BYTES_OF_STRING; i++)
		{
			if(CurrentString[i] == 'D')
			{
				CurrentString[i] = 'E';
			}
		}
		sscanf(CurrentString, "%*s%lf", &RINEXNav->TauC);
	}
	Pch = FindStringWithData(EphemerisFile, "LEAP SECONDS");
	if(Pch)
	{
		strcpy(CurrentString, Pch);
		sscanf(CurrentString, "%f", &RINEXNav->LeapSeconds);
	}
	RINEXNav->TauC = -RINEXNav->TauC;
	FindStringWithData(EphemerisFile, "END OF HEADER");
    if(!RINEXNav->NOfGPSEphemeris)
	{
		RINEXNav->GPSEphemeris =
			(struct RINEXGPSEphemeris*)calloc(1, SizeOfRINEXGPSEphemeris);
	}

	if(!RINEXNav->NOfGLOEphemeris)
	{
		RINEXNav->GLOEphemeris =
			(struct RINEXGLOEphemeris*)calloc(1, SizeOfRINEXGLOEphemeris);
	}

	if(!RINEXNav->NOfGALEphemeris)
	{
		RINEXNav->GALEphemeris =
			(struct RINEXGALEphemeris*)calloc(1, SizeOfRINEXGALEphemeris);
	}

	if(!RINEXNav->NOfBDSEphemeris)
	{
		RINEXNav->BDSEphemeris =
			(struct RINEXBDSEphemeris*)calloc(1, SizeOfRINEXBDSEphemeris);
	}

	if(RINEXNav->Version < 3.00)
	{
		if(EphemerisFileName[strlen(EphemerisFileName) - 1] == 'n')
		{
		   	while(!feof(EphemerisFile))
			   //	FindStringWithData(EphemerisFile, "END OF HEADER");
	   //	while(fgetc(EphemerisFile) != EOF)
		//while(Result != NULL)
			{
				RINEXNav->GPSEphemeris = (struct RINEXGPSEphemeris*)
										 realloc(RINEXNav->GPSEphemeris,
												 (RINEXNav->NOfGPSEphemeris + 1)
												 * SizeOfRINEXGPSEphemeris);
				for(i = 0; i < 8; i++)
				{
					fgets(CurrentString, BYTES_OF_STRING, EphemerisFile);
					for(j = 0; j < BYTES_OF_STRING; j++)
					{
						if(CurrentString[j] == 'D')
						{
							CurrentString[j] = 'E';
						}
					}
					strcpy(NavMessage[i], CurrentString);
				}
				RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Number[0] = 'G';
				sscanf(NavMessage[0], "%hhd%hd%hhd%hhd%hhd%hhd%f%lf%lf%lf",
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Number[1],
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Year,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Month,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Day,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Hours,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Minutes,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Seconds,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].a0,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].a1,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].a2);
//				if((RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Minutes % 10) == 0 ||
//				   ((int)RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Seconds % 10) == 0)
//				{
					sscanf(NavMessage[1], "%*lf%lf%lf%lf",
						   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Crs,
						   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].dn,
						   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].M0);
					sscanf(NavMessage[2], "%lf%lf%lf%lf",
						   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Cuc,
						   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].e,
						   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Cus,
						   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].sqrta);
					sscanf(NavMessage[3], "%lf%lf%lf%lf",
						   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].toe,
						   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Cic,
						   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].OMEGA0,
						   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Cis);
					sscanf(NavMessage[4], "%lf%lf%lf%lf",
						   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].i0,
						   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Crc,
						   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].omega,
						   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].OMEGAi);
					sscanf(NavMessage[5], "%lf%*lf%*lf%*lf",
						   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].ii);
					sscanf(NavMessage[6], "%*lf%lf%lf%*lf",
						   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Health,
						   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].TGD);
					if(RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Year >= 80 &&
					   RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Year <= 99)
					{
						RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Year += 1900;
					}

					if(RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Year >= 0 &&
					   RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Year <= 79)
					{
						RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Year += 2000;
					}

//					if(RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Seconds > 30.0)
//					{
//						RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Seconds = 0.0;
//						RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Minutes = 0;
//                        RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Hours += 1;
//                    }
					RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].toc =
					DateTimeToUNIXTime(RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Year,
									   RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Month,
									   RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Day,
									   RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Hours,
									   RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Minutes,
									   RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Seconds);
					RINEXNav->NOfGPSEphemeris++;
               // }

			}
		}

		if(EphemerisFileName[strlen(EphemerisFileName) - 1] == 'g')
		{
			while(!feof(EphemerisFile))
			//while(fgetc(EphemerisFile) != EOF)
			//while(Result != NULL)
			{
				RINEXNav->GLOEphemeris = (struct RINEXGLOEphemeris*)
										 realloc(RINEXNav->GLOEphemeris,
												 (RINEXNav->NOfGLOEphemeris + 1)
												 * SizeOfRINEXGLOEphemeris);
				for(i = 0; i < 4; i++)
				{
					fgets(CurrentString, BYTES_OF_STRING, EphemerisFile);
					for(j = 0; j < BYTES_OF_STRING; j++)
					{
						if(CurrentString[j] == 'D')
						{
							CurrentString[j] = 'E';
						}
					}
					strcpy(NavMessage[i], CurrentString);
				}
				RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Number[0] = 'R';
				sscanf(NavMessage[0], "%hhd%hd%hhd%hhd%hhd%hhd%f%lf%lf%*lf",
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Number[1],
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Year,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Month,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Day,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Hours,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Minutes,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Seconds,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].TauN,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].GammaN);
				sscanf(NavMessage[1], "%lf%lf%lf%lf",
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].x0,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].vx0,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].ax,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Bn);
				sscanf(NavMessage[2], "%lf%lf%lf%lf",
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].y0,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].vy0,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].ay,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].k);
				sscanf(NavMessage[3], "%lf%lf%lf%*lf",
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].z0,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].vz0,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].az);
			   if(RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Year >= 80 &&
				  RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Year <= 99)
				{
					RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Year += 1900;
				}

				if(RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Year >= 0 &&
				   RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Year <= 79)
				{
					RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Year += 2000;
				}
				RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].toc =
				DateTimeToUNIXTime(RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Year,
								   RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Month,
								   RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Day,
								   RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Hours,
								   RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Minutes,
								   RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Seconds);
				RINEXNav->NOfGLOEphemeris++;
			}
		}

		if(EphemerisFileName[strlen(EphemerisFileName) - 1] == 'l')
		{
			while(!feof(EphemerisFile))
			{
				fgets(CurrentString, BYTES_OF_STRING, EphemerisFile);
				sscanf(CurrentString, "%c", &RINEXNav->SatteliteMask);
				RINEXNav->GALEphemeris = (struct RINEXGALEphemeris*)
										 realloc(RINEXNav->GALEphemeris,
												 (RINEXNav->NOfGALEphemeris + 1)
												 * SizeOfRINEXGALEphemeris);
				for(i = 0; i < 8; i++)
				{
					if(i)
					{
						fgets(CurrentString, BYTES_OF_STRING, EphemerisFile);
					}

					for(j = 0; j < BYTES_OF_STRING; j++)
					{
						if(CurrentString[j] == 'D')
						{
							CurrentString[j] = 'E';
						}
					}
					strcpy(NavMessage[i], CurrentString);
				}

				if(RINEXNav->SatteliteMask == 'E')
				{
					sscanf(NavMessage[0], "%c%hhd%hd%hhd%hhd%hhd%hhd%f%lf%lf%lf",
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Number[0],
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Number[1],
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Year,
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Month,
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Day,
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Hours,
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Minutes,
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Seconds,
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].a0,
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].a1,
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].a2);
				}
				else
				{
					RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Number[0] = 'E';
					sscanf(NavMessage[0], "%hhd%hd%hhd%hhd%hhd%hhd%f%lf%lf%lf",
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Number[1],
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Year,
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Month,
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Day,
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Hours,
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Minutes,
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Seconds,
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].a0,
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].a1,
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].a2);
				}
				sscanf(NavMessage[1], "%*lf%lf%lf%lf",
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Crs,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].dn,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].M0);
				sscanf(NavMessage[2], "%lf%lf%lf%lf",
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Cuc,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].e,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Cus,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].sqrta);
				sscanf(NavMessage[3], "%lf%lf%lf%lf",
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].toe,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Cic,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].OMEGA0,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Cis);
				sscanf(NavMessage[4], "%lf%lf%lf%lf",
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].i0,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Crc,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].omega,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].OMEGAi);
				sscanf(NavMessage[5], "%lf%lf%*lf%*lf",
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].ii,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Ds);
				sscanf(NavMessage[6], "%*lf%lf%lf%lf",
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Health,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].BGD_E1_E5a,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].BGD_E1_E5b);
				if(RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Year >= 80 &&
				   RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Year <= 99)
				{
					RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Year += 1900;
				}

				if(RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Year >= 0 &&
				   RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Year <= 79)
				{
					RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Year += 2000;
				}
				RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].toc =
				DateTimeToUNIXTime(RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Year,
								   RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Month,
								   RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Day,
								   RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Hours,
								   RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Minutes,
								   RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Seconds);
				if((int)RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Ds & (1 << 8))
				{
					RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].GALNavMsgType = F_NAV;
				}

				if((int)RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Ds & (1 << 9))
				{
					RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].GALNavMsgType = I_NAV;
				}
				RINEXNav->NOfGALEphemeris++;
			}
		}
	}
	else
	{
		while(!feof(EphemerisFile))
		{
			fgets(CurrentString, BYTES_OF_STRING, EphemerisFile);
			sscanf(CurrentString, "%c", &RINEXNav->SatteliteMask);
			if(RINEXNav->SatteliteMask == 'G')
			{
				RINEXNav->GPSEphemeris = (struct RINEXGPSEphemeris*)
										 realloc(RINEXNav->GPSEphemeris,
												 (RINEXNav->NOfGPSEphemeris + 1)
												 * SizeOfRINEXGPSEphemeris);
				for(i = 0; i < 8; i++)
				{
					if(i)
					{
						fgets(CurrentString, BYTES_OF_STRING, EphemerisFile);
					}

					for(j = 0; j < BYTES_OF_STRING; j++)
					{
						if(CurrentString[j] == 'D')
						{
							CurrentString[j] = 'E';
						}
					}
					strcpy(NavMessage[i], CurrentString);
				}
				sscanf(NavMessage[0], "%c%hhd%hd%hhd%hhd%hhd%hhd%f%lf%lf%lf",
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Number[0],
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Number[1],
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Year,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Month,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Day,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Hours,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Minutes,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Seconds,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].a0,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].a1,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].a2);
				sscanf(NavMessage[1], "%*lf%lf%lf%lf",
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Crs,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].dn,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].M0);
				sscanf(NavMessage[2], "%lf%lf%lf%lf",
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Cuc,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].e,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Cus,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].sqrta);
				sscanf(NavMessage[3], "%lf%lf%lf%lf",
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].toe,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Cic,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].OMEGA0,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Cis);
				sscanf(NavMessage[4], "%lf%lf%lf%lf",
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].i0,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Crc,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].omega,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].OMEGAi);
				sscanf(NavMessage[5], "%lf%*lf%*lf%*lf",
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].ii);
				sscanf(NavMessage[6], "%*lf%lf%lf%*lf",
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Health,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].TGD);
				if(RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Year >= 80 &&
				   RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Year <= 99)
				{
					RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Year += 1900;
				}

				if(RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Year >= 0 &&
				   RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Year <= 79)
				{
					RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Year += 2000;
				}
				RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].toc =
				DateTimeToUNIXTime(RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Year,
								   RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Month,
								   RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Day,
								   RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Hours,
								   RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Minutes,
								   RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Seconds);
				RINEXNav->NOfGPSEphemeris++;
			}

			if(RINEXNav->SatteliteMask == 'R')
			{
				RINEXNav->GLOEphemeris = (struct RINEXGLOEphemeris*)
										 realloc(RINEXNav->GLOEphemeris,
												 (RINEXNav->NOfGLOEphemeris + 1)
												 * SizeOfRINEXGLOEphemeris);
				for(i = 0; i < 4; i++)
				{
					if(i)
					{
						fgets(CurrentString, BYTES_OF_STRING, EphemerisFile);
					}

					for(j = 0; j < BYTES_OF_STRING; j++)
					{
						if(CurrentString[j] == 'D')
						{
							CurrentString[j] = 'E';
						}
					}
					strcpy(NavMessage[i], CurrentString);
				}
				sscanf(NavMessage[0], "%c%hhd%hd%hhd%hhd%hhd%hhd%f%lf%lf%*lf",
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Number[0],
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Number[1],
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Year,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Month,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Day,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Hours,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Minutes,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Seconds,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].TauN,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].GammaN);
				sscanf(NavMessage[1], "%lf%lf%lf%lf",
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].x0,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].vx0,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].ax,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Bn);
				sscanf(NavMessage[2], "%lf%lf%lf%lf",
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].y0,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].vy0,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].ay,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].k);
				sscanf(NavMessage[3], "%lf%lf%lf%*lf",
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].z0,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].vz0,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].az);
				if(RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Year >= 80 &&
				   RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Year <= 99)
				{
					RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Year += 1900;
				}

				if(RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Year >= 0 &&
				   RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Year <= 79)
				{
					RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Year += 2000;
				}
				RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].toc =
				DateTimeToUNIXTime(RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Year,
								   RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Month,
								   RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Day,
								   RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Hours,
								   RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Minutes,
								   RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Seconds);
				RINEXNav->NOfGLOEphemeris++;
			}

			if(RINEXNav->SatteliteMask == 'E')
			{
				RINEXNav->GALEphemeris = (struct RINEXGALEphemeris*)
										 realloc(RINEXNav->GALEphemeris,
												 (RINEXNav->NOfGALEphemeris + 1)
												 * SizeOfRINEXGALEphemeris);
				for(i = 0; i < 8; i++)
				{
					if(i)
					{
						fgets(CurrentString, BYTES_OF_STRING, EphemerisFile);
					}

					for(j = 0; j < BYTES_OF_STRING; j++)
					{
						if(CurrentString[j] == 'D')
						{
							CurrentString[j] = 'E';
						}
					}
					strcpy(NavMessage[i], CurrentString);
				}
				sscanf(NavMessage[0], "%c%hhd%hd%hhd%hhd%hhd%hhd%f%lf%lf%lf",
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Number[0],
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Number[1],
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Year,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Month,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Day,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Hours,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Minutes,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Seconds,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].a0,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].a1,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].a2);
				sscanf(NavMessage[1], "%*lf%lf%lf%lf",
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Crs,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].dn,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].M0);
				sscanf(NavMessage[2], "%lf%lf%lf%lf",
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Cuc,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].e,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Cus,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].sqrta);
				sscanf(NavMessage[3], "%lf%lf%lf%lf",
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].toe,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Cic,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].OMEGA0,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Cis);
				sscanf(NavMessage[4], "%lf%lf%lf%lf",
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].i0,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Crc,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].omega,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].OMEGAi);
				sscanf(NavMessage[5], "%lf%lf%*lf%*lf",
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].ii,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Ds);
				sscanf(NavMessage[6], "%*lf%lf%lf%lf",
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Health,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].BGD_E1_E5a,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].BGD_E1_E5b);
				if(RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Year >= 80 &&
				   RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Year <= 99)
				{
					RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Year += 1900;
				}

				if(RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Year >= 0 &&
				   RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Year <= 79)
				{
					RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Year += 2000;
				}
				RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].toc =
				DateTimeToUNIXTime(RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Year,
								   RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Month,
								   RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Day,
								   RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Hours,
								   RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Minutes,
								   RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Seconds);
				if((int)RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Ds & (1 << 8))
				{
					RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].GALNavMsgType = F_NAV;
				}

				if((int)RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Ds & (1 << 9))
				{
					RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].GALNavMsgType = I_NAV;
				}
				RINEXNav->NOfGALEphemeris++;
			}

			if(RINEXNav->SatteliteMask == 'C')
			{

				RINEXNav->BDSEphemeris = (struct RINEXBDSEphemeris*)
										 realloc(RINEXNav->BDSEphemeris,
												 (RINEXNav->NOfBDSEphemeris + 1)
												 * SizeOfRINEXBDSEphemeris);
				for(i = 0; i < 8; i++)
				{
					if(i)
					{
						fgets(CurrentString, BYTES_OF_STRING, EphemerisFile);
					}

					for(j = 0; j < BYTES_OF_STRING; j++)
					{
						if(CurrentString[j] == 'D')
						{
							CurrentString[j] = 'E';
						}
					}
					strcpy(NavMessage[i], CurrentString);
				}
				sscanf(NavMessage[0], "%c%hhd%hd%hhd%hhd%hhd%hhd%f%lf%lf%lf",
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Number[0],
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Number[1],
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Year,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Month,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Day,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Hours,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Minutes,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Seconds,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].a0,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].a1,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].a2);
				sscanf(NavMessage[1], "%*lf%lf%lf%lf",
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Crs,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].dn,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].M0);
				sscanf(NavMessage[2], "%lf%lf%lf%lf",
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Cuc,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].e,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Cus,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].sqrta);
				sscanf(NavMessage[3], "%lf%lf%lf%lf",
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].toe,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Cic,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].OMEGA0,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Cis);
				sscanf(NavMessage[4], "%lf%lf%lf%lf",
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].i0,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Crc,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].omega,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].OMEGAi);
				sscanf(NavMessage[5], "%lf%*lf%*lf%*lf",
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].ii);
				sscanf(NavMessage[6], "%*lf%lf%lf%lf",
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].SatH1,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].TGD1_B1_B3,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].TGD2_B2_B3);
				if(RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Year >= 80 &&
				   RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Year <= 99)
				{
					RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Year += 1900;
				}

				if(RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Year >= 0 &&
				   RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Year <= 79)
				{
					RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Year += 2000;
				}
				RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].toc =
				DateTimeToUNIXTime(RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Year,
								   RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Month,
								   RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Day,
								   RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Hours,
								   RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Minutes,
								   RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Seconds);
				RINEXNav->NOfBDSEphemeris++;
			}
		}
	}
	fclose(EphemerisFile);
	return 1;
}

int ReadRINEXObsHeader(char *ObservFileName, struct RINEXObs *RINEXObs)
{
	int i, j, k;
	char *Pch, CurrentString[BYTES_OF_STRING];
	//size_t SizeOfChar = sizeof(char);
	//size_t SizeOfCharPtr = sizeof(char*);
	FILE *ObservFile = fopen(ObservFileName, "r");
	if(!ObservFile)
	{
		return 0;
	}
	Pch = FindStringWithData(ObservFile, "RINEX VERSION / TYPE");
	if(Pch)
	{
		strcpy(CurrentString, Pch);
		sscanf(CurrentString, "%f", &RINEXObs->Version);
	}
	Pch = FindStringWithData(ObservFile, "APPROX POSITION XYZ");
	if(Pch)
	{
		strcpy(CurrentString, Pch);
		sscanf(CurrentString, "%lf%lf%lf", &RINEXObs->x, &RINEXObs->y, &RINEXObs->z);
	}

    for(i = 0; i < TYPES_OF_SYSTEMS; i++)
	{
		RINEXObs->IndexOfC1C[i] = -1;
		RINEXObs->IndexOfC2C[i] = -1;
		RINEXObs->IndexOfC1P[i] = -1;
		RINEXObs->IndexOfC2P[i] = -1;
	   //	RINEXObs->IndexOfC1A[i] = -1;
		RINEXObs->IndexOfC5Q[i] = -1;
        RINEXObs->IndexOfC7Q[i] = -1;
		RINEXObs->IndexOfC2I[i] = -1;
		RINEXObs->IndexOfC7I[i] = -1;
	}

	if(RINEXObs->Version < 3.0)
	{
		strcpy(CurrentString, FindStringWithData(ObservFile, "# / TYPES OF OBSERV"));
		sscanf(CurrentString, "%hhd", &RINEXObs->NOfTypesOfObserv[0]);
//		RINEXObs->TypesOfObserv = (char**)calloc(RINEXObs->NOfTypesOfObserv,
//												 SizeOfCharPtr);
//		for(i = 0; i < RINEXObs->NOfTypesOfObserv; i++)
//		{
//			RINEXObs->TypesOfObserv[i] = (char*)calloc(2, SizeOfChar);
//		}

		j = 0;
		Pch = strtok(CurrentString, " ");
		Pch = strtok(NULL, " ");
		for(i = 0; i < RINEXObs->NOfTypesOfObserv[0]; i += 9)
		{
			if(i == 0 && RINEXObs->NOfTypesOfObserv[0] > 9)
			{
				RINEXObs->NOfTypesOfObservInString = 9;
			}
			else
			{
				RINEXObs->NOfTypesOfObservInString = RINEXObs->NOfTypesOfObserv[0];
			}

			while(Pch && j < RINEXObs->NOfTypesOfObservInString)
			{
				strcpy(RINEXObs->TypesOfObserv[j], Pch);
				if(strstr(RINEXObs->TypesOfObserv[j], "C1"))
				{
					RINEXObs->IndexOfC1C[0] = j;
                    //RINEXObs->IndexOfC1A[0] = j;
				}

				if(strstr(RINEXObs->TypesOfObserv[j], "C2"))
				{
					RINEXObs->IndexOfC2C[0] = j;
				}

				if(strstr(RINEXObs->TypesOfObserv[j], "P1"))
				{
					RINEXObs->IndexOfC1P[0] = j;
				}

				if(strstr(RINEXObs->TypesOfObserv[j], "P2"))
				{
					RINEXObs->IndexOfC2P[0] = j;
				}

				if(strstr(RINEXObs->TypesOfObserv[j], "C5"))
				{
					RINEXObs->IndexOfC5Q[0] = j;
				}

				if(strstr(RINEXObs->TypesOfObserv[j], "C7"))
				{
					RINEXObs->IndexOfC7I[0] = j;
				}
				Pch = strtok(NULL, " ");
				j++;
			}
			fgets(CurrentString, BYTES_OF_STRING, ObservFile);
			Pch = strtok(CurrentString, " ");
		}
	}
	else
	{
		strcpy(CurrentString, FindStringWithData(ObservFile, "SYS / # / OBS TYPES"));
		i = 0;
		while(strstr(CurrentString, "SYS / # / OBS TYPES"))
		{
			sscanf(CurrentString, "%c %hhd", &RINEXObs->Systems[i], &RINEXObs->NOfTypesOfObserv[i]);
			//printf("\n%c %hhd %s", RINEXObs->Systems[i], RINEXObs->NOfTypesOfObserv, RINEXObs->Systems);
            //getch();
			k = 0;
			Pch = strtok(CurrentString, " ");
			Pch = strtok(NULL, " ");
			Pch = strtok(NULL, " ");
			for(j = 0; j < RINEXObs->NOfTypesOfObserv[i]; j += 13)
			{
				if(j == 0 && RINEXObs->NOfTypesOfObserv[i] > 13)
				{
					RINEXObs->NOfTypesOfObservInString = 13;
				}
				else
				{
					RINEXObs->NOfTypesOfObservInString = RINEXObs->NOfTypesOfObserv[i];
				}

				while(Pch && k < RINEXObs->NOfTypesOfObservInString)
				{
					strcpy(RINEXObs->TypesOfObserv[k], Pch);
				   // printf("\n%s", Pch);
					if(strstr(RINEXObs->TypesOfObserv[k], "C1C"))
					{
						RINEXObs->IndexOfC1C[i] = k;
					}

                    if(strstr(RINEXObs->TypesOfObserv[k], "C2C"))
					{
						RINEXObs->IndexOfC2C[i] = k;
					}

					if(strstr(RINEXObs->TypesOfObserv[k], "C1P"))
					{
						RINEXObs->IndexOfC1P[i] = k;
					}

					if(strstr(RINEXObs->TypesOfObserv[k], "C2P"))
					{
						RINEXObs->IndexOfC2P[i] = k;
					}

					if(strstr(RINEXObs->TypesOfObserv[k], "C5Q"))
					{
						RINEXObs->IndexOfC5Q[i] = k;
					}

					if(strstr(RINEXObs->TypesOfObserv[k], "C7Q"))
					{
						RINEXObs->IndexOfC7Q[i] = k;
					}

                    if(strstr(RINEXObs->TypesOfObserv[k], "C2I"))
					{
						RINEXObs->IndexOfC2I[i] = k;
					}

					if(strstr(RINEXObs->TypesOfObserv[k], "C7I"))
					{
						RINEXObs->IndexOfC7I[i] = k;
					}
					//printf(" %d", k);
					Pch = strtok(NULL, " ");
					k++;
				}

				if(RINEXObs->NOfTypesOfObserv[i] > 13 && RINEXObs->NOfTypesOfObservInString == 13)
				{
					fgets(CurrentString, BYTES_OF_STRING, ObservFile);
                }
				Pch = strtok(CurrentString, " ");
			}
			i++;
			fgets(CurrentString, BYTES_OF_STRING, ObservFile);
		}
	}

//   printf("\n%d ", RINEXObs->NOfTypesOfObserv);
//	printf("%d %d %d", RINEXObs->IndexOfC1, RINEXObs->IndexOfP1, RINEXObs->IndexOfP2);
//	 puts(RINEXObs->TypesOfObserv[0]);
	Pch = FindStringWithData(ObservFile, "INTERVAL");
	if(Pch)
	{
		strcpy(CurrentString, Pch);
		sscanf(CurrentString, "%f", &RINEXObs->Interval);
	}
	else
	{
		RINEXObs->Interval = 0.0;
    }
	Pch = FindStringWithData(ObservFile, "LEAP SECONDS");
	if(Pch)
	{
		strcpy(CurrentString, Pch);
		sscanf(CurrentString, "%f", &RINEXObs->LeapSeconds);
	}
	FindStringWithData(ObservFile, "END OF HEADER");
	fclose(ObservFile);
	return 1;
}

int ReadRINEXObs(char *ObservFileName, struct RINEXObs *RINEXObs)
{
	char Month, Day, Hours, Minutes, NOfSattelites,
		 *Pch, CurrentString[BYTES_OF_STRING], SattelitesString[BYTES_OF_STRING];
	short Year;
	int i, j, k, l, m, IndexOfSystemType, IndexOfObservType, NumberOfEmptyValues,
		NumberOfReadValues;
	float Seconds;
	double Value;
	size_t SizeOfChar = sizeof(char);
	size_t SizeOfCharPtr = sizeof(char*);
	size_t SizeOfDouble = sizeof(double);
	size_t SizeOfDoublePtr = sizeof(double*);
	size_t SizeOfEpoch = sizeof(struct Epoch);
    FILE *ObservFile = fopen(ObservFileName, "r");
	if(!ObservFile)
	{
		return 0;
	}

	if(!RINEXObs->NOfEpochs)
	{
		RINEXObs->Epochs = (struct Epoch*)calloc(1, SizeOfEpoch);
	}

	while(!feof(ObservFile))
	{
		if(RINEXObs->Version < 3.0)
		{
			fgets(CurrentString, BYTES_OF_STRING, ObservFile);
			NumberOfReadValues =
			sscanf(CurrentString, "%hd%hhd%hhd%hhd%hhd%f%d%hhd", &Year, &Month, &Day,
																 &Hours, &Minutes,
																 &Seconds, &m,
																 &NOfSattelites);
			if(NumberOfReadValues == 8 && m <= 1)
			{
				RINEXObs->Epochs = (struct Epoch*)realloc(RINEXObs->Epochs,
								   (RINEXObs->NOfEpochs + 1) * SizeOfEpoch);
				RINEXObs->Epochs[RINEXObs->NOfEpochs].Year = Year;
				RINEXObs->Epochs[RINEXObs->NOfEpochs].Month = Month;
				RINEXObs->Epochs[RINEXObs->NOfEpochs].Day = Day;
				RINEXObs->Epochs[RINEXObs->NOfEpochs].Hours = Hours;
				RINEXObs->Epochs[RINEXObs->NOfEpochs].Minutes = Minutes;
				RINEXObs->Epochs[RINEXObs->NOfEpochs].Seconds = Seconds;
				RINEXObs->Epochs[RINEXObs->NOfEpochs].NOfSattelites = NOfSattelites;
				if(RINEXObs->Epochs[RINEXObs->NOfEpochs].Year >= 80 &&
				   RINEXObs->Epochs[RINEXObs->NOfEpochs].Year <= 99)
				{
					RINEXObs->Epochs[RINEXObs->NOfEpochs].Year += 1900;
				}

				if(RINEXObs->Epochs[RINEXObs->NOfEpochs].Year >= 0 &&
				   RINEXObs->Epochs[RINEXObs->NOfEpochs].Year <= 79)
				{
					RINEXObs->Epochs[RINEXObs->NOfEpochs].Year += 2000;
				}
				RINEXObs->Epochs[RINEXObs->NOfEpochs].t =
				DateTimeToUNIXTime(RINEXObs->Epochs[RINEXObs->NOfEpochs].Year,
								   RINEXObs->Epochs[RINEXObs->NOfEpochs].Month,
								   RINEXObs->Epochs[RINEXObs->NOfEpochs].Day,
								   RINEXObs->Epochs[RINEXObs->NOfEpochs].Hours,
								   RINEXObs->Epochs[RINEXObs->NOfEpochs].Minutes,
								   RINEXObs->Epochs[RINEXObs->NOfEpochs].Seconds);
				RINEXObs->Epochs[RINEXObs->NOfEpochs].Sattelites =
				(char**)calloc(RINEXObs->Epochs[RINEXObs->NOfEpochs].NOfSattelites,
							   SizeOfCharPtr);
				RINEXObs->Epochs[RINEXObs->NOfEpochs].Values =
				(double***)calloc(RINEXObs->Epochs[RINEXObs->NOfEpochs].NOfSattelites,
								 SizeOfDoublePtr);
				for(i = 0; i < RINEXObs->Epochs[RINEXObs->NOfEpochs].NOfSattelites; i++)
				{
					RINEXObs->Epochs[RINEXObs->NOfEpochs].Sattelites[i] =
					(char*)calloc(2, SizeOfChar);
					RINEXObs->Epochs[RINEXObs->NOfEpochs].Values[i] =
					(double**)calloc(MAX_TYPES_OF_OBSERV, SizeOfDouble);
					for(j = 0; j < MAX_TYPES_OF_OBSERV; j++)
					{
						RINEXObs->Epochs[RINEXObs->NOfEpochs].Values[i][j] =
					(double*)calloc(2, SizeOfDouble);
                    }
				}

				if(!strchr(CurrentString, 'G') &&
				   !strchr(CurrentString, 'R') &&
				   !strchr(CurrentString, 'E') &&
				   !strchr(CurrentString, 'S'))
				{
					i = 0;
					j = 0;
					Pch = strtok(CurrentString, " ");
					while(Pch && i < 12 && i <
						  RINEXObs->Epochs[RINEXObs->NOfEpochs].NOfSattelites)
					{
						if(j > 7)
						{
							RINEXObs->Epochs[RINEXObs->NOfEpochs].Sattelites[i][0] = 'G';
							RINEXObs->Epochs[RINEXObs->NOfEpochs].Sattelites[i][1] = atoi(Pch);
							i++;
						}
						Pch = strtok(NULL, " ");
						j++;
					}

					if(RINEXObs->Epochs[RINEXObs->NOfEpochs].NOfSattelites > 12)
					{
						fgets(CurrentString, BYTES_OF_STRING, ObservFile);
						i = 12;
						Pch = strtok(CurrentString, " ");
						while(Pch)
						{
							RINEXObs->Epochs[RINEXObs->NOfEpochs].Sattelites[i][0] = 'G';
							RINEXObs->Epochs[RINEXObs->NOfEpochs].Sattelites[i][1] = atoi(Pch);
							Pch = strtok(NULL, " ");
							i++;
						}
					}
				}
				else
				{
					i = 0;
					Pch = strpbrk(CurrentString, "GRES");
					while(Pch)
					{
						RINEXObs->Epochs[RINEXObs->NOfEpochs].Sattelites[i][0] = Pch[0];
						Pch = strpbrk(Pch + 1, "GRES");
						i++;
					}
					i = 0;
					Pch = strtok(CurrentString, "GRES");
					Pch = strtok(NULL, " GRES");
					while(Pch && i < 12 && i <
						  RINEXObs->Epochs[RINEXObs->NOfEpochs].NOfSattelites)
					{
						RINEXObs->Epochs[RINEXObs->NOfEpochs].Sattelites[i][1] = atoi(Pch);
						Pch = strtok(NULL, " GRES");
						i++;
					}

					if(RINEXObs->Epochs[RINEXObs->NOfEpochs].NOfSattelites > 12)
					{
						fgets(CurrentString, BYTES_OF_STRING, ObservFile);
						Pch = strpbrk(CurrentString, "GRES");
						while(Pch)
						{
							RINEXObs->Epochs[RINEXObs->NOfEpochs].Sattelites[i][0] = Pch[0];
							Pch = strpbrk(Pch + 1, "GRES");
							i++;
						}
						i = 12;
						Pch = strtok(CurrentString, "GRES");
						Pch = strtok(NULL, " GRES");
						while(Pch)
						{
							RINEXObs->Epochs[RINEXObs->NOfEpochs].Sattelites[i][1] =
							atoi(Pch);
							Pch = strtok(NULL, " GRES");
							i++;
						}
					}

					if(RINEXObs->Epochs[RINEXObs->NOfEpochs].NOfSattelites > 24)
					{
						fgets(CurrentString, BYTES_OF_STRING, ObservFile);
						Pch = strpbrk(CurrentString, "GRES");
						while(Pch)
						{
							RINEXObs->Epochs[RINEXObs->NOfEpochs].Sattelites[i][0] = Pch[0];
							Pch = strpbrk(Pch + 1, "GRES");
							i++;
						}
						i = 24;
						Pch = strtok(CurrentString, "GRES");
						Pch = strtok(NULL, " GRES");
						while(Pch)
						{
							RINEXObs->Epochs[RINEXObs->NOfEpochs].Sattelites[i][1] =
							atoi(Pch);
							Pch = strtok(NULL, " GRES");
							i++;
						}
					}

					if(RINEXObs->Epochs[RINEXObs->NOfEpochs].NOfSattelites > 36)
					{
						fgets(CurrentString, BYTES_OF_STRING, ObservFile);
						Pch = strpbrk(CurrentString, "GRES");
						while(Pch)
						{
							RINEXObs->Epochs[RINEXObs->NOfEpochs].Sattelites[i][0] = Pch[0];
							Pch = strpbrk(Pch + 1, "GRES");
							i++;
						}
						i = 36;
						Pch = strtok(CurrentString, "GRES");
						Pch = strtok(NULL, " GRES");
						while(Pch)
						{
							RINEXObs->Epochs[RINEXObs->NOfEpochs].Sattelites[i][1] =
							atoi(Pch);
							Pch = strtok(NULL, " GRES");
							i++;
						}
					}
				}

				for(i = 0; i < RINEXObs->Epochs[RINEXObs->NOfEpochs].NOfSattelites; i++)
				{
					for(j = 0; j < RINEXObs->NOfTypesOfObserv[0]; j++)
					{
						RINEXObs->Epochs[RINEXObs->NOfEpochs].Values[i][j][0] = 0.0;
                        RINEXObs->Epochs[RINEXObs->NOfEpochs].Values[i][j][1] = 0.0;
					}

					IndexOfObservType = 0;
					l = 0;
					for(j = 0; j < RINEXObs->NOfTypesOfObserv[0]; j += 5)
					{
						NumberOfEmptyValues = 0;
						do
						{
							fgets(CurrentString, BYTES_OF_STRING, ObservFile);
						}
						while(!strlen(CurrentString));
						k = 0;
						while(k < BYTES_OF_STRING && CurrentString[k] != '\0')
						{
							if(CurrentString[k] == ' ')
							{
								NumberOfEmptyValues++;
							}
							else
							{
								if(NumberOfEmptyValues &&
								   CurrentString[k + 1] != ' ' &&
								   CurrentString[k + 1] != '\n' &&
								   CurrentString[k + 1] != '\0')
								{
									IndexOfObservType++;
								}

								if(j < 5)
								{
									if(CurrentString[k] == '\n' &&
									   IndexOfObservType < 5 &&
									   IndexOfObservType < RINEXObs->NOfTypesOfObserv[0])
									{
										CurrentString[k] = ' ';
										CurrentString[k + 1] = '0';
										CurrentString[k + 2] = '\n';
										CurrentString[k + 3] = '\0';
										IndexOfObservType++;
									}
								}

								if(j == 5)
								{
									if(CurrentString[k] == '\n' &&
									   IndexOfObservType < 10 &&
									   IndexOfObservType < RINEXObs->NOfTypesOfObserv[0])
									{
										CurrentString[k] = ' ';
										CurrentString[k + 1] = '0';
										CurrentString[k + 2] = '\n';
										CurrentString[k + 3] = '\0';
										IndexOfObservType++;
									}
								}

								if(j == 10)
								{
									if(CurrentString[k] == '\n' &&
									   IndexOfObservType < 15 &&
									   IndexOfObservType < RINEXObs->NOfTypesOfObserv[0])
									{
										CurrentString[k] = ' ';
										CurrentString[k + 1] = '0';
										CurrentString[k + 2] = '\n';
										CurrentString[k + 3] = '\0';
										IndexOfObservType++;
									}
								}

								if(j == 15)
								{
									if(CurrentString[k] == '\n' &&
									   IndexOfObservType < 20 &&
									   IndexOfObservType < RINEXObs->NOfTypesOfObserv[0])
									{
										CurrentString[k] = ' ';
										CurrentString[k + 1] = '0';
										CurrentString[k + 2] = '\n';
										CurrentString[k + 3] = '\0';
										IndexOfObservType++;
									}
								}

								if(j == 20)
								{
									if(CurrentString[k] == '\n' &&
									   IndexOfObservType < 25 &&
									   IndexOfObservType < RINEXObs->NOfTypesOfObserv[0])
									{
										CurrentString[k] = ' ';
										CurrentString[k + 1] = '0';
										CurrentString[k + 2] = '\n';
										CurrentString[k + 3] = '\0';
										IndexOfObservType++;
									}
								}
								NumberOfEmptyValues = 0;
							}

							if(NumberOfEmptyValues == 15)
							{
								CurrentString[k] = '0';
								IndexOfObservType++;
								NumberOfEmptyValues = 0;
							}
							k++;
						}
						Pch = strtok(CurrentString, " ");
						while(Pch && l < RINEXObs->NOfTypesOfObserv[0])
						{
							Value = atof(Pch);
							if(strlen(Pch) > 1 || !Value)
							{
								RINEXObs->Epochs[RINEXObs->NOfEpochs].Values[i][l][0] =
								Value;
								l++;
							}

							if(strlen(Pch) == 1 && Value)
							{
                                RINEXObs->Epochs[RINEXObs->NOfEpochs].Values[i][l - 1][1] =
								Value;
                            }
							Pch = strtok(NULL, " ");
						}
					}
				}
				RINEXObs->NOfEpochs++;
			}
		}
		else
		{
			fgets(CurrentString, BYTES_OF_STRING, ObservFile);
			NumberOfReadValues =
			sscanf(CurrentString, "> %hd%hhd%hhd%hhd%hhd%f%d%hhd", &Year, &Month, &Day,
																 &Hours, &Minutes,
																 &Seconds, &m,
																 &NOfSattelites);
			if(NumberOfReadValues == 8 && m <= 1)
			{
				RINEXObs->Epochs = (struct Epoch*)realloc(RINEXObs->Epochs,
								   (RINEXObs->NOfEpochs + 1) * SizeOfEpoch);
				RINEXObs->Epochs[RINEXObs->NOfEpochs].Year = Year;
				RINEXObs->Epochs[RINEXObs->NOfEpochs].Month = Month;
				RINEXObs->Epochs[RINEXObs->NOfEpochs].Day = Day;
				RINEXObs->Epochs[RINEXObs->NOfEpochs].Hours = Hours;
				RINEXObs->Epochs[RINEXObs->NOfEpochs].Minutes = Minutes;
				RINEXObs->Epochs[RINEXObs->NOfEpochs].Seconds = Seconds;
				RINEXObs->Epochs[RINEXObs->NOfEpochs].NOfSattelites = NOfSattelites;
				if(RINEXObs->Epochs[RINEXObs->NOfEpochs].Year >= 80 &&
				   RINEXObs->Epochs[RINEXObs->NOfEpochs].Year <= 99)
				{
					RINEXObs->Epochs[RINEXObs->NOfEpochs].Year += 1900;
				}

				if(RINEXObs->Epochs[RINEXObs->NOfEpochs].Year >= 0 &&
				   RINEXObs->Epochs[RINEXObs->NOfEpochs].Year <= 79)
				{
					RINEXObs->Epochs[RINEXObs->NOfEpochs].Year += 2000;
				}
				RINEXObs->Epochs[RINEXObs->NOfEpochs].t =
				DateTimeToUNIXTime(RINEXObs->Epochs[RINEXObs->NOfEpochs].Year,
								   RINEXObs->Epochs[RINEXObs->NOfEpochs].Month,
								   RINEXObs->Epochs[RINEXObs->NOfEpochs].Day,
								   RINEXObs->Epochs[RINEXObs->NOfEpochs].Hours,
								   RINEXObs->Epochs[RINEXObs->NOfEpochs].Minutes,
								   RINEXObs->Epochs[RINEXObs->NOfEpochs].Seconds);
				RINEXObs->Epochs[RINEXObs->NOfEpochs].Sattelites =
				(char**)calloc(RINEXObs->Epochs[RINEXObs->NOfEpochs].NOfSattelites,
							   SizeOfCharPtr);
				RINEXObs->Epochs[RINEXObs->NOfEpochs].Values =
				(double***)calloc(RINEXObs->Epochs[RINEXObs->NOfEpochs].NOfSattelites,
								 SizeOfDoublePtr);
				for(i = 0; i < RINEXObs->Epochs[RINEXObs->NOfEpochs].NOfSattelites; i++)
				{
					RINEXObs->Epochs[RINEXObs->NOfEpochs].Sattelites[i] =
					(char*)calloc(2, SizeOfChar);
					RINEXObs->Epochs[RINEXObs->NOfEpochs].Values[i] =
					(double**)calloc(MAX_TYPES_OF_OBSERV, SizeOfDouble);
					for(j = 0; j < MAX_TYPES_OF_OBSERV; j++)
					{
						RINEXObs->Epochs[RINEXObs->NOfEpochs].Values[i][j] =
								(double*)calloc(2, SizeOfDouble);
					}
				}

				for(i = 0; i < RINEXObs->Epochs[RINEXObs->NOfEpochs].NOfSattelites; i++)
				{
					for(j = 0; j < MAX_TYPES_OF_OBSERV; j++)
					{
						RINEXObs->Epochs[RINEXObs->NOfEpochs].Values[i][j][0] = 0.0;
                        RINEXObs->Epochs[RINEXObs->NOfEpochs].Values[i][j][1] = 0.0;
					}
					IndexOfObservType = 0;
					l = 0;
					NumberOfEmptyValues = 0;
					do
					{
						fgets(CurrentString, BYTES_OF_STRING, ObservFile);
					}
					while(!strlen(CurrentString));
					sscanf(CurrentString, "%c%hhd",
						   &RINEXObs->Epochs[RINEXObs->NOfEpochs].Sattelites[i][0],
						   &RINEXObs->Epochs[RINEXObs->NOfEpochs].Sattelites[i][1]);
                    IndexOfSystemType =
					GetIndex(RINEXObs->Systems, RINEXObs->Epochs[RINEXObs->NOfEpochs].Sattelites[i][0]);
//					printf("\n%c%hhd",
//						   RINEXObs->Epochs[RINEXObs->NOfEpochs].Sattelites[i][0],
//						   RINEXObs->Epochs[RINEXObs->NOfEpochs].Sattelites[i][1]);
                    k = 0;
					while(k < BYTES_OF_STRING && CurrentString[k] != '\0')
					{
						if(CurrentString[k] == ' ')
						{
							NumberOfEmptyValues++;
						}
						else
						{
							if(NumberOfEmptyValues &&
							   CurrentString[k + 1] != ' ' &&
							   CurrentString[k + 1] != '\n' &&
							   CurrentString[k + 1] != '\0')
							{
								IndexOfObservType++;
							}


							if(CurrentString[k] == '\n' &&
							   IndexOfObservType < RINEXObs->NOfTypesOfObserv[IndexOfSystemType])
							{
								CurrentString[k] = ' ';
								CurrentString[k + 1] = '0';
								CurrentString[k + 2] = '\n';
								CurrentString[k + 3] = '\0';
								IndexOfObservType++;
							}
							NumberOfEmptyValues = 0;
						}

						if(NumberOfEmptyValues == 15)
						{
							CurrentString[k] = '0';
							IndexOfObservType++;
							NumberOfEmptyValues = 0;
						}
						k++;
					}
					Pch = strtok(CurrentString, " ");
					Pch = strtok(NULL, " ");
//					while(Pch && l < RINEXObs->NOfTypesOfObserv)
//					{
//						Value = atof(Pch);
//						RINEXObs->Epochs[RINEXObs->NOfEpochs].Values[i][l] =
//						Value;
//						l++;
//						Pch = strtok(NULL, " ");
//					}

					while(Pch && l < RINEXObs->NOfTypesOfObserv[IndexOfSystemType])
					{
						Value = atof(Pch);
						if(strlen(Pch) > 1 || !Value)
						{
							RINEXObs->Epochs[RINEXObs->NOfEpochs].Values[i][l][0] =
							Value;
							l++;
						   //	printf(" %lf ", Value);
						}

                        if(strlen(Pch) == 1 && Value)
						{
							RINEXObs->Epochs[RINEXObs->NOfEpochs].Values[i][l - 1][1] =
							Value;
						}
						Pch = strtok(NULL, " ");
					}

				}
				RINEXObs->NOfEpochs++;
			}
		}
	}
	fclose(ObservFile);
	return 1;
}
