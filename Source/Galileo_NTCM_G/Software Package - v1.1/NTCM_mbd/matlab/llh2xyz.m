%%*****************************************************************
%   NTCMproj File: llh2xyz.m
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
%   This module converts the geodetic WGS84 coordinates to ECEF (Described in Sec.2.5.1)
%
%   CONSTANTS:
%   --------
%   a  = 6378137.0         => Semi-major axis [m]
%   b  = 6356752.3142      => Semi-major axis [m]
%   e2 = 0.006694380004261 => Eccentricity (e) of the ellipsoid squared (e2=1-b^2/a^2)
%
%   INPUT:
%   --------
%   llh => WGS84 coordinates in Latitude[rad], Longitude[rad], height[m]
%
%   OUTPUT:
%   --------
%   xyz => ECEF coordinates in x[m], y[m], z[m]
%
%   REFERENCE:
%   --------
%	[1] European GNSS (Galileo) Open Service - NTCM G Ionospheric Model Description, Issue 1.0, European Commission (EC)
% ******************************************************************
%%
function [xyz] = llh2xyz(llh)

% Semi-major axis [m]
a  = 6378137.0;
% Eccentricity (e) of the ellipsoid squared (e2)
% (It can be also obtained as <e2 = f*(2-f)> where f is the flattening)
e2 = 0.006694380004261;

% (Eq. 19)
v = a ./ sqrt(1-e2*sin(llh(:,1)).^2);
% Compute x component (Eq. 16)
xyz(:,1) = (v+llh(:,3)) .* cos(llh(:,1)) .* cos(llh(:,2));
% Compute y component (Eq. 17)
xyz(:,2) = (v+llh(:,3)) .* cos(llh(:,1)) .* sin(llh(:,2));
% Compute z component (Eq. 18)
xyz(:,3) = (v*(1-e2)+llh(:,3)) .* sin(llh(:,1));
end