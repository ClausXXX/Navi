/*****************************
(c) Copyright European Space Agency, 2017
This file is subject to the terms and conditions defined in file 'LICENCE.txt', 
which is part of this [source code/executable] package. No part of the package, 
including this file, may be copied, modified, propagated, or distributed except
according to the terms contained in the file 'LICENCE.txt'.
******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "NeQuick.h"

/* Test files */
#define CONFIGFILE "NeQGalTest_config.dat"
#define INPUTFILELIST "NeQGalTest_input_files.dat"

/* Data files used by NeQuick */
#define MODIPFILE "modip2001_wrapped.asc"
#define CCIRFILEPATTERN "ccir%d.asc"

/* NeQuick configuration options */
#define KRONROD_TOL_1 (0.001)
#define KRONROD_TOL_2 (0.01)
#define RECURSION_LIMIT (50)

/* constants */
#define MAXCFGS (10)
#define MAXFILES (1024)
#define MAXFILENAME (1024)
#define MAXSTRINGLEN (80)

struct Config_st {
  double pdCoeffs[3];
  char pchIonoAct[MAXSTRINGLEN];
  char pchInputDir[MAXFILENAME];
  char pchOutputDir[MAXFILENAME];
};

static int default_mode(void);
static int file_mode(double pdCoeffs[3],const char *pinFilename, const char *poutFilename);
static int extended_file_mode(const char *pinFilename, const char *poutFilename);
static int std_extended_file_mode(void);
static int command_mode(double *pdCoeffs, int siMonth, double dTime, double *pdRecvLLHdeg, double *pdSatLLHdeg);
static int loadconfig(char *pchFilename, struct Config_st pstConfigs[MAXCFGS]);
static int loadfilelist(char *pchFilename, char pchFiles[MAXFILES][MAXFILENAME]);
static MODIP_st* loadmodipgrid(char *pchModipFile, MODIP_st *pstModip);
static CCIR_st* loadccirdata(char *pchCcirFilePattern, CCIR_st *pstCCIR);
static void performtest(int siMonth, double dTime,
                        double pdRecvLLHdeg[3], double pdSatLLHdeg[3],
                        double pdCoeffs[3], MODIP_st *pstModip, CCIR_st *pstCCIR,
                        double *pdModip, double *pdStec);
static void usage(void);

