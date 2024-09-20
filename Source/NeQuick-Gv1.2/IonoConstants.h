/*****************************
(c) Copyright European Space Agency, 2017
This file is subject to the terms and conditions defined in file 'LICENCE.txt', 
which is part of this [source code/executable] package. No part of the package, 
including this file, may be copied, modified, propagated, or distributed except
according to the terms contained in the file 'LICENCE.txt'.
 ******************************************************************************/

#ifndef IONOCONSTANTS_H
#define IONOCONSTANTS_H

/** Constants for floating point numbers */
#define TWO_D (2.0) 
#define THREE_D (3.0)
#define NINE_D (9.0)
#define SIXTEEN_D (16.0)

#define MAX_PERCENTAGE_D (100.0) /* Value of 100 procent */

#define CIRCLEDEG_D (360.0)                   /* Degrees in a cirle */
#define HALFCIRCLEDEG_D (CIRCLEDEG_D / TWO_D) /* Degrees in half a circle */
#define TWOCIRCLEDEG_D (CIRCLEDEG_D * TWO_D)  /* Degreeds in two circles */

#define DEGPERHOUR_D (15.0) /* Number of degrees per hour (deg) */
#define SAFE_GAMMA_D (10.0) /* Save gamma value */


#define CONVSTEC_D (40.3) /* Value for convertion of delay in metres to TEC  */

#define MAX_OBSSTEC_EPOCHS_SI (300) /* Maximum number of Epoch Observations*/
#define MAX_OBSSTEC_OBS_SI (MAX_OBSSTEC_EPOCHS_SI * MAX_SIV_ONE_GSS_SI)
                                /* Maximum number of Observations*/

#define COORD_EAST_SI (0)       /* East-coordinate value */
#define COORD_NORTH_SI (1)      /* North-coordinate value */
#define COORD_UP_SI (2)         /* Up-coordinate value */
#define NUMCOORDS_ENU_SI (3)    /* Total number of coordinates */

#define NUMCOORDS_XYZ_SI (3)    /* Total number of coordinates */

#define COORD_LAT_SI (0)        /* Latitude value */
#define COORD_LNG_SI (1)        /* Longitude value */
#define COORD_H_SI (2)          /* Height value */
#define NUMCOORDS_LLH_SI (3)    /* Total number of coordinates */

#define WEEKDAYS_SI (7)         /* Total number of weekdays */
#define DAYHOURS_SI (24)        /* Integer number of hours in a day */
#define DAYHOURS_D (24.0)       /* Floating point number of hours in a day */

#define SEMAXE_D  (6378136.6)   /* Semi-major axis of Earth (m) */
#define FLATTE_D  (1.0 / 298.25642)  /* Earths Flattening factor */
#define CUBEPOWER_D (3.0)       /* Raise to power 3 i.e. cubed */

#define GAL_START_DAY_SI  (234)   /* Galileo start time day of year*/
#define GAL_START_YEAR_SI (1999)  /* Galileo start time year */

#define DAYS_IN_NONLEAPYEAR_SI (365)  /* Number of days in non-leap year */

#define YEARS_1_SI   (1) /* 1 year */
#define YEARS_4_SI   (4) /* 4 years */
#define YEARS_100_SI (100) /* 100 years */
#define YEARS_400_SI (400) /* 400 years */

/* Number of days in each month of non-leap year */
#define NUMDAYS_JAN_SI (31) /* Number of days in January */
#define NUMDAYS_FEB_SI (28) /* Number of days in Febuary (non leap year) */
#define NUMDAYS_MAR_SI (31) /* Number of days in March */
#define NUMDAYS_APR_SI (30) /* Number of days in April */
#define NUMDAYS_MAY_SI (31) /* Number of days in May */
#define NUMDAYS_JUN_SI (30) /* Number of days in June */
#define NUMDAYS_JUL_SI (31) /* Number of days in July */
#define NUMDAYS_AUG_SI (31) /* Number of days in August */
#define NUMDAYS_SEP_SI (30) /* Number of days in September */
#define NUMDAYS_OCT_SI (31) /* Number of days in October */
#define NUMDAYS_NOV_SI (30) /* Number of days in November */
#define NUMDAYS_DEC_SI (31) /* Number of days in December */

#define LEAPYEAR_EVERY_SI  (YEARS_4_SI)   /* Nominal Frequency of leap-years */
#define LEAPYEAR_EXCEPT_SI (YEARS_100_SI) /* Exception leap year rule (multiple
                                           * 100 ) */ 
#define LEAPYEAR_ALSO_SI   (YEARS_400_SI) /* Include multiple of 400 in leap 
                                           * year */

#define RANGE_LATDEG_MIN_D (-90.0)  /* Minimum latitude in degrees */
#define RANGE_LATDEG_MAX_D  (90.0)  /* Maximum latitude in degrees */
#define RANGE_LONDEG_MIN_D (-180.0) /* Minimum longitude in degrees */
#define RANGE_LONDEG_MAX_D  (180.0) /* Maximum longitude in degrees */
#define MODIP_DIFF_THRES_D   (0.1)  /* Modip Difference Threshold */

#define MODIP_LAT_95N_SI       (0) /* Modip value at Latitude 95 deg N */
#define MODIP_LAT_90N_SI       (1) /* Modip value at Latitude 90 deg N */
#define MODIP_LAT_85N_SI       (2) /* Modip value at Latitude 85 deg N */
#define MODIP_LAT_85S_SI      (36) /* Modip value at Latitude 85 deg S */
#define MODIP_LAT_90S_SI      (37) /* Modip value at Latitude 90 deg S */
#define MODIP_LAT_95S_SI      (38) /* Modip value at Latitude 95 deg S */

#define MODIP_LNG_190W_SI      (0) /* Modip value at Longitude 190 deg W*/
#define MODIP_LNG_180W_SI      (1) /* Modip value at Longitude 180 deg W*/
#define MODIP_LNG_170W_SI      (2) /* Modip value at Longitude 170 deg W*/
#define MODIP_LNG_0_SI        (19) /* Modip value at Longitude 0 deg */
#define MODIP_LNG_170E_SI     (36) /* Modip value at Longitude 170 deg E*/
#define MODIP_LNG_180E_SI     (37) /* Modip value at Longitude 180 deg E*/
#define MODIP_LNG_190E_SI     (38) /* Modip value at Longitude 190 deg E*/

#define MODIP_LNG_180DEG_SI   (18) /* Modip value at Longitude 180 */
#define MODIP_LNG_360DEG_SI   (36) /* Modip value at Longitude 360 */

#define RADTODEG_D (180.0 / PI_D) /* Value of constant required to convert */
                                  /* from radians to degrees */
#define LON_STEP_SI (10)  /* Size of longitude step in Modip grid */
#define NUM_LONS_SI (36)  /* Number of unique longitudes in Modip map */
#define LAT_STEP_SI (5)   /* Size of latitude step in Modip grid */

#define INT_PT0_SI (0)        /* Interpolation point 0 */
#define INT_PT1_SI (1)        /* Interpolation point 1 */
#define INT_PT2_SI (2)        /* Interpolation point 2 */
#define INT_PT3_SI (3)        /* Interpolation point 3 */
#define NUM_INT_PTS_SI (4)    /* Number of points used for interpolation */

#define MIN_DELTAX_D (1e-10)  /* Minimum value for delta X */

#define TECU_D (1.0e+16)      /* Total Electron Content Unit */

#endif /* IONOCONSTANTS_H */

/* End of file: $RCSfile: IonoConstants.h,v $ */
