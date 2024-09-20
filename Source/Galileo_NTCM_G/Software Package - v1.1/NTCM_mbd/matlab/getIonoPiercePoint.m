%%*****************************************************************
%   NTCMproj File: getIonoPiercePoint.m
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
%   This module computes the geographic latitude and longitude of the Ionospheric Pierce Point (IPP) (Described in Section 2.5.3). 
%   IPP is the point where the Line of Sight (LoS) intersects the reference ionospheric layer, this latter set at an altitude of 450Km.
%
%   CONSTANTS:
%   --------
%   Re     = 6371.0*1e3       => Earth mean radius (see Table 2)
%   h_IPP  = 450e3            => Ionospheric Pierce Point height (see Table 2)
%   pi_gal = 3.1415926535898  => Ratio of a circle's circumference to its diameter (see Table 2)
%
%   INPUT:
%   --------
%   llhUser    => Receiver position in  Latitude[rad], Longitude[rad], height[m]
%   ElRad      => Elevation (DoA) [rad]
%   AzRad      => Azimuth (DoA)   [rad]
%
%   OUTPUT:
%   --------
%   IppCoord => Pierce Point coordinate in  Latitude[rad], Longitude[rad]   
%
%
%   REFERENCE:
%   --------
%	[1] European GNSS (Galileo) Open Service - NTCM G Ionospheric Model Description, Issue 1.0, European Commission (EC)
% ******************************************************************
%% 
function [IppCoord] = getIonoPiercePoint(llhUser,ElRad,AzRad)
Re        = 6371.0*1e3;        % Earth mean radius (see Table 2)
h_IPP     = 450e3;             % Ionospheric Pierce Point height (see Table 2)
pi_gal    = 3.1415926535898;   % Ratio of a circle's circumference to its diameter (see Table 2)

r = h_IPP + Re;

nSmp      = length(ElRad);
IppCoord  = nan(length(ElRad),2);

for ipos = 1 : nSmp
    % Calculate Earth's central angle (Eq. 24)
    Psi_pp            = pi_gal/2 - ElRad(ipos) - asin(Re/r.*cos(ElRad(ipos)));
    sinPsi_pp         = sin(Psi_pp);
    % Calculate pierce Point latitude (Eq. 25)
    IppCoord(ipos,1)  = asin(sin(llhUser(1)).*cos(Psi_pp)+cos(llhUser(1)).*sinPsi_pp.*cos(AzRad(ipos)));
    % Calculate pierce Point longitude (Eq. 26)
    IppCoord(ipos,2)  = llhUser(2) + asin(sinPsi_pp.*sin(AzRad(ipos))./cos(IppCoord(ipos,1)));       
end
