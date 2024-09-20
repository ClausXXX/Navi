/*****************************
(c) Copyright European Space Agency, 2017
This file is subject to the terms and conditions defined in file 'LICENCE.txt', 
which is part of this [source code/executable] package. No part of the package, 
including this file, may be copied, modified, propagated, or distributed except
according to the terms contained in the file 'LICENCE.txt'.
 ******************************************************************************/

#ifndef NEQUTILS_H
#define NEQUTILS_H

/* System Includes */

/* NeQuick Includes */
#include "NeqTypes.h"

/**
 *******************************************************************************
 * @par Name: NeqClipExp
 * @par Purpose:
 *   (corresponds to 'fexp' in fortran and in C++)
 *   This function is a clipped exponential function. It always returns a valid
 *   double.
 *
 * @par Argument I/O:
 *   @param dPower (I) Power for exponential
 *
 * @par I/O files:
 *   None
 *
 * @return the clipped exponential value
 ******************************************************************************/
double NeqClipExp(double dPower);


/**
 *******************************************************************************
 * @par Name: NeqJoin
 * @par Purpose:
 *   (corresponds to 'djoin' in fortran and in C++)
 *   This function calculates:
 *
 *                 (dAlpha * dX)
 *         (dF1 * e             ) + dF2
 *         ----------------------------
 *               (dAlpha * dX)
 *              e              + 1
 *
 *   where the exponential terms are clipped to valid values by using
 *   NeqClipExp.
 *
 * @par Argument I/O:
 *   @param dF1    (I) A term in the exponential shown above
 *   @param dF2    (I) A term in the exponential shown above
 *   @param dAlpha (I) A term in the exponential shown above
 *   @param dX     (I) A term in the exponential shown above
 *
 * @par I/O files:
 *   None
 *
 * @return The value of the exponential above
 ******************************************************************************/
double NeqJoin(double dF1, double dF2, double dAlpha, double dX);


/**
 *******************************************************************************
 * @par Name: NeqSquared
 * @par Purpose:
 *   This function calculates the square of a number.
 *
 * @par Argument I/O:
 *   @param dValue (I) Value to be squared
 *
 * @par I/O files:
 *   None
 *
 * @return The square of the value
 ******************************************************************************/
double NeqSquared(double dValue);

#endif /* NEQUTILS_H */
