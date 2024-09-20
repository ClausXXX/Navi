Galileo NTCM G Readme file
--------------------
Please run <Start Doxygen> to start Online Documentation or refer to the "Galileo NTCM G Software Package User Guide" Document.

Questions? Submit your query at https://www.gsc-europa.eu

Version: 1.1
Release date: April 2024

Copyright
---------
Copyright (c) 2024 European Union

Licensed under the EUPL, Version 1.2 or – as soon as they will be approved by the European Commission – subsequent versions of the EUPL (the "Licence");
You may not use this work except in compliance with the Licence.
You may obtain a copy of the Licence at:
https://joinup.ec.europa.eu/collection/eupl/eupl-text-eupl-12

Background
----------
Galileo is the European global navigation satellite system providing a highly accurate and global positioning service
under civilian control. Galileo, and in general current GNSS, are based on the broadcasting of electromagnetic ranging
signals in the L frequency band. Those satellite signals suffer from a number of impairments when propagating
through the Earth’s ionosphere. Receivers operating in single frequency mode may use the single frequency
ionospheric correction algorithm NTCM to estimate the ionospheric delay on each satellite link.
The Galileo NTCM G Software Package (NTCM-G SWP) aims at providing a reference implementation of the Galileo NTCM G algorithm and the auxiliary functions described in [1].
The target is to provide a portable and validated C/C++ version of the Galileo NTCM G model that can be easily deployed in different
GNSS application and simulation environments. The development approach is based on modern industrial rapid prototyping practice
of Model Based Design (MBD) using the Matlab/Simulink toolchain, and includes the deployment of the generated code into the target
C/C++ application [2].

The implementation has been written in the C programming language by using autocoding and model based design approach.
The package includes:
- NTCM function library (NTCM_src/src/lib) allowing to use the NTCM_Procedure Module and the NTCM_G algorithm Module
- Implementation auxiliary function (NTCM_src/src/aux_fun)
- The driver programs for running or test the package (src)

Spec(s):
--------
[1] European GNSS (Galileo) Open Service - NTCM-G Ionospheric Model Description, Issue 1.0, European Commission (EC)
[2] MISRA C:2012 Guidelines for the use of the C language in critical systems (3rd Edition), ISBN 978-1-906400-10-1 (paperback), ISBN 978-1-906400-11-8 (PDF), MIRA Limited, Nuneaton, March 2013.

Requirements
------------
- Matlab/Simulink Model Based Design Design,Implementation and test ( Model in the loop VS Sw in the loop)
- Embedded Coder Autocoding with the following Code generation objectives:
1. Execution efficiency
2. RAM efficiency
3. MISRA C:2012 guidelines
- Cross platform compatibility Windows/Linux
- Doxygen, a documentation generator

Build Configuration
-------------------
Two build toolchains are supported:
- Microsoft C++ Compiler (Microsoft Visual Studio 2022)
- GNU compiler gcc (GCC for Windows Subsystem MVS via WSL distribution)

- Pre compilation options
Each driver program source code support pre-compilation options as preprocessor flag. This allows to compile them for
different usages and according different configurations of the NTCM_G.
Note : a general #isLx (idef/ifndef) flag is used to distinguish between MVS and GCC target compilation

Microsoft Visual Studio Build with MVS and GCC
----------------------------------------------
A unified approach for the driver program has been selected by using differen MVS solutions:
- NTCM_app/build/mvs/NTCM_mvs ==> Microsoft C++ Compiler
- NTCM_app/build/gcc/NTCM_lx ==> GCC ( via WSL)

Open the target solution using IDE Microsoft Visual Studio 2022. The solutions includes different available projects
that share NTCM library and differs in terms of:
- driver program main function (run_NTCM, run_NTCM_UT, etc.)
- preprocessor options for different analysis and debugging purposes (see preprocessor definition in solution properties)
- input which are passed per argument in the same way the user will experience in the driver programs (see command argument in solution properties)
 All Input/Output file are included in NTCM_app/script/Data and NTCM_app/script/Out.

Note1 : NTCM_lx solution needs WSL distribution with UBUNTU 18 LTS. If not available the user can ghange the project
configuration properties and point to a different client linux machine
Note2 : NTCM_mvs directly refer to NTCM_app/script Data/Out content for debugging and testing purposes
Note3 : NTCM_lx need running the provided xxxx auxiliary script that copies NTCM_app/script Data/Out content in the target folder
for debugging and testing of the solution
Note4 : debug/relase and x64, x86 configuration can be controlled by changing configuration manager of the solution

