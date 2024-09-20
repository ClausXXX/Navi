/*****************************
(c) Copyright European Space Agency, 2017
This file is subject to the terms and conditions defined in file 'LICENCE.txt', 
which is part of this [source code/executable] package. No part of the package, 
including this file, may be copied, modified, propagated, or distributed except
according to the terms contained in the file 'LICENCE.txt'.
 ******************************************************************************/

#ifndef NEQTYPES_H
#define NEQTYPES_H

#include "BasicTypes.h"
#include "Constants.h"
#include "NeqConstants.h"


/* Input data */
typedef struct {
  MODIP_st *pstModip;   /* Config: Array for Modip grid values */
  CCIR_st *pstCCIR;     /* Config: Structure containing CCIR maps */
  double *pdKronrodTol; /* Tolerances for the Kronrod integration */
  int siMaxRecurse;     /* Maximum level of integration recursion allowed */
  double *pdGssPosLLH;  /* GSS position in WGS-84 
	  ellipsoidal coordinates: geodetic latitude, geodetic longitude and ellipsoidal height.*/
  double *pdSatPosLLH;  /* Satellite position in WGS-84 
	  ellipsoidal coordinates: geodetic latitude, geodetic longitude and ellipsoidal height.*/
  int siMonth;          /* The current month (january=1, december=12) */
  double dUT;           /* The universal time (GMT) */
  int siNumCoeff;       /* Number of items in the pdCoeff array */
  double *pdCoeff;      /* Array containing the Az or a0,a1,a2... values */
  double dAzBase;       /* Az value at the receiver location - filled when NeQuick.c is called */
} NeQuickInputData_st;

typedef struct {
  double dLat;          /* Latitude (deg. N) */
  double dLng;          /* Longitude (deg. E) */
  double dH;            /* Height (km) */
  double dR;            /* Radius (km) */
  double dS;            /* Slants (km) */
  double dSinLat;       /* Sin of latitude point */
  double dCosLat;       /* Cosine of latitude point */
} SPoint_st;

typedef struct  {
  int siMonth;          /* Month for interpolated values */
  double dR12;          /* Average sunspot number for interpolated values */

  /* Interpolated foF2 (F2-peak plasma frequency) values for this month/R12
   * which is closely related to F2-peak density (NmF2) by the formula:
   *   NmF2 = 1.24x10^10 * foF2 */
  double pdF0F2[NUM_CCIR_F2_PTS_SI];

  /* Interpolated M(3000)F2 values for this month/R12 which =MUF(3000)/foF2 and
   * is a propagation factor closely related to the height of the F2-peak.
   * MUF(3000) is the highest frequency that, refracted in the ionosphere, can
   * be received at a distance of 3000km. */
  double pdM3000F2[NUM_CCIR_M3000_PTS_SI];

  double dUT;           /* Time for current Spherical Legendre coefficients */
  double pdLegCoeffs_F0[CCIR_F2_DIM_2_SI];
                        /* Spherical Legendre coeffs for calculating FoF2 */
  double pdLegCoeffs_M3000[CCIR_M3000_DIM_2_SI];
                        /* Spherical Legendre coeffs for calculating M3000F2 */
} CurrentCCIR_st;

typedef struct  {
  double pdAmp[LAYER_COUNT_SI];/* Amplitude for the Epstein function */
  double pdPeakHeight[LAYER_COUNT_SI];
                            /* Hm, height of the electron density peak for the
                             * F2, F1 and E layers respectively */
  double pdBotThick[LAYER_COUNT_SI];
                            /* Thickness parameters for bottom of F2, F1 and E
                             * layers respectively */
  double pdTopThick[LAYER_COUNT_SI];
                            /* Thickness parameters for top of F2, F1 and E
                             * layers respectively */
  double dM3000;            /* Current M(3000)F2 */
  double pdF0[LAYER_COUNT_SI]; /* Current fo (peak plasma frequency) for the
                                * F2, F1and E layers respectively */
} LayerProperties_st;

typedef struct  {
  SPoint_st  stP1;      /* Positional information (km) at point P2 */
  SPoint_st  stP2;      /* Positional information (km) at point P2 */ 
  SPoint_st  stRay;     /* Coords of the ray perigee (only dLat,dLng,dR used) */
  SPoint_st  stPactual; /* Only lat,lng,h used */
  double    dZeta;      /* Zenith angle of point 2 seen from point 1 (deg)
                         * ie. angle from vertical to point 2. */
  double    dSinDelta;  /* sine of declination of sun */
  double    dCosDelta;  /* cosine of declination of sun */
  double    dSinSig;    /* Sine of ray azimuth */
  double    dCosSig;    /* Cosine of ray azimuth */
                        /* (Point 2 seen from point 1) (N over E to S) */
} GeometryData_st;

typedef struct  {
  NeQuickInputData_st *pstNeQuickInputData; /* NeQuick input data */
  GeometryData_st* pstGeom; /*Geometry Data */
  CurrentCCIR_st* pstCurrCCIR;  /* Current data containing CCIR maps */
  double dTolerance; /* Tolerance for the integration */
  Boolean_b bVert;  /* Flag indicating vertical (TRUE) or slanted (FALSE) ray */
} IntegrateData_st;

#endif /* NEQTYPES_H */
