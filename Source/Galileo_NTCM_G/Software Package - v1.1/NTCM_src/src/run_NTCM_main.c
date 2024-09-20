/**
 * ******************************************************************
 * NTCMproj File: main.c
 * ******************************************************************
 *  @author      Francesco Menzione
 *  @reviewer    Matteo Sgammini
 *  @ingroup     NTCM_JRC
 *  @copyright   Joint Research Centre (JRC), 2019
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), Version 1.2 or later
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * ******************************************************************
 * ******************************************************************
 */

 /* Include Basic Libraries */
#include "run_NTCM_main.h"
 /* Include I/O functions */
#include "ReadWrite.h"
#include "ConfigRun.h"

#ifdef NTCM_PROCEDURE_TEST
#include "NTCM_Procedure.h" 
#endif
#ifdef NTCM_PROCEDURE_FUN
#include "NTCM_Procedure.h" 
#endif
#ifdef NTCM_G_TEST
#include "set_NTCM_G.h"
#include "NTCM_G.h" 
#endif
#ifdef NTCM_G_FUN
#include "set_NTCM_G.h"
#include "NTCM_G.h" 
#endif


/* ******************************************************************/
/* HEAP Allocation */
/* ******************************************************************/

/* Common I/O Data Matrices */
double Data_I_double[ENTRIES][NTCM_I_DOUBLE_dim];
int Data_I_int[ENTRIES][NTCM_I_INT_dim];
double Data_O_double[ENTRIES][NTCM_O_DOUBLE_dim];
int Data_O_int[ENTRIES][NTCM_O_INT_dim];

/* Input frequency Parameter */
real_T GNSScarrierFrequency;

/* Read/Write  */
long int readRows;
FILE* fp = NULL;
FILE* fp2 = NULL;
char* eptr;
int jj = 0, ii = 0, cnt = 0, wFlag, Rows, Iter;

/* Error*/
int ErrFlag = 0;
int ErrIndex = 0;

/* clock variables */
real_T time_spent;
real_T mean_time = 0, sample_time = 0;

/* ******************************************************************/
/* HEAP NTCM Interface Allocation (To be removed) */
/* ******************************************************************/
/* Specific test heap pre allocation (not strictly necessary),only to improve readability*/

/* NTCM_Procedure Interface */
#ifdef NTCM_PROCEDURE_TEST
/* NTCM_Procedure Input */
real_T azpvec[3];
real_T llhRecDeg[3], llhSatDeg[3];
real_T aoi1[ENTRIES], aoi2[ENTRIES], aoi3[ENTRIES], doy[ENTRIES], utc[ENTRIES];
real_T rx_lon_deg[ENTRIES], rx_lat_deg[ENTRIES], rx_alt_m[ENTRIES];
real_T sv_lon_deg[ENTRIES], sv_lat_deg[ENTRIES], sv_alt_m[ENTRIES];
/* NTCM_Procedure Output */
real_T vTEC_out[ENTRIES], sTEC_out[ENTRIES], delay_m_out[ENTRIES];
real_T vTEC_ref[ENTRIES], sTEC_ref[ENTRIES];
real_T vTEC_err[ENTRIES], sTEC_err[ENTRIES];
uint16_T ErrorCode[16]; // (not stored/event)
#elif NTCM_PROCEDURE_FUN
/* NTCM_Procedure Input */
real_T azpvec[3];
real_T llhRecDeg[3], llhSatDeg[3];
real_T aoi1[ENTRIES], aoi2[ENTRIES], aoi3[ENTRIES], doy[ENTRIES], utc[ENTRIES];
real_T rx_lon_deg[ENTRIES], rx_lat_deg[ENTRIES], rx_alt_m[ENTRIES];
real_T sv_lon_deg[ENTRIES], sv_lat_deg[ENTRIES], sv_alt_m[ENTRIES];
/* NTCM_Procedure Output */
real_T vTEC_out[ENTRIES], sTEC_out[ENTRIES], delay_m_out[ENTRIES];
uint16_T ErrorCode[16]; // (static stub not used)
#endif

/* NTCM_G_FUN Interface */

#ifdef NTCM_G_TEST
/* NTCM_G_FUN Procedure Input */
real_T brdcIonoParam[3];
real_T IppCoord[2];
real_T doy, LT;
real_T NTCM_Coeff_Vect[12];
real_T NTCM_Phys_Cons[10];
/* NTCM_G_FUN Procedure Output */
real_T vTEC_out;
real_T vTEC_err[ENTRIES];
#elif NTCM_G_FUN
/* NTCM_G_FUN Procedure Input */
real_T brdcIonoParam[3];
real_T IppCoord[2];
real_T doy, LT;
real_T NTCM_Coeff_Vect[12];
real_T NTCM_Phys_Cons[10];
/* NTCM_G_FUN Procedure Output */
real_T vTEC_out;
#endif


/* ******************************************************************/
/* RUN NTCM FUNCTION / TEST */
/* ******************************************************************/


