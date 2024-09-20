/    
    @mainpage NTCM SW package    
    NTCM JRC Readme file    
       
    @copyright European Union, 2022<br>    
    This software has been released as free and open source software    
    under the terms of the European Union Public Licence (EUPL), version 1.2,    
    https://joinup.ec.europa.eu/sites/default/files/custom-page/attachment/eupl_v1.2_en.pdf <br>    
    Questions? Submit your query at https://www.gsc-europa.eu    
    Version: 1.0 <br>    
    Release date: <b>****</b>    
       
    <h3>Background</h3>    
    Galileo is the European global navigation satellite system providing a highly accurate and global positioning service    
    under civilian control. Galileo, and in general current GNSS, are based on the broadcasting of electromagnetic ranging    
    signals in the L frequency band. Those satellite signals suffer from a number of impairments when propagating    
    through the Earth’s ionosphere. Receivers  operating  in  single  frequency  mode  may  use  the  single  frequency    
    ionospheric  correction  algorithm  NTCM to  estimate  the ionospheric delay on each satellite link.    
    The NTCM-G SWP aims at providing a reference implementation of the NTCM G algorithm and the auxiliary functions described in [].    
    The target is to provide a portable and validated C/C++ version of the NTCM G model that can be easily deployed in different    
    GNSS application and simulation environments. The development approach is based on modern industrial rapid prototyping practice    
    of Model Based Design (MBD) using the Matlab/Simulink toolchain, and includes the deployment of the generated code into the target    
    C/C++ application.    
    <br>    
       
    The implementation has been written in the C programming language by using autocoding and model based design approach.     
    The package includes:    
     - NTCM function library (NTCM_src/src/lib) allowing to use the NTCM_Procedure Module and the NTCM_G algorithm Module     
     - Implementation auxiliary function (NTCM_src/src/aux_fun)    
     - The driver programs for running or test the package (src)    
       
    <h3>Spec(s):</h3>    
     - European GNSS (Galileo) Open Service - NTCM G Ionospheric Model Description, Issue 1.0, European Commission (EC)    
     - C standard ( Misra-C guidelines )    
       
    <h3>Requirements</h3>    
     - Matlab/Simulink Model Based Design Design,Implementation and test ( Model in the loop VS Sw in the loop)    
     - Embedded Coder Autocoding with the following Code generation objectives:    
   		 *    1. Execution efficiency    
   		 *    2. RAM efficiency    
   		 *    3. MISRA C:2012 guidelines    
     - Cross platform compatibility Windows/Linux    
     - Doxygen, a documentation generator    
        
       
    <h3> Build Configuration</h3>    
    Two build toolchains are supported:    
     - Microsoft C++ Compiler (Microsoft Visual Studio 2022)    
     - GNU compiler gcc (GCC for Windows Subsystem MVS via WSL distribution)    
       
    <h5> Pre compilation options</h5>    
        
     Each driver program source code support pre-compilation options as preprocessor flag. This allows to compile them for    
     different usages and according different configurations of the NTCM_G.    
       
     Note : a general #isLx (idef/ifndef) flag is used to distinguish between MVS and GCC target compilation    
        
    <h3>Microsoft Visual Studio Build with MVS and GCC </h3>    
        
     A unified approach for the driver program has been selected by using differen MVS solutions:    
     -  NTCM_app/build/mvs/NTCM_mvs ==>  Microsoft C++ Compiler    
     -  NTCM_app/build/gcc/NTCM_lx  ==>  GCC ( via WSL) <br>    
     Open the target solution using IDE Microsoft Visual Studio 2022. The solutions includes different available projects    
     that share NTCM library and differs in terms of:    
     * driver program main function (run_NTCM, run_NTCM_UT, etc.)    
     * preprocessor options for different analysis and debugging purposes (see preprocessor definition in solution properties)    
     * input which are passed per argument in the same way the user will experience in the driver programs (see command argument in solution properties)    
     All Input/Output file are included in NTCM_app/script/Data and NTCM_app/script/Out.    
       
     Note1 : NTCM_lx solution needs WSL distribution with UBUNTU 18 LTS. If not available the user can ghange the project    
             configuration properties and point to a different client linux machine    
     Note2 : NTCM_mvs directly refer to NTCM_app/script Data/Out content for debugging and testing purposes     
     Note3 : NTCM_lx need running the provided xxxx auxiliary script that copies NTCM_app/script Data/Out content in the target folder    
   		  for debugging and testing of the solution    
     Note4 : debug/relase and x64, x86 configuration can be controlled by changing configuration manager of the solution    
        
    <h3> Usage of the driver programs</h3>    
       
   	Script for Windows (NTCM_app/script/mvs)    
   		    
   	  <h4>NTCM Procedure function</h4>    
   	  run ./run_NTCM_procedure 			    
   		** -h 		command description for the NTCM procedure function    
   		** -run 	direct function run (fastest I/O without check)    
   		** -check  output comparison with reference input vector (see -h)    
       
   	  <h4>NTCM Procedure benchmark</h4>    
       
   	  run ./NTCM_procedure_banchmark    
   		** -h  	banchmark header 		    
   		** -run 	run tests    
   	    
         <h4>NTCM_G Model</h4>    
   	  run ./run_NTCM_G 		 			    
   		** -h 		command description for the NTCM_G Model    
   		** -run 	direct function run (fastest I/O without check)    
   		** -check  output comparison with reference input vector (see -h)    
       
   	  <h4>NTCM_G Model benchmark</h4>    
       
   	  run ./NTCM_G_banchmark    
   		** -h  	banchmark heade.   
   		** -run 	run tests    
         
      	Script for Linux (NTCM_app/script/gcc)    
   	    
   	  <h4>NTCM Procedure function</h4>    
   	  run ./run_NTCM_procedure_lx    
   		** -h 		command description for the NTCM procedure function    
   		** -run 	direct function run (fastest I/O without check)    
   		** -check  output comparison with reference input vector (see -h)    
       
   	  <h4>NTCM Procedure benchmark</h4>    
       
   	  run ./NTCM_procedure_banchmark_lx    
   		** -h  	banchmark header    
   		** -run 	run tests    
   	    
         <h4>NTCM_G Model</h4>    
   	  run ./run_NTCM_G_lx    
   		** -h 		command description for the NTCM_G Model    
   		** -run 	direct function run (fastest I/O without check)    
   		** -check  output comparison with reference input vector (see -h)    
       
   	  <h4>NTCM_G Model benchmark</h4>    
       
   	  run ./NTCM_G_banchmark_lx    
   		** -h  	banchmark header    
   		** -run 	run tests    
   	    
   	    
       
    <h3>CHANGELOG</h3>    
      First version.    
       
    <h3>Acknowlegements</h3>    
   