int main(int argc, char *argv[])
{
  char pchInputName[MAXFILENAME];
  char pchOutputName[MAXFILENAME];
  char configFileName[] = CONFIGFILE;
  char inputFileListName[] = INPUTFILELIST;
  double pdCoeffs[3];

  switch (argc)  
    {
    case 1:
      usage();        
    case 2:
      if (argv[1][0] != '-') 
        usage();        
      // else
      switch (argv[1][1])
        {
        case 'h':
          usage();
        case 'd':
          printf("Default usage with predefined files...\n"
                 "\tConfiguration file: %s \n"
                 " \tInput files list: %s\n", configFileName, 
                 inputFileListName);
          exit(default_mode());
        case 'i':
          // printf("Standard input/output file based usage including a set of Az coefficients per calculation...\n");
          exit(std_extended_file_mode());          
        default:
          usage();
        }
      break;
    default:
      if (argv[1][0] != '-') 
        usage();
      //else
      switch (argv[1][1])
        {
        case 'h':
          usage();
        case 'f':
          if (argc != 7 && argc != 6)
            {
              usage();
            }
          else
            {
              printf("File based usage with single set of Az coefficients...\n");
              pdCoeffs[0]= strtod(argv[2],(char**)NULL);
              pdCoeffs[1]= strtod(argv[3],(char**)NULL);
              pdCoeffs[2]= strtod(argv[4],(char**)NULL);

              sprintf(pchInputName, "%s", argv[5]);
              if (argc == 7){
                sprintf(pchOutputName, "%s",argv[6]);
              }else{ //argc = 6
                sprintf(pchOutputName, "%s",argv[5]);
                sprintf(pchOutputName + strlen(pchOutputName), ".out");
              }
              printf("\tOutput file: %s\n",pchOutputName);
              exit(file_mode(pdCoeffs,pchInputName,pchOutputName));
            }
        case 'e': 
          if (argc != 3 && argc != 4)
            {
              usage();
            }
          else
            {
              printf("File based usage including a set of Az coefficients per calculation...\n");

              sprintf(pchInputName, "%s", argv[2]);
              if (argc == 4){
                sprintf(pchOutputName, "%s",argv[3]);
              }else{ //argc = 6
                sprintf(pchOutputName, "%s",argv[2]);
                sprintf(pchOutputName + strlen(pchOutputName), ".out");
              }
              printf("\tOutput file: %s\n",pchOutputName);
              exit(extended_file_mode(pchInputName,pchOutputName));
            }                                        
        case 'c':
          if (argc != 13)
            {
              usage();
            }
          else
            {
              int siMonth;
              double dTime;
              double pdRecvLLHdeg[3];
              double pdSatLLHdeg[3];

              //printf("Command-line usage ...\n");
              pdCoeffs[0]= strtod(argv[2],(char**)NULL);
              pdCoeffs[1]= strtod(argv[3],(char**)NULL);
              pdCoeffs[2]= strtod(argv[4],(char**)NULL);
              siMonth = atoi(argv[5]);
              dTime = strtod(argv[6],(char**)NULL);
              pdRecvLLHdeg[1] = strtod(argv[7],(char**)NULL);
              pdRecvLLHdeg[0] = strtod(argv[8],(char**)NULL);
              pdRecvLLHdeg[2] = strtod(argv[9],(char**)NULL);
              pdSatLLHdeg[1] = strtod(argv[10],(char**)NULL);
              pdSatLLHdeg[0] = strtod(argv[11],(char**)NULL);
              pdSatLLHdeg[2] = strtod(argv[12],(char**)NULL);
              
              exit(command_mode(pdCoeffs,siMonth,dTime,pdRecvLLHdeg,pdSatLLHdeg));
            }
        case 'm':
          if (argc != 4)
            {
              usage();
            }
          else
            {
              static MODIP_st stModip;
              double dModip;
              double dLongitude, dLatitude;

              //printf("Modip calculation for a given longitude,latitude...\n");
              dLongitude= strtod(argv[2],(char**)NULL);
              dLatitude= strtod(argv[3],(char**)NULL);
              if (dLongitude < -180 || dLongitude > 360 || dLatitude < -90 || dLatitude > 90)
                usage();
              //dLongitude = dLongitude * PI_D / 180.0;
              //dLatitude = dLatitude * PI_D / 180.0;
              loadmodipgrid(MODIPFILE, &stModip);
              
 			  dModip = NeqCalcModip(dLatitude, dLongitude, &stModip);
              fprintf(stdout,"%12.5f\n",dModip);
            }        
          break;
        default:
          printf("Wrong Argument: %s\n", argv[1]);
          usage();
        }

    }
                
  exit(0);
}

/* 
 * Function name; default_mode
 * Purpose: run NeQuick in predefined mode with predefined configuration files (option -d)
 * Input: none
 * Output: integer with status of function run
 */
