%%*****************************************************************
%   NTCMproj File: checkRanges.m
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
%   This module checks if the input parameters are within the ranges as
%   specified in [2] NTCM G Software Package User Guide
%
%   CONSTANTS:
%   --------
%   ai0_range => Effective Ionisation Level 1st order parameter, Range	[0, 512]sfu
%   ai1_range => Effective Ionisation Level 2nd order parameter	Range [-4, 4]sfu/deg
%   ai2_range => Effective Ionisation Level 3rd order parameter	Range [-0.25, 0.25]sfu/deg
%   doy_range => Day of Year Range [1, 366] dimensionless
%   utc_range => Universal time [0,24]hours
%   rlt_range => User receiver Geodetic latitude Range [-90, 90]deg
%   rln_range => User receiver Geodetic longitude Range [-180, 180]deg
%   rht_range => User receiver Geodetic height Range [-4000, 400000]meters
%   slt_range => Satellite Geodetic latitude Range [-90, 90]deg
%   sln_range => Satellite Geodetic longitude Range [-180, 180]deg
%   sht_range => Satellite Geodetic height Range [450000, 60000000]meters
%   crr_range => Carrier Frequency Range [0, 20000e6]Hz
%
%   INPUT:
%   --------
%   inputData => Matrix containign for each row the following paramters: 
%     - Column Index:  [ 1  | 2   | 3   | 4   | 5   | 6            | 7     | 8         | 9            | 10          | 11             ]
%     - Column Param   [ai0 | ai1 | ai2 | DoY | UTC | Rx-longitude | Rx-latitude | Rx-Height | SV-longitude | SV-latitude | SV-Height] 
%   carrFreq => Signal Carrier Frequency [Hz]
%
%   OUTPUT:
%   --------
%   isValid => Vector indicating for each row of <inputData> if the parameters are within the ranges
%   isCarrValid => Flag indicating if the Carrier frequency is within the range
%
%   REFERENCE:
%   --------
%   [2] Galileo NTCM G Software Package User Guide
% ******************************************************************
%%
function [isValid, isCarrValid] = checkRanges(inputData, carrFreq)

ai0_range = [0,512];       
ai1_range = [-4,4];
ai2_range = [-0.25,0.25];
doy_range = [1,366];
utc_range = [0,24];
rlt_range = [-90,90];
rln_range = [-180,180];
rht_range = [-4e3,400e3];
slt_range = [-90,90];
sln_range = [-180,180];
sht_range = [450e3,60000e3];
crr_range = [0,20000e6];

% Check input ranges
isValid = (inputData(:,1) >=ai0_range(1) & inputData(:,1) <=ai0_range(2)) & ...
          (inputData(:,2) >=ai1_range(1) & inputData(:,2) <=ai1_range(2)) & ...
          (inputData(:,3) >=ai2_range(1) & inputData(:,3) <=ai2_range(2)) & ...
          (inputData(:,4) >=doy_range(1) & inputData(:,4) <=doy_range(2)) & ...
          (inputData(:,5) >=utc_range(1) & inputData(:,5) <=utc_range(2)) & ...
          (inputData(:,6) >=rln_range(1) & inputData(:,6) <=rln_range(2)) & ...
          (inputData(:,7) >=rlt_range(1) & inputData(:,7) <=rlt_range(2)) & ...
          (inputData(:,8) >=rht_range(1) & inputData(:,8) <=rht_range(2)) & ...
          (inputData(:,9) >=sln_range(1) & inputData(:,9) <=sln_range(2)) & ...
          (inputData(:,10)>=slt_range(1) & inputData(:,10)<=slt_range(2)) & ...
          (inputData(:,11)>=sht_range(1) & inputData(:,11)<=sht_range(2));
      
isCarrValid = (carrFreq >=crr_range(1) & carrFreq <=crr_range(2));

