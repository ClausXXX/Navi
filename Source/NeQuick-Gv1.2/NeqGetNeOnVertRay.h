/*****************************
(c) Copyright European Space Agency, 2017
This file is subject to the terms and conditions defined in file 'LICENCE.txt', 
which is part of this [source code/executable] package. No part of the package, 
including this file, may be copied, modified, propagated, or distributed except
according to the terms contained in the file 'LICENCE.txt'.
 ******************************************************************************/

#ifndef NEQGETNEONVERTRAY_H
#define NEQGETNEONVERTRAY_H

/* System Includes */

/* NeQuick Includes */
#include "NeqTypes.h"


/**
 *******************************************************************************
 * @par Name: NeqGetNeOnVertRay
 * @par Purpose:
 *   (corresponds to 'vert' in fortran and in C++)
 *   This function calculates the electron content at a specified point along a
 *   vertical ray.
 *
 * @par Argument I/O:
 *   @param dH       (I) Height of the point (km)
 *   @param ptLayers (I) Properties for all the Epstein layers
 *                       (pdAmp, pdPeakHeight, pdBotThick, pdTopThick for all)
 *   @param pdNmax   (I/O) Electron content at F2 peak
 *                         In: if already set
 *                         Out: if not set before
 *
 * @par I/O files:
 *   None
 *
 * @return Electron content at specified height
 ******************************************************************************/
double NeqGetNeOnVertRay(double dH,
                         LayerProperties_st* pstLayers, double* pdNmax);


#endif /* NEQGETNEONVERTRAY_H */