static int default_mode(void)
{
  static MODIP_st stModip;
  static CCIR_st stCCIR;
  struct Config_st pstConfigs[MAXCFGS];
  char pchFileList[MAXFILES][MAXFILENAME];
  int siNumCfgs;
  int siNumFiles;
  int siCfg;
  int siDataFile;
  
  siNumCfgs = loadconfig(CONFIGFILE, pstConfigs);
  siNumFiles = loadfilelist(INPUTFILELIST, pchFileList);
  loadmodipgrid(MODIPFILE, &stModip);
  loadccirdata(CCIRFILEPATTERN, &stCCIR);
  
  for (siCfg=0; siCfg<siNumCfgs; siCfg=siCfg+1) {
    for (siDataFile=0; siDataFile<siNumFiles; siDataFile=siDataFile+1) {
      FILE *fpInputFile;
      FILE *fpOutputFile;
      char pchInputName[MAXFILENAME];
      char pchOutputName[MAXFILENAME];
      char pchBuffer[1024];
      sprintf(pchInputName, "%s/%s", pstConfigs[siCfg].pchInputDir, pchFileList[siDataFile]);
      sprintf(pchOutputName, "%s/%s", pstConfigs[siCfg].pchOutputDir, pchFileList[siDataFile]);
      sprintf(pchOutputName + strlen(pchOutputName) - 7, "_out_%s.dat", pstConfigs[siCfg].pchIonoAct);
      fpInputFile = fopen(pchInputName, "r");
      if (fpInputFile == NULL) {
        fprintf(stderr, "Unable to open input file: %s\n", pchInputName);
        continue;
      }
      fpOutputFile = fopen(pchOutputName, "w");
      if (fpOutputFile == NULL) {
        fprintf(stderr, "Unable to open output file: %s\n", pchOutputName);
        fclose(fpInputFile);
        continue;
      }
      
      /* process input lines */
      while (fgets(pchBuffer, 1024, fpInputFile) != NULL) {
        int siMonth;
        double dTime;
        double pdRecvLLHdeg[3];
        double pdSatLLHdeg[3];
        double dModip;
        double dStec;

        /* read input */
        sscanf(pchBuffer, "%d %lf %lf %lf %lf %lf %lf %lf", 
               &siMonth, &dTime,
               &(pdRecvLLHdeg[1]), &(pdRecvLLHdeg[0]), &(pdRecvLLHdeg[2]), 
               &(pdSatLLHdeg[1]), &(pdSatLLHdeg[0]), &(pdSatLLHdeg[2]));

        /* perform the test */
        performtest(siMonth, dTime, pdRecvLLHdeg, pdSatLLHdeg,
                    pstConfigs[siCfg].pdCoeffs, &stModip, &stCCIR,
                    &dModip, &dStec);
                    
        /* make receiver longitude positive for output (so matches fortran version) */
        pdRecvLLHdeg[1] = fmod(pdRecvLLHdeg[1] + 360.0, 360.0);
      
        /* write output line */
        fprintf(fpOutputFile, "%3d %12.5f %12.5f %12.5f %16.5f %12.5f %12.5f %16.5f %12.5f %12.5f\n",
                siMonth, dTime,
                pdRecvLLHdeg[1], pdRecvLLHdeg[0], pdRecvLLHdeg[2],
                pdSatLLHdeg[1], pdSatLLHdeg[0], pdSatLLHdeg[2],
                dModip, dStec);
      }
      
      fclose(fpInputFile);
      fclose(fpOutputFile);
    }
  }
  
  return 0; 
}

/* 
 * Function name; file_mode
 * Purpose: run NeQuick in file mode with a single set of Az coefficients and input / output files 
 *                provided by user (option -f)
 * Input: 
 *         pdCoeffs - array of doubles with Effective Ionisation Parameter 
 *                 Az coefficients (a0, a1, a2)
 *         *pinFilename - pointer to input file name
 *        *poutFilename - point to output file name
 * Output: integer with status of function run
 */
static int file_mode(double *pdCoeffs,const char *pinFilename, const char *poutFilename)
{
  static MODIP_st stModip;
  static CCIR_st stCCIR;
  char pchFileList[MAXFILES][MAXFILENAME];

  FILE *fpInputFile;
  FILE *fpOutputFile;
  char pchBuffer[1024];

  int siMonth;
  double dTime;
  double pdRecvLLHdeg[3];
  double pdSatLLHdeg[3];
  double dModip;
  double dStec;

        
  loadmodipgrid(MODIPFILE, &stModip);
  loadccirdata(CCIRFILEPATTERN, &stCCIR);

  fpInputFile = fopen(pinFilename, "r");
  if (fpInputFile == NULL) {
    fprintf(stderr, "Unable to open input file: %s\n", pinFilename);
    return 1;
    //        continue;
  }        
  fpOutputFile = fopen(poutFilename, "w");
  if (fpOutputFile == NULL) {
    fprintf(stderr, "Unable to open output file: %s\n", poutFilename);
    fclose(fpInputFile);
    return 1;
    //        continue;
  }

    
  /* process input lines */
  while (fgets(pchBuffer, 1024, fpInputFile) != NULL) {
    
    /* read input */
    sscanf(pchBuffer, "%d %lf %lf %lf %lf %lf %lf %lf", 
           &siMonth, &dTime,
           &(pdRecvLLHdeg[1]), &(pdRecvLLHdeg[0]), &(pdRecvLLHdeg[2]), 
           &(pdSatLLHdeg[1]), &(pdSatLLHdeg[0]), &(pdSatLLHdeg[2]));

          /* perform the test */
    performtest(siMonth, dTime, pdRecvLLHdeg, pdSatLLHdeg,
                pdCoeffs, &stModip, &stCCIR,
                &dModip, &dStec);
                    
    /* make receiver longitude positive for output (so matches fortran version) */
    pdRecvLLHdeg[1] = fmod(pdRecvLLHdeg[1] + 360.0, 360.0);
      
    /* write output line */
    fprintf(fpOutputFile, "%3d %12.5f %12.5f %12.5f %16.5f %12.5f %12.5f %16.5f %12.5f %12.5f\n",
            siMonth, dTime,
            pdRecvLLHdeg[1], pdRecvLLHdeg[0], pdRecvLLHdeg[2],
            pdSatLLHdeg[1], pdSatLLHdeg[0], pdSatLLHdeg[2],
            dModip, dStec);
  }
      
  fclose(fpInputFile);
  fclose(fpOutputFile);

  return 0;
}

