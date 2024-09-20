#include "Troposphere.h"

#include <math.h>

#include "Constants.h"
#include "Common.h"

// Простейшая тропосферной задержки
// Подходит только для El >= 15°
// El - угол возвышения спутника над горизонтом, рад
// Возвращаемое значение - тропосферная задержка, м
//
//     Антонович К.М. Использование спутниковых радионавигационных систем
// в геодезии: в 2 т. – М.: Картгеоцентр, 2005. – Т. 1.  – 334 с.
//
double PrimitiveTrop(double El)
{
	return 2.41 / sin(El);
}

double PrimitiveMapping(double Zd, double Zw, double El)
{
	return (Zd + Zw) * 1.0 / sin(El);
}

// Простейшая тропосферной задержки, модифицированная в части функции отображения
// El - угол возвышения спутника над горизонтом, рад
// Возвращаемое значение - тропосферная задержка, м
//
//     James Bao-Yen Tsui Fundamentals of Global Positioning System Receivers:
// A Software Approach // John Wiley & Sons Limited, 2005
//
double ModifiedPrimitiveTrop(double El)
{
	return 2.47 / (0.0121 + sin(El));
}

// Функция отображения из "модифицированной" простейшей модели
//
//     James Bao-Yen Tsui Fundamentals of Global Positioning System Receivers:
// A Software Approach // John Wiley & Sons Limited, 2005
//
double ModifiedPrimitiveMapping(double Zd, double Zw, double El)
{
	return (Zd + Zw) * 1.0 / (0.0121 + sin(El));
}

// Модель тропосферной задержки GCAT (GPSCode Analysis Tool)
// Функция отображения использована [Black and Eisner, 1984] для
// El >= 5°
// H - высота приёмника над уровнем моря, км
// El - угол возвышения спутника над горизонтом, рад
// Возвращаемое значение - тропосферная задержка, м
//
//     M. Hernandez-Pajares, GPS data processing: code and phase Algorithms,
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

// Расчёт зенитной тропосферной задержки модели GCAT
double GCATZ(double H, double El, double *Zd, double *Zw)
{
	if(H < -5.0 || H > ht)
	{
        *Zd = 0.0;
		*Zw = 0.0;
		return 0;
	}
	*Zd = 2.3 * exp(-0.116E-3 * H);
	*Zw = 0.1;
	return 1;
}