int main(int argc, const char* const argv[])
{

	/* Variable arguments */
		/* argv[1] : GNSS Frequency							*/
		/* argv[2] : Input Files (.txt,.csv)				*/
		/* argv[3] : Output file (.txt,.csv)				*/
		/* argv[4] : Line to be processet (Rows)			*/
		/* argv[5] : Optional Iterate Runs (Clock Mean)		*/
	    /* argv[6] : clear Output File befor writing		*/


	(void)argc;
	(void)argv;

	/*  GNSS frequency setting */
	GNSScarrierFrequency = strtod(argv[1],&eptr);

	/* Rows setting */ 
	if (argc > 4)
	{		Rows = strtod(argv[4], &eptr) ;	}
	else
	{		Rows = ENTRIES;	}

	/* Iter setting */
	if (argc > 5)
	{
		Iter = strtod(argv[5], &eptr);
	}
	else
	{
		Iter = REPEAT;
	}

		/* ******************************************************************/
		/* Read File (generalized)
		/* ******************************************************************/
		
		/* Start  Read Input file */
		printf("The input file is %s\n", argv[2]);
		printf(" ... Start Reading\n");

		/*Test Input FOrmat */
		char format[] = NTCM_I_format;
		readRows = readFileDataDI(&fp, argv[2], Data_I_double, Data_I_int,
			format, NTCM_I_dim, Rows, ENTRIES);
		/* End  Read Input file */
		printf(" ... End Reading\n");


		/* ******************************************************************/
		/* Run (generalized)
		/* ******************************************************************/

		printf("Initialize NTCM Function/Test \n");

		/* Iteration Cycle */
		while (jj <= Iter) {
		/* Check data  */
			if (readRows > 0)
			{
				/* clear file berfor writing it (optional) */
				if (argc > 6)
				{
					clearFile(argv[3]);
				}
				printf("Start execution!\n");

				/* Start Clock */
				clock_t begin = clock();

				/* Rows Cycle */
				for (ii = 0; ii < readRows; ii++) {

					/* ******************************************************************/
					/* SET/CONFIG (Custom)
					/* ******************************************************************/
#ifdef NTCM_PROCEDURE_TEST
// No Set
#endif
#ifdef NTCM_PROCEDURE_FUN
// No Set
#endif
#ifdef NTCM_G_TEST
					set_NTCM_G(NTCM_Coeff_Vect, NTCM_Phys_Cons);
#endif
#ifdef NTCM_G_FUN
					set_NTCM_G(NTCM_Coeff_Vect, NTCM_Phys_Cons);
#endif



					/* ******************************************************************/
					/* NTCM_PROCEDURE_TEST (Custom)
					/* ******************************************************************/
#ifdef NTCM_PROCEDURE_TEST


					//Input  Interface
					azpvec[0]		=		*(Data_I_double[ii] + 0);
					azpvec[1]		=		*(Data_I_double[ii]+ 1) ;
					azpvec[2]		=		*(Data_I_double[ii]+ 2) ;
					doy[ii]			=		*(Data_I_double[ii] + 3);
					utc[ii]			=		*(Data_I_double[ii] + 4);
					llhRecDeg[1]	=		*(Data_I_double[ii] + 5) ;
					llhRecDeg[0]	=		*(Data_I_double[ii] + 6) ;
					llhRecDeg[2]	=		*(Data_I_double[ii] + 7) ;
					llhSatDeg[1]	=		*(Data_I_double[ii] + 8) ;
					llhSatDeg[0]	=		*(Data_I_double[ii] + 9) ;
					llhSatDeg[2]	=		*(Data_I_double[ii] + 10);	

					// Run Function
					NTCM_Procedure(azpvec, &doy[ii], &utc[ii], llhRecDeg, llhSatDeg,
						&GNSScarrierFrequency, &vTEC_out[ii], &sTEC_out[ii], &delay_m_out[ii],ErrorCode);

					// Trap error 
					for (int Errcount=0; Errcount<16; Errcount++)
					{
						if (ErrorCode[Errcount] != 0) { ErrFlag = 1; ErrIndex = Errcount; break; }
					}
					if (ErrFlag == 1)
					{
							printf("An error occured with code %i at %i th position", ErrorCode[ErrIndex], ErrIndex+1);
							break;
					}
				
					//Output Interface
					*(Data_O_double[ii] + 0) = vTEC_out[ii];
					*(Data_O_double[ii] + 1) = sTEC_out[ii];
					*(Data_O_double[ii] + 2) = delay_m_out[ii];


						//Error
					*(Data_O_double[ii] + 3) = (vTEC_out[ii] - (*(Data_I_double[ii] + 11)));
					*(Data_O_double[ii] + 4) = (sTEC_out[ii] - (*(Data_I_double[ii] + 12)));

#endif

					/* ******************************************************************/
					/* NTCM_PROCEDURE_FUN (Custom)
					/* ******************************************************************/

#ifdef NTCM_PROCEDURE_FUN

					//Input  Interface
					azpvec[0] = *(Data_I_double[ii] + 0);
					azpvec[1] = *(Data_I_double[ii] + 1);
					azpvec[2] = *(Data_I_double[ii] + 2);
					doy[ii] = *(Data_I_double[ii] + 3);
					utc[ii] = *(Data_I_double[ii] + 4);
					llhRecDeg[1] = *(Data_I_double[ii] + 5);
					llhRecDeg[0] = *(Data_I_double[ii] + 6);
					llhRecDeg[2] = *(Data_I_double[ii] + 7);
					llhSatDeg[1] = *(Data_I_double[ii] + 8);
					llhSatDeg[0] = *(Data_I_double[ii] + 9);
					llhSatDeg[2] = *(Data_I_double[ii] + 10);

					// Run Function
					NTCM_Procedure(azpvec, &doy[ii], &utc[ii], llhRecDeg, llhSatDeg,
						&GNSScarrierFrequency, &vTEC_out[ii], &sTEC_out[ii], &delay_m_out[ii], ErrorCode);
					
					// Trap error 
					for (int Errcount = 0; Errcount < 16; Errcount++)
					{
						if (ErrorCode[Errcount] != 0) { ErrFlag = 1; ErrIndex = Errcount; break; }
					}
					if (ErrFlag == 1)
					{
						printf("An error occured with code %i at %i th position", ErrorCode[ErrIndex], ErrIndex + 1);
						break;
					}

					//Output Interface
					*(Data_O_double[ii] + 0) = vTEC_out[ii];
					*(Data_O_double[ii] + 1) = sTEC_out[ii];
					*(Data_O_double[ii] + 2) = delay_m_out[ii];
					
#endif

					/* ******************************************************************/
					/* NTCM_G_TEST (Custom)
					/* ******************************************************************/

#ifdef NTCM_G_TEST

					//Input  Interface
					brdcIonoParam[0] = *(Data_I_double[ii] + 0);
					brdcIonoParam[1] = *(Data_I_double[ii] + 1);
					brdcIonoParam[2] = *(Data_I_double[ii] + 2);
					doy = *(Data_I_double[ii] + 3);
					LT = *(Data_I_double[ii] + 4);
					IppCoord[0] = *(Data_I_double[ii] + 5);
					IppCoord[1] = *(Data_I_double[ii] + 6);

					// Run Function
					NTCM_G(&brdcIonoParam[0], &doy, &LT,
						&IppCoord[0], &NTCM_Coeff_Vect[0], &NTCM_Phys_Cons[0], &vTEC_out);

					//Output Interface
					*(Data_O_double[ii] + 0) = vTEC_out;
					*(Data_O_double[ii] + 1) = (vTEC_out - (*(Data_I_double[ii] + 7)));

#endif
					/* ******************************************************************/
					/* NTCM_G_FUN (Custom)
					/* ******************************************************************/

#ifdef NTCM_G_FUN

					//Input  Interface
					brdcIonoParam[0] = *(Data_I_double[ii] + 0);
					brdcIonoParam[1] = *(Data_I_double[ii] + 1);
					brdcIonoParam[2] = *(Data_I_double[ii] + 2);
					doy = *(Data_I_double[ii] + 3);
					LT = *(Data_I_double[ii] + 4);
					IppCoord[0] = *(Data_I_double[ii] + 5);
					IppCoord[1] = *(Data_I_double[ii] + 6);

					// Run Function
					NTCM_G(&brdcIonoParam[0], &doy, &LT,
						&IppCoord[0], &NTCM_Coeff_Vect[0], &NTCM_Phys_Cons[0], &vTEC_out);

					//Output Interface
					*(Data_O_double[ii] + 0) = vTEC_out;

#endif

		/* ******************************************************************/
		/* Count Processed Data (generalized)
		/* ******************************************************************/
					cnt++;
				}

		/* ******************************************************************/
		/* Count Iterations (generalized)
		/* ******************************************************************/
				jj++;

		/* ******************************************************************/
		/* Evaluate Run (generalized)
		/* ******************************************************************/				
				clock_t end = clock();
				time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
				printf("The elapsed time is %lf seconds ", time_spent);
				printf("for %i processed rows\n", readRows);
				mean_time = mean_time + time_spent;
			}
			else {
			    printf("zero Rows ");
				break;
			}
	
	}
		/* ******************************************************************/
		/* Ex Time (generalized)
		/* ******************************************************************/
		sample_time = mean_time/ cnt;
		mean_time = mean_time / (Iter + 1);
		printf("mean time %0.10f for %i iteration  \n", mean_time, Iter + 1);
		printf("mean time %0.10f for 1 run on %i total runs\n", sample_time, cnt);
		printf("End execution!\n");

		printf(" ... Start Printing\n");

		/* ******************************************************************/
		/* Write Output (generalized)
		/* ******************************************************************/
		char format2[] = NTCM_O_format;
		wFlag = writeFileDataDI(argv[3], "w", Data_O_double, Data_O_int, format2, NTCM_O_label, NTCM_O_dim, readRows);
		if (wFlag > 0)
		{	printf("Results printed in %s\n", argv[3]);}
		printf(" ... End Printing\n");

	return 0;
}

/*
* File trailer for main.c
*
* [EOF]
*/