/* 
 * Function name: extended_file_mode
 * Purpose: run NeQuick in file mode with a set of Az coefficients for each line of data, with input / output files
 *                provided by user (option -e)
 * Input: 
 *         *pinFilename - pointer to input file name
 *        *poutFilename - point to output file name
 * Output: integer with status of function run
 */
static int extended_file_mode(const char *pinFilename, const char *poutFilename)
{
  static MODIP_st stModip;
  static CCIR_st stCCIR;
  char pchFileList[MAXFILES][MAXFILENAME];

  FILE *fpInputFile;
  FILE *fpOutputFile;
  char pchBuffer[1024];

  double pdCoeffs[3]; // array for Az coefficients a0, a1 and a2
  int siMonth;
  double dTime;
  double pdRecvLLHdeg[3];
  double pdSatLLHdeg[3];
  double dModip;
  double dStec;
        
  loadmodipgrid(MODIPFILE, &stModip);
  loadccirdata(CCIRFILEPATTERN, &stCCIR);

  fpInputFile = fopen(pinFilename, "r");
  if (fpInputFile == NULL) {
    fprintf(stderr, "Unable to open input file: %s\n", pinFilename);
    return 1;
    //        continue;
  }        
  fpOutputFile = fopen(poutFilename, "w");
  if (fpOutputFile == NULL) {
    fprintf(stderr, "Unable to open output file: %s\n", poutFilename);
    fclose(fpInputFile);
    return 1;
    //        continue;
  }

  //  return 0;

      
  /* process input lines */
  while (fgets(pchBuffer, 1024, fpInputFile) != NULL) {

        
    /* read input */
    sscanf(pchBuffer, "%lf %lf %lf %d %lf %lf %lf %lf %lf %lf %lf", 
           &(pdCoeffs[0]), &(pdCoeffs[1]), &(pdCoeffs[2]), &siMonth, &dTime,
           &(pdRecvLLHdeg[1]), &(pdRecvLLHdeg[0]), &(pdRecvLLHdeg[2]), 
           &(pdSatLLHdeg[1]), &(pdSatLLHdeg[0]), &(pdSatLLHdeg[2]));
      
    /* perform the test */
    performtest(siMonth, dTime, pdRecvLLHdeg, pdSatLLHdeg,
                pdCoeffs, &stModip, &stCCIR,
                &dModip, &dStec);
                    
    /* make receiver longitude positive for output (so matches fortran version) */
    pdRecvLLHdeg[1] = fmod(pdRecvLLHdeg[1] + 360.0, 360.0);
      
    /* write output line */
    fprintf(fpOutputFile, "%12.5f %12.5f %12.5f %3d %12.5f %12.5f %12.5f %16.5f %12.5f %12.5f %16.5f %12.5f %12.5f\n",
            pdCoeffs[0], pdCoeffs[1], pdCoeffs[2], siMonth, dTime,
            pdRecvLLHdeg[1], pdRecvLLHdeg[0], pdRecvLLHdeg[2],
            pdSatLLHdeg[1], pdSatLLHdeg[0], pdSatLLHdeg[2],
            dModip, dStec);
  }
      
  fclose(fpInputFile);
  fclose(fpOutputFile);

  return 0;
}

/* 
 * Function name: std_extended_file_mode
 * Purpose: run NeQuick in file mode with a set of Az coefficients for each line of data, with standard input / output 
 *                provided by user (option -e)
 * Input: 
 *         <none> - data is introduced through standard input
 * Output: integer with status of function run
 */