Usage of the driver programs
----------------------------

--> Script for Windows (NTCM_app/script/mvs)
    ----------------------------------------

- NTCM Procedure function

run ./run_NTCM_procedure
** -h command description for the NTCM procedure function
** -run direct function run (fastest I/O without check), see example in -h
** -check output comparison with reference input vector, see example in -h

- NTCM Procedure benchmark

run ./NTCM_procedure_banchmark
** -h banchmark header
** -run run tests , see example in -h

- NTCM_G Model

run ./run_NTCM_G
** -h command description for the NTCM_G Model
** -run direct function run (fastest I/O without check), see example in -h
** -check output comparison with reference input vector , see example in -h

- NTCM_G Model benchmark

run ./NTCM_G_banchmark
** -h banchmark help.
** -run run tests , see example in -h

--> Script for Linux (NTCM_app/script/gcc)
    --------------------------------------

- NTCM Procedure function

run ./run_NTCM_procedure_lx
** -h command description for the NTCM procedure function
** -run direct function run (fastest I/O without check), see example in -h
** -check output comparison with reference input vector , see example in -h

- NTCM Procedure benchmark

run ./NTCM_procedure_banchmark_lx
** -h banchmark header
** -run run tests , see example in -h

- NTCM_G Model

run ./run_NTCM_G_lx
** -h command description for the NTCM_G Model
** -run direct function run (fastest I/O without check)
** -check output comparison with reference input vector , see example in -h

- NTCM_G Model benchmark

run ./NTCM_G_banchmark_lx
** -h banchmark header
** -run run tests, see example in -h

TEST VECTOR FORMAT
------------------
Exemplary test vectors in Matlab ‘.mat’ format are located in the folder <./NTCM_mbd/mbd_test/TestPattern>.
Exemplary test vectors for target applications (.exe, .out) are located in the folder <./NTCM_app/scripts/Data>.

The following test vectors are available for selection:
- testVectors_RefDoc_Table_7_1_High_Solar	--> Test vector for high solar activity as described in Section 7.1 of [1]
- testVectors_RefDoc_Table_7_1_Medium_Solar	--> Test vector for medium solar activity as described in Section 7.1 of [1]
- testVectors_RefDoc_Table_7_1_Low_Solar	--> Test vector for low solar activity as described in Section 7.1 of [1]
- testVectors_Global_1_Year_High_Solar		--> Global test vector containing equally distributed user locations for 12 days, one day at each first day of a month, and sampled at 2 hours. This test vector is for high solar activity.
- testVectors_Global_1_Year_Medium_Solar	--> Global test vector containing equally distributed user locations for 12 days, one day at each first day of a month, and sampled at 2 hours. This test vector is for medium solar activity.
- testVectors_Global_1_Year_Low_Solar		--> Global test vector containing equally distributed user locations for 12 days, one day at each first day of a month, and sampled at 2 hours. This test vector is for low solar activity.

The test vectors are composed of 13 columns listing the input parameters as indicated below:

Column 1	ai0 	 	Effective Ionisation Level 1st order parameter [sfu]
Column 2	ai1 		 Effective Ionisation Level 2nd order parameter [sfu/deg]
Column 3	ai2 	 	Effective Ionisation Level 3rd order parameter[sfu/deg]
Column 4	doy	 	Day of Year [dimensionless]
Column 5	utc 	 	Universal time [hours]
Column 6	rx_lon_deg 	 User receiver Geodetic longitude [deg]
Column 7	rx_lat_deg 	 User receiver Geodetic latitude [deg]
Column 8	rx_alt_m	 User receiver Geodetic height [meters]
Column 9	sv_lon_deg 	 Satellite Geodetic longitude [deg]
Column 10	sv_lat_deg 	 Satellite Geodetic latitude [deg]
Column 11	sv_alt_m	 Satellite Geodetic height Range [meters]
Column 12	vTEC_expected	Expected vertical TEC
Column 13	sTEC_expected	Expected slant TEC


CHANGELOG
---------
1.1 - Test vectors updated
    - Improved Help menu of the shell scripts 

1.0 - First issue