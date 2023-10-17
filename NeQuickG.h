#ifndef NEQUICKCOMMANDS_H
#define NEQUICKCOMMANDS_H

#include "NeQuick-Gv1.2\\NeQuick.h"

/* Data files used by NeQuick */
#define MODIPFILE "NeQuickG\\modip2001_wrapped.asc"
#define CCIRFILEPATTERN "NeQuickG\\ccir%d.asc"

/* NeQuick configuration options */
#define KRONROD_TOL_1 (0.001)
#define KRONROD_TOL_2 (0.01)
#define RECURSION_LIMIT (50)

MODIP_st* loadmodipgrid(char *pchModipFile, MODIP_st *pstModip);
CCIR_st* loadccirdata(char *pchCcirFilePattern, CCIR_st *pstCCIR);
void performtest(int siMonth, double dTime,
                        double pdRecvLLHdeg[3], double pdSatLLHdeg[3],
                        double pdCoeffs[3], MODIP_st *pstModip, CCIR_st *pstCCIR,
                        double *pdModip, double *pdStec);
#endif

