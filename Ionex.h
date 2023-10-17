#ifndef IONEX_H
#define IONEX_H

#include <time.h>

#define PI          3.1415926535897932  /* pi */
#define D2R         (PI/180.0)          /* deg to rad */
#define R2D         (180.0/PI)          /* rad to deg */
#define CLIGHT      299792458.0         /* speed of light (m/s) */

#define MAXEXFILE   1024
#define MAXSAT 63

#define FREQ1       1.57542E9           /* L1/E1  frequency (Hz) */

typedef struct {        /* time struct */
	time_t time;        /* time (s) expressed by standard time_t */
	double sec;         /* fraction of second under 1 s */
} gtime_t;

typedef struct {        /* TEC grid type */
    gtime_t time;       /* epoch time (GPST) */
    int ndata[3];       /* TEC grid data size {nlat,nlon,nhgt} */
    double rb;          /* earth radius (km) */
    double lats[3];     /* latitude start/interval (deg) */
    double lons[3];     /* longitude start/interval (deg) */
    double hgts[3];     /* heights start/interval (km) */
    double *data;       /* TEC grid data (tecu) */
    float *rms;         /* RMS values (tecu) */
} tec_t;

typedef struct {        /* navigation data type */
	int nt,ntmax;       /* number of tec grid data */
	tec_t *tec;         /* tec grid data */
    double cbias[MAXSAT][3]; /* satellite dcb (0:p1-p2,1:p1-c1,2:p2-c2) (m) */

} nav_t;

gtime_t epoch2time(const double *ep);
void readtec(const char *file, nav_t *nav, int opt);
int iontec(gtime_t time, const nav_t *nav, const double *pos,
				  const double *azel, int opt, double *delay, double *var);

#endif
