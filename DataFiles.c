#include "DataFiles.h"
#include "Solution.h"

#include <stdlib.h>
#undef abs
#include <math.h>
//#include <alloc.h>
#include <string.h>
#include <time.h>

char LeapYear(short Year)
{
	if((!(Year % 4) && Year % 100) || !(Year % 400))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

short DateToDOY(short Year, char Month, char Day)
{
	short DOY = 0;
	int i;
	char NDaysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	NDaysInMonth[1] += LeapYear(Year);
	for(i = 0; i < Month - 1; i++)
	{
		DOY += NDaysInMonth[i];
	}
	DOY += Day;
	return DOY;
}

void DOYToDate(short Year, short DOY, char *Month, char *Day)
{
	int i = 0, S = 0;
	char NDaysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    NDaysInMonth[1] += LeapYear(Year);
	if(DOY >= 1 && DOY <= (365 + LeapYear(Year)))
	{
		while(S < DOY)
		{
			S += NDaysInMonth[i++];
		}
		S -= NDaysInMonth[--i] ;
		S = DOY - S ;
		*Month = i + 1;
		*Day = S;
	}
}

void UTToTime(double UT, char *Hours, char *Minutes, float *Seconds)
{
	struct tm *tm;
	time_t t = (time_t)UT;
	tm = gmtime(&t);
	*Hours = tm->tm_hour;
	*Minutes = tm->tm_min;
	*Seconds = tm->tm_sec + (UT - floor(UT));
}

void SetDefaultSettings(struct Settings *Settings)
{
	int i;
	strcpy(Settings->RINEXObsFile, "");
	for(i = 0; i < MAX_OF_SYSTEMS + 1; i++)
	{
		strcpy(Settings->EphemerisFiles[i], "");
	}
	strcpy(Settings->System, "G");
	Settings->InitialHours = 0;
	Settings->InitialMinutes = 0;
	Settings->InitialSeconds = 0.0;
	Settings->FinalHours = 0;
	Settings->FinalMinutes = 0;
	Settings->FinalSeconds = 0.0;
	Settings->Step = 30.0;
	Settings->x = 0.0;
	Settings->y = 0.0;
	Settings->z = 0.0;
	Settings->x0 = 0.0;
	Settings->y0 = 0.0;
	Settings->z0 = 0.0;
	strcpy(Settings->FrequencyMode, "SC1");
	Settings->GalileoDataType = 'I';
	strcpy(Settings->ExcludeSattelitesString, "");
	strcpy(Settings->SpecSatFile, "");
	Settings->ElevationMask = 15.0;
    Settings->SNRMask = 5;
	Settings->Ionosphere = 'K';
	Settings->Alpha[0] = 0.0;
	Settings->Alpha[1] = 0.0;
	Settings->Alpha[2] = 0.0;
	Settings->Alpha[3] = 0.0;
	Settings->Beta[0] = 0.0;
	Settings->Beta[1] = 0.0;
	Settings->Beta[2] = 0.0;
	Settings->Beta[3] = 0.0;
	Settings->A = 0.8;
	Settings->F107 = 120.0;
	Settings->Ap = 30.0;
	Settings->Az[0] = 0.0;
	Settings->Az[1] = 0.0;
	Settings->Az[2] = 0.0;
	strcpy(Settings->BDGIMFile, "");
	strcpy(Settings->IONEXFile, "");
	strcpy(Settings->TECFile, "");
	strcpy(Settings->TayAbsTECPath, "");
	Settings->Trophosphere = 'S';
	Settings->p = 0.0;
	Settings->t = 0.0;
	Settings->r = 50.0;
	Settings->Iterations = 5;
	Settings->RMSThreshold = 50.0;
	Settings->Geodethic = 1;
	Settings->OutputType = 1;
	strcpy(Settings->OutputPath, "Output");
	Settings->AccumulateOutputData = 0;
	Settings->DTM = 0;
}

int ReadSettings(char *SettingsFileName, struct Settings *Settings)
{
	int i;
	char *Pch, CurrentString[BYTES_OF_STRING];
	FILE *SettingsFile = fopen(SettingsFileName, "r");
	if(!SettingsFile)
	{
		return 0;
	}

	while(!feof(SettingsFile))
	{
		fgets(CurrentString, BYTES_OF_STRING, SettingsFile);
		sscanf(CurrentString, "%[^=]=", Settings->SettingName);
		if(!strcmp(Settings->SettingName, "RINEXObsFilename"))
		{
			if(!strcmp(CurrentString, "RINEXObsFilename=\n"))
			{
				strcpy(Settings->RINEXObsFile, "");
			}
			else
			{
				Pch = strtok(CurrentString, "=\n");
				Pch = strtok(NULL, "=\n");
				strcpy(Settings->RINEXObsFile, Pch);
			}
		}

		if(!strcmp(Settings->SettingName, "EphemerisFilename"))
		{
            if(!strcmp(CurrentString, "EphemerisFilename=\n"))
			{
				for(i = 0; i < MAX_OF_SYSTEMS + 1; i++)
				{
					strcpy(Settings->EphemerisFiles[i], "");
				}
			}
			else
			{
				i = 0;
				Pch = strtok(CurrentString, " =\n");
				Pch = strtok(NULL, " \n");
				while(Pch)
				{
					strcpy(Settings->EphemerisFiles[i], Pch);
					Pch = strtok(NULL, " \n");
					i++;
				}

				while(i < MAX_OF_SYSTEMS + 1)
				{
					strcpy(Settings->EphemerisFiles[i], "");
                    i++;
                }
			}
		}

		if(!strcmp(Settings->SettingName, "System"))
		{
            if(!strcmp(CurrentString, "System=\n"))
			{
				strcpy(Settings->System, "");
			}
			else
			{
				Pch = strtok(CurrentString, "=\n");
				Pch = strtok(NULL, "=\n");
				strcpy(Settings->System, Pch);
			}
		}

		if(!strcmp(Settings->SettingName, "InitialTime"))
		{
			sscanf(CurrentString, "%*[^=]=%hhd %hhd %f", &Settings->InitialHours,
														 &Settings->InitialMinutes,
														 &Settings->InitialSeconds);
		}

		if(!strcmp(Settings->SettingName, "FinalTime"))
		{
			sscanf(CurrentString, "%*[^=]=%hhd %hhd %f", &Settings->FinalHours,
														 &Settings->FinalMinutes,
														 &Settings->FinalSeconds);
		}

		if(!strcmp(Settings->SettingName, "Step"))
		{
			sscanf(CurrentString, "%*[^=]=%f", &Settings->Step);
		}

		if(!strcmp(Settings->SettingName, "TruePosition"))
		{
			sscanf(CurrentString, "%*[^=]=%lf %lf %lf", &Settings->x,
														&Settings->y,
														&Settings->z);
		}

		if(!strcmp(Settings->SettingName, "ApproximatePosition"))
		{
			sscanf(CurrentString, "%*[^=]=%lf %lf %lf", &Settings->x0,
														&Settings->y0,
														&Settings->z0);
		}

		if(!strcmp(Settings->SettingName, "FrequencyMode"))
		{
			if(!strcmp(CurrentString, "FrequencyMode=\n"))
			{
				strcpy(Settings->FrequencyMode, "");
			}
			else
			{
				Pch = strtok(CurrentString, "=\n");
				Pch = strtok(NULL, "=\n");
				strcpy(Settings->FrequencyMode, Pch);
			}
		}

        if(!strcmp(Settings->SettingName, "GalileoDataType"))
		{
			sscanf(CurrentString, "%*[^=]=%c", &Settings->GalileoDataType);
		}

		if(!strcmp(Settings->SettingName, "ExcludeSattelites"))
		{
            if(!strcmp(CurrentString, "ExcludeSattelites=\n"))
			{
				strcpy(Settings->ExcludeSattelitesString, "");
			}
			else
			{
				Pch = strtok(CurrentString, "=\n");
				Pch = strtok(NULL, "=\n");
				strcpy(Settings->ExcludeSattelitesString, Pch);
			}
		}

        if(!strcmp(Settings->SettingName, "SpecSatFilename"))
		{
			if(!strcmp(CurrentString, "SpecSatFilename=\n"))
			{
				strcpy(Settings->SpecSatFile, "");
			}
			else
			{
				Pch = strtok(CurrentString, "=\n");
				Pch = strtok(NULL, "=\n");
				strcpy(Settings->SpecSatFile, Pch);
			}
		}

		if(!strcmp(Settings->SettingName, "ElevationMask"))
		{
			sscanf(CurrentString, "%*[^=]=%f", &Settings->ElevationMask);
		}

        if(!strcmp(Settings->SettingName, "SNRMask"))
		{
			sscanf(CurrentString, "%*[^=]=%hhd", &Settings->SNRMask);
		}

		if(!strcmp(Settings->SettingName, "CorrectIonosphereDelay"))
		{
			sscanf(CurrentString, "%*[^=]=%c", &Settings->Ionosphere);
			if(Settings->Ionosphere == '\n')
			{
                Settings->Ionosphere = 0;
            }
		}

		if(!strcmp(Settings->SettingName, "Alpha"))
		{
			sscanf(CurrentString, "%*[^=]=%lf %lf %lf %lf", &Settings->Alpha[0],
															&Settings->Alpha[1],
															&Settings->Alpha[2],
															&Settings->Alpha[3]);
		}

		if(!strcmp(Settings->SettingName, "Beta"))
		{
			sscanf(CurrentString, "%*[^=]=%lf %lf %lf %lf", &Settings->Beta[0],
															&Settings->Beta[1],
															&Settings->Beta[2],
				                                            &Settings->Beta[3]);
		}

		if(!strcmp(Settings->SettingName, "A"))
		{
			sscanf(CurrentString, "%*[^=]=%f", &Settings->A);
		}

		if(!strcmp(Settings->SettingName, "F10.7"))
		{
			sscanf(CurrentString, "%*[^=]=%f", &Settings->F107);
		}

		if(!strcmp(Settings->SettingName, "Ap"))
		{
			sscanf(CurrentString, "%*[^=]=%f", &Settings->Ap);
		}

		if(!strcmp(Settings->SettingName, "Az"))
		{
			sscanf(CurrentString, "%*[^=]=%lf %lf %lf", &Settings->Az[0],
														&Settings->Az[1],
													    &Settings->Az[2]);
		}

		if(!strcmp(Settings->SettingName, "BDGIMFilename"))
		{
			if(!strcmp(CurrentString, "BDGIMFilename=\n"))
			{
				strcpy(Settings->BDGIMFile, "");
			}
			else
			{
				Pch = strtok(CurrentString, "=\n");
				Pch = strtok(NULL, "=\n");
				strcpy(Settings->BDGIMFile, Pch);
			}
		}

		if(!strcmp(Settings->SettingName, "IONEXFilename"))
		{
        	if(!strcmp(CurrentString, "IONEXFilename=\n"))
			{
				strcpy(Settings->IONEXFile, "");
			}
			else
			{
				Pch = strtok(CurrentString, "=\n");
				Pch = strtok(NULL, "=\n");
				strcpy(Settings->IONEXFile, Pch);
			}
		}

		if(!strcmp(Settings->SettingName, "TECFilename"))
		{
        	if(!strcmp(CurrentString, "TECFilename=\n"))
			{
				strcpy(Settings->TECFile, "");
			}
			else
			{
				Pch = strtok(CurrentString, "=\n");
				Pch = strtok(NULL, "=\n");
				strcpy(Settings->TECFile, Pch);
			}
		}

		if(!strcmp(Settings->SettingName, "TayAbsTECPath"))
		{
			if(!strcmp(CurrentString, "TayAbsTECPath=\n"))
			{
				strcpy(Settings->TayAbsTECPath, "");
			}
			else
			{
				Pch = strtok(CurrentString, "=\n");
				Pch = strtok(NULL, "=\n");
				strcpy(Settings->TayAbsTECPath, Pch);
			}
		}

		if(!strcmp(Settings->SettingName, "CorrectTroposphereDelay"))
		{
			sscanf(CurrentString, "%*[^=]=%c", &Settings->Trophosphere);
			if(Settings->Trophosphere == '\n')
			{
				Settings->Trophosphere = 0;
            }
		}

		if(!strcmp(Settings->SettingName, "Weather"))
		{
			sscanf(CurrentString, "%*[^=]=%f %f %f", &Settings->p, &Settings->t,
													 &Settings->r);
		}

		if(!strcmp(Settings->SettingName, "NumberOfIterations"))
		{
			sscanf(CurrentString, "%*[^=]=%hhd", &Settings->Iterations);
		}

		if(!strcmp(Settings->SettingName, "RMSThreshold"))
		{
			sscanf(CurrentString, "%*[^=]=%f", &Settings->RMSThreshold);
		}

		if(!strcmp(Settings->SettingName, "GeodethicCoordinates"))
		{
			sscanf(CurrentString, "%*[^=]=%hhd", &Settings->Geodethic);
		}

		if(!strcmp(Settings->SettingName, "OutputType"))
		{
			sscanf(CurrentString, "%*[^=]=%hhd", &Settings->OutputType);
		}

		if(!strcmp(Settings->SettingName, "OutputPath"))
		{
            if(!strcmp(CurrentString, "OutputPath=\n"))
			{
				strcpy(Settings->OutputPath, "");
			}
			else
			{
				Pch = strtok(CurrentString, "=\n");
				Pch = strtok(NULL, "=\n");
				strcpy(Settings->OutputPath, Pch);
			}
		}

		if(!strcmp(Settings->SettingName, "AccumulateOutputData"))
		{
			sscanf(CurrentString, "%*[^=]=%hhd", &Settings->AccumulateOutputData);
		}

		if(!strcmp(Settings->SettingName, "DifferentialTimeMode"))
		{
			sscanf(CurrentString, "%*[^=]=%hhd", &Settings->DTM);
		}
	}
	fclose(SettingsFile);
	return 1;
}

int WriteSettings(char* SettingsFileName, struct Settings *Settings)
{
	int i;
	FILE *SettingsFile = fopen(SettingsFileName, "w");
	if(!SettingsFile)
	{
		return 0;
	}
	fputs("[Settings]\n", SettingsFile);
	fprintf(SettingsFile, "RINEXObsFilename=%s\n", Settings->RINEXObsFile);
	fprintf(SettingsFile, "EphemerisFilename=");
	for(i = 0; i < MAX_OF_SYSTEMS + 1; i++)
	{
		fprintf(SettingsFile, " %s", Settings->EphemerisFiles[i]);
	}
    fprintf(SettingsFile, "\n");
	fprintf(SettingsFile, "System=%s\n", Settings->System);
	fprintf(SettingsFile, "InitialTime=%hhd %hhd %.1f\n",
			Settings->InitialHours,
			Settings->InitialMinutes,
			Settings->InitialSeconds);
	fprintf(SettingsFile, "FinalTime=%hhd %hhd %.1f\n",
			Settings->FinalHours,
			Settings->FinalMinutes,
			Settings->FinalSeconds);
	fprintf(SettingsFile, "Step=%.1f\n", Settings->Step);
	fprintf(SettingsFile, "TruePosition=%.3lf %.3lf %.3lf\n", Settings->x,
															  Settings->y,
															  Settings->z);
	fprintf(SettingsFile, "ApproximatePosition=%.3lf %.3lf %.3lf\n", Settings->x0,
																	 Settings->y0,
																	 Settings->z0);
	fprintf(SettingsFile, "FrequencyMode=%s\n", Settings->FrequencyMode);
	fprintf(SettingsFile, "GalileoDataType=%c\n", Settings->GalileoDataType);
	fprintf(SettingsFile, "ExcludeSattelites=%s\n",
			Settings->ExcludeSattelitesString);
    fprintf(SettingsFile, "SpecSatFilename=%s\n", Settings->SpecSatFile);
	fprintf(SettingsFile, "ElevationMask=%.1f\n", Settings->ElevationMask);
	fprintf(SettingsFile, "SNRMask=%hhd\n", Settings->SNRMask);
	fprintf(SettingsFile, "CorrectIonosphereDelay=%c\n", Settings->Ionosphere);
	fprintf(SettingsFile, "Alpha=%E %E %E %E\n", Settings->Alpha[0],
												 Settings->Alpha[1],
												 Settings->Alpha[2],
												 Settings->Alpha[3]);
	fprintf(SettingsFile, "Beta=%E %E %E %E\n", Settings->Beta[0],
												Settings->Beta[1],
												Settings->Beta[2],
												Settings->Beta[3]);
	fprintf(SettingsFile, "A=%.1f\n", Settings->A);
	fprintf(SettingsFile, "F10.7=%.1f\n", Settings->F107);
	fprintf(SettingsFile, "Ap=%.1f\n", Settings->Ap);
	fprintf(SettingsFile, "Az=%E %E %E\n", Settings->Az[0],
										   Settings->Az[1],
										   Settings->Az[2]);
	fprintf(SettingsFile, "BDGIMFilename=%s\n", Settings->BDGIMFile);
	fprintf(SettingsFile, "IONEXFilename=%s\n", Settings->IONEXFile);
	fprintf(SettingsFile, "TECFilename=%s\n", Settings->TECFile);
    fprintf(SettingsFile, "TayAbsTECPath=%s\n", Settings->TayAbsTECPath);
	fprintf(SettingsFile, "CorrectTroposphereDelay=%c\n",
			Settings->Trophosphere);
	fprintf(SettingsFile, "Weather=%.1f %.1f %.1f\n",
			Settings->p, Settings->t, Settings->r);
	fprintf(SettingsFile, "NumberOfIterations=%hhd\n", Settings->Iterations);
	fprintf(SettingsFile, "RMSThreshold=%.1lf\n", Settings->RMSThreshold);
	fprintf(SettingsFile, "GeodethicCoordinates=%hhd\n", Settings->Geodethic);
	fprintf(SettingsFile, "OutputType=%hhd\n", Settings->OutputType);
	fprintf(SettingsFile, "OutputPath=%s\n", Settings->OutputPath);
	fprintf(SettingsFile, "AccumulateOutputData=%hhd\n",
			Settings->AccumulateOutputData);
	fprintf(SettingsFile, "DifferentialTimeMode=%hhd\n", Settings->DTM);
	fclose(SettingsFile);
    return 1;
}

void WriteOutputFileHeader(FILE *outputfile, struct Settings *Settings,
						struct CurSolution *CurSolution,
						struct ComSolution *ComSolution)
{
	fprintf(outputfile, " RINEXObsFilename = %s\n", Settings->RINEXObsFile);
	fprintf(outputfile, " EphemerisFilename = %s %s %s %s %s\n", Settings->EphemerisFiles[0],
																 Settings->EphemerisFiles[1],
																 Settings->EphemerisFiles[2],
																 Settings->EphemerisFiles[3],
																 Settings->EphemerisFiles[4]);
	fprintf(outputfile, " System = %s\n", Settings->System);
	fprintf(outputfile, " InitialTime = %hhd %hhd %.1f\n", Settings->InitialHours,
														   Settings->InitialMinutes,
														   Settings->InitialSeconds);
	fprintf(outputfile, " FinalTime = %hhd %hhd %.1f\n", Settings->FinalHours,
														 Settings->FinalMinutes,
														 Settings->FinalSeconds);
	fprintf(outputfile, " Step = %.1f\n", Settings->Step);
	fprintf(outputfile, " FrequencyMode = %s\n", Settings->FrequencyMode);
	fprintf(outputfile, " GalileoDataType = %c\n", Settings->GalileoDataType);
	fprintf(outputfile, " ExcludeSattelites = %s\n", Settings->ExcludeSattelitesString);
    fprintf(outputfile, " SpecSatFilename = %s\n", Settings->SpecSatFile);
	fprintf(outputfile, " ElevationMask = %.1f\n", Settings->ElevationMask);
	fprintf(outputfile, " SNRMask = %hhd\n", Settings->SNRMask);
	fprintf(outputfile, " CorrectIonosphereDelay = %c\n", Settings->Ionosphere);
	fprintf(outputfile, " Alpha = %E %E %E %E\n", Settings->Alpha[0], Settings->Alpha[1],
												  Settings->Alpha[2], Settings->Alpha[3]);
	fprintf(outputfile, " Beta = %E %E %E %E\n", Settings->Beta[0], Settings->Beta[1],
												 Settings->Beta[2], Settings->Beta[3]);
	fprintf(outputfile, " A = %.1f\n", Settings->A);
	fprintf(outputfile, " F10.7 = %.1f\n", Settings->F107);
	fprintf(outputfile, " Ap = %.1f\n", Settings->Ap);
	fprintf(outputfile, " Az = %.3lf %.3lf %.3lf\n", Settings->Az[0], Settings->Az[1], Settings->Az[2]);
	fprintf(outputfile, " BDGIMFilename = %s\n", Settings->BDGIMFile);
	fprintf(outputfile, " IONEXFilename = %s\n", Settings->IONEXFile);
	fprintf(outputfile, " TECFilename = %s\n", Settings->TECFile);
	fprintf(outputfile, " TayAbsTECPath= %s\n", Settings->TayAbsTECPath);
	fprintf(outputfile, " CorrectTroposphereDelay = %c\n", Settings->Trophosphere);
	fprintf(outputfile, " Weather = %.1f %.1f %.1f\n", Settings->p, Settings->t, Settings->r);
	fprintf(outputfile, " NumberOfIterations = %hhd\n", Settings->Iterations);
	fprintf(outputfile, " RMSThreshold = %.3lf\n", Settings->RMSThreshold);
	fprintf(outputfile, " DifferentialTimeMode = %hhd\n", Settings->DTM);
	DecartToGeo(ComSolution->Qp[0], ComSolution->Qp[1], ComSolution->Qp[2], &Settings->B, &Settings->L, &Settings->H);
	DecartToGeo(CurSolution[0].Q[0], CurSolution[0].Q[1], CurSolution[0].Q[2], &CurSolution[0].B, &CurSolution[0].L, &CurSolution[0].H);
	fprintf(outputfile, "\n True receiver position:");
	fprintf(outputfile, "\n  x = %12.3lf m   B = %13.8lf °", ComSolution->Qp[0], Settings->B);
	fprintf(outputfile, "\n  y = %12.3lf m   L = %13.8lf °", ComSolution->Qp[1], Settings->L);
	fprintf(outputfile, "\n  z = %12.3lf m   H = %13.3lf m", ComSolution->Qp[2], Settings->H);
	fprintf(outputfile, "\n\n Approximate receiver position:");
	fprintf(outputfile, "\n x0 = %12.3lf m  B0 = %13.8lf °", CurSolution[0].Q[0], CurSolution[0].B);
	fprintf(outputfile, "\n y0 = %12.3lf m  L0 = %13.8lf °", CurSolution[0].Q[1], CurSolution[0].L);
	fprintf(outputfile, "\n z0 = %12.3lf m  H0 = %13.3lf m", CurSolution[0].Q[2], CurSolution[0].H);
	fprintf(outputfile, "\n\n ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
	fprintf(outputfile, "\n    y  m  d  h  m  s                x(m)          y(m)         z(m)          cdt(m)");
	if(Settings->Geodethic)
	{
		fprintf(outputfile, "                                             B(°)          L(°)           H(m)");
		fprintf(outputfile, "      NS        dx(m)        dy(m)        dz(m)                                              SEN(m)       SU(m)       SENU(m)                                               HDOP         VDOP         PDOP         TDOP");
	}
	else
	{
		fprintf(outputfile, "                                           NS        dx(m)        dy(m)        dz(m)                                              SEN(m)       SU(m)       SENU(m)                                               HDOP         VDOP         PDOP         TDOP");
	}
	fprintf(outputfile, "\n ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
}

void WriteSolutionToFile(FILE *outputfile, struct Epoch *Epochs,
						struct CurSolution *CurSolution, int N,
						struct Settings *Settings)
{
	int i;
	for(i = 0; i < N; i++)
	{
		if(CurSolution[i].Valid)
		{
			Epochs[i].TimeInHours =
				(float)Epochs[i].Hours +
				(float)Epochs[i].Minutes / 60.0 +
				Epochs[i].Seconds / 3600.0;
				fprintf(outputfile, "\n %4hd %2hhd %2hhd %2hhd %2hhd %4.1f",
				Epochs[i].Year,
				Epochs[i].Month,
				Epochs[i].Day,
				Epochs[i].Hours,
				Epochs[i].Minutes,
				Epochs[i].Seconds);
			fprintf(outputfile, "  %07.4f", Epochs[i].TimeInHours);
			fprintf(outputfile, " %12.3lf %12.3lf %12.3lf", CurSolution[i].Q[0], CurSolution[i].Q[1], CurSolution[i].Q[2]);
			fprintf(outputfile, " %12.3lf", CurSolution[i].Q[3]);
			fprintf(outputfile, " %12.3lf", CurSolution[i].Q[4]);
			fprintf(outputfile, " %12.3lf", CurSolution[i].Q[5]);
			fprintf(outputfile, " %12.3lf", CurSolution[i].Q[6]);
			if(Settings->Geodethic)
			{
				fprintf(outputfile, " %13.8lf %13.8lf %12.3lf", CurSolution[i].B, CurSolution[i].L, CurSolution[i].H);
			}
			fprintf(outputfile, " %5d %12.3lf %12.3lf %12.3lf", CurSolution[i].NOfValidSattelites, CurSolution[i].dQp[0], CurSolution[i].dQp[1], CurSolution[i].dQp[2]);
            fprintf(outputfile, " %12.3lf %12.3lf %12.3lf", CurSolution[i].dQm[0], CurSolution[i].dQm[1], CurSolution[i].dQm[2]);
			fprintf(outputfile, " %12.3lf %12.3lf %12.3lf", CurSolution[i].RMSEHp, CurSolution[i].RMSEVp, CurSolution[i].RMSE3Dp);
			fprintf(outputfile, " %12.3lf %12.3lf %12.3lf", CurSolution[i].RMSEHm, CurSolution[i].RMSEVm, CurSolution[i].RMSE3Dm);
			fprintf(outputfile, " %12.3lf %12.3lf %12.3lf %12.3lf", CurSolution[i].HDOP, CurSolution[i].VDOP, CurSolution[i].PDOP, CurSolution[i].TDOP);
		}
	}
}

void ConvertTECToIONEX(char *TECFileName, char *IONEXFileName)
{
	short Year, DOY;
	int i, NOfTECMap = 1, NumberOfReadValues;
	char *Pch, Month, Day, Hours, Minutes, CurrentString[BYTES_OF_STRING],
			OutputFileName[MAXPATH];
	float TimeInHours, TimeInHours0 = -1.0, Seconds;
	double B, B0 = -1.0, L, TEC;
	FILE *OtherTECfile, *IONEXfile;
	OtherTECfile = fopen(TECFileName, "r");
	strcpy(CurrentString, "");
	strcat(CurrentString, TECFileName);
	strcpy(OutputFileName, "");
	Pch = strtok(CurrentString, "\\");
	while(Pch)
	{
		strcpy(OutputFileName, Pch);
		Pch = strtok(NULL, "\\");
	}
	mkdir("IONEXConv");
	Pch = strtok(OutputFileName, ".");
	while(!feof(OtherTECfile))
	{
		fgets(CurrentString, BYTES_OF_STRING, OtherTECfile);
		NumberOfReadValues =
		sscanf(CurrentString, "%hd%hd%f%lf%lf%lf", &Year, &DOY,
												   &TimeInHours, &L, &B, &TEC);
		if(NumberOfReadValues == 6)
		{
			DOYToDate(Year, DOY, &Month, &Day);
            UTToTime(TimeInHours * 3600, &Hours, &Minutes, &Seconds);
			if(TimeInHours != TimeInHours0)
			{
				if(NOfTECMap == 1)
				{
					if(Year >= 1900 && Year <= 1999)
					{
						Year -= 1900;
					}

					if(Year >= 2000 && Year <= 2099)
					{
						Year -= 2000;
					}
					sprintf(IONEXFileName, "IONEXConv\\%s.%02hdi", Pch, Year);
					IONEXfile = fopen(IONEXFileName, "w");
					fprintf(IONEXfile, "%8.1f            IONOSPHERE MAPS     MIX                 IONEX VERSION / TYPE\n", 1.1);
					//fprintf(IONEXfile, "  %s %hd%hhd                                                   PGM / RUN BY / DATE\n", "TEC To IONEX Converter");
					fprintf(IONEXfile, "%8.1f                                                    BASE RADIUS\n", 6371.0);
					fprintf(IONEXfile, "  %6.1f%6.1f%6.1f                                        HGT1 / HGT2 / DHGT\n", 450.0, 450.0, 0.0);
					fprintf(IONEXfile, "  %6.1lf%6.1lf%6.1f                                        LAT1 / LAT2 / DLAT\n", B, -B, -2.5);
					fprintf(IONEXfile, "  %6.1lf%6.1lf%6.1f                                        LON1 / LON2 / DLON\n", L, -L + 5.0, 5.0);
					fprintf(IONEXfile, "%6d                                                      EXPONENT\n", -1);
					fprintf(IONEXfile, "                                                            END OF HEADER");
				}

				if(NOfTECMap > 1)
				{
					fprintf(IONEXfile, "\n%6d                                                      END OF TEC MAP", NOfTECMap - 1);
				}
				fprintf(IONEXfile, "\n%6d                                                      START OF TEC MAP\n", NOfTECMap);
				fprintf(IONEXfile, "%6hd%6hhd%6hhd%6hhd%6hhd%6hhd                        EPOCH OF CURRENT MAP", Year, Month, Day, Hours, Minutes, (char)Seconds);
				NOfTECMap++;
			}

			if(B != B0)
			{
				i = 0;
				fprintf(IONEXfile, "\n  %6.1lf%6.1lf%6.1lf%6.1lf%6.1lf                            LAT/LON1/LON2/DLON/H\n", B, L, -L + 5.0, 5.0, 450.0);
			}

			if(i == 16 || i == 32 || i == 48 || i == 64)
			{
				fprintf(IONEXfile, "\n");
			}
			fprintf(IONEXfile, " %4.0lf", TEC * 10);

		   //	printf("%d %lf\n", Year, TEC);
			TimeInHours0 = TimeInHours;
			B0 = B;
			i++;
        }
	}
	fprintf(IONEXfile, "\n%6d                                                      END OF TEC MAP\n", NOfTECMap - 1);
	fprintf(IONEXfile, "                                                            END OF FILE");
	fclose(OtherTECfile);
	fclose(IONEXfile);
}

int ReadTayAbsTECData(char *TayAbsTECFileName, struct TayAbsTEC *TayAbsTEC)
{
	int NumberOfReadValues;
	char CurrentString[BYTES_OF_STRING];
	size_t SizeOfTECRecord = sizeof(struct TECRecord);
	FILE *TayAbsTECFile = fopen(TayAbsTECFileName, "r");
	if(!TayAbsTECFile)
	{
		return 0;
	}

	if(!TayAbsTEC->NOfRecords)
	{
		TayAbsTEC->TECRecords = (struct TECRecord*)calloc(1, SizeOfTECRecord);
	}

	while(!feof(TayAbsTECFile) && !strstr(CurrentString, "G_lon"))
	{
		fgets(CurrentString, BYTES_OF_STRING, TayAbsTECFile);
		if(strstr(TayAbsTECFileName, "_G") || strstr(TayAbsTECFileName, "_R"))
		{
			NumberOfReadValues =
			sscanf(CurrentString, "%f%*lf%*lf%lf",
				   &TayAbsTEC->TECRecords[TayAbsTEC->NOfRecords].UT,
				   &TayAbsTEC->TECRecords[TayAbsTEC->NOfRecords].TEC);
		}
		else
		{
            NumberOfReadValues =
			sscanf(CurrentString, "%f%lf",
				   &TayAbsTEC->TECRecords[TayAbsTEC->NOfRecords].UT,
				   &TayAbsTEC->TECRecords[TayAbsTEC->NOfRecords].TEC);
		}

		if(NumberOfReadValues == 2)
		{
			TayAbsTEC->TECRecords = (struct TECRecord*)realloc(TayAbsTEC->TECRecords,
									 (++TayAbsTEC->NOfRecords + 1) *  SizeOfTECRecord);
		}
	}
    fclose(TayAbsTECFile);
	return 1;
}

int SortFunctionTEC(const void *j, const void *i)
{
	double r = ((struct TECRecord*)j)->UT - ((struct TECRecord*)i)->UT;
	if(r == 0.0)
	{
		return 0;
	}
	return r < 0.0 ? -1 : 1;
}

int ReadSpecSatData(char *SpecSatFileName, struct RINEXObs *RINEXObs)
{
	char *Pch, Find, Hours, Minutes, CurrentString[BYTES_OF_STRING],
		 Sattelites[MAX_OF_VISIBLE_SATS][2];
	int i, j, k, l;
	float Seconds;
	//double t;
	FILE *SpecSatFile = fopen(SpecSatFileName, "r");
	if(!SpecSatFile)
	{
		return 0;
	}

	while(!feof(SpecSatFile))
	{
		fgets(CurrentString, BYTES_OF_STRING, SpecSatFile);
		sscanf(CurrentString, "%hhd%hhd%f", &Hours, &Minutes, &Seconds);
		for(i = 0; i < RINEXObs->NOfEpochs; i++)
		{
			if(RINEXObs->Epochs[i].t == DateTimeToUNIXTime(RINEXObs->Epochs[i].Year,
														   RINEXObs->Epochs[i].Month,
														   RINEXObs->Epochs[i].Day,
														   Hours, Minutes, Seconds))
			{
				j = 0;
				Pch = strpbrk(CurrentString, "GRESC");
				while(Pch)
				{
					Sattelites[j][0] = Pch[0];
					Pch = strpbrk(Pch + 1, "GRESC");
				  	j++;
				}
				j = 0;
				Pch = strtok(CurrentString, "GRESC");
				Pch = strtok(NULL, " GRESC");
				while(Pch)
				{
					Sattelites[j][1] = atoi(Pch);
					Pch = strtok(NULL, " GRESC");
					j++;
				}

				for(k = 0; k < RINEXObs->Epochs[i].NOfSattelites; k++)
				{
					Find = 0;
					for(l = 0; l < j; l++)
					{
						if(RINEXObs->Epochs[i].Sattelites[k][0] == Sattelites[l][0] &&
						   RINEXObs->Epochs[i].Sattelites[k][1] == Sattelites[l][1])
						{
							Find = 1;
						}
					}

					if(!Find)
					{
						RINEXObs->Epochs[i].Sattelites[k][0] = '0';
						RINEXObs->Epochs[i].Sattelites[k][1] = 0;
					}
				}
				break;
			}
		}
	}
	fclose(SpecSatFile);
    return 1;
}

