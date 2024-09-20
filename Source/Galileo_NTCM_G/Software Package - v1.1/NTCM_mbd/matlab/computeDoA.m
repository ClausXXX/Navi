%%*****************************************************************
%   NTCMproj File: computeDoA.m
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
%    This module computes the satellite azimuth and elevation angles (Described in Section 2.5.2)    
%
%   CONSTANTS:
%   --------
%   pi_gal = 3.1415926535898  => Ratio of a circle's circumference to its diameter (see Table 2)
%
%   INPUT:
%   --------
%   llhUserRad => Receiver position in  Latitude[rad], Longitude[rad], height[m]
%   losVec     => Line of Sight (LoS) vector
%
%   OUTPUT:
%   --------
%   svDoa.ele_rad      => Elevation (DoA) [rad]
%   svDoa.azi_rad      => Azimuth (DoA)   [rad]
%
%   REFERENCE:
%   --------
%	[1] European GNSS (Galileo) Open Service - NTCM G Ionospheric Model Description, Issue 1.0, European Commission (EC)
% ******************************************************************
%%
function svDoa = computeDoA(llhUserRad, losVec)
% 
pi_gal = 3.1415926535898; % Ratio of a circle's circumference to its diameter (see Table 2)

phi = llhUserRad(:,1);
lam = llhUserRad(:,2);
sinphi = sin(phi);
cosphi = cos(phi);
sinlam = sin(lam);
coslam = cos(lam);
cosphicoslam = cosphi .* coslam;
cosphisinlam = cosphi .* sinlam;
sinphicoslam = sinphi .* coslam;
sinphisinlam = sinphi .* sinlam; 

% Compute rotation matrix (Eq. 21)
dV_rot = [-sinphicoslam -sinphisinlam cosphi ;...
          -sinlam        coslam       0;...
           cosphicoslam  cosphisinlam sinphi] * losVec';
       
% Calculate azimuth of DoA (Eq. 22)
svDoa.azi_rad  = atan2(dV_rot(2),dV_rot(1));
if svDoa.azi_rad<0
    svDoa.azi_rad = svDoa.azi_rad + 2*pi_gal;
end       

% Calculate elevation of DoA (Eq. 23)      
svDoa.ele_rad  = ( 0.5 - atan2( sqrt(dV_rot(1).^2+dV_rot(2).^2), dV_rot(3)) ./ pi_gal ) .* pi_gal;

end