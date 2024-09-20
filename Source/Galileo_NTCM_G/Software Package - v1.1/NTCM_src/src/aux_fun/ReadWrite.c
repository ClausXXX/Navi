/**
 * ******************************************************************
 * NTCMproj File: ReadWrite.c
 * ******************************************************************
 *  @author      Francesco Menzione
 *  @reviewer    Matteo Sgammini
 *  @ingroup     NTCM_JRC
 *  @copyright   Joint Research Centre (JRC), 2019
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), Version 1.2 or later
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * ******************************************************************
 */

 /* ******************************************************************/
 /* Interface */
 /* ******************************************************************/
 
 /* Standard Include */
#include "ReadWrite.h"
 /* Include Common Variables */

/* Set String  */
#define MAXC 1024
#define MAXNM 64

/* ******************************************************************/
/* Function Definitions */
/* ******************************************************************/

/**
* Clear File 
*/
void clearFile(char* filename)
{
	FILE* fp = NULL;
#ifdef isLx
	fp=fopen(filename, "w+");
#else
	fopen_s(&fp, filename, "w+");
#endif

	if (fp != NULL)\
	{
		fclose(fp);
	}

}

/**
* Read File Data Input
* This function read .txt or CSV fle [sample]x[Input] considering different data type format and allocating
* them in corrispondent matrices to feed different function (I/O)
* Currently supported types : double (%f,%fl) , int (%d,%i)
* adding new data types :to do
*/
int readFileDataDI(FILE** fp, char* filename, 
	double Data_I_double[ENTRIES][NTCM_I_DOUBLE_dim],
	int Data_I_int[ENTRIES][NTCM_I_INT_dim],
	char* format,int n_In,
	int length, int max_length)
{
	// Read line counter
	int retVal = 0;
	// Input dimension counter
	int fieldnum=0;
	// reading counters
	int i = 0, jj = 0, kk=0;

	// Buffers and tokens
	char* EndOF = " ";

	char buff[MAXC], buff_c[MAXC], format_c[MAXC];
	char* buff_1;
    char* buff_2;
	char* next = " ";
	char* next_2 = " ";
	char* token = " ";
	char* token_1 = " ";
	char* unconverted = " ";
	char* delim = " "; 
	char* appo = " ";
	// Data sample
	double d1=0;
	int i1 = 0;

	if ((*fp) == NULL)
		
	//  opeen file 
	{
		#ifdef isLx
		fp = fopen(filename, "r");
		#else
		fopen_s(fp, filename, "r");
		#endif	
		
		appo= strstr(filename, ".csv");
		// Select delimiter
		if (appo != NULL)
		{delim=",";}
	}

	//  Read file according to format 
	if ((*fp) != NULL)
	{
		while ( EndOF != NULL && i < max_length && jj< length)
		{
#ifdef isLx
			EndOF = fgets(buff, MAXC, fp);
#else
			EndOF = fgets(buff, MAXC, *fp);
#endif

			if (EndOF == NULL)
			{
				break;
			}
#ifdef isLx
			strcpy(buff_c, buff);
			strcpy(format_c, format);

			token= strtok_r(buff_c, delim, &next);
			token = strtok(buff_c, delim);
			token_1 = strtok_r(format_c, "%", &next_2);
			token_1 = strtok(format_c, "%");
			strcpy(&buff_1, &next);
			strcpy(&buff_2, &next_2);


#else
			strcpy_s(buff_c, MAXC, buff);
			token = strtok_s(buff_c,delim, &next);
			token_1 = strtok_s(format,"%", &next_2);
#endif

			int qq = 0;
			int pp = 0;
			//  Parse data
			if (token != NULL && token_1 != NULL) 						//  First token or format not valid else fieldnum==0 (invalid)
			{
				//  First token
				if (strcmp(token_1, "f") || strcmp(token_1, "lf"))		// isdouble
				{
					d1 = strtod(token, NULL);
					*(Data_I_double[i] + qq) = d1;
					qq++;
				} 
				else if(strcmp(token_1, "d") || strcmp(token_1, "i")) // isint
				{
					*(Data_I_int[i] + pp) = i1;
					pp++;
				}
				else { printf("Not valid type\n");  break; }

				//  Read line
				for (kk = 1; kk < n_In; kk++)
				{
#ifdef isLx
					
					token = strtok_r(NULL, delim, &next);
					token = strtok(buff_1, delim);
					token_1 = strtok_r(NULL, "%",&next_2);
					token_1 = strtok(buff_2, "%");
					strcpy(&buff_1, &next);
					strcpy(&buff_2, &next_2);
#else
					char* token = strtok_s(NULL, delim, &next);
					char* token_1 = strtok_s(format, "%", &next_2);
#endif
						
					if (token != NULL && token_1 != NULL)
					{
						//printf(" %s\n", token);
						if (strcmp(token_1, "f") || strcmp(token_1, "lf"))		// isdouble
						{
							d1 = strtod(token, NULL);
							*(Data_I_double[i] + qq) = d1;
							qq++;
						}
						else if (strcmp(token_1, "d") || strcmp(token_1, "i")) // isint
						{
							*(Data_I_int[i] + pp) = d1;
							pp++;
						}
						else { printf(" Not valid type\n");  break;  }
					}
				}
				fieldnum = kk ;
			}
			else {
				fieldnum = 0; break;
			}
			// Line increment 
			if (fieldnum == n_In)
			{
				retVal++;
				i++;
				jj++;
			}
			else
			{
				printf("Not enough input in %s or wrong setting of input ( dimension mismatch )\n", filename);
				break;
			}
		}
	}
	
	// Log	
	if (retVal != length) {
		if (retVal < length)
		{
			if (retVal == max_length)
			{
				printf("Max vector allocation reached  %d\n", max_length);
			}
			else if (length == max_length)
			{
				printf("Processed up to EOF\n");
				printf("Read line : %d\n", retVal);
			}
		}
		else
		{
			printf("read line mismatch : valid lines %d less then requested rows  %d\n", retVal, length);
		}
	}
	else { printf("Read line : %d\n", retVal);}

	//Close file
#ifdef isLx
	fclose((fp));
#else
	fclose((*fp));
#endif
	return retVal;
}

