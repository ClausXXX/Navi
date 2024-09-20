%% Reset Project

clear all
close all
fclose all
clear mex
bdclose all
clc

if exist('./slprj')
rmdir('./slprj','s');
end
delete *.slxc
delete *.mexw64
delete *.mexw32
delete *.mex


S=pwd;
Index=findstr(S,'NTCM_mbd');
Check=strcmp(S(Index:end),'NTCM_mbd');
if Check==0
    disp('Please run project in ./NTCM_mbd');
end
