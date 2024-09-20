Introduction
------------
The baseline Matlab code available under this folder <./NTCM_mbd/matlab> has been used as the reference code for the development of the Simulink MBD solution and the following generation of the C/C++ source code. 

The Galileo NTCM G core function <NTCM_G.m> and the functions used to derive the auxiliary parameters strictly follow the algorithm description given in [1]


Function <runNTCM.m>
--------------------
The function <runNTCM.m> can be used to execute the Step-by-step procedure described in [1]. The input <inputData> shall be a matrix which columns contain the input parameters as indicated below:

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

For each row of the input matrix, the <runNTCM.m> function computes the vertical TEC (vTEC), the slant TEC (sTEC), and the ionospheric delay for the input frequency <carrFreq>.


Test Script <testNTCM_MATLAB.m>
-------------------------------
In order to test the Galileo NTCM G Matlab implementation, a test script testNTCM_MATLAB.m is available under the folder <./NTCM_mbd/mbd_script>. When executing the script, the user is asked to provide an input test vector and the carrier frequency. 
Exemplary test vectors in Matlab ‘.mat’ format are located in the folder <./NTCM_mbd/mbd_test/TestPattern>.

The following test vectors are available for selection:
- testVectors_RefDoc_Table_7_1_High_Solar.mat	--> Test vector for high solar activity as described in Section 7.1 of [1]
- testVectors_RefDoc_Table_7_1_Medium_Solar.mat	--> Test vector for medium solar activity as described in Section 7.1 of [1]
- testVectors_RefDoc_Table_7_1_Low_Solar.mat	--> Test vector for low solar activity as described in Section 7.1 of [1]
- testVectors_Global_1_Year_High_Solar.mat	--> Global test vector containing equally distributed user locations for 12 days, one day at each first day of a month, and sampled at 2 hours. This test vector is for high solar activity.
- testVectors_Global_1_Year_Medium_Solar.mat	--> Global test vector containing equally distributed user locations for 12 days, one day at each first day of a month, and sampled at 2 hours. This test vector is for medium solar activity.
- testVectors_Global_1_Year_Low_Solar.mat	--> Global test vector containing equally distributed user locations for 12 days, one day at each first day of a month, and sampled at 2 hours. This test vector is for low solar activity.

The text vectors contain a Matlab Table Array data type composed of 13 columns listing the input parameters as indicated below:

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

The script passes the table to the function runNTCM.m, reads the vTEC and sTEC output and compares them with the expected values. The script also provides some statistics on the minimum, maximum, and mean values for vTEC and sTEC for both absolute values and the obtained residuals.


References
----------
[1] European GNSS (Galileo) Open Service - NTCM G Ionospheric Model Description, Issue 1.0, European Commission (EC).