/*****************************
(c) Copyright European Space Agency, 2017
This file is subject to the terms and conditions defined in file 'LICENCE.txt', 
which is part of this [source code/executable] package. No part of the package, 
including this file, may be copied, modified, propagated, or distributed except
according to the terms contained in the file 'LICENCE.txt'.
 ******************************************************************************/

#ifndef NEQINTEGRATE_H
#define NEQINTEGRATE_H

/* System Includes */

/* NeQuick Includes */
#include "NeqTypes.h"


/**
 *******************************************************************************
 * @par Name: NeqIntegrate
 * @par Purpose:
 *   (replaces Gaussian integration implemented in 'gint' and 'gintv' in fortran
 *    and gintg7k15ad in C++)
 *
 *   This function performs intgeration to calculate the TEC along rays using
 *   the Kronrod G7-K15 adaptive quadrature method (see Knezevich and Radicella
 *   paper).
 *
 *   This method involves sampling values at 15 points and calculating the
 *   integration from them. At the same time it misses out half of the points to
 *   see what difference it makes and therefore the likely error contained in
 *   the result, before deciding whether to accept the result, or to split the
 *   portion into two and try again in order to improve accuracy.
 *
 *   This function is recursive and an appropriate safeguards has been
 *   implemented to prevent infinite looping.
 *
 *   Interestingly, when an integration is split into two equal parts the
 *   tolerance isn't halved. So, the accuracy of the result is +- (2*dTolerance)
 *   
 *
 * @par Argument I/O:
 *   @param pstIntegrateData  (I) Structure containing readonly NeQuick inputs
 *   @param dH1               (I) Height of point 1
 *   @param dH2               (I) Height of point 2
 *   @param siCurrLevel       (I) Current level of integration recursion
 *   @param pstPactual        (O) dLat/dLng/dH updated for current position
 *                                along slant
 *   @param pdNmax            (O) Electron content at F2 peak
 *   @param pstLayers         (O) Current ionospheric properties (see
 *                                NeqCalcEpstParams)
 *
 * @par I/O files:
 *   None
 *
 * @return Result of numerical integration
 ******************************************************************************/
double NeqIntegrate( IntegrateData_st * pstIntegrateData, double dH1, 
  double dH2, int siCurrLevel, SPoint_st* pstPactual, double* pdNmax,
  LayerProperties_st* pstLayers);

#endif /* NEQINTEGRATE_H */
