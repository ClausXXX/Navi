/*****************************
(c) Copyright European Space Agency, 2017
This file is subject to the terms and conditions defined in file 'LICENCE.txt', 
which is part of this [source code/executable] package. No part of the package, 
including this file, may be copied, modified, propagated, or distributed except
according to the terms contained in the file 'LICENCE.txt'.
 ******************************************************************************/

#ifndef NEQCALCTOPSIDENE_H
#define NEQCALCTOPSIDENE_H


/* System Includes */

/* NeQuick Includes */
#include "NeqTypes.h"


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
                        double* pdNmax);


#endif /* NEQCALCTOPSIDENE_H */