// Функция отображения Блэка и Эйснера из модели GCAT
//
//     Black, H.D. Correcting Satellite Doppler Data for Tropospheric Effects
// Text. / H.D. Black, A. Eisener // Journal of Geophysical Research. 1984.
// - Vol. 89. -No. D2. - P. 2616 - 2626.
//
double BlackEisnerMapping(double Zd, double Zw, double El)
{
	return (Zd + Zw) * 1.0 / sqrt(1.0 - sqr(cos(El) / (1.0 + 0.001)));
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
//     Hopfield H. S. Two-Quartic Tropospheric Refractivity Profile
// for Correcting Satellite Data //Journal of Geophysical Research. 1969. 18:
// Vol. 74. P. 4487–4499
//
//     Красильников М. Н. Современные информационные технологии в задачах
// навигации и наведения беспилотных маневренных летательных аппаратов /
// Под. ред. М. Н. Красильникова, Г. Г. Серебрякова. М.: ФИЗМАТЛИТ, 2009. 556 с.
//
double Hopfield(double El, struct Weather *Weather)
{
	double Td, Tw;
	Td = 1.0E-6 / 5.0 * 77.64 / sin(sqrt(sqr(El) + 1.904E-3)) *
		 Weather->p / Weather->T * (40136.0 + 148.72 * (Weather->T - 273.16));
	Tw = 1.0E-6 / 5.0 * (-12.96 * Weather->T + 3.718E+5) /
		 sin(sqrt(sqr(El) + 0.6854E-3)) * (Weather->e / sqr(Weather->T)) * 11000.0;
	return Td + Tw;
}

// Расчёт зенитной тропосферной задержки модели Х. Хопфилд
void HopfieldZ(struct Weather *Weather, double *Zd, double *Zw)
{
	*Zd = 1.0E-6 / 5.0 * 77.64 * Weather->p / Weather->T *
		  (40136.0 + 148.72 * (Weather->T - 273.16));
	*Zw = 1.0E-6 / 5.0 * (-12.96 * Weather->T + 3.718E+5) *
		  (Weather->e / sqr(Weather->T)) * 11000.0;
}

// Функция отображения из модели Х. Хопфилд
//
//     Красильников М. Н. Современные информационные технологии в задачах
// навигации и наведения беспилотных маневренных летательных аппаратов /
// Под. ред. М. Н. Красильникова, Г. Г. Серебрякова. М.: ФИЗМАТЛИТ, 2009. 556 с.
//
double HopfieldMapping(double Zd, double Zw, double El)
{
	return Zd * 1.0 / sin(sqrt(sqr(El) + 1.904E-3)) +
		   Zw * 1.0 / sin(sqrt(sqr(El) + 0.6854E-3));
}

// Модель тропосферной задержки Х. Бдэка
// Month - номер месяца в году (1 - январь...)
// B - географическая широта расчётной точки, °
// El - угол возвышения спутника над горизонтом, рад
// *Weather - указатель на структуру с переменными:
// p - атмосферное давление, мбар
// T - температура, К
// e - парциальное давление водяного пара, мбар
// Возвращаемое значение - значение тропосферной задержки, м
//
//    Black, H. D.: An easily implemented algorithm for the tropospheric range
// correction, J. Geophys. Res., 83, 1825–1828,
// https://doi.org/10.1029/JB083iB04p01825, 1978.
//
// 0.28 for summer in tropics or mid-latitudes,
// 0.20 for spring or fall in mid-latitudes,
// 0.12 for winter in maritime mid-latitudes,
// 0.06 for winter in continental mid-latitudes, and
// 0.05 for polar regions
double Black(char Month, double B, double El, double R, struct Weather *Weather)
{
	double Hd, Zw = 0.1, Ic, Td, Tw;
    B = fabs(B);
	Hd = 148.98 * (Weather->T - 4.12);
	Ic = 0.85;
	if(Month >= 6 && Month <= 8)
	{
		if(B <= 60.0)
		{
			Zw = 0.28;
		}
	}

	if((Month >= 3 && Month <= 5) || (Month >= 9 && Month <= 11))
	{
		if(B >= 30.0 && B <= 60.0)
		{
			Zw = 0.20;
		}
	}

	if(Month == 1 || Month == 2 || Month == 12)
	{
		if(B <= 60.0)
		{
			Zw = 0.06;
		}
	}

	if(B > 60.0)
	{
		Zw = 0.05;
	}
	Td = 2.343 * Weather->p / 1013.25 * (Weather->T - 4.12) / Weather->T /
		 sqrt(1.0 - sqr(cos(El) / (1.0 + 0.15 * Ic * Hd / R)));
	Tw = Zw / sqrt(1.0 - sqr(cos(El) / (1.0 + 0.15 * Ic * 13000.0 / R)));
	return Td + Tw;
}

// Расчёт зенитной тропосферной задержки модели Х. Блэка
void BlackZ(char Month, double B, struct Weather *Weather, double *Zd, double *Zw)
{
	B = fabs(B);
	*Zd = 2.343 * Weather->p / 1013.25 * (Weather->T - 4.12) / Weather->T;
    *Zw = 0.1;
	if(Month >= 6 && Month <= 8)
	{
		if(B <= 60.0)
		{
			*Zw = 0.28;
		}
	}

	if((Month >= 3 && Month <= 5) || (Month >= 9 && Month <= 11))
	{
		if(B >= 30.0 && B <= 60.0)
		{
			*Zw = 0.20;
		}
	}

	if(Month == 1 || Month == 2 || Month == 12)
	{
		if(B <= 60.0)
		{
			*Zw = 0.06;
		}
	}

	if(B > 60.0)
	{
		*Zw = 0.05;
	}
}

// Функция отображения модели Х. Блэка
//
//    Black, H. D.: An easily implemented algorithm for the tropospheric range
// correction, J. Geophys. Res., 83, 1825–1828,
// https://doi.org/10.1029/JB083iB04p01825, 1978.
//
double BlackMapping(double Zd, double Zw, double El, double R, struct Weather *Weather)
{
	double Hd, Ic, md, mw;
	Hd = 148.98 * (Weather->T - 4.12);
	Ic = 0.85;
	md = 1.0 / sqrt(1.0 - sqr(cos(El) / (1.0 + 0.15 * Ic * Hd / R)));
	mw = 1.0 / sqrt(1.0 - sqr(cos(El) / (1.0 + 0.15 * Ic * 13000.0 / R)));
	return Zd * md + Zw * mw;
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
//     Saastamoinen J. Int. Symp. on the Use of Artificial Satellite //
// Atmospheric Correction for the Troposphere and Stratosphere in Radio Ranging
// of Satellite. Washington, 1971. P. 247–251.
//
//     Saastamoinen J. Contributions to the theory of atmospheric refraction
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
	return 0.002277 * (1.0 + 0.00266 * cos(2.0 * B * DegToRad) + 0.00028 * H) /
		   cos(z) * (Weather->p + (1255.0 / Weather->T + 0.05) * Weather->e -
		   B * sqr(tan(z))) + SigmaR;
}

// Модель тропосферной задержки на основе модели Ю. Саастамойнена
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
//     Saastamoinen J. Int. Symp. on the Use of Artificial Satellite //
// Atmospheric Correction for the Troposphere and Stratosphere in Radio Ranging
// of Satellite. Washington, 1971. P. 247–251.
//
//     Davis, J.L., T.A. Herring, 1.1. Shapiro, A.E.E. Rogers, and G. Elgered,
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

// Расчёт зенитной задержки модели Саастамойнена в интерпретации Дэвиса
double SaastamoinenDavisZ(double B, double H, struct Weather *Weather,
                          double *Zd, double *Zw)
{
	if(H < -5.0 || H > ht)
	{
		*Zd = 0.0;
		*Zw = 0.0;
		return 0;
	}
	*Zd = 0.0022768 * Weather->p / (1.0 - 0.00266 * cos(2.0 * B * DegToRad) -
		  0.00028 * H);
	*Zw = 0.0022768 / (1.0 - 0.00266 * cos(2.0 * B * DegToRad) -
		  0.00028 * H) * (1255.0 / Weather->T + 0.05) * Weather->e;
	return 1;
}

// Функция отображения Дэвиса CfA-2.2
//
//     Davis, J.L., T.A. Herring, 1.1. Shapiro, A.E.E. Rogers, and G. Elgered,
// Geodesy by radio interferometry: effects of atmospheric modeling errors
// on estimates of baseline length, Radio Science, 20, 1593-1607, 1985.
//
double CfA22Mapping(double Zd, double Zw, double El, struct Weather *Weather)
{
    double a, b, c1, m;
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

// Модель тропосферной задержки на основе модели Ю. Саастамойнена
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
//     Saastamoinen J. Int. Symp. on the Use of Artificial Satellite //
// Atmospheric Correction for the Troposphere and Stratosphere in Radio Ranging
// of Satellite. Washington, 1971. P. 247–251.
//
//     Niell, A., 1996. Global Mapping Functions for the Atmosphere Delay
// at Radio Wavelengths. Journal of Geophysical Research 101, pp. 3227–3246.
//
double SaastamoinenNeil(int DOY, double B, double H, double El, struct Weather *Weather)
{
	double Zd, Zw, md, mw, aAv, bAv, cAv, aAm, bAm, cAm, a, b, c1;
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
	a = aAv - aAm * cos(2.0 * M_PI * ((DOY - 28.0) / 365.25  + 0.5 * (B > 0.0)));
	b = bAv - bAm * cos(2.0 * M_PI * ((DOY - 28.0) / 365.25  + 0.5 * (B > 0.0)));
	c1 = cAv - cAm * cos(2.0 * M_PI * ((DOY - 28.0) / 365.25  + 0.5 * (B > 0.0)));
	md = (1.0 + a / (1.0 + b / (1.0 + c1))) / (sin(El) + a / (sin(El) + b / (sin(El) + c1))) +
		 H * (1.0 / sin(El) - (1.0 + ah / (1.0 + bh / (1.0 + ch))) / (sin(El) + ah / (sin(El) + bh / (sin(El) + ch))));
	a = Linear(a0, B0, B, 5);
	b = Linear(b0, B0, B, 5);
	c1 = Linear(c0, B0, B, 5);
	mw = (1.0 + a / (1.0 + b / (1.0 + c1))) / (sin(El) + a / (sin(El) + b / (sin(El) + c1)));
	return Zd * md + Zw * mw;
}

// Функция отображения А. Нейла
//
//     Niell, A., 1996. Global Mapping Functions for the Atmosphere Delay
// at Radio Wavelengths. Journal of Geophysical Research 101, pp. 3227–3246.
//
double NeilMapping(double Zd, double Zw, int DOY, double B, double H, double El, struct Weather *Weather)
{
	double md, mw, aAv, bAv, cAv, aAm, bAm, cAm, a, b, c1;
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
	aAv = Linear(aAv0, B0, B, 5);
	bAv = Linear(bAv0, B0, B, 5);
	cAv = Linear(cAv0, B0, B, 5);
	aAm = Linear(aAm0, B0, B, 5);
	bAm = Linear(bAm0, B0, B, 5);
	cAm = Linear(cAm0, B0, B, 5);
	a = aAv - aAm * cos(2.0 * M_PI * ((DOY - 28.0) / 365.25  + 0.5 * (B > 0.0)));
	b = bAv - bAm * cos(2.0 * M_PI * ((DOY - 28.0) / 365.25  + 0.5 * (B > 0.0)));
	c1 = cAv - cAm * cos(2.0 * M_PI * ((DOY - 28.0) / 365.25  + 0.5 * (B > 0.0)));
	md = (1.0 + a / (1.0 + b / (1.0 + c1))) / (sin(El) + a / (sin(El) + b / (sin(El) + c1))) +
		 H * (1.0 / sin(El) - (1.0 + ah / (1.0 + bh / (1.0 + ch))) / (sin(El) + ah / (sin(El) + bh / (sin(El) + ch))));
	a = Linear(a0, B0, B, 5);
	b = Linear(b0, B0, B, 5);
	c1 = Linear(c0, B0, B, 5);
	mw = (1.0 + a / (1.0 + b / (1.0 + c1))) / (sin(El) + a / (sin(El) + b / (sin(El) + c1)));
	return Zd * md + Zw * mw;
}

// Функция отображения Чао
//
//     Chao, C.C. A new method to predict wet zenith range refraction from
// surface measurements of meteorological parameters Text. / C.C. Chao //
// JPL Technical Report 32-1526. 1973. - Vol. XIV. - P. 33 -41.
//
double ChaoMapping(double Zd, double Zw, double El)
{
	return Zd * 1.0 / (sin(El) + 0.00143 / (tan(El) + 0.00035)) +
		   Zw * 1.0 / (sin(El) + 0.00035 / (tan(El) + 0.0017));
}

// Функция отображения И. Айфадиса
//
//     Ifadis I. M. 1986, The atmospheric delay of radio waves: modelling the
// elevation dependence on a global scale, Technical Report No. 38L, School
// of Electrical and Computer Engineering, Chalmers University of Technology,
// Gothenburg, Sweden.
double IfadisMapping(double Zd, double Zw, double El, struct Weather *Weather)
{
	double a, b, c1, md, mw;
	a = 0.1237E-2 + 0.1316E-6 * (Weather->p - 1000.0) + 0.1378E-5 *
		(Weather->t - 15.0) + 0.8057E-5 * sqrt(Weather->e);
	b = 0.3333E-2 + 0.1946E-6 * (Weather->p - 1000.0) + 0.1040E-6 *
		(Weather->t - 15.0) + 0.1747E-6 * sqrt(Weather->e);
	c1 = 0.078;
	md = 1.0 / (sin(El) + a / (sin(El) + b / (sin(El) + c1)));
	a = 0.5236E-2 + 0.2471E-6 * (Weather->p - 1000.0) - 0.1724E-6 *
		(Weather->t - 15.0) + 0.1328E-4 * sqrt(Weather->e);
	b = 0.1705E-2 + 0.7384E-6 * (Weather->p - 1000.0) + 0.3767E-6 *
		(Weather->t - 15.0) + 0.2147E-4 * sqrt(Weather->e);
	c1 = 0.05917;
	mw = 1.0 / (sin(El) + a / (sin(El) + b / (sin(El) + c1)));
	return Zd * md + Zw * mw;
}
