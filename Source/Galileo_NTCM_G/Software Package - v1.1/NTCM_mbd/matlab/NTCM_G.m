%%*****************************************************************
%   NTCMproj File: NTCM_G.m
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
%   This module implements the NTCM G model.
%   Output of the module is the integrated VTEC. 
%
%   CONSTANTS:
%   --------
%
%   k1,k2,..,k12 = (see Table 3) => NTCM model coefficients (Described in Section 2.4.3)
%   Pf1          = 0.4 => Phase shift for the solar zenith angle dependency [dimensionless]
%   LTd          = 14  => Time phase shift for the Local Time [hours]
%   doy_DAV      = 18  => Time phase shift for the Annual variation [days]
%   doy_DSAV     = 6   => Time phase shift for the Semi-annual variation [days]
%   phi_c1       = 16  => Northward ionisation crest [deg]
%   phi_c2       = -10 => Southward ionisation crest [deg]
%   sigma_c1     = 12  => Best-fit value for the 1st component of the Ionisation crests  [deg]
%   sigma_c2     = 13  => Best-fit value for the 2nd component of the Ionisation crests  [deg]
%   phiGNP_deg   =  79.74 => Latitude of the geomagnetic North pole [deg]
%   lamGNP_deg   = -71.78 => Longitude of the geomagnetic North pole [deg]
%   pi_gal       = 3.1415926535898  => Ratio of a circle's circumference to its diameter (see Table 2)
%
%   INPUT:
%   --------
%   brdcIonoParam  => ai0,ai1,ai2 Effective Ionisation Level parameters broadcast in the Galileo Navigation Message
%   doy            => Day of the Year
%   LT             => Local Time [hours]
%   IppCoord       => Pierce Point Coordinates in  Latitude[rad], Longitude[rad] 
%
%   OUTPUT:
%   --------
%   vTEC => Vertical TEC in [TECU]
%
%   REFERENCE:
%   --------
%	[1] European GNSS (Galileo) Open Service - NTCM G Ionospheric Model Description, Issue 1.0, European Commission (EC)
%   [3] Hoque MM, N Jakowski, R Orus Perez, "Fast ionospheric correction using Galileo Az coefficients and the NTCM model,
%       GPS Solutions, 2019, doi: 10.1007/s10291-019-0833-3. https://doi.org/10.1007/s10291-019-0833-3
% ******************************************************************
%%
function [vTEC] = NTCM_G(brdcIonoParam,doy,LT,IppCoord)

k1 = 0.92519; k2 = 0.16951; k3 = 0.00443; k4 = 0.06626; k5 = 0.00899; k6 = 0.21289; k7 = -0.15414; k8 = -0.38439; k9 = 1.14023; k10 = 1.20556; k11 = 1.41808; k12 = 0.13985; % rms
Pf1        = 0.4;
LTd        = 14;
doy_DAV    = 18;
doy_DSAV   = 6;
phi_c1     = 16;     %degrees
phi_c2     = -10;    %degrees
sigma_c1   = 12;     %degrees
sigma_c2   = 13;     %degrees
phiGNP_deg =  79.74; %degrees
lamGNP_deg = -71.78; %degrees
pi_gal     = 3.1415926535898;   % Ratio of a circle's circumference to its diameter (see Table 2)

deg2rad    = pi_gal/180;

%Geomagnetic North Pole coordinate
phiGNP     = phiGNP_deg*deg2rad; % latitude in radian
lamGNP     = lamGNP_deg*deg2rad; % longitude in radian 
% Proxy measure of the solar activity level
azPar      = sqrt(          brdcIonoParam(1).^2 + ...
                  1633.33 * brdcIonoParam(2).^2 + ...
                  4802000 * brdcIonoParam(3).^2 + ...
                  3266.67 * brdcIonoParam(1) .* brdcIonoParam(3) );                              % ******  Eq.2

phiIPP = IppCoord(:,1);
lamIPP = IppCoord(:,2);

% Sun's declination [rad]  
delta = 23.44*sin(0.9856*(doy-80.7)*deg2rad)*deg2rad;                                            % ******  Eq.28

cos_lat_delta    = cos(phiIPP-delta);
solzenith_factor = cos_lat_delta + Pf1;                                                          % ******  Eq.29
cos_solzenith    = cos_lat_delta - phiIPP/(pi_gal/2).*sin(delta);                                % ******  Eq.30

latm_rad = asin(sin(phiIPP).*sin(phiGNP)+cos(phiIPP).*cos(phiGNP).*cos(lamIPP-lamGNP));          % ******  Eq.31
latm_deg = latm_rad/deg2rad; 

Vd     = 2*pi_gal*(LT-LTd)/24;                                                                   % ******  Eq.5
Vsd    = 2*pi_gal*LT/12;                                                                         % ******  Eq.6
Vtd    = 2*pi_gal*LT/8;                                                                          % ******  Eq.7
Va     = 2*pi_gal*(doy-doy_DAV)/365.25;                                                          % ******  Eq.9
Vsa    = 4*pi_gal*(doy-doy_DSAV)/365.25;                                                         % ******  Eq.10

cosVd  = cos(Vd);
cosVsd = cos(Vsd);
sinVsd = sin(Vsd);
cosVtd = cos(Vtd);
sinVtd = sin(Vtd);

EC1 = -(latm_deg-phi_c1)^2/2/sigma_c1^2;                                                         % ******  Eq.13
EC2 = -(latm_deg-phi_c2)^2/2/sigma_c2^2;                                                         % ******  Eq.14
exp_EC1 = exp(EC1);
exp_EC2 = exp(EC2);

% F1: Local time dependency
F1 = solzenith_factor + cos_solzenith.*(k1*cosVd+k2*cosVsd+k3*sinVsd+k4*cosVtd+k5*sinVtd);       % ******  Eq.4
% F2: Seasonal Dependency
F2 = 1 + k6.*cos(Va)+k7.*cos(Vsa);                                                               % ******  Eq.8;
% F3: Geomagnetic field dependency
F3 = 1 + k8.*cos(latm_rad);                                                                      % ******  Eq.11
% F4: Equatorial anomaly dependency
F4 = 1 + k9.*exp_EC1+k10.*exp_EC2;                                                               % ******  Eq.12
% F5: Solar activity dependency
F5 = k11 + k12.*azPar;                                                                           % ******  Eq.15

% Compute the vertical TEC
vTEC = F1 .* F2 .* F3 .* F4 .* F5;                                                               % ******  Eq.3
end