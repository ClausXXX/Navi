/**
 * ******************************************************************
 * NTCMproj File: ReadWrite.c
 * ******************************************************************
 *  @author      Francesco Menzione
 *  @reviewer    Matteo Sgammini
 *  @ingroup     NTCM_JRC
 *  @copyright   Joint Research Centre (JRC), 2019
 *  This software has been released as free and open source software
 *  under the terms of the European Union Public Licence (EUPL), version 1
 *  Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
 * ******************************************************************
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ConfigRun.h"




extern void clearFile(char* filename);

extern int readFileDataDI(FILE** fp, char* filename,double Data_I_double[ENTRIES][NTCM_I_DOUBLE_dim],
	int Data_I_int[ENTRIES][NTCM_I_INT_dim], char* format, int n_In,
	int length, int max_length);


extern int writeFileDataDI(char* filename, char* mode, double Data_O_double[ENTRIES][NTCM_O_DOUBLE_dim],
	int Data_O_int[ENTRIES][NTCM_O_INT_dim], char* format, char* Label,int n_In,int length);