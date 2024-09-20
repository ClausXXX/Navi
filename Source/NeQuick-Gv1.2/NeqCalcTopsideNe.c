/*****************************
(c) Copyright European Space Agency, 2017
This file is subject to the terms and conditions defined in file 'LICENCE.txt', 
which is part of this [source code/executable] package. No part of the package, 
including this file, may be copied, modified, propagated, or distributed except
according to the terms contained in the file 'LICENCE.txt'.
 ******************************************************************************/

/* System Includes */
#include <math.h>

/* NeQuick Includes */
#include "NeqCalcTopsideNe.h"
#include "NeqTypes.h"
#include "NeqConstants.h"
#include "NeqCalcBottomsideNe.h"
#include "NeqUtils.h"

/**
 *******************************************************************************
 * @par Name: NeqCalcTopsideNe
 * @par Purpose:
 *   (corresponds to 'topq' in fortran and in C++)
 *   This function calculates electron content at the specified height (in the
 *   top part of the ionosphere above pdPeakHeight[LAYER_F2], the F2 electron
 *   density peak point).
 *
 * @par Argument I/O:
 *   @param dH       (I)   Height of the point
 *   @param ptLayers (I)   Properties for all the Epstein layers
 *                         (pdAmp, pdPeakHeight, pdBotThick, pdTopThick) 
 *   @param pdNmax   (I/O) Electron content at the F2 peak
 *                         In: if already set, Out: if not set before
 *
 * @par I/O files:
 *   None
 *
 * @return Electron content at specified height
 ******************************************************************************/
double NeqCalcTopsideNe(double dH, LayerProperties_st* pstLayers, 
                        double* pdNmax)
{
  double dDeltaH;  /* Height above F2 Peak */
  double dGDeltaH; /* G_D * dDeltaH */
  double dEE;      /* temporary variable */
  double dEP;      /* temporary variable */
  double dReturn;  /* Return value, electron content at point */


  /* Calculate dEE. N.B. The formula quoted in 'Radicella & Leitinger' 2001 is
   * incorrect, i.e. ... ((1.0 + R_FAC_D * dGDeltaH) / ...) */
  dDeltaH = dH - pstLayers->pdPeakHeight[LAYER_F2_SI];
  dGDeltaH = TOPSIDE_CORRN1_D * dDeltaH;
  dEE =
    NeqClipExp(dDeltaH /
               (pstLayers->pdTopThick[LAYER_F2_SI] *
                (1.0 +
                 ((TOPSIDE_CORRN2_D * dGDeltaH) /
                  ((TOPSIDE_CORRN2_D * pstLayers->pdTopThick[LAYER_F2_SI]) +
                   dGDeltaH)))));

  /* Check if dEE is large */
  if (dEE > LARGE_EE_D) {
    /* dEE is large so can approximate the formula for ep
     * from         4.dEE       to    4
     *         ---------------      -----
     *          (dEE + 1) ^ 2        dEE  */
    dEP = EPSTNORMALISATION_D / dEE;
  } else {
    /* dEE is not large to allow approximation so use full formula */
    dEP = (EPSTNORMALISATION_D * dEE) / NeqSquared(dEE + 1.0);
  }

  /* fortran doesn't do this [see 'NeqGetNeOnSlantRay' C function] */
  if (fabs(*pdNmax + 1.0) < UNSET_NMAX_TOL_D) {
    *pdNmax = NeqCalcBottomsideNe(pstLayers->pdPeakHeight[LAYER_F2_SI],
                                  pstLayers);
  }

  /* Calculate the electron content */
  dReturn = *pdNmax * dEP;

  /* Return the electron content */
  return(dReturn);
}

/* End of file: $RCSfile: NeqCalcTopsideNe.c,v $ */