/**
* Write File Data Outpuyt
* This function write .txt or CSV fle [sample]x[Output] considering different data type format passed
* in corrispondent matrices (I/O)
* Currently supported types : double (%f,%fl) , int (%d,%i)
* adding new data types : TODO
*/

int writeFileDataDI(char* filename, char* mode,
	double Data_O_double[ENTRIES][NTCM_O_DOUBLE_dim],
	int Data_O_int[ENTRIES][NTCM_O_INT_dim], char* format, char* Label, int n_Out, int length)
{
	// file 
	FILE* fp = NULL;
	// line counter
	int retVal = 0;

	// Buffers
	char  format_c[MAXC];
	char buff_2[MAXC];
	char* next_2 = " ";
	char* token_1 = " ";
	int delim = 1;
	char* appo = " ";

	// files
	char* token_csv = strstr(filename, ".csv");
	// Select delimiter
	if (token_csv != NULL)
	{
		delim = 2;
	}

#ifdef isLx
	fp=fopen(filename, mode);
#else
	fopen_s(&fp, filename, mode);
#endif
	if (fp != NULL)
	{
		int i,j;
		fprintf(fp, Label);

		for (i = 0; i < length ; i++)
		{
			int pp = 0;
			int qq = 0;
#ifdef isLx
			strcpy(format_c, format);
#else
			strcpy_s(format_c, MAXC, format);
#endif

			for (j = 0; j < n_Out; j++)
			{
#ifdef isLx
				if (j == 0)
				{
					token_1 = strtok_r(format_c, "%", &next_2);
					token_1 = strtok(format_c, "%");
				}
				else
				{
					token_1 = strtok_r(NULL, "%", &next_2);
					token_1 = strtok(format_c, "%");
				}
				
#else
				if (j == 0)
				{
					token_1 = strtok_s(format_c, "%", &next_2);
				}
				else
				{
					token_1 = strtok_s(NULL, "%", &next_2);
				}
#endif

				if (j < (n_Out -1))
				{
					if (strcmp(token_1, "f") || strcmp(token_1, "lf"))
					{

						if (delim==1)
						{
							fprintf(fp, "%f ", *(Data_O_double[i] + pp));
						}
						else if (delim == 2)
						{
							fprintf(fp, "%f,", *(Data_O_double[i] + pp));
						}
						else { fprintf(fp, "%f ", *(Data_O_double[i] + pp)); }

							pp++;
					}
					else
					{
						fprintf(fp, "%d%c", *(Data_O_int[i] + qq));
						qq++;
					}

				}
				else 
				{

					if (strcmp(token_1, "f") || strcmp(token_1, "lf"))
					{
						fprintf(fp, "%f \n", *(Data_O_double[i] + pp));
						pp++;
					}
					else
					{
						fprintf(fp, "%d \n", *(Data_O_int[i] + qq));
						qq++;
					}

				}


				
			}

			retVal++;
		}
#ifdef isLx
		fclose((fp));
#else
		fclose(fp);
#endif
	}
	if (retVal != length) {
		printf("Error writing file %s: written %d data instead of %d", filename, retVal, length);
	}
	return retVal;
}


