#ifndef TROPOSPHERE_H
#define TROPOSPHERE_H

struct Weather
{
    double p, t, T, e, r;
};

double GCAT(double, double);
void StandartAtmosphere(double, struct Weather*);
double Saastamoinen(double, double, struct Weather*);
double SaastamoinenDavis(double, double, double, struct Weather*);

#endif
