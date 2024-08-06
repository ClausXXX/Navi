#include "Troposphere.h"

#include <math.h>

#include "Constants.h"
#include "Common.h"

// Простейшая тропосферной задержки
// Подходит только для El >= 15°
// El - угол возвышения спутника над горизонтом, рад
// Возвращаемое значение - тропосферная задержка, м
//
// Антонович К.М. Использование спутниковых радионавигационных систем
// в геодезии: в 2 т. – М.: Картгеоцентр, 2005. – Т. 1.  – 334 с.
//
double PrimitiveTrop(double El)
{
	return 2.41 / sin(El);
}

// Простейшая тропосферной задержки, модифицированная в части функции отображения
// El - угол возвышения спутника над горизонтом, рад
// Возвращаемое значение - тропосферная задержка, м
//
// James Bao-Yen Tsui Fundamentals of Global Positioning System Receivers:
// A Software Approach // John Wiley & Sons Limited, 2005
//
double ModifiedPrimitiveTrop(double El)
{
	return 2.47 / (0.0121 + sin(El));
}

// Модель тропосферной задержки GCAT (GPSCode Analysis Tool)
// Функция отображения использована [Black and Eisner, 1984] для
// El >= 5°
// H - высота приёмника над уровнем моря, км
// El - угол возвышения спутника над горизонтом, рад
// Возвращаемое значение - тропосферная задержка, м
//
// M. Hernandez-Pajares, GPS data processing: code and phase Algorithms,
// Techniques and Recipes, Barcelona, 2005
//
double GCAT(double H, double El)
{
	double Zd, Zw;
	if(H < -5.0 || H > ht)
	{
		return 0.0;
	}
	Zd = 2.3 * exp(-0.116E-3 * H);
	Zw = 0.1;
	return (Zd + Zw) * 1.001 / sqrt(0.002001 + sqr(sin(El)));
}

// Модель параметров Международной Стандартной атмосферы
// H - высота над уровнем моря, м
// *Weather - указатель на структуру с рассчитываемыми переменными:
// p - атмосферное давление, мбар
// T - температура, К
// e - парциальное давление водяного пара, мбар
void StandartAtmosphere(double H, struct Weather *Weather)
{
	if(H >= -5000.0 && H <= ht * 1.0E+3)
	{
		Weather->p = p0 * pow((1.0 - 2.2557E-5 * H), 5.2568);
		Weather->T = t0 + Beta * 1.0E-3 * H + 273.15;
		Weather->e = 6.108 * r0 / 100.0 * exp((17.15 * Weather->T - 4684.0) /
					 (Weather->T - 38.45));
	}
	else
	{
		Weather->p = p0;
		Weather->t = t0;
		Weather->T = Weather->t + 273.15;
		Weather->e = 8.0;
	}

}

// Модель тропосферной задержки Х. Хопфилд
// El - угол возвышения спутника над горизонтом, рад
// *Weather - указатель на структуру с переменными:
// p - атмосферное давление, мбар
// T - температура, К
// e - парциальное давление водяного пара, мбар
// Возвращаемое значение - значение тропосферной задержки, м
//
// Hopfield H. S. Two-Quartic Tropospheric Refractivity Profile for Correcting
// Satellite Data //Journal of Geophysical Research. 1969. 18: Vol. 74. P. 4487–4499
//
// Красильников М. Н. Современные информационные технологии в задачах навигации
// и наведения беспилотных маневренных летательных аппаратов / Под. ред.
// М. Н. Красильникова, Г. Г. Серебрякова. М.: ФИЗМАТЛИТ, 2009. 556 с.
//
double Hopfield(double El, struct Weather *Weather)
{
	double Zd, Zw;
	Zd = 1.0E-6 / 5.0 * 77.64 / sin(sqrt(sqr(El) + 1.904E-3)) *
		 Weather->p / Weather->T * (40136.0 + 148.72 * (Weather->T - 273.16));
	Zw = 1.0E-6 / 5.0 * (-12.96 * Weather->T + 3.718E+5) /
		 sin(sqrt(sqr(El) + 0.6854E-3)) * (Weather->e / sqr(Weather->T)) * 11000.0;
	return (Zd + Zw);
}