static int std_extended_file_mode()
{
  static MODIP_st stModip;
  static CCIR_st stCCIR;
  char pchFileList[MAXFILES][MAXFILENAME];

  FILE *fpInputFile;
  FILE *fpOutputFile;
  char pchBuffer[1024];

  double pdCoeffs[3]; // array for Az coefficients a0, a1 and a2
  int siMonth;
  double dTime;
  double pdRecvLLHdeg[3];
  double pdSatLLHdeg[3];
  double dModip;
  double dStec;
        
  loadmodipgrid(MODIPFILE, &stModip);
  loadccirdata(CCIRFILEPATTERN, &stCCIR);

      
  /* process input lines */
  while (fgets(pchBuffer, 1024, stdin) != NULL) {

        
    /* read input */
    sscanf(pchBuffer, "%lf %lf %lf %d %lf %lf %lf %lf %lf %lf %lf", 
           &(pdCoeffs[0]), &(pdCoeffs[1]), &(pdCoeffs[2]), &siMonth, &dTime,
           &(pdRecvLLHdeg[1]), &(pdRecvLLHdeg[0]), &(pdRecvLLHdeg[2]), 
           &(pdSatLLHdeg[1]), &(pdSatLLHdeg[0]), &(pdSatLLHdeg[2]));
      
    /* perform the test */
    performtest(siMonth, dTime, pdRecvLLHdeg, pdSatLLHdeg,
                pdCoeffs, &stModip, &stCCIR,
                &dModip, &dStec);
                    
    /* make receiver longitude positive for output (so matches fortran version) */
    pdRecvLLHdeg[1] = fmod(pdRecvLLHdeg[1] + 360.0, 360.0);
      
    /* write output line */
    fprintf(stdout, "%12.5f\n", dStec);
  }

  return 0;
}

/* 
 * Function name: command_mode
 * Purpose: run NeQuick in command-line mode parameters (option -c)
 * Input: 
 *         pdCoeffs - array of doubles with Effective Ionisation Parameter 
 *                 Az coefficients (a0, a1, a2)
 *      siMonth - Month
 *      dTime - UT hour as a real number
 *      pdRecvLLHdeg - array with latitude, longitude and height of receiver
 *      pdSatLLHdeg - array with latitude, longitude and height of satellite 
 * Output: integer with status of function run
 */
static int command_mode(double *pdCoeffs, int siMonth, double dTime, double *pdRecvLLHdeg, double *pdSatLLHdeg)
{
  static MODIP_st stModip;
  static CCIR_st stCCIR;
  char pchFileList[MAXFILES][MAXFILENAME];

  FILE *fpInputFile;
  FILE *fpOutputFile;
  char pchBuffer[1024];

  double dModip;
  double dStec;
        
  loadmodipgrid(MODIPFILE, &stModip);
  loadccirdata(CCIRFILEPATTERN, &stCCIR);

  
  /* perform the test */
  performtest(siMonth, dTime, pdRecvLLHdeg, pdSatLLHdeg,
              pdCoeffs, &stModip, &stCCIR,
              &dModip, &dStec);
  
  /* make receiver longitude positive for output (so matches fortran version) */
  pdRecvLLHdeg[1] = fmod(pdRecvLLHdeg[1] + 360.0, 360.0);
  
  fprintf(stdout, "%12.5f\n", dStec);

  return 0;
}

static int loadconfig(char *pchFilename, struct Config_st pstConfigs[MAXCFGS])
{
  FILE *fpFile;
  int siLine;
  char pchBuffer[1024];
  int siNumCfgs = 0;
  
  fpFile = fopen(pchFilename, "r");
  
  if (fpFile == NULL) {
    fprintf(stderr, "Unable to open file: %s\n", pchFilename);
    return 0;
  }
  
  fgets(pchBuffer, 1024, fpFile);
  
  for (siLine=0; siLine<MAXCFGS; siLine++) {
    if (fgets(pchBuffer, 1024, fpFile) == NULL) {
      break;
    }
    sscanf(pchBuffer, "%lf %lf %lf %s %s %s",
           &(pstConfigs[siLine].pdCoeffs[0]),
           &(pstConfigs[siLine].pdCoeffs[1]),
           &(pstConfigs[siLine].pdCoeffs[2]),
           pstConfigs[siLine].pchIonoAct,
           pstConfigs[siLine].pchInputDir,
           pstConfigs[siLine].pchOutputDir);
    siNumCfgs++;
  }
  
  fclose(fpFile);
  
  return siNumCfgs;
}

