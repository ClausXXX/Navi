/**
*******************************************************************
* NTCMproj File : ConfigRun.c
*******************************************************************
* @author      Francesco Menzione
* @reviewer    Matteo Sgammini
* @ingroup     NTCM_JRC
* @copyright   Joint Research Centre(JRC), 2019
* This software has been released as free and open source software
* under the terms of the European Union Public Licence(EUPL), version 1
* Questions ? Submit your query at https ://www.gsc-europa.eu/contact-us/helpdesk
*******************************************************************
*/

/* ******************************************************************/
/* Memory Allocation */
/* ******************************************************************/
#ifndef ENTRIES
#define ENTRIES (5000000)
#endif // !ENTRIES
#ifndef REPEAT
#define REPEAT 0
#endif // !REPEAT

/* ******************************************************************/
/* Configure Different Run/TESTs */
/* ******************************************************************/

 /* NTCM_PROCEDURE_FUN */

#ifdef NTCM_PROCEDURE_TEST

	#define NTCM_I_dim 13
	#define NTCM_I_format "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf"
	#define NTCM_I_DOUBLE_dim 13
	#define NTCM_I_INT_dim 1
	#define NTCM_O_format "%lf %lf %lf %lf %lf \n"
	#define NTCM_O_dim 5
	#define NTCM_O_DOUBLE_dim 5
	#define NTCM_O_INT_dim 1
	#define NTCM_O_label "vTEC sTEC IonoDelay err_vTEC err_sTEC \n"

#elif NTCM_PROCEDURE_FUN

	#define NTCM_I_dim 11
	#define NTCM_I_format "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf"
	#define NTCM_I_DOUBLE_dim 11
	#define NTCM_I_INT_dim 1
	#define NTCM_O_format "%lf %lf %lf \n"
	#define NTCM_O_dim 3
	#define NTCM_O_DOUBLE_dim 3 
	#define NTCM_O_INT_dim 1
	#define NTCM_O_label "vTEC sTEC IonoDelay  \n"

#endif


 /* NTCM_G_FUN */

#ifdef NTCM_G_TEST

#define NTCM_I_dim 8
#define NTCM_I_format "%lf %lf %lf %lf %lf %lf %lf %lf"
#define NTCM_I_DOUBLE_dim 8
#define NTCM_I_INT_dim 1
#define NTCM_O_format "%lf %lf \n"
#define NTCM_O_dim 2
#define NTCM_O_DOUBLE_dim 2
#define NTCM_O_INT_dim 1
#define NTCM_O_label "vTEC err_vTEC \n"

#elif NTCM_G_FUN

#define NTCM_I_dim 7
#define NTCM_I_format "%lf %lf %lf %lf %lf %lf %lf"
#define NTCM_I_DOUBLE_dim 7
#define NTCM_I_INT_dim 1
#define NTCM_O_format "%lf \n"
#define NTCM_O_dim 1
#define NTCM_O_DOUBLE_dim 1 
#define NTCM_O_INT_dim 1
#define NTCM_O_label "vTEC \n"

#endif

