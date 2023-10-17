#ifndef EPHEMERIS_H
#define EPHEMERIS_H

#include "DataFiles.h"
#include "Solution.h"
#include "RINEX.h"
#include "SP3.h"

#define COS_MINUS_5_DEG 0.99619469809
#define SIN_MINUS_5_DEG -0.08715574274

enum Ephemeris
{
	BOARD,
	PRECISE
};

void FindRINEXEphemeris(struct Settings*, struct RINEXObs*,
						struct RINEXNav*, struct Sattelite*);
void FindSP3Ephemeris(struct Settings*, struct RINEXObs*, struct SP3*,
					  struct Sattelite*, struct InterpolationPoints*);
void GPSSatteliteXV(struct Sattelite*, char Velocity);
void GLODiffEqCoefficients(double*, double*, double*, double);
void GLOSatteliteXV(struct Sattelite*);

#endif