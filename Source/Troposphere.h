#ifndef TROPOSPHERE_H
#define TROPOSPHERE_H

struct Weather
{
    double p, t, T, e, r;
};

double PrimitiveTrop(double);
double PrimitiveMapping(double, double, double);
double ModifiedPrimitiveTrop(double);
double ModifiedPrimitiveMapping(double, double, double);
double GCAT(double, double);
double GCATZ(double, double, double*, double*);
double BlackEisnerMapping(double, double, double);
void StandartAtmosphere(double, struct Weather*);
double Hopfield(double, struct Weather*);
void HopfieldZ(struct Weather*, double*, double*);
double HopfieldMapping(double, double, double);
double Black(char, double, double, double, struct Weather*);
void BlackZ(char, double, struct Weather*, double*, double*);
double BlackMapping(double, double, double, double, struct Weather *Weather);
double Saastamoinen(double, double, struct Weather*);
double SaastamoinenDavis(double, double, double, struct Weather*);
double SaastamoinenDavisZ(double, double, struct Weather*, double*, double*);
double CfA22Mapping(double, double, double, struct Weather*);
double SaastamoinenNeil(int, double, double, double, struct Weather*);
double NeilMapping(double, double, int, double, double, double, struct Weather*);
double ChaoMapping(double, double, double);
double IfadisMapping(double, double, double, struct Weather*);

#endif
