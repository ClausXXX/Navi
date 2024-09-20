/*****************************
(c) Copyright European Space Agency, 2017
This file is subject to the terms and conditions defined in file 'LICENCE.txt', 
which is part of this [source code/executable] package. No part of the package, 
including this file, may be copied, modified, propagated, or distributed except
according to the terms contained in the file 'LICENCE.txt'.
 ******************************************************************************/

#ifndef NEQCALCBOTTOMSIDENE_H
#define NEQCALCBOTTOMSIDENE_H

/* System Includes */
#include <math.h>

/* NeQuick Includes */
#include "NeqTypes.h"


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
 *   @param pstOnLineCfg (I) Pointer to the OnLine Configuration data
 *
 * @par I/O files:
 *   None
 *
 * @return Electron content at specified height
 ******************************************************************************/
double NeqCalcBottomsideNe(double dHH, LayerProperties_st* pstLayers);

#endif /* NEQCALCBOTTOMSIDENE_H */
