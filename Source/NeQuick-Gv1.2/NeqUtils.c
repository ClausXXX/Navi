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
#include "NeqUtils.h"

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
double NeqClipExp(double dPower)
{
  double dReturn; /* Return value, clipped value of exponential */


  /* Check if dPower is outside the limits and set dReturn accordingly */
  if (dPower > CLIPEXP_CUTOFF_D) {
    dReturn = CLIPEXP_HIGH_D;
  } else if (dPower < -CLIPEXP_CUTOFF_D) {
    dReturn = CLIPEXP_LOW_D;
  } else {
    dReturn = exp(dPower);
  }

  /* Return the result */
  return(dReturn);
}


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
double NeqJoin(double dF1, double dF2, double dAlpha, double dX)
{
  double dTemp; /* Intermediate e ^ (dAlpha * dX) term (via NeqClipExp) */


  /* Calculate and return the value */
  dTemp = NeqClipExp(dAlpha * dX);
  return(((dF1 * dTemp) + dF2) / (dTemp + 1.0));
}


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
double NeqSquared(double dValue)
{
  return(dValue * dValue);
}

/* End of file: $RCSfile: NeqUtils.c,v $ */
