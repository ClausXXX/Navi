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
#include "NeqTypes.h"
#include "NeqCalcBottomsideNe.h"
#include "NeqUtils.h"

/**
 *******************************************************************************
 * @par Name: NeqCalcBottomsideNe
 * @par Purpose:
 *   (corresponds to 'NeMdGR' in fortran and in C++)
 *   This function calculates electron content at the specified height (in the
 *   bottom part of the ionosphere below pdPeakHeight[LAYER_F2], the F2 electron
 *   density peak point).
 *
 * @par Argument I/O:
 *   @param dHH          (I) Height of the point
 *   @param ptLayers     (I) Properties for the Epstein layers
 *                           (pdAmp[all], pdPeakHeight[all],
 *                            pdBotThick[all], pdTopThick[all except F2])
 * @par I/O files:
 *   None
 *
 * @return Electron content at specified height
 ******************************************************************************/
double NeqCalcBottomsideNe(double dHH, LayerProperties_st* pstLayers)
{
  double dAN0;             /* Anchor Points */
  double dBF;              /* Thickness parameters for bottom of F2, */
  double dSum = 0.0;       /* Sumation variable*/
  double dDSum = 0.0;      /* Sumation variable*/
  double dArg;             /* temporary argument*/
  double dS0;              /* temporary variable */
  double dDS;              /* temporary variable */
  double pdB[LAYER_COUNT_SI]; /* Thickness parameters for bottom and top */
  int siLayer;             /* Loop variable for Epstein layers */
  double dZ;               /* temporary variable */
  double dEE;              /* temporary variable */
  double dReturn;          /* Return value, electron content at point */


  /* Set dB[LAYER_F2] to F2 bottom side density (as this function is only called
   * for points beneath the F2 peak) */
  pdB[LAYER_F2_SI] = pstLayers->pdBotThick[LAYER_F2_SI];

  /* Set dB[LAYER_F1] to F1 top/bottom-side density (dependent on current
   * height) */
  if (dHH > pstLayers->pdPeakHeight[LAYER_F1_SI]) {
    pdB[LAYER_F1_SI] = pstLayers->pdTopThick[LAYER_F1_SI];
  } else {
    pdB[LAYER_F1_SI] = pstLayers->pdBotThick[LAYER_F1_SI];
  }

  /* Set dB[LAYER_E] to E top/bottom-side density (dependent on current
   * height) */
  if (dHH > pstLayers->pdPeakHeight[LAYER_E_SI]) {
    pdB[LAYER_E_SI] = pstLayers->pdTopThick[LAYER_E_SI];
  } else {
    pdB[LAYER_E_SI] = pstLayers->pdBotThick[LAYER_E_SI];
  }

  /* Check if we're beneath 100km */
  if (dHH < BOTTOM_CORRN_ANCHOR_D) {
    /* We are beneath 100km */
    for (siLayer = LAYER_F2_SI; siLayer < LAYER_COUNT_SI; 
         siLayer = siLayer + 1) {
      /* Calculate dArg */
      dArg = (BOTTOM_CORRN_ANCHOR_D -
              pstLayers->pdPeakHeight[siLayer]) / pdB[siLayer];
      if (siLayer > LAYER_F2_SI) {
        dArg = dArg *
          exp(BOTTOM_CORRN_FAC1_D /
              ((BOTTOM_CORRN_FAC2_D *
                fabs(BOTTOM_CORRN_ANCHOR_D -
                     pstLayers->pdPeakHeight[LAYER_F2_SI])) + 1.0));
      } /* end if not F2 layer */

      /* Calculate dS0 and dDS */
      if (fabs(dArg) > ARG_CLIP_THRESH_D) {
        dS0 = 0.0;
        dDS = 0.0;
      } else {
        dEE = exp(dArg);
        dS0 = pstLayers->pdAmp[siLayer] * dEE / NeqSquared(dEE + 1.0);
        dDS = ((1.0 - dEE) / (1.0 + dEE)) / pdB[siLayer];
      } /* end if |dArg| > ARG_CLIP_THRESH_D */

      /* Add dS0 and (dS0 * dDS) to the summations */
      dSum = dSum + dS0;
      dDSum = dDSum + (dS0 * dDS);
    } /* end for each layer */

    dBF = 1.0 - ((dDSum / dSum) * BOTTOM_CORRN_SCALE_D);
    dAN0 = dSum * NE_SCALING_D;

    dZ = (dHH - BOTTOM_CORRN_ANCHOR_D) / BOTTOM_CORRN_SCALE_D;
    dReturn = dAN0 * NeqClipExp(1 - (dBF * dZ) - NeqClipExp(-dZ));
  } else {
    /* We are above 100km */
    for (siLayer = LAYER_F2_SI; siLayer < LAYER_COUNT_SI; 
         siLayer = siLayer + 1) {
      /* Calculate dArg */
      dArg = (dHH - pstLayers->pdPeakHeight[siLayer]) / pdB[siLayer];
      if (siLayer > LAYER_F2_SI) {
        dArg = dArg *
          exp(BOTTOM_CORRN_FAC1_D /
              ((BOTTOM_CORRN_FAC2_D *
                fabs(dHH - pstLayers->pdPeakHeight[LAYER_F2_SI])) + 1.0));
      } /* end if not N2 layer */

      /* if |dArg| > ARG_CLIP_THRESH_D */
      if (fabs(dArg) > ARG_CLIP_THRESH_D) {
         dS0 = 0.0;
      } else {
         dEE = exp(dArg);
         dS0 = pstLayers->pdAmp[siLayer] * dEE / NeqSquared(dEE + 1.0);
      } /* end if |dArg| > ARG_CLIP_THRESH_D */

      /* Add dS0 to the summation */
      dSum = dSum + dS0;
    } /* end for each layer */
    dReturn = dSum * NE_SCALING_D;
  } /* end if dHH < H0_D */

  /* Return the electron content */
  return(dReturn);
}

/* End of file: $RCSfile: NeqCalcBottomsideNe.c,v $ */
