#ifndef TROPOSPHERE_H
#define TROPOSPHERE_H

struct Weather
{
    double p, t, T, e, r;
};

double PrimitiveTrop(double);
double ModifiedPrimitiveTrop(double);
double GCAT(double, double);
double Hopfield(double, struct Weather*);
void StandartAtmosphere(double, struct Weather*);
double Saastamoinen(double, double, struct Weather*);
double SaastamoinenDavis(double, double, double, struct Weather*);
double SaastamoinenNeil(int, double, double, double, struct Weather*);

#endif
