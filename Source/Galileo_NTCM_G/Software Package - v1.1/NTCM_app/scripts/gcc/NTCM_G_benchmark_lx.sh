#!/bin/bash
if [ "$1" = "-h" ];
then
    echo "*************************************************************************************"
	echo "-run to run benchmark on NTCM_G Model on High,Medium and Low Solar Activity Test File "
	echo "*************************************************************************************"
	echo "Example : ./NTCM_G_benchmark_lx.sh -run 0"
	read -p "Press enter to close"
	echo "Closing ...."		
else 
	sleep 1
    echo "*************************************************************************************"
	echo "TEST 1 (Low Solar)"
	echo "*************************************************************************************"
	if [ $# -lt 2 ]
	  then
	  exit -1
	fi
	Frequency=$2
	echo "*************************************************************************************"
	echo "NTCM PROCEDURE RUN Low Solar"
	cp ../Data/NTCM_G_Vector_Global_1_Year_Low_Solar.txt ./
	InputFile=NTCM_G_Vector_Global_1_Year_Low_Solar.txt
	OutputFile=NTCM_G_Vector_Global_1_Year_Low_Solar_out.txt
	../../bin/gcc/NTCM_G_check_lx.out ${Frequency} ${InputFile} ${OutputFile}  
	cp  ./NTCM_G_Vector_Global_1_Year_Low_Solar_out.txt  ../Out/NTCM_G_Vector_Global_1_Year_Low_Solar_out.txt
	rm ./NTCM_G_Vector_Global_1_Year_Low_Solar_out.txt
	rm ./NTCM_G_Vector_Global_1_Year_Low_Solar.txt
	if [ $? != "0" ]
	  then
	  exit -1;
	fi
	echo "success"
	echo "*************************************************************************************"
	sleep 1
    echo "*************************************************************************************"
	echo "TEST 2 (High Solar)"
	echo "*************************************************************************************"
	if [ $# -lt 2 ]
	  then
	  exit -1
	fi
	Frequency=$2

	echo "*************************************************************************************"
	echo "NTCM PROCEDURE RUN High Solar"
	cp ../Data/NTCM_G_Vector_Global_1_Year_High_Solar.txt ./
	InputFile=NTCM_G_Vector_Global_1_Year_High_Solar.txt;
	OutputFile=NTCM_G_Vector_Global_1_Year_High_Solar_out.txt;
	../../bin/gcc/NTCM_G_check_lx.out ${Frequency} ${InputFile} ${OutputFile}  
	cp  ./NTCM_G_Vector_Global_1_Year_High_Solar_out.txt  ../Out/NTCM_G_Vector_Global_1_Year_High_Solar_out.txt
	rm ./${OutputFile}
	rm ./${InputFile}
	if [ $? != "0" ]
	  then
	  exit -1;
	fi
	echo "success"
	echo "*************************************************************************************"
    sleep 1
	
    echo "*************************************************************************************"
	echo "TEST 3 (Medium Solar)"
	echo "*************************************************************************************"
	if [ $# -lt 2 ]
	  then
	  exit -1
	fi
	Frequency=$2

	echo "*************************************************************************************"
	echo "NTCM PROCEDURE RUN Medium Solar"
    InputFile=NTCM_G_Vector_Global_1_Year_Medium_Solar.txt;
	OutputFile=NTCM_G_Vector_Global_1_Year_Medium_Solar_out.txt;
	cp ../Data/NTCM_G_Vector_Global_1_Year_Medium_Solar.txt ./
	../../bin/gcc/NTCM_G_check_lx.out ${Frequency} ${InputFile} ${OutputFile}  
	cp  ./NTCM_G_Vector_Global_1_Year_Medium_Solar_out.txt  ../Out/NTCM_G_Vector_Global_1_Year_Medium_Solar_out.txt
	rm ./${OutputFile}
	rm ./${InputFile}
	if [ $? != "0" ]
	  then
	  exit -1;
	fi
	echo "success"
	echo "*************************************************************************************"
	
fi
	read -p "Press enter to close"
	echo "Closing ...."		
