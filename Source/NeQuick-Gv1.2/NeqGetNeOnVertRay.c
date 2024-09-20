/*****************************
(c) Copyright European Space Agency, 2017
This file is subject to the terms and conditions defined in file 'LICENCE.txt', 
which is part of this [source code/executable] package. No part of the package, 
including this file, may be copied, modified, propagated, or distributed except
according to the terms contained in the file 'LICENCE.txt'.
 ******************************************************************************/

/* System Includes */

/* NeQuick Includes */
#include "NeqGetNeOnVertRay.h"
#include "NeqCalcTopsideNe.h"
#include "NeqCalcBottomsideNe.h"

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
                         LayerProperties_st* pstLayers, double* pdNmax)
{
  double dReturn; /* Return value, electron content at point */


  /* Check if the current point is above the F2 electron density peak point */
  if (dH > pstLayers->pdPeakHeight[LAYER_F2_SI]) {
    /* Point is above F2 peak, so call NeqCalcTopsideNe */
    dReturn = NeqCalcTopsideNe(dH, pstLayers, pdNmax);
  } else {
    /* Point is at or below F2 peak, so call NeqCalcBottomsideNe */
    dReturn = NeqCalcBottomsideNe(dH, pstLayers);
  } /* end if above F2 layer peak */

  /* Return the electron content */
  return(dReturn);
}

/* End of file: $RCSfile: NeqGetNeOnVertRay.c,v $ */
