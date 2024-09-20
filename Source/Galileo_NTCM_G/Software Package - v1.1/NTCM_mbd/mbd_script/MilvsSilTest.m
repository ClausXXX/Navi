%% MiLvsSiL Test
clear all
clc
close all
bdclose all

% Load Test
cd(fullfile('.','mbd_test','TestPattern'))
[filename, pathname, filterindex] = uigetfile('*.mat', 'Pick a test vector');
cd ../..
Data=load(fullfile('.','mbd_test','TestPattern',filename));
% StructName=filename(1:end-4);
% TestData=table2struct(Data.(StructName),'ToScalar',true);
TestData = table2struct(Data.(subsref(fieldnames(Data),substruct('{}',{1}))),'ToScalar',true);
% Define discrete time of simulation
Sample_Time=1;
% Define Signals 
Var=fieldnames(TestData);
grid=0:Sample_Time:length(TestData.(Var{1}))-1;
End_Time=grid(end);

for ii=1:length(Var)
    
    TestDataS.(Var{ii}).signals.values=TestData.(Var{ii});
    TestDataS.(Var{ii}).time=grid;
    TestDataS.(Var{ii}).signals.dimensions=1;

end

disp('Opening MiLvsSiL Test')
open('NTCM_TEST_HARNESS_MiLvsSiL');
open('NTCM_Procedure');
open('NTCM_G');

disp('VariantFlag, 0/1 ChecksOff/ChecksOn ');
disp('Default 1');
load('Check_on.mat');
disp('GNSS Carrier Frequency : Galileo E1 ');
GNSScarrierFrequency=1.575420000000000e+09
disp('To change Frequency overwrite GNSScarrierFrequency');
disp('workspace variable before running the simulation');

cs = getActiveConfigSet('NTCM_Procedure');
set_param(cs,'StateflowObjectComments','On');
set_param(cs,'IncludeHyperlinkInReport','On');
set_param(cs,'GenerateTraceInfo','On');
set_param(cs,'MATLABSourceComments','On');
set_param(cs,'MATLABFcnDesc','On');
set_param(cs,'SFDataObjDesc','On');
set_param(cs,'CheckMdlBeforeBuild','Off');
save_system('NTCM_Procedure');

cs = getActiveConfigSet('NTCM_G');
set_param(cs,'StateflowObjectComments','On');
set_param(cs,'MATLABSourceComments','On');
set_param(cs,'IncludeHyperlinkInReport','On');
set_param(cs,'GenerateTraceInfo','On');
set_param(cs,'MATLABFcnDesc','On');
set_param(cs,'SFDataObjDesc','On');
set_param(cs,'CheckMdlBeforeBuild','Off');
save_system('NTCM_G');