// Модель тропосферной задержки Ю. Саастамойнена
// H - высота приёмника над уровнем моря, км
// El - угол возвышения спутника над горизонтом, рад
// *Weather - указатель на структуру с переменными:
// p - атмосферное давление, мбар
// T - температура, К
// e - парциальное давление водяного пара, мбар
// Возвращаемое значение - значение тропосферной задержки, м
//
// Saastamoinen J. Int. Symp. on the Use of Artificial Satellite // Atmospheric
// Correction for the Troposphere and Stratosphere in Radio Ranging of Satellite. Washington, 1971. P. 247–251.
//
// Saastamoinen J. Contributions to the theory of atmospheric refraction
// Part II. Refraction Corrections in Sattelie Geodesy, 1973, Bulletin Geodesique, 107, 13-34.
//
double Saastamoinen(double H, double El, struct Weather *Weather)
{
	double H0[13] = {0.0, 0.2, 0.4, 0.6, 0.8, 1.0, 1.5,
	                 2.0, 2.5, 3.0, 4.0, 5.0, 6.0};
	double B0[13] = {1.16, 1.13, 1.10, 1.07, 1.04, 1.01, 0.94,
					 0.88, 0.82, 0.76, 0.66, 0.57, 0.49};
	double H0S[8] = {0.0, 0.5, 1.0, 1.5, 2.0, 3.0, 4.0, 5.0};
	double Z0[13] = {60.0,
					 66.0,
					 70.0,
					 73.0,
					 75.0,
					 76.0,
					 77.0,
					 78.0,
					 78.5,
					 79.0,
					 79.5,
					 79.75,
					 80.0};
	double SigmaR0[13][8] =
	{{0.003, 0.003, 0.002, 0.002, 0.002, 0.002, 0.001, 0.001},
	 {0.006, 0.006, 0.005, 0.005, 0.004, 0.003, 0.003, 0.002},
	 {0.012, 0.011, 0.010, 0.009, 0.008, 0.006, 0.005, 0.004},
	 {0.020, 0.018, 0.017, 0.015, 0.013, 0.011, 0.009, 0.007},
	 {0.031, 0.028, 0.025, 0.023, 0.021, 0.017, 0.014, 0.011},
	 {0.039, 0.035, 0.032, 0.029, 0.026, 0.021, 0.017, 0.014},
	 {0.050, 0.045, 0.041, 0.037, 0.033, 0.027, 0.022, 0.018},
	 {0.065, 0.059, 0.054, 0.049, 0.044, 0.036, 0.030, 0.024},
	 {0.075, 0.068, 0.062, 0.056, 0.051, 0.042, 0.034, 0.028},
	 {0.087, 0.079, 0.072, 0.065, 0.059, 0.049, 0.040, 0.033},
	 {0.102, 0.093, 0.085, 0.077, 0.070, 0.058, 0.047, 0.039},
	 {0.111, 0.101, 0.092, 0.083, 0.076, 0.063, 0.052, 0.043},
	 {0.121, 0.110, 0.100, 0.091, 0.083, 0.068, 0.056, 0.047}};
	int i;
	double z, dz, B, SigmaR;
	double Res[13];
	if(H < -5.0 || H > ht)
	{
		return 0.0;
	}
	z = M_PI_2 - El;
	dz = 16.0 / 3600.0 * DegToRad * tan(z) / Weather->T *
		 (Weather->p + 4800.0 * Weather->e / Weather->T) -
		 0.07 / 3600.0 * DegToRad * (pow(tan(z), 3.0) + tan(z)) *
		 (Weather->p / 1000.0);
	z -= dz;
//	if(z * RadToDeg > 80.0)
//	{
//		return 0.0;
//	}
	B = Linear(B0, H0, H, 13);
	for(i = 0; i < 13; i++)
	{
		Res[i] = Linear(SigmaR0[i], H0S, H, 8);
	}
	SigmaR = Linear(Res, Z0, z * RadToDeg, 13);
	return 0.002277 / cos(z) * (Weather->p + (1255.0 / Weather->T + 0.05) *
		   Weather->e - B * sqr(tan(z))) + SigmaR;
}

// Модель тропосферной задержки на основе модели Саастамойнена
// с функциейй отображения CfA-2.2 Дэвиса для El >= 5°
// B - географическая широта расчётной точки, °
// H - высота расчётной точки над уровнем моря, км
// El - угол возвышения спутника над горизонтом, рад
// *Weather - указатель на структуру с переменными:
// p - атмосферное давление, мбар
// T - температура, К
// e - парциальное давление водяного пара, мбар
// Возвращаемое значение - значение тропосферной задержки, м
//
// Saastamoinen J. Int. Symp. on the Use of Artificial Satellite // Atmospheric
// Correction for the Troposphere and Stratosphere in Radio Ranging
// of Satellite. Washington, 1971. P. 247–251.
//
// Davis, J.L., T.A. Herring, 1.1. Shapiro, A.E.E. Rogers, and G. Elgered,
// Geodesy by radio interferometry: effects of atmospheric modeling errors
// on estimates of baseline length, Radio Science, 20, 1593-1607, 1985.
//
double SaastamoinenDavis(double B, double H, double El, struct Weather *Weather)
{
	double Zd, Zw, a, b, c1, m;
	if(H < -5.0 || H > ht)
	{
		return 0.0;
	}
	Zd = 0.0022768 * Weather->p / (1.0 - 0.00266 * cos(2.0 * B * DegToRad) -
		 0.00028 * H);
	Zw = 0.0022768 / (1.0 - 0.00266 * cos(2.0 * B * DegToRad) -
		 0.00028 * H) * (1255.0 / Weather->T + 0.05) * Weather->e;
	a = 0.001185 * (1.0 + 0.6071E-4 * (Weather->p - 1000.0) - 0.1471E-3 *
		Weather->e + 0.3072E-2 * (Weather->t - 20.0) + 0.1965E-1 *
		(Beta + 6.5) - 0.5645E-2 * (ht - 11.231));
	b = 0.001144 * (1.0 + 0.1164E-4 * (Weather->p - 1000.0) + 0.2795E-3 *
		Weather->e + 0.3109E-2 * (Weather->t - 20.0) + 0.3038E-1 *
		(Beta + 6.5) - 0.1217E-1 * (ht - 11.231));
	c1 = -0.0090;
	m = 1.0 / (sin(El) + a / (tan(El) + b / (sin(El) + c1)));
	return (Zd + Zw) * m;
}

