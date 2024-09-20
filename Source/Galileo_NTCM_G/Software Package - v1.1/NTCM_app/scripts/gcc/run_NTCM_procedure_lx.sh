#!/bin/bash
sleep 1
if [ "$1" = "-h" ];
then
    echo "*************************************************************************************"
	echo "The run_NTCM script runs the NTCM procedure module by using the general command structure: "
	echo " bash run_NTCM [-option] [arg1] [arg 2] .... (arg N)  []=mandatory ()=optional   "
	echo "The following option are available "
	echo ""
	echo " [1] Run NTCM algorithm "
    echo " -run  GNSS frequency Input.txt Output.txt (Rows) (Iterations)   	"
	echo " Input.txt 11 colunms : broad_ao1 broad_ao2 broad_ao3 doy utc rx_lat_deg rx_lon_deg rx_alt_m sv_lat_deg sv_lon_deg sv_alt_m "
    echo " Output.txt 3 columns : vTEC, sTEC and Ionodelay  "	
	echo " Note 1  : Input.txt Output.txt shall be included in ../Data folder "
	echo " Note 2  : Rows must be </= number of Input.txt lines. "
    echo " Example : ./run_NTCM_procedure_lx.sh -run 1575420000 testVectors_Global_1_Year_High_Solar.txt Out.txt "
	echo ""
    echo " [2]Compare vTEC/sTEC"
	echo " -check  GNSS frequency Input.txt Output.txt (Rows) (Iterations)   	"
	echo " Input.txt 11+2 colunms : broad_ao1 broad_ao2 broad_ao3 doy utc rx_lat_deg rx_lon_deg rx_alt_m sv_lat_deg sv_lon_deg sv_alt_m Ref_vTEC Ref_sTEC"
    echo " Output.txt 3+2 columns : vTEC, sTEC and Ionodelay Err_vTEC Err_sTEC  "	
	echo " Note 1  : Input.txt Output.txt shall be included in ../Data folder "
	echo " Note 2  : Rows must be </= number of Input.txt lines. "
    echo " Note 3  : the last two columns of Input contain the reference vTEC/sTEC values" 
	echo " to be compared with NTCM correspondent output  "
	echo " Example : ./run_NTCM_procedure_lx.sh -check 1575420000 testVectors_Global_1_Year_High_Solar.txt Out2.txt "
	echo "*************************************************************************************"
	read -p "Press enter to close"
	echo "Closing ...."		
fi 
if [ "$1" = "-run" ];
then
	read -p "Press enter to continue"
	echo "Running ...."		
echo $1
	if [ $# -lt 4 ]
	  then
	  exit -1
	fi
	Frequency=$2
	InputFile=$3
	OutputFile=$4
	
	
	if [ $# -eq 4 ]
	then
	echo "*************************************************************************************"
	echo "NTCM RUN..."
	cp ../Data/${InputFile} ./
	../../bin/gcc/run_NTCM_lx.out ${Frequency} ${InputFile} ${OutputFile} 
	cp  ./${OutputFile}  ../Out/${OutputFile}
	rm ./${OutputFile}
	rm ./${InputFile}
	if [ $? != "0" ]
	  then
	  exit -1;
	fi
	echo "success"
	echo "*************************************************************************************"
	fi
	
	if [ $# -eq 5 ]
	then
	NumSample=$5
	echo "*************************************************************************************"
	echo "NTCM RUN..."
	cp ../Data/${InputFile} ./
	../../bin/gcc/run_NTCM_lx.out ${Frequency} ${InputFile} ${OutputFile} ${NumSample} 
	cp  ./${OutputFile}  ../Out/${OutputFile}
	rm ./${OutputFile}
	rm ./${InputFile}
	if [ $? != "0" ]
	  then
	  exit -1;
	fi
	echo "success"
	echo "*************************************************************************************"
	fi 
	if [ $# -eq 6 ]
	then
	NumSample=$5
	NumIter=$6
	echo "*************************************************************************************"
	echo "NTCM RUN..."
	cp ../Data/${InputFile} ./
	../../bin/gcc/run_NTCM_lx.out ${Frequency} ${InputFile} ${OutputFile} ${NumSample} ${NumIter} 
	cp  ./${OutputFile}  ../Out/${OutputFile}
	rm ./${OutputFile}
	rm ./${InputFile}
		if [ $? != "0" ]
		  then
		  exit -1;
		fi
	fi
######################################################################################################################
elif [ "$1" = "-check" ]
then
	read -p "Press enter to continue"
	echo "Running ...."		
	if [ $# -lt 4 ]
	  then
	  exit -1
	fi
	Frequency=$2
	InputFile=$3
	OutputFile=$4
	
    if [ $# -eq 4 ]
	then
	echo "*************************************************************************************"
	echo "NTCM RUN..."
	cp ../Data/${InputFile} ./
	../../bin/gcc/NTCM_check_lx.out ${Frequency} ${InputFile} ${OutputFile} 
	cp  ./${OutputFile}  ../Out/${OutputFile}
	rm ./${OutputFile}
	rm ./${InputFile}
	if [ $? != "0" ]
	  then
	  exit -1;
	fi
	echo "success"
	echo "*************************************************************************************"
	fi
	
	if [ $# -eq 5 ]
	then
    NumSample=$5
	echo "*************************************************************************************"
	echo "NTCM RUN..."
	cp ../Data/${InputFile} ./
	../../bin/gcc/NTCM_check_lx.out ${Frequency} ${InputFile} ${OutputFile} ${NumSample} 
	cp  ./${OutputFile}  ../Out/${OutputFile}
	rm ./${OutputFile}
	rm ./${InputFile}
	if [ $? != "0" ]
	  then
	  exit -1;
	fi
	echo "success"
	echo "*************************************************************************************"
	fi 
	if [ $# -eq 6 ]
	then
	NumSample=$5
	NumIter=$6
	echo "*************************************************************************************"
	echo "NTCM RUN..."
	cp ../Data/${InputFile} ./
	../../bin/gcc/NTCM_check_lx.out ${Frequency} ${InputFile} ${OutputFile} ${NumSample} ${NumIter} 
	cp  ./${OutputFile}  ../Out/${OutputFile}
	rm ./${OutputFile}
	rm ./${InputFile}
		if [ $? != "0" ]
		  then
		  exit -1;
		fi
	echo "success"
	echo "*************************************************************************************"
	fi 
else
exit -1
fi
	read -p "Press enter to close"
	echo "Closing ...."		
