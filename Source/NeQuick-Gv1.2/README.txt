===============================
> > NeQuick-G < <
Version: 1.1
Release date: 28/09/2017 (see CHANGELOG)
===============================
NeQuick electron density model is used by the Galileo ionospheric algorithm 
for single frequency receivers for correcting the ionospheric error contribution 
due to group delay on the signal. NeQuick is an empirical climatological 
ionospheric model, which generates electron density from analytical profiles, 
for the input values: R12 or F10.7, month, geographic latitude and longitude, 
height and UT (see [RD1, RD2]). Changing the height leads to the height profile 
of electron density. The use of an additional great circle subroutine allows 
calculating electron density along arbitrary ground-to-satellite or 
satellite-to-satellite ray paths. The model depends on geophysical parameters 
(R12 or F10.7) and empirical estimated coefficients such as the CCIR files. 

This version NeQuickGalileoSF v1.0.3 was developed by Nottingham Scientific Limited 
and the European Space Agency. This version is based on the monthly mean 
climatological NeQuickv1 model recommended by ITU-R with a number of adaptations
as required for Galileo real-time use based on broadcast parameters and incorporating 
a number of enhancements proposed by some of the original ITU-R version authors.

NeQuickv1 recommended by ITU-R was developed at Abdus Salam ICTP Trieste 
and at the University of Graz. It is based on the Di Giovanni - Radicella (DGR) model
(Di Giovanni, G., S. R. Radicella, An analytical model of the electron density 
profile in the ionosphere, Adv. Space Res. 10, 27-30, 1990) which was modified 
to the requirements of the COST 238 Action PRIME to give vertical electron content 
from ground to 1000 km consistent with the COST 238 regional electron content 
model (Radicella, S.M., M.-L. Zhang, The improved DGR analytical model of electron 
density height profile and total electron content in theionosphere. A. Geofisica, 38, 35-41, 1995).
Authors of the modified Di Giovanni - Radicella (DGR) model code:
	Man-Lian Zhang and S.M. Radicella, Abdus Salam ICTP Trieste.
Author of modifications of DGR code, of adaptations of "CCIR map code" and responsible 
author for ITU-R NeQuickv1 package: Dr. Reinhart Leitinger (IGAM, UGraz) with 
contributions by Gerald Hochegger (IGAM), Bruno Nava and Pierdavide Coisson (Abdus Salam ICTP)

===============================
> > Usage and examples: < <
===============================
An usage guide of the sample NeQuick driver NeQuick_command and several usage examples are provided in Examples/NeQuick_command_usage_examples.pdf

===============================
> > Validation example: <  <
===============================
Input - output validation files for other implementations are provided in Input_data/ and Output_data/ directories.

===============================
> > Compilation: < < 
===============================
A Makefile is provided
Compilation instructions are given in COMPILING

===============================
> > Changes: < < 
===============================
Version changes are included in CHANGELOG

===============================
> > Software terms and conditions < <
===============================
European Space Agency - ESA Software Community Licence Permissive - v2.3
See LICENCE.txt

===============================
> > Contact < <
===============================
For further questions contact:
Raul Orus Perez
European Space Agency, Noordwijk, The Netherlands
GNSS.Science@esa.int / Raul.Orus.Perez@esa.int