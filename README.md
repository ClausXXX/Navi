# Navi
Program for solving GNSS navigation problem

This program allows you to find the coordinates of a navigation receiver using source data from RINEX format files 
at an arbitrary point in time, and also calculates additional characteristics of the solution and statistically 
processes the resulting solutions. It is used for research aimed at improving the positioning accuracy of global 
navigation satellite systems, especially in relation to the study of the ionospheric layer of the earth's atmosphere.

Main features of the program:
- support for RINEX format files versions 2.xx and 3.xx, precise ephemeris files .sp3;
- control of source data;
- the ability to fine-tune the program to study certain aspects of the solution;
- the ability to solve with different models of ionospheric (Klobuchar, GLONASS, NeQuickG, BDGIM, GEMTEC, Ionex, TEC-mapped) and tropospheric (GCAT, Saastamoinen, Saastamoinen-Davis) signal delays;
- finding the coordinates of the receiver in a certain time interval with a step that is a multiple of the step in the observation file;
- comparison of the results obtained with exact geodetic and calculated median coordinates of the receiver, calculation of the absolute and root-mean-square positioning error;
- output data to the screen and to a text file.

© Zatolokin D.A. 2015-2023
clausxxx@rambler.ru