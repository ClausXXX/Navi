#ifndef IONOSPHERE_H
#define IONOSPHERE_H

struct GLOIon
{
	double hmax, A, Bbot, Btop;
};

struct GEMTECData
{
	short evt[4][13], evm[4][4][13], evl[4][4][5][15], evf[4][4][5][2][2],
		  A[4][4][5][2][13];
};

//double UTCToGPSTime(short, char, char, char, char, float);
double UTCToMJD(short, char, char, char, char, float);
double Klobuchar(double, double, double, double, double, double*, double*);
double KlobMapping(double);
int GLOIonosphere(float, char, double, double, double, double, double, double*,
				   double*, double*, double*);
double Ne(double, double, double, double, double);
double SimpleTECv(double, double, double);
double IntegralTECv(double(*)(double, double, double, double, double), double,
					double, double, double, double, double, double);
double GLOMapping(double, double, double, double, double);
double BDGIM(short, char, char, char, char, float,
			 double, double, double, double, double(*)[9]);
float GEMTEC(float, float, float, float, int, int,
			  short (*)[13], short (*)[4][13], short (*)[4][5][15],
			  short (*)[4][5][2][2], short (*)[4][5][2][13]);
double IonoMapping(double, double, double);
double IonoMappingT(double, double, double, double);
double BDSTimeToMJD(int, double);

#endif
