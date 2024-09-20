%%*****************************************************************
%   NTCMproj File: testNTCM_MATLAB.m
%%*****************************************************************
%   @author      Matteo Sgammini
%   @reviewer    Francesco Menzione
%   @ingroup     NTCM_JRC
%   @copyright   Joint Research Centre (JRC), 2022
%   This software has been released as free and open source software
%   under the terms of the European Union Public Licence (EUPL), version 1
%   Questions? Submit your query at https://www.gsc-europa.eu/contact-us/helpdesk
%%*****************************************************************
%   Code generated for Matlab model 'NTCM_G'
%   Model version                  : 1.0
%   MatLab version                 : 9.7.0.1190202 (R2019b)
%
%%*****************************************************************
%   FUNCTION:
%   --------
%   This module asks the user to select a test vector and a carrier
%   frequency and retrieves the TEC and Ionosheric delay by calling the
%   auxiliary procedures and the NTCM G function.
%   It also compares the obtained TEC and delay with the expected one and
%   show a simple statistics on the residuals.
%
%   INPUT:
%   --------
%   -
%
%   OUTPUT:
%   --------
%    -
%
%   REFERENCE:
%   --------
%	[1] European GNSS (Galileo) Open Service - NTCM G Ionospheric Model Description, Issue 1.0, European Commission (EC)
%   [2] NTCM G Software Package User Guide
% ******************************************************************
%%
function testNTCM_MATLAB()
%%
% ---------------------
% Init path and variables
% ---------------------
clc;

% Retrieve path of script file
[filepath,~,~]  = fileparts(mfilename('fullpath'));
% Set the path to the location of NTCM scripts
cd(filepath);
% Add NTCM Matlab folder to search path
addpath('..\matlab');

% Init vector of Galileo Signals 
carr_list       = {'E1','E5','E5a','E5b','E6'};

% Read all the test vector files 
testVectors_dir = '..\mbd_test\TestPattern\';
d = dir([testVectors_dir,'test*.mat']);
fn = {d.name};

% ---------------------
% Query input data
% ---------------------
[fn_indx,fn_ok] = listdlg('PromptString',{'Select a test vector:',...
    '(Only one file can be selected at a time)',''},...
    'SelectionMode','single','ListString',fn,'ListSize',[250,150]);

if ~fn_ok
    disp('No file selected, please try again!');
    return;
else
    fn_test = [testVectors_dir,fn{fn_indx}];  
end

[carr_indx,carr_ok] = listdlg('PromptString',{'Select a frequency.','(Only one frequency can be selected at a time)'},...
                              'ListString',carr_list,'SelectionMode','single');
% Check if a carrier frequency has been selected
if ~carr_ok
    carr_indx = 1;
end
% Select Carrier frequency [Hz]
switch carr_indx
    case 1 %'E1'
        carrFreq  = 1575.420e6;
    case 2 %'E5'
        carrFreq  = 1191.795e6;
    case 3 %'E5a'
        carrFreq  = 1176.450e6;
    case 4 %'E5b'
        carrFreq  = 1207.140e6;
    case 5 %'E6'
        carrFreq  = 1278.750e6;        
end 
% ---------------------
% Read input file
% ---------------------
% read input Table
dataIn     = load(fn_test);
inputTable = dataIn.(subsref(fieldnames(dataIn),substruct('{}',{1})));
% Convert table to matrix
inputData  = table2array(inputTable);
clear inputTable;

% ---------------------
% Run NTCM and retrieve TEC and Iono delay estimates
% ---------------------
% Run auxiliary functions and NTCM G
[vTEC, sTEC, IonoDelay_m] = runNTCM(inputData(:,1:11), carrFreq);

% Calculate difference between TEC estimates obtained using the NTCM Matlab
% implementation and the expected values
deltaVTEC = vTEC - inputData(:,12);
deltaSTEC = sTEC - inputData(:,13);

% Display test statistics
disp('>')
disp('> Test statistics:')
fprintf('> vTEC (values in TECU): max=%.5g, min=%.5g, average=%.5g\n',...
        max(vTEC,[],1,'omitnan'), min(vTEC,[],1,'omitnan'), mean(vTEC,1,'omitnan'));
fprintf('> vTEC residuals (values in TECU) : max=%.5g, min=%.5g, average=%.5g\n',...
        max(deltaVTEC,[],1,'omitnan'), min(deltaVTEC,[],1,'omitnan'), mean(deltaVTEC,1,'omitnan'));
fprintf('> sTEC (values in TECU): max=%.5g, min=%.5g, average=%.5g\n',...
        max(sTEC,[],1,'omitnan'), min(sTEC,[],1,'omitnan'), mean(sTEC,1,'omitnan'));
fprintf('> sTEC residuals (values in TECU) : max=%.5g, min=%.5g, average=%.5g\n',...
        max(deltaSTEC,[],1,'omitnan'), min(deltaSTEC,[],1,'omitnan'), mean(deltaSTEC,1,'omitnan'));    
fprintf('> Ionospheric delay (values in m): max=%.5g, min=%.5g, average=%.5g\n',...
        max(IonoDelay_m,[],1,'omitnan'), min(IonoDelay_m,[],1,'omitnan'), mean(IonoDelay_m,1,'omitnan'));
disp('Test finished!')
end