#include "SP3.h"
#include "Constants.h"
#include "Common.h"

#include <stdio.h>
#include <time.h>
//#include <alloc.h>
#include <stdlib.h>

int ReadSP3Ephemeris(char *EphemerisFileName, struct SP3 *SP3)
{
	int i;
	char CurrentString[BYTES_OF_STRING];
	float IntPart;
	struct tm tm;
	size_t SizeOfSP3Ephemeris = sizeof(struct SP3Ephemeris);
    FILE *EphemerisFile = fopen(EphemerisFileName, "r");
	if(!EphemerisFile)
	{
		return 0;
	}

	if(!SP3->NOfGPSEphemeris)
	{
		SP3->GPSEphemeris = (struct SP3Ephemeris*)calloc(1, SizeOfSP3Ephemeris);
	}

	if(!SP3->NOfGLOEphemeris)
	{
		SP3->GLOEphemeris = (struct SP3Ephemeris*)calloc(1, SizeOfSP3Ephemeris);
	}

	if(!SP3->NOfGALEphemeris)
	{
		SP3->GALEphemeris = (struct SP3Ephemeris*)calloc(1, SizeOfSP3Ephemeris);
	}

	if(!SP3->NOfBDSEphemeris)
	{
		SP3->BDSEphemeris = (struct SP3Ephemeris*)calloc(1, SizeOfSP3Ephemeris);
	}
	fgets(CurrentString, BYTES_OF_STRING, EphemerisFile);
	sscanf(CurrentString, "%*c%*c%c", &SP3->Type);
	fgets(CurrentString, BYTES_OF_STRING, EphemerisFile);
	sscanf(CurrentString, "%*c%*c%*d%*f%f", &SP3->Step);
	fgets(CurrentString, BYTES_OF_STRING, EphemerisFile);
	sscanf(CurrentString, "%*c%hhd", &SP3->NOfSattelites);
	for(i = 0; i < 85; i += 17)
	{
		fgets(CurrentString, BYTES_OF_STRING, EphemerisFile);
	}

	while(i++ < SP3->NOfSattelites)
	{
		fgets(CurrentString, BYTES_OF_STRING, EphemerisFile);
	}

    	for(i = 0; i < 85; i += 17)
	{
		fgets(CurrentString, BYTES_OF_STRING, EphemerisFile);
	}

	while(i++ < SP3->NOfSattelites)
	{
		fgets(CurrentString, BYTES_OF_STRING, EphemerisFile);
	}
	sscanf(CurrentString, "%*c%*c %c  %*c%*c %s",
		   &SP3->System, SP3->TimeSystem);
	for(i = 0; i < 9; i++)
	{
		fgets(CurrentString, BYTES_OF_STRING, EphemerisFile);
	}

	while(!feof(EphemerisFile))
	{
	   	fgets(CurrentString, BYTES_OF_STRING, EphemerisFile);
		sscanf(CurrentString, "%*c%hd%hhd%hhd%hhd%hhd%f",
			   &SP3->Year, &SP3->Month, &SP3->Day,
			   &SP3->Hours, &SP3->Minutes, &SP3->Seconds);
		for(i = 0; i < SP3->NOfSattelites; i++)
		{
			fgets(CurrentString, BYTES_OF_STRING, EphemerisFile);
			sscanf(CurrentString, "%*c%c", &SP3->SatteliteMask);
			if(SP3->SatteliteMask == 'G')
			{
				SP3->GPSEphemeris = (struct SP3Ephemeris*)
									realloc(SP3->GPSEphemeris,
											(SP3->NOfGPSEphemeris + 1)
											* SizeOfSP3Ephemeris);
				sscanf(CurrentString, "%*c%c%hhd%lf%lf%lf%lf",
					   &SP3->GPSEphemeris[SP3->NOfGPSEphemeris].Number[0],
					   &SP3->GPSEphemeris[SP3->NOfGPSEphemeris].Number[1],
					   &SP3->GPSEphemeris[SP3->NOfGPSEphemeris].x,
					   &SP3->GPSEphemeris[SP3->NOfGPSEphemeris].y,
					   &SP3->GPSEphemeris[SP3->NOfGPSEphemeris].z,
					   &SP3->GPSEphemeris[SP3->NOfGPSEphemeris].dt);
				SP3->GPSEphemeris[SP3->NOfGPSEphemeris].Year = SP3->Year;
				SP3->GPSEphemeris[SP3->NOfGPSEphemeris].Month = SP3->Month;
				SP3->GPSEphemeris[SP3->NOfGPSEphemeris].Day = SP3->Day;
				SP3->GPSEphemeris[SP3->NOfGPSEphemeris].Hours = SP3->Hours;
				SP3->GPSEphemeris[SP3->NOfGPSEphemeris].Minutes = SP3->Minutes;
				SP3->GPSEphemeris[SP3->NOfGPSEphemeris].Seconds = SP3->Seconds;
				SP3->GPSEphemeris[SP3->NOfGPSEphemeris].toc =
				DateTimeToUNIXTime(SP3->GPSEphemeris[SP3->NOfGPSEphemeris].Year,
								   SP3->GPSEphemeris[SP3->NOfGPSEphemeris].Month,
								   SP3->GPSEphemeris[SP3->NOfGPSEphemeris].Day,
								   SP3->GPSEphemeris[SP3->NOfGPSEphemeris].Hours,
								   SP3->GPSEphemeris[SP3->NOfGPSEphemeris].Minutes,
								   SP3->GPSEphemeris[SP3->NOfGPSEphemeris].Seconds);
				SP3->NOfGPSEphemeris++;
			}

			if(SP3->SatteliteMask == 'R')
			{
				SP3->GLOEphemeris = (struct SP3Ephemeris*)
									realloc(SP3->GLOEphemeris,
											(SP3->NOfGLOEphemeris + 1)
											* SizeOfSP3Ephemeris);
				sscanf(CurrentString, "%*c%c%hhd%lf%lf%lf%lf",
					   &SP3->GLOEphemeris[SP3->NOfGLOEphemeris].Number[0],
					   &SP3->GLOEphemeris[SP3->NOfGLOEphemeris].Number[1],
					   &SP3->GLOEphemeris[SP3->NOfGLOEphemeris].x,
					   &SP3->GLOEphemeris[SP3->NOfGLOEphemeris].y,
					   &SP3->GLOEphemeris[SP3->NOfGLOEphemeris].z,
					   &SP3->GLOEphemeris[SP3->NOfGLOEphemeris].dt);
				SP3->GLOEphemeris[SP3->NOfGLOEphemeris].Year = SP3->Year;
				SP3->GLOEphemeris[SP3->NOfGLOEphemeris].Month = SP3->Month;
				SP3->GLOEphemeris[SP3->NOfGLOEphemeris].Day = SP3->Day;
				SP3->GLOEphemeris[SP3->NOfGLOEphemeris].Hours = SP3->Hours;
				SP3->GLOEphemeris[SP3->NOfGLOEphemeris].Minutes = SP3->Minutes;
				SP3->GLOEphemeris[SP3->NOfGLOEphemeris].Seconds = SP3->Seconds;
				SP3->GLOEphemeris[SP3->NOfGLOEphemeris].toc =
				DateTimeToUNIXTime(SP3->GLOEphemeris[SP3->NOfGLOEphemeris].Year,
								   SP3->GLOEphemeris[SP3->NOfGLOEphemeris].Month,
								   SP3->GLOEphemeris[SP3->NOfGLOEphemeris].Day,
								   SP3->GLOEphemeris[SP3->NOfGLOEphemeris].Hours,
								   SP3->GLOEphemeris[SP3->NOfGLOEphemeris].Minutes,
								   SP3->GLOEphemeris[SP3->NOfGLOEphemeris].Seconds);
				SP3->NOfGLOEphemeris++;
			}

			if(SP3->SatteliteMask == 'E')
			{
				SP3->GALEphemeris = (struct SP3Ephemeris*)
									realloc(SP3->GALEphemeris,
											(SP3->NOfGALEphemeris + 1)
											* SizeOfSP3Ephemeris);
				sscanf(CurrentString, "%*c%c%hhd%lf%lf%lf%lf",
					   &SP3->GALEphemeris[SP3->NOfGALEphemeris].Number[0],
					   &SP3->GALEphemeris[SP3->NOfGALEphemeris].Number[1],
					   &SP3->GALEphemeris[SP3->NOfGALEphemeris].x,
					   &SP3->GALEphemeris[SP3->NOfGALEphemeris].y,
					   &SP3->GALEphemeris[SP3->NOfGALEphemeris].z,
					   &SP3->GALEphemeris[SP3->NOfGALEphemeris].dt);
				SP3->GALEphemeris[SP3->NOfGALEphemeris].Year = SP3->Year;
				SP3->GALEphemeris[SP3->NOfGALEphemeris].Month = SP3->Month;
				SP3->GALEphemeris[SP3->NOfGALEphemeris].Day = SP3->Day;
				SP3->GALEphemeris[SP3->NOfGALEphemeris].Hours = SP3->Hours;
				SP3->GALEphemeris[SP3->NOfGALEphemeris].Minutes = SP3->Minutes;
				SP3->GALEphemeris[SP3->NOfGALEphemeris].Seconds = SP3->Seconds;
				SP3->GALEphemeris[SP3->NOfGALEphemeris].toc =
				DateTimeToUNIXTime(SP3->GALEphemeris[SP3->NOfGALEphemeris].Year,
								   SP3->GALEphemeris[SP3->NOfGALEphemeris].Month,
								   SP3->GALEphemeris[SP3->NOfGALEphemeris].Day,
								   SP3->GALEphemeris[SP3->NOfGALEphemeris].Hours,
								   SP3->GALEphemeris[SP3->NOfGALEphemeris].Minutes,
								   SP3->GALEphemeris[SP3->NOfGALEphemeris].Seconds);
				SP3->NOfGALEphemeris++;
			}

			if(SP3->SatteliteMask == 'C')
			{
				SP3->BDSEphemeris = (struct SP3Ephemeris*)
									realloc(SP3->BDSEphemeris,
											(SP3->NOfBDSEphemeris + 1)
											* SizeOfSP3Ephemeris);
				sscanf(CurrentString, "%*c%c%hhd%lf%lf%lf%lf",
					   &SP3->BDSEphemeris[SP3->NOfBDSEphemeris].Number[0],
					   &SP3->BDSEphemeris[SP3->NOfBDSEphemeris].Number[1],
					   &SP3->BDSEphemeris[SP3->NOfBDSEphemeris].x,
					   &SP3->BDSEphemeris[SP3->NOfBDSEphemeris].y,
					   &SP3->BDSEphemeris[SP3->NOfBDSEphemeris].z,
					   &SP3->BDSEphemeris[SP3->NOfBDSEphemeris].dt);
				SP3->BDSEphemeris[SP3->NOfBDSEphemeris].Year = SP3->Year;
				SP3->BDSEphemeris[SP3->NOfBDSEphemeris].Month = SP3->Month;
				SP3->BDSEphemeris[SP3->NOfBDSEphemeris].Day = SP3->Day;
				SP3->BDSEphemeris[SP3->NOfBDSEphemeris].Hours = SP3->Hours;
				SP3->BDSEphemeris[SP3->NOfBDSEphemeris].Minutes = SP3->Minutes;
				SP3->BDSEphemeris[SP3->NOfBDSEphemeris].Seconds = SP3->Seconds;
				SP3->BDSEphemeris[SP3->NOfBDSEphemeris].toc =
				DateTimeToUNIXTime(SP3->BDSEphemeris[SP3->NOfBDSEphemeris].Year,
								   SP3->BDSEphemeris[SP3->NOfBDSEphemeris].Month,
								   SP3->BDSEphemeris[SP3->NOfBDSEphemeris].Day,
								   SP3->BDSEphemeris[SP3->NOfBDSEphemeris].Hours,
								   SP3->BDSEphemeris[SP3->NOfBDSEphemeris].Minutes,
								   SP3->BDSEphemeris[SP3->NOfBDSEphemeris].Seconds);
				SP3->NOfBDSEphemeris++;
			}
		}
	}
	fclose(EphemerisFile);
	return 1;
}