static int loadfilelist(char *pchFilename, char pchFiles[MAXFILES][MAXFILENAME])
{
  FILE *fpFile;
  int siLine;
  int siNumLines = 0;
  
  fpFile = fopen(pchFilename, "r");
  
  if (fpFile == NULL) {
    fprintf(stderr, "Unable to open file: %s\n", pchFilename);
    return 0;
  }
  
  for (siLine=0; siLine<MAXFILES; siLine++) {
    if (fgets(pchFiles[siLine], MAXFILENAME, fpFile) == NULL) {
      break;
    } else {
      int siPos;
      for (siPos=strlen(pchFiles[siLine])-1; siPos>=0; siPos--) {
        if ((pchFiles[siLine][siPos]=='\n') ||
            (pchFiles[siLine][siPos]=='\r') ||
            (pchFiles[siLine][siPos]==' ') ||
            (pchFiles[siLine][siPos]=='\t')) {
          pchFiles[siLine][siPos] = '\0';
        } else {
          break;
        }
      }
      siNumLines++;
    }
  }
  
  fclose(fpFile);
  
  return siNumLines;
}

static MODIP_st* loadmodipgrid(char *pchModipFile, MODIP_st *pstModip)
{
  FILE *fpFile;
  int siLatIndex;
  int siLngIndex;

  fpFile = fopen(pchModipFile, "r");
  if (fpFile == NULL) {
    fprintf(stderr, "Unable to open file: %s\n", pchModipFile);
    return NULL;
  }  
 
  for (siLatIndex = 0; siLatIndex < NUM_MODIP_DIM_1_SI; siLatIndex++) {
    for (siLngIndex = 0; siLngIndex < NUM_MODIP_DIM_2_SI; siLngIndex++) {
      if (fscanf(fpFile, "%lf", &(pstModip->pdModip[siLatIndex][siLngIndex])) == EOF) {
        fprintf(stderr, "Unexpected EOF in %s\n\n", pchModipFile);
        return NULL;
      }
    }
  }

  return pstModip;
}


static CCIR_st* loadccirdata(char *pchCcirFilePattern, CCIR_st *pstCCIR)
{
  int siMonth;
  char fname[32];
  FILE* pAsc[MONTHS_SI];
  int siIndex1;
  int siIndex2;
  int siSolar;


  /* Open the input files */
  for (siMonth = 0; siMonth < MONTHS_SI; siMonth++) {
    /* Attempt to open the files ccir11.asc -> ccir22.asc */
    sprintf(fname, pchCcirFilePattern, siMonth + 11);
    pAsc[siMonth] = fopen(fname, "r");

    /* Check if file was opened correctly */
    if (pAsc[siMonth] == NULL) {
      fprintf(stderr, "Unable to open file: %s\n", fname);
      
      /* Open has failed so need to close all files already opened */
      for (siIndex1 = 0; siIndex1 < siMonth; siIndex1++) {
        fclose(pAsc[siIndex1]);
      }
      
      return NULL;
    }
  }

  /* Files are all open, so read in the data */
  for (siSolar = 0; siSolar < CCIR_F2_DIM_3_SI; siSolar++) {
    for (siIndex2 = 0; siIndex2 < CCIR_F2_DIM_2_SI; siIndex2++) {
      for (siIndex1 = 0; siIndex1 < CCIR_F2_DIM_1_SI; siIndex1++) {
        for (siMonth = 0; siMonth < MONTHS_SI; siMonth++) {
          fscanf(pAsc[siMonth], "%lf",
                 &(pstCCIR->pdF2[siMonth][siIndex1][siIndex2][siSolar]));
        }
      }
    }
  }

  for (siSolar = 0; siSolar < CCIR_M3000_DIM_3_SI; siSolar++) {
    for (siIndex2 = 0; siIndex2 < CCIR_M3000_DIM_2_SI; siIndex2++) {
      for (siIndex1 = 0; siIndex1 < CCIR_M3000_DIM_1_SI; siIndex1++) {
        for (siMonth = 0; siMonth < MONTHS_SI; siMonth++) {
          fscanf(pAsc[siMonth], "%lf",
                 &(pstCCIR->pdM3000[siMonth][siIndex1][siIndex2][siSolar]));
        }
      }
    }
  }

  /* Close the files */
  for (siMonth = 0; siMonth < MONTHS_SI; siMonth++) {
    fclose(pAsc[siMonth]);
  }
 
  return pstCCIR;
}




