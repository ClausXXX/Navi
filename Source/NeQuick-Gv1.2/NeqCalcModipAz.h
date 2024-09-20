/*****************************
(c) Copyright European Space Agency, 2017
This file is subject to the terms and conditions defined in file 'LICENCE.txt', 
which is part of this [source code/executable] package. No part of the package, 
including this file, may be copied, modified, propagated, or distributed except
according to the terms contained in the file 'LICENCE.txt'.
******************************************************************************/

#ifndef NEQCALCAZMODIP_H
#define NEQCALCAZMODIP_H

#include "NeqTypes.h"

/**
 *******************************************************************************
 * @par Name: NeqCalcModip
 * @par Purpose:
 *   (corresponds to 'philam' in fortran and in C++)
 *   This function uses the current lat/lng to calculate the corresponding
 *   modified dip latitude (MODIP) value.
 *
 * @par Argument I/O:
 *   @param dLat     (I) The current latitude (deg)
 *   @param dLng     (I) The current longitude (deg)
 *   @param pstModip (I) Modip grid from config
 *
 * @par I/O files:
 *   None
 *
 * @return Interpolated Modip for position lat/lng (deg)
 ******************************************************************************/
double NeqCalcModip(double dLat, double dLng, MODIP_st* pstModip);

/**
 *******************************************************************************
 * @par Name: NeqModipToAz
 * @par Purpose:
 *   (corresponds to 'gflx' in fortran and in C++)
 *   This function calculates Az from the provided coefficients and modified dip
 *   latitude value (dModip). If only one coefficient (A0) is supplied then that
 *   is Az (ie. it doesn't vary with Modip).
 *
 * @par Argument I/O:
 *   @param dModip     (I) The modified dip latitude of current position
 *   @param siNumCoeff (I) The number of items in the pdCoeff array
 *   @param pdCoeff    (I) Array containing values of A0, A1, A2, etc.
 *
 * @par I/O files:
 *   None
 *
 * @return The calculated Az for given Modip
 ******************************************************************************/
double NeqModipToAz(double dModip, int siNumCoeff, double* pdCoeff);


#endif /* NEQCALCAZMODIP_H */
