/*****************************
(c) Copyright European Space Agency, 2017
This file is subject to the terms and conditions defined in file 'LICENCE.txt', 
which is part of this [source code/executable] package. No part of the package, 
including this file, may be copied, modified, propagated, or distributed except
according to the terms contained in the file 'LICENCE.txt'.
 ******************************************************************************/

#ifndef NEQGETRAYPROPERTIES_H
#define NEQGETRAYPROPERTIES_H

/* System Includes */

/* NeQuick Includes */
#include "NeqTypes.h"
#include "IonoConstants.h"

/**
 *******************************************************************************
 * @par Name: NeqGetRayProperties
 * @par Purpose:
 *   (corresponds to 'rays' in fortran and 'rays2' in C++)
 *   This function checks if the ray is vertical and set P2 if it is. It then
 *   calculates the radius of P1 and P2 before calling NeqCalcRayProperties1 to
 *   calculate the properties of the ray and checks if it is valid. If the ray
 *   is slanted it then calculates some other ray properties by calling
 *   NeqCalcRayProperties2.
 *
 * @par Argument I/O:
 *   @param ptP1     (I/O) In: dLat/dLng(deg) and dH(km) only
 *                         Out: dSinLat/dCosLat(slant) & dR(km) only
 *   @param ptP2     (I/O) In: dLat/dLng(deg) and dH(km) only
 *                         Out: dLng(deg)(vert), dSinLat/dCosLat(slant) & dR(km)
 *   @param ptRay    (O) Ray perigee (dLat/dLng(deg) and dR(km) only)
 *   @param pdZeta   (O) Ray zenith angle (deg)
 *   @param pdSinSig (O) Sin of azimuth(slant)
 *   @param pdCosSig (O) Cos of azimuth(slant)
 *
 * @par I/O files:
 *   None
 *
 * @return TRUE if error found, otherwise FALSE
 ******************************************************************************/
Boolean_b NeqGetRayProperties(SPoint_st* pstP1, SPoint_st* pstP2, 
                              SPoint_st* pstRay, double* pdZeta, 
                              double* pdSinSig,
                              double* pdCosSig);

#endif /* NEQGETRAYPROPERTIES_H */
