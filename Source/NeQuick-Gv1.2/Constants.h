/*****************************
(c) Copyright European Space Agency, 2017
This file is subject to the terms and conditions defined in file 'LICENCE.txt', 
which is part of this [source code/executable] package. No part of the package, 
including this file, may be copied, modified, propagated, or distributed except
according to the terms contained in the file 'LICENCE.txt'.
*******************************************************************************/

#ifndef CONSTANTS_H
#define CONSTANTS_H

#ifndef FALSE
#define FALSE (0)
#endif
#ifndef TRUE
#define TRUE (1)
#endif

/** Function returned type */
typedef enum  {
  E_OK = 0,   /**< No error */
  E_ERROR     /**< Error    */
} ReturnValue_e;

/** Constants for integer numbers */
#define ZERO_SI  (0)
#define ONE_SI   (1)
#define TWO_SI   (2)

/** Value of mathematical constant PI*2 */
#define TWOPI_D      (6.283185307179586476925287)         

/** Value of mathematical constant PI */
#define PI_D         (TWOPI_D/2.0)

/** Value of mathematical constant PI/2 */
#define HALFPI_D     (PI_D/2.0)  

/* Degrees to radians */
#define DEG_TO_RAD_D (PI_D/180.0)

/** Number of months of a year */
#define MONTHS_SI (12) 

/**-------------------------------------------------------
 *  Constants related to CCIR and MODIP files.
 * ----------------------------------------------------- */

/** First dimension of parameters in MODIP files */
#define NUM_MODIP_DIM_1_SI (39)

/** Second dimension of parameters in MODIP files */
#define NUM_MODIP_DIM_2_SI (39)

/** First dimension of CCIR F2 parameters */
#define CCIR_F2_DIM_1_SI (13)

/** Second dimension of CCIR F2 parameters */
#define CCIR_F2_DIM_2_SI (76)

/** Third dimension of CCIR F2 parameters */
#define CCIR_F2_DIM_3_SI (2)

/** First dimension of CCIR M3000 parameters */
#define CCIR_M3000_DIM_1_SI (9)

/** Second dimension of CCIR M3000 parameters */
#define CCIR_M3000_DIM_2_SI (49)

/** Third dimension of CCIR M3000 parameters */
#define CCIR_M3000_DIM_3_SI (2)


#endif /* CONSTANTS_H */

/* End of file */
