%%*****************************************************************
%   NTCMproj File: runNTCM.m
%%*****************************************************************
%   @author      Matteo Sgammini
%   @reviewer    Francesco Menzione
%   @ingroup     NTCM_JRC
%   @copyright   Joint Research Centre (JRC), 2022
%   This software has been released as free and open source software
%   under the terms of the European Union Public Licence (EUPL), Version 1.2 or later
%   Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
%%*****************************************************************
%   Code generated for Matlab model 'NTCM_G'
%   Model version                  : 1.0
%   MatLab version                 : 9.7.0.1190202 (R2019b)
%
%%*****************************************************************
%   FUNCTION:
%   --------
%   This module output the vTEC(s), sTEC(s), and Ionospheric delay(s) computed for each row of the input data matrix.
%   It calls all the auxiliary functions and the NTCM G implementation.
%
%   CONSTANTS:
%   --------
%   pi_gal = 3.1415926535898  => Ratio of a circle's circumference to its diameter (see Table 2)
%
%   INPUT:
%   --------
%   inputData => Matrix containign for each row the following paramters: 
%     - Column Index:  [ 1  | 2   | 3   | 4   | 5   | 6            | 7     | 8         | 9            | 10          | 11             ]
%     - Column Param   [ai0 | ai1 | ai2 | DoY | UTC | Rx-longitude | Rx-latitude | Rx-Height | SV-longitude | SV-latitude | SV-Height] 
%     where:
%      - ai0 is the Effective Ionisation Level 1st order parameter [sfu]
%      - ai1 is the Effective Ionisation Level 2nd order parameter [sfu/deg]
%      - ai2 is the Effective Ionisation Level 3rd order parameter[sfu/deg]
%      - DoY is the Day of Year [dimensionless]
%      - UTC is the Universal time [hours]
%      - Rx-longitude is the User receiver Geodetic longitude [deg]
%      - Rx-latitude is the User receiver Geodetic latitude [deg]
%      - Rx-Height is the User receiver Geodetic height [meters]
%      - SV-longitude is the Satellite Geodetic longitude [deg]
%      - SV-latitude is the Satellite Geodetic latitude [deg]
%      - SV-Height is the Satellite Geodetic height Range [meters]
%
%   carrFreq => Carrier Frequency [Hz]
%
%   OUTPUT:
%   --------
%   vTEC => Vertical TEC [TECU]
%   sTEC => Slant TEC [TECU]
%   IonoDelay_m => Ionospheric delay for the input carrier frequency (carrFreq) [m]
%
%   REFERENCE:
%   --------
%	[1] European GNSS (Galileo) Open Service - NTCM G Ionospheric Model Description, Issue 1.0, European Commission (EC)
%   [2] Galileo NTCM G Software Package User Guide
% ******************************************************************
%%
function [vTEC, sTEC, IonoDelay_m] = runNTCM(inputData, carrFreq)

% ---------------------
% Init Constants
% ---------------------
pi_gal = 3.1415926535898;

% ---------------------
% Check range of input parameters
% ---------------------
[isValid, isCarrValid] = checkRanges(inputData,carrFreq);

% ---------------------
% Read input parameters
% ---------------------
brdcIonoParam = inputData(:,1:3);
matrixParam   = inputData(:,4:end);

% ---------------------
% Start Procedure
% ---------------------
nrOfExc = length(matrixParam(:,1));
deg2rad = pi_gal/180;

% Init output vectors
vTEC        = nan(nrOfExc,1);
sTEC        = nan(nrOfExc,1);
IonoDelay_m = nan(nrOfExc,1);

if ~isCarrValid
    carrFreq = nan;
end


disp(['> ',num2str(sum(isValid),'%d'),' valid set(s) found'])
disp('> NTCM is running...')

% Start loop 
for itc = 1 : nrOfExc
    % If any of the input parameters is not within the range, skip the Iono
    % estimation for that set
    if ~isValid
        continue;
    end
    doy     = matrixParam(itc,1);
    UTC     = matrixParam(itc,2);
    % Get User position in Geodetic coordinates
    rxPos.llh_deg  = [matrixParam(itc,4),matrixParam(itc,3),matrixParam(itc,5)];
    % Get Satellite position in Geodetic coordinates
    svPos.llh_deg  = [matrixParam(itc,7),matrixParam(itc,6),matrixParam(itc,8)];
    rxPos.llh_rad  = [rxPos.llh_deg(1).*deg2rad,rxPos.llh_deg(2).*deg2rad,rxPos.llh_deg(3)];
    svPos.llh_rad  = [svPos.llh_deg(1).*deg2rad,svPos.llh_deg(2).*deg2rad,svPos.llh_deg(3)];
    
    % *********************************************
    % STEP 1: convert Geodetic to Cartesian
    svPos.xyz  = llh2xyz(svPos.llh_rad);
    rxPos.xyz  = llh2xyz(rxPos.llh_rad);
    % *********************************************
    % STEP 2: get Azimuth and Elevation
    relVec     = svPos.xyz - rxPos.xyz; % ******   Eq. 21
    iDoa       = computeDoA(rxPos.llh_rad,relVec);
    % *********************************************
    % STEP 3: get Iono Pierce Point (IPP)
    [ippCoord]      = getIonoPiercePoint(rxPos.llh_rad,iDoa.ele_rad,iDoa.azi_rad);
    % *********************************************
    % STEP 4: convert to Local Time
    timeZone   = -((ippCoord(2)/deg2rad) /15); % ******  Eq.28 - First part
    % apply local time-zone
    LT = (UTC - timeZone);                % ******  Eq.28 - Second part
    % *********************************************
    % STEP 5: Run NTCM
    % Check elevation
    if iDoa.ele_rad<0
        vTEC(itc) = nan;
        sTEC(itc) = nan;
        continue;
    else
        % Run NTCM-G and obtain vertical TEC [TECU]
        [vTEC(itc)]      = NTCM_G(brdcIonoParam(itc,:), doy, LT, ippCoord);
        % Obtain the Mapping Function (MF)  
        % Re=6371Km is the Earth mean radius, and Hi=450Km is the IPP height
        MF               = 1./ sqrt(1-(0.934027268728925.*sin(0.9782*(pi_gal/2-iDoa.ele_rad))).^2);       % ******  Eq.33
        % Obtain slant TEC [TECU]
        sTEC(itc)        = vTEC(itc) .* MF;                                % ******  Eq.16
        % Convert slant TEC to propagation delay expressed in [m]
        IonoDelay_m(itc) = sTEC(itc) .*(40.3.*1e16)./carrFreq^2;           % ******  Eq.1
    end
end

disp('Finished!')
end