// Модель тропосферной задержки на основе модели Саастамойнена
// с функциейй отображения Нейла для El >= 3°
// B - географическая широта расчётной точки, °
// H - высота расчётной точки над уровнем моря, км
// El - угол возвышения спутника над горизонтом, рад
// *Weather - указатель на структуру с переменными:
// p - атмосферное давление, мбар
// T - температура, К
// e - парциальное давление водяного пара, мбар
// Возвращаемое значение - значение тропосферной задержки, м
//
// Saastamoinen J. Int. Symp. on the Use of Artificial Satellite // Atmospheric
// Correction for the Troposphere and Stratosphere in Radio Ranging
// of Satellite. Washington, 1971. P. 247–251.
//
// Niell, A., 1996. Global Mapping Functions for the Atmosphere Delay at Radio
// Wavelengths. Journal of Geophysical Research 101, pp. 3227–3246.
//
double SaastamoinenNeil(int DOY, double B, double H, double El, struct Weather *Weather)
{
	double Zd, Zw, md, mw, aAv, bAv, cAv, aAm, bAm, cAm, ad, bd, cd, aw, bw, cw;
	double B0[5] = {15.0, 30.0, 45.0, 60.0, 75.0};
	double aAv0[5] = {1.2769934E-3, 1.2683230E-3, 1.2465397E-3, 1.2196049E-3, 1.2045996E-3};
	double bAv0[5] = {2.9153695E-3, 2.9152299E-3, 2.9288445E-3, 2.9022565E-3, 2.9024912E-3};
	double cAv0[5] = {62.610505E-3, 62.837393E-3, 63.721774E-3, 63.824265E-3, 64.258455E-3};
	double aAm0[5] = {0.0,          1.2709626E-5, 2.6523662E-5, 3.4000452E-5, 4.1202191E-5};
	double bAm0[5] = {0.0,          2.1414979E-5, 3.0160779E-5, 7.2562722E-5, 11.723375E-5};
	double cAm0[5] = {0.0,          9.0128400E-5, 4.3497037E-5, 84.795348E-5, 170.37206E-5};
	double ah = 2.53E-5;
	double bh = 5.49E-3;
	double ch = 1.14E-3;
	double a0[5] = {5.8021897E-4, 5.6794847E-4, 5.8118019E-4, 5.9727542E-4, 6.1641693E-4};
	double b0[5] = {1.4275268E-3, 1.5138625E-3, 1.4572752E-3, 1.5007428E-3, 1.7599082E-3};
	double c0[5] = {4.3472961E-2, 4.6729510E-2, 4.3908931E-2, 4.4626982E-2, 5.4736038E-2};
    if(H < -5.0 || H > ht)
	{
		return 0.0;
	}
	Zd = 0.0022768 * Weather->p / (1.0 - 0.00266 * cos(2.0 * B * DegToRad) -
		 0.00028 * H);
	Zw = 0.0022768 / (1.0 - 0.00266 * cos(2.0 * B * DegToRad) - 0.00028 * H) *
		 (1255.0 / Weather->T + 0.05) * Weather->e;
	aAv = Linear(aAv0, B0, B, 5);
	bAv = Linear(bAv0, B0, B, 5);
	cAv = Linear(cAv0, B0, B, 5);
	aAm = Linear(aAm0, B0, B, 5);
	bAm = Linear(bAm0, B0, B, 5);
	cAm = Linear(cAm0, B0, B, 5);
	ad = aAv - aAm * cos(2.0 * M_PI * ((DOY - 28.0) / 365.25  + 0.5 * (B > 0.0)));
	bd = bAv - bAm * cos(2.0 * M_PI * ((DOY - 28.0) / 365.25  + 0.5 * (B > 0.0)));
	cd = cAv - cAm * cos(2.0 * M_PI * ((DOY - 28.0) / 365.25  + 0.5 * (B > 0.0)));
	aw = Linear(a0, B0, B, 5);
	bw = Linear(b0, B0, B, 5);
	cw = Linear(c0, B0, B, 5);
	md = (1.0 + ad / (1.0 + bd / (1.0 + cd))) / (sin(El) + ad / (sin(El) + bd / (sin(El) + cd))) +
		 H * (1.0 / sin(El) - (1.0 + ah / (1.0 + bh / (1.0 + ch))) / (sin(El) + ah / (sin(El) + bh / (sin(El) + ch))));
	mw = (1.0 + aw / (1.0 + bw / (1.0 + cw))) / (sin(El) + aw / (sin(El) + bw / (sin(El) + cw)));
	return Zd * md + Zw * mw;
}

