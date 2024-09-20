/*****************************
(c) Copyright European Space Agency, 2017
This file is subject to the terms and conditions defined in file 'LICENCE.txt', 
which is part of this [source code/executable] package. No part of the package, 
including this file, may be copied, modified, propagated, or distributed except
according to the terms contained in the file 'LICENCE.txt'.
*******************************************************************************/


#ifndef BASICTYPES_H
#define BASICTYPES_H


#include "Constants.h"

/** Boolean_b type.
 *  Type representing a boolean value. A boolean variable can take two
 *  possible values: TRUE or FALSE.
 */
typedef unsigned short Boolean_b;


/**  MODIP file
*   map of dip latitude
*/
typedef struct {
 /** A grid point map of dip latitude
  * Range:-90.0...+90.0
  * Units: degress
  */
  double pdModip[NUM_MODIP_DIM_1_SI][NUM_MODIP_DIM_2_SI];
} MODIP_st;


/**  CCIR file
*/
typedef struct {
 /** F2 ionogram parameters for Nequick model
  *  Range: NA
  *  Units: NA
  */
  double pdF2[MONTHS_SI][CCIR_F2_DIM_1_SI]
             [CCIR_F2_DIM_2_SI][CCIR_F2_DIM_3_SI];

  /** M3000 ionogram parameters for Nequick model
  *   Range: NA
  *   Units: NA
  */
  double pdM3000[MONTHS_SI][CCIR_M3000_DIM_1_SI]
                [CCIR_M3000_DIM_2_SI][CCIR_M3000_DIM_3_SI];
} CCIR_st;


#endif /* BASICTYPES_H */

/* End of file */
