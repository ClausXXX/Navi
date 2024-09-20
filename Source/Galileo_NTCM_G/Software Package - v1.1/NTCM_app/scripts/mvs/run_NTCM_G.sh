#!/bin/bash
if [ "$1" = "-h" ];
then
    echo "*************************************************************************************"
	echo "The run_NTCM script runs the NTCM procedure module by using the general command structure: "
	echo " bash run_NTCM [-option] [arg1] [arg 2] .... (arg N)  []=mandatory ()=optional   "
	echo "The following option are available "
	echo ""
	echo " [1] Run NTCM_G Model "
    echo "-run  GNSS frequency Input.txt Output.txt (Rows) (Iterations)   	"
	echo " Input.txt 8 colunms : broad_ao1 broad_ao2 broad_ao3 doy LT ipp_lat_deg ipp_lon_deg ipp_alt sTEC"
    echo " Output.txt 3 columns : vTEC"	
	echo "Note 1 : Rows must be </= number of Input.txt lines. "
	echo "Note 2 : GNSS frequency is redundant information (removed in the next issue)"
	echo ""
    echo "[2]Compare vTEC/sTEC"
	echo "-check  GNSS frequency Input.txt Output.txt (Rows) (Iterations)   	"
	echo " Input.txt 11+2 colunms : broad_ao1 broad_ao2 broad_ao3 doy utc rx_lat_deg rx_lon_deg rx_alt_m sv_lat_deg sv_lon_deg sv_alt_m sTEC Ref_vTEC Ref_sTEC"
    echo " Output.txt 3+2 columns : vTEC Err_vTEC   "	
	echo " Note 1 : Rows must be </= number of Input.txt lines. "
    echo " Note 2 : the last two columns of Input contain the reference vTEC/sTEC values to be compared with NTCM correspondent output  "
	echo " Note 3 : GNSS frequency is redundant information (removed in the next issue)"
	echo "*************************************************************************************"
fi 
if [ "$1" = "-run" ];
then
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
	../../bin/mvs/run_NTCM.exe ${Frequency} ${InputFile} ${OutputFile} 
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
	../../bin/mvs/run_NTCM.exe ${Frequency} ${InputFile} ${OutputFile} ${NumSample} 
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
	../../bin/mvs/run_NTCM.exe ${Frequency} ${InputFile} ${OutputFile} ${NumSample} ${NumIter} 
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
	../../bin/mvs/NTCM_check.exe ${Frequency} ${InputFile} ${OutputFile} 
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
	../../bin/mvs/NTCM_check.exe ${Frequency} ${InputFile} ${OutputFile} ${NumSample} 
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
	../../bin/mvs/NTCM_check.exe ${Frequency} ${InputFile} ${OutputFile} ${NumSample} ${NumIter} 
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