static void performtest(int siMonth, double dTime,
                        double pdRecvLLHdeg[3], double pdSatLLHdeg[3],
                        double pdCoeffs[3], MODIP_st *pstModip, CCIR_st *pstCCIR,
                        double *pdModip, double *pdStec)
{
  static CurrentCCIR_st stCurrCCIR;
  static NeQuickInputData_st stNeQuickInputData;
  double pdKronrodTol[2] = {KRONROD_TOL_1, KRONROD_TOL_2};
  double pdRecvLLHrad[3];
  double pdSatLLHrad[3];
  
  pdRecvLLHrad[0] = pdRecvLLHdeg[0] * PI_D / 180.0;
  pdRecvLLHrad[1] = pdRecvLLHdeg[1] * PI_D / 180.0;
  pdRecvLLHrad[2] = pdRecvLLHdeg[2];
  pdSatLLHrad[0] = pdSatLLHdeg[0] * PI_D / 180.0;
  pdSatLLHrad[1] = pdSatLLHdeg[1] * PI_D / 180.0;
  pdSatLLHrad[2] = pdSatLLHdeg[2];

  stNeQuickInputData.pstModip = pstModip;
  stNeQuickInputData.pstCCIR = pstCCIR;
  stNeQuickInputData.pdGssPosLLH = pdRecvLLHrad;
  stNeQuickInputData.pdSatPosLLH = pdSatLLHrad;
  stNeQuickInputData.pdCoeff = pdCoeffs;
  stNeQuickInputData.pdKronrodTol = pdKronrodTol;
  stNeQuickInputData.siMaxRecurse = RECURSION_LIMIT;
  stNeQuickInputData.siMonth = siMonth;
  stNeQuickInputData.dUT = dTime;
  
  *pdModip = NeqCalcModip(pdRecvLLHdeg[0], pdRecvLLHdeg[1], pstModip);
  
  stNeQuickInputData.pdCoeff = pdCoeffs;
  stNeQuickInputData.siNumCoeff = 3;
  
  NeQuick(&stNeQuickInputData, &stCurrCCIR, pdStec);
  
  *pdStec = *pdStec / 1.0E16;
}

static void usage(void) { 
        
  char configFileName[] = CONFIGFILE;
  char inputFileListName[] = INPUTFILELIST;
  
  printf("Usage:\n");
  printf(" -h : help - shows this message\n\n");
  printf(" -d : default configuration, uses default input / output config files: \n" 
         "\t %s %s\n\n", configFileName, inputFileListName);
  printf(" -f <a0> <a1> <a2> <input_file> [<output_file>]\n");
  printf("\t- a0, a1 and a2 are the Effective Ionisation Coefficient parameters\n");
  printf("\t- input_file including the observations (month, UT, receiver and satellite lon(deg), lat(deg) and heights(m)\n");
  printf("\t- output_file is file including input file parameters, modip(deg) and sTEC(TECU)\n");
  printf("\t- if output_file is not specified, the name will be '<input_file>.out' = \n\n");
  printf(" -e <extended_input_file> [<extended_output_file>]\n");
  printf("\t- as -f but the input_file includes the Effective Ionisation Parameter coefficients \n\n");
  printf(" -i \n");
  printf("\t- as -e but the input parameters are provided through the standard input and sTEC is written in the standard output\n\n");
  printf(" -c <a0> <a1> <a2> <month> <UT> " 
         "<lon_station> <lat_station> <hei_station> "
         "<lon_satellite> <lat_satellite> <hei_satellite> \n");
  printf("\t- estimates STEC according to the input parameters and prints it TECU in the standard output\n\n");        
  printf(" -m <longitude> <latitude>\n");
  printf("\t- estimates modip according to the input parameters and prints it in degrees in the standard output\n\n");        
  exit (8);
}
