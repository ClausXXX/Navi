#include "Ionosphere.h"
#include "Constants.h"
#include "Common.h"

#include <math.h>

// Модель ионосферной задержки Клобучара
// B - географическая широта расчётной точки, °
// L - географическая долгота расчётной точки, °
// Az - азимут спутника относительно расчётной точки, рад
// El - угол возвышения спутника над горизонтом, рад
// GPSTime - время проведения измерений UTC с начала суток, с
// *Alpha, *Beta - массивы коэффицентов модели на текущие сутки
// Возвращаемое значение - вертикальное значение полного электронного
//                         содержания, TECU (10^16 эл/м^(-2))
double Klobuchar(double B, double L, double Az, double El, double GPSTime,
                 double *Alpha, double *Beta)
{
	int i;
	double psi, phi, lambda, phim, t, PER, x, AMP, Tiono;
	B = B * DegToRad;
	L = L * DegToRad;
	psi =  0.0137 / (El / M_PI + 0.11) - 0.022;
	phi = B / M_PI + psi * cos(Az);
	if(phi > 0.416)
	{
		phi = 0.416;
	}

	if(phi < -0.416)
	{
		phi = -0.416;
	}
	lambda = L / M_PI + psi * sin(Az) / cos(phi * M_PI);
	phim = phi + 0.064 * cos((lambda - 1.617) * M_PI);
	t = 4.32E+4 * lambda + GPSTime;
   // t = (int)t % 86400;
	if(t > 86400.0)
	{
		t -= 86400.0;
	}

	if(t < 0.0)
	{
        t += 86400.0;
    }
	PER = Beta[0] + phim * (Beta[1] + phim * (Beta[2] + phim * Beta[3]));
	if(PER < 72000.0)
	{
		PER = 72000.0;
	}
	x = 2.0 * M_PI * (t - 50400.0) / PER;
	AMP = Alpha[0] + phim * (Alpha[1] + phim * (Alpha[2] + phim * Alpha[3]));

	if(AMP < 0.0)
	{
		AMP = 0.0;
	}

	if(fabs(x) < 1.57)
	{
		Tiono = 5.0E-9 + AMP * (1.0 - sqr(x) / 2.0 + sqr(x) * sqr(x) / 24.0);
	}
	else
	{
		Tiono = 5.0E-9;
	}
	return c * Tiono * sqr(fL1_GPS) / 40.308E+4;
}

// Функция отображения (пересчёта вертикального значения полного электронного
// содержания в наклонное), рекомендованная для модели Клобучара
// Возвращаемое значение - коэффициент пересчёта вертикального значения ПЭС
// в наклонное
double KlobMapping(double El)
{
	return 1.0 + 16.0 * (0.53 - El / M_PI) * sqr(0.53 - El / M_PI);
}

// Адаптивная модель ионосферной задержки ГЛОНАСС
// (подпрограмма рассчёта величин hmax, A, Bbot, Btop)
// UTC - время проведения измерений c начала суток, ч
// Month - номер месяца в году проведения измерений
// _B - широта расчётной точки, °
// _L - долгота расчётной точки, °
// _A - коэффициент адаптации
// F107 - скорректированный индекс солнечной активности на дату проведения
//        измерения
// Ap - среднесуточный индекс геомагнитной активности на дату проведения
//      измерения
// *hmax - указатель на рассчитываемую высоту максимума F2-слоя
// *A - указатель на рассчитываемую 4 * Nmax (концентрацию максимума F2-слоя)
// *Bbot - указатель на рассчитываемый масштаб высоты ниже максимума F2-слоя
// *Btop - указатель на рассчитываемый масштаб высоты выше максимума F2-слоя
int GLOIonosphere(float UTC, char Month, double _B, double _L,
						 double _A, double F107, double Ap,
						 double *hmax, double *A, double *Bbot, double *Btop)
{
	double Slt, W, r, Dec, Mlat, Smlon, Cmlon, Mlong, Dip, B, C,
		   m3000, Nmax, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U,
		   V, X, Y, Z, Fof2, bok, Tns, ans, bns, zns, cns, dLt,
		   Cslt, y, cN, Tnd, dLog, dSlag1, dSlag2, dExp, ch, Ne, TEC, Ie, dS;
	Slt = (UTC + _L / 15.0) * M_PI / 12.0;
	W = sqrt(167273.0 + 1123.6 * (F107 - 63.7)) - 408.99;
	r = 0.01 * W;
	Dec = asin(0.39795 * sin(M_PI * (Month - 3.167) / 6.0));
	_B *= DegToRad;
	_L *= DegToRad;
    if(fabs(0.98 * sin(_B) + 0.2 * cos(_B) * cos(_L + 1.2)) > 1)
	{
		return 0;
    }
	Mlat = asin(0.98 * sin(_B) + 0.2 * cos(_B) * cos(_L + 1.2));
	Smlon = 0.2 * cos(_B) * sin(_L + 1.2);
	Cmlon = 0.98 * sin(Mlat) - sin(_B);
	Mlong = atan2(Smlon, Cmlon);
	Dip = atan(2.0 * tan(Mlat));
	*A = 10.0 * cos(Mlat) * cos(M_PI * (Month / 3.0 - 1.5));
	B = r * (75.0 + 83.0 * cos(Mlat) * sin(Mlat) * sin(Dec));
	C = 30.0 * cos(Slt - 4.5 * fabs(Mlat) - M_PI);
	*hmax = 240.0 + *A + B + C;
	m3000 = 1490.0 / (*hmax + 176.0);
	K = 0.9 + 0.32 * sin(Dec) * sin(Mlat);
	L = 1.0 + sin(Mlat) * sqr(cos(Slt + M_PI_4)) * sin(Dec);
	D = K * L * exp(-1.1 * (cos(Slt - 0.873) + 1.0));
	M = 1.0 + pow(cos(Mlat), 8.0) * pow(cos(fabs(Mlat) - 0.2618), 12) *
		(1.0 - 0.2 * r + 0.6 * sqrt(r)) * exp(0.25 * (1.0 - cos(Slt - 0.873)));
	N = 1.0 + 0.05 * (0.5 - cos(2.0 * Month * M_PI / 6.0) +
		cos(Month * M_PI / 6.0));
	E = (1.0 - 0.4 * pow(cos(Mlat), 10.0)) * (1.0 + 0.6 * pow(cos(Mlat), 10.0) *
		 sqr(cos(Slt + M_PI_4))) * M * N;
	O = 1.0 + 0.05 * r * cos(Month * M_PI / 6.0) * pow(sin(Mlat), 3.0);
	P = 1.0  - 0.15 * exp(- sqrt(sqr(12.0 * Mlat + 4.0 * M_PI / 3.0) +
		sqr(Month / 2.0 - 3.0)));
	F = (1.2 - 0.5 * sqr(cos(Mlat))) * O * P *
		exp(3.0 * cos(Mlat * (sin(Slt) - 1.0) / 2.0));
	if(r < 1.1)
	{
		G = 1.0 + r + (0.204 + 0.03 * r) * sqr(r);
	}
	else
	{
		G = 2.39 + 1.53 * sqr(sin(Mlat)) *
			((1.0 + r + (0.204 + 0.03 * r) * sqr(r)) - 2.39);
	}
	H = 1.0 + 0.1 * pow(cos(Mlat), 3) * cos(2.0 * (Mlong - 7.0 * M_PI / 18.0));
	R = 1.0 + (0.15 - 0.5 * (1.0 + r) * (1.0 - cos(Mlat)) *
		exp(-0.33 * sqr(Month - 6))) *
		exp(-18.0 * sqr(fabs(Dip) - 4.0 * M_PI / 18.0));
	I = R * (1.0 + 0.03 * (0.5 - cos(2.0 * Month * M_PI / 6.0) +
		cos(Month * M_PI / 6.0)));
	S = 0.2 * (1.0 - sin(fabs(Mlat) - 0.5236)) *
		(1.0 + 0.6 * cos(M_PI / 3.0 * (Month - 4))) *
		cos(M_PI / 6.0 * (Month - 1.0));
	if((1.0 - cos(Slt)) <= 0.0001)
	{
		V = 0.0;
	}
	else
	{
		V = pow(cos(Mlat + Dec), 3) * (0.15 + 0.3 * sin(fabs(Mlat)))
			* exp(0.25 * log(1.0 - cos(Slt)));
	}
	T = (0.13 - 0.06 * sin(fabs(fabs(Mlat) - M_PI / 9.0))) *
		cos(M_PI / 3.0 * (Month - 4.5)) - V;
	X = 1.0 + 0.085 * (pow(cos(M_PI / 6.0 * (0.5 * Month - 1.0)), 3) *
		cos(Mlat - M_PI / 6.0) + cos(Mlat + M_PI / 4.0) *
		pow(cos(M_PI / 6.0 * (0.5 * Month - 4.0)), 2));
	Y = 1.3 + (0.139 * (1.0 + cos(Mlat - M_PI / 4.0)) + 0.0517 * r) * sqr(r);
	Z = cos(Mlat + Dec * cos(Slt)) - cos(Mlat);
	U = 0.7 * (X + 0.1778 * sqr(r) / G * cos(M_PI / 3.0 * (Month - 4.3))) *
		exp(- Y * Z);
	J = (S + T) / G + U;
	Nmax = 0.66 * D * E * F * G * H * I * J;
	Fof2 = sqrt(Nmax / 0.124);
	*Bbot = 0.385 * Nmax / (0.01 * exp(-3.467 + 0.857 * log(sqr(Fof2)) +
			2.02 * log(m3000)));
	if(Month >= 4 && Month <= 9)
	{
		bok = 6.705 - 0.01 * W - 0.008 * *hmax;
	}
	else
	{
		bok = -7.77 + 0.097 * sqr(*hmax / *Bbot) + 0.153 * Nmax;
	}

	if(bok < 2.0)
	{
		bok = 2.0;
	}

	if(bok > 8.0)
	{
		bok = 8.0;
	}
	*Btop = *Bbot * bok / ((0.041163 * (*Bbot * bok - 150.0) /
			100.0 - 0.183981) * (*Bbot * bok - 150.0) / 100.0 + 1.424472);
	if(Ap > 27.0)
	{
		ans = 3.3 * W + 705.0;
		zns = (Month * 30.5 - 96) / 365.0;
		bns = 0.2 * sin(2.0 * M_PI * zns) * sin(_B);
		_L *= RadToDeg;
		dLt = UTC + _L / 15.0;
		if(dLt < 0.0)
		{
			dLt += 24.0;
		}

		if(dLt >= 24.0)
		{
			dLt -= 24.0;
		}
		Cslt = cos(15.0 * (dLt - 15.0) * M_PI / 180.0);
		cns = 0.12 * cos(_B) * Cslt;
		Tns = ans * (1.0 + bns + cns) * 0.001;
		Tnd = Tns + (4.5 * Ap - 100.0) * sqr(sin(Mlat)) * 0.001;
		dLog = log((2.0 - 1.8 * Tns + 0.8 * sqr(Tns)) /
			   (2.0 - 1.8 * Tnd + 0.8 * sqr(Tnd)));
		dSlag1 = -0.0022 * (1.0 - 0.3 * Cslt) * (4.5 * Ap - 100.0) *
				 sqr(sin(Mlat));
		dSlag2 = 0.0007 * (1.0 - 0.3 * Cslt) * Ap * pow(cos(Mlat), 4);
		dExp = 1.3 * (dLog + dSlag1 + dSlag2);
		cN = exp(dExp);
		ch = 0.2 * Ap * (1.0 - 0.001 * Ap) * (1.0 + 0.01 * Mlat);
		*hmax += ch;
		Nmax *= cN;
		*Btop *= (1.0 - log(cN));
		*Bbot *= (1.0 - log(cN) / 2.0);
	}
	Nmax *= _A ;
	*A = 4.0 * Nmax;
	return 1;
}

// Расчёт электронной концентрации в заданной точке пространства
// для адаптивной модели ионосферной задержки ГЛОНАСС
// H - высота расчётной точки над уровнем моря, км
// hmax - высота максимума F2-слоя
// A - 4 * Nmax (концентрация максимума F2-слоя)
// Bbot - масштаб высоты ниже максимума F2-слоя
// Btop - масштаб высоты выше максимума F2-слоя
// Возвращаемое значение - значение электронной концентрации в точке,
//                         TECU (10^16 эл/м^(-2))
double Ne(double H, double hmax, double A, double Bbot, double Btop)
{
	double y, Ne;
	if(H < hmax)
	{
		y = (H - hmax) / Bbot;
	}

	if(H > hmax)
	{
		y = (H - hmax) / Btop;
		y /= 1.0 + 12.5 * y / (100.0 + 0.1 * y);
	}
	Ne = A * exp(y) / sqr(1.0 + exp(y)) * 0.01;
	return Ne;
}

// Упрощённый расчёт интегрального значения ПЭС вдоль вертикального луча
// для адаптивной модели ионосферной задержки ГЛОНАСС
// A - 4 * Nmax (4 * концентрация максимума F2-слоя)
// Bbot - масштаб высоты ниже максимума F2-слоя
// Btop - масштаб высоты выше максимума F2-слоя
// Возвращаемое значение - вертикальное значение ПЭС, TECU (10^16 эл/м^(-2))
double SimpleTECv(double A, double Bbot, double Btop)
{
	return A * (0.5 * Bbot + 0.9 * Btop) * 0.01;
}

// Функция отображения (пересчёта вертикального значения полного электронного
// содержания в наклонное), рекомендованная для адаптивной модели ионосферной
// задержки ГЛОНАСС
// B - широта расположения приёмника, °
// L - долгота расположения приёмника, °
// b - широта расположения передатчика, °
// l - долгота расположения передатчика, °
// h - высота расположения передатчика, км
// Возвращаемое значение - коэффициент пересчёта вертикального значения ПЭС
//                         в наклонное
double GLOMapping(double B, double L, double b, double l, double h)
{
	double Cdel, Sdel, r2, Zet;
	B *= DegToRad;
	L *= DegToRad;
	b *= DegToRad;
	l *= DegToRad;
	Cdel = sin(B) * sin(b) + cos(B) * cos(b) * cos (l - L);
	Sdel = sqrt(1.0 - sqr(Cdel));
	r2 = Re_WGS84 + h;
	Zet = atan2(Sdel, Cdel - Re_WGS84 / r2);
	return 1.0 / sqrt(1.0 - sqr(sin(Zet) / (1.0 + 400.0 / Re_WGS84)));
}

// Интегрирование ПЭС по высоте методом Симпсона
// (*f) - указатель на интегрируемую функцию (Ne())
// a - нижний предел интегрирования, км
// b - верхний предел интегрирования, км
// dx - шаг интегрирования, км
// hmax - высота максимума F2-слоя
// A - 4 * Nmax (концентрация максимума F2-слоя)
// Bbot - масштаб высоты ниже максимума F2-слоя
// Btop - масштаб высоты выше максимума F2-слоя
// Возвращаемое значение - вертикальное значение ПЭС, TECU (10^16 эл/м^(-2))
double IntegralTECv(double (*f)(double, double, double, double, double),
					double a, double b, double dx, double hmax, double A,
					double Bbot, double Btop)
{
	int i = 1;
	double x, S = 0.0;
	for(x = a + dx; x < b; x += dx)
	{
		S += ((i++ % 2) * 2.0 + 2.0) * (*f)(x, hmax, A, Bbot, Btop);
	}
	S = ((*f)(a, hmax, A, Bbot, Btop) + (*f)(b, hmax, A, Bbot, Btop) + S) *
	dx / 3.0;
	return S;
}

long Factorial(int n)
{
	long F = 1;
	while(n > 1)
	{
		F *= n--;
	}
	return F;
}

long DoubleFactorial(int n)
{
	long DF = 1;
	while(n > 1)
	{
		DF *= n;
		n -= 2;
	}
	return DF;
}

double PSin(int n, int m, double phii)
{
	double P;
	if(n == m)
	{
		P = DoubleFactorial(2 * n - 1) * pow(1.0 - sqr(sin(phii)), n / 2.0);
	}
	else
	{
		if(n == m + 1)
		{
			P = sin(phii) * (2 * m + 1) * PSin(m, m, phii);
		}
		else
		{
			P = ((2 * n - 1) * sin(phii) * PSin(n - 1, m, phii) - (n + m - 1) *
				 PSin(n - 2, m, phii)) / (n - m);
		}
	}
	return P;
}

int Sigma0(int m)
{
	if(m == 0)
	{
		return 1;
	}
	else
	{
		if(m > 0)
		{
			return 0;
		}
	}
    return -1;
}

double N(int n, int m)
{
	return sqrt(Factorial(n - m) * (2.0 * n + 1.0) * (2.0 - Sigma0(m)) /
		   Factorial(n + m));
}

double UTCToMJD(short Year, char Month, char Day,
                char Hours, char Minutes, float Seconds)
{
	int a, y, m;
	double JD, MJD;
	a = (14 - Month) / 12;
	y = Year + 4800 - a;
	m = Month + 12 * a - 3;
	JD = Day + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045 +
		 (Hours - 12) / 24.0 + Minutes / 1440.0 + Seconds / 86400.0;
	MJD = JD - 2400000.5;
	return MJD;
}

//double UTCToGPSTime(short Year, char Month, char Day,
//                    char Hours, char Minutes, float Seconds)
//{
//	int a, y, m, GPSWN;
//	double JD, GPSTOW;
//	a = (14 - Month) / 12;
//	y = Year + 4800 - a;
//	m = Month + 12 * a - 3;
//	JD = Day + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045 +
//		 (Hours - 12) / 24.0 + Minutes / 1440.0 + Seconds / 86400.0;
//	GPSWN = floor((JD - 2444244.5) / 7);
//	GPSTOW = (((JD - 2444244.5) / 7) - GPSWN) * 604800.0;
//	return GPSTOW;
//}


//double BDSTimeToMJD(int BDSWN, double BDSTOW)
//{
//	double MJD;
//	// 2006.01.01-->53736.0
//
//	MJD = BDSWN * 7.0 + BDSTOW / 86400.0;
//	MJD += 53736.0;
//	return MJD;
//}

// Модель ионосферной задержки BDGIM 
// (BeiDou Global Ionospheric delay correction Model)
// Year - год эпохи наблюдений
// Month - номер месяца в году эпохи наблюдений
// Day - номер дня в месяце эпохи наблюдений
// Hours - час эпохи наблюдений
// Minutes - минута эпохи наблюдений
// Seconds - секунда эпохи наблюдений
// _B - географическая широта расчётной точки, °
// L - географическая долгота расчётной точки, °
// Az - азимут спутника относительно приёмника, рад
// El - угол возвышения спутника над горизонтом, рад
// (*Alpha)[9] (Alpha[12][9]) - массив коэффициентов модели на текущие сутки,
// TECU
// (на каждые 2 часа свой набор из 9 коэффициентов)
// Возвращаемое значение - значение вертикального полного электронного
// содержания, TECU (10^16 эл/м^(-2))
double BDGIM(short Year, char Month, char Day,
			 char Hours, char Minutes, float Seconds,
			 double _B, double L, double Az, double El, double (*Alpha)[9])
{
	const int ni[9] = {0, 1, 1,  1, 2, 2,  2, 2,  2};
	const int mi[9] = {0, 0, 1, -1, 0, 1, -1, 2, -2};
	const int nj[17] = {3, 3,  3, 3,  3, 3,  3, 4, 4,  4, 4,  4, 5, 5,  5, 5,  5};
	const int mj[17] = {0, 1, -1, 2, -2, 3, -3, 0, 1, -1, 2, -2, 0, 1, -1, 2, -2};
	const float T[12] =
	{1, 0.5, 0.33, 14.6, 27.0, 121.6, 182.51, 365.25, 4028.71, 2014.35, 1342.90, 1007.18};

	const float a[13][17] =
	{{-0.61, -1.31, -2.00, -0.03,  0.15, -0.48, -0.40,  2.28, -0.16, -0.21, -0.10, -0.13,  0.21,  0.68,  1.06,   0.0, -0.12},
	 {-0.51, -0.43,  0.34, -0.01,  0.17,  0.02, -0.06,  0.30,  0.44, -0.28, -0.31, -0.17,  0.04,  0.39, -0.12,  0.12,   0.0},
	 {-0.06, -0.05,  0.06,  0.17,  0.15,  0.00,  0.11, -0.05, -0.16,  0.02,  0.11,  0.04,  0.12,  0.07,  0.02, -0.14, -0.14},
	 { 0.01, -0.03,  0.01, -0.01,  0.05, -0.03,  0.05, -0.03, -0.01,   0.0, -0.08, -0.04,   0.0, -0.02, -0.03,   0.0, -0.03},
	 {-0.01,   0.0,  0.01,   0.0,  0.01,  0.00, -0.01, -0.01,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
	 {  0.0,  0.00,  0.03,  0.01,  0.02,  0.01,   0.0, -0.02,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
	 {-0.19, -0.02,  0.12, -0.10,  0.06,   0.0, -0.02, -0.08, -0.02, -0.07,  0.01,  0.03,  0.15,  0.06, -0.05, -0.03, -0.10},
	 {-0.18,  0.06, -0.55, -0.02,  0.09, -0.08,   0.0,  0.86, -0.18, -0.05, -0.07,  0.04,  0.14, -0.03,  0.37, -0.11, -0.12},
	 { 1.09, -0.14, -0.21,  0.52,  0.27,   0.0,  0.11,  0.17,  0.23,  0.35, -0.05,  0.02, -0.60,  0.02,  0.01,  0.27,  0.32},
	 {-0.34, -0.09, -1.22,  0.05,  0.15, -0.29, -0.17,  1.58, -0.06, -0.15,  0.00,  0.13,  0.28, -0.08,  0.62, -0.01, -0.04},
	 {-0.13,  0.07, -0.37,  0.05,  0.06, -0.11, -0.07,  0.46,   0.0, -0.04,  0.01,  0.07,  0.09, -0.05,  0.15, -0.01,  0.01},
	 {-0.06,  0.13, -0.07,  0.03,  0.02, -0.05, -0.05,  0.01,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
	 {-0.03,  0.08, -0.01,  0.04,  0.01, -0.02, -0.02, -0.04,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0}};

	const float b[12][17] =
	{{ 0.23, -0.20, -0.31,  0.16, -0.03,  0.02,  0.04,  0.18,  0.34,  0.45,  0.19, -0.25, -0.12,  0.18,  0.40, -0.09,  0.21},
	 { 0.02, -0.08, -0.06, -0.11,  0.15, -0.14,  0.01,  0.01,  0.04, -0.14, -0.05,  0.08,  0.08, -0.01,  0.01,  0.11, -0.12},
	 {  0.0, -0.02, -0.03, -0.05, -0.01, -0.07, -0.03, -0.01,  0.02, -0.01,  0.03, -0.10,  0.01,  0.05, -0.01,  0.04,  0.00},
	 {  0.0, -0.02,  0.01,   0.0, -0.01,  0.01,   0.0, -0.02,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
	 { 0.01,   0.0,   0.0,  0.01,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
	 {-0.09,  0.07,  0.03,  0.06,  0.09,  0.01,  0.02,   0.0, -0.04, -0.02, -0.01,  0.01, -0.10,   0.0, -0.01,  0.02,  0.05},
	 { 0.15, -0.31,  0.13,  0.05, -0.09, -0.03,  0.06, -0.36,  0.08,  0.05,  0.06, -0.02, -0.05,  0.06, -0.20,  0.04,  0.07},
	 { 0.50, -0.08, -0.38,  0.36,  0.14,  0.04,   0.0,  0.25,  0.17,  0.27, -0.03, -0.03, -0.32, -0.10,  0.20,  0.10,  0.30},
	 {  0.0, -0.11, -0.22,  0.01,  0.02, -0.03, -0.01,  0.49, -0.03, -0.02,  0.01,  0.02,  0.04, -0.04,  0.16, -0.02, -0.01},
	 { 0.05,  0.03,  0.07,  0.02, -0.01,  0.03,  0.02, -0.04, -0.01, -0.01,  0.02,  0.03,  0.02, -0.04, -0.04, -0.01,   0.0},
	 { 0.03, -0.02,  0.04, -0.01, -0.03,  0.02,  0.01,  0.04,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0},
	 { 0.04, -0.02, -0.04,  0.00, -0.01,   0.0,  0.01,  0.07,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0}};
	int i, j, k;
	float tp1, tp2;
	double psi, phig, lambdag, phim, lambdam, t, Slon, phii, lambdai,
		   A[10], B[17], beta, omega, tp, VTEC;
	_B *= DegToRad;
	L *= DegToRad;
	psi = M_PI_2 - El - asin(Re_BDS * cos(El) / (Re_BDS + hi_BDGIM));
	phig = asin(sin(_B) * cos(psi) + cos(_B) * sin(psi) * cos(Az));
	lambdag = L + atan2(sin(psi) * sin(Az) * cos(_B), (cos(psi) - sin(_B) *
			  sin(phig)));
	phim = asin(sin(phiM) * sin(phig) + cos(phiM) * cos(phig) *
		   cos(lambdag - lambdaM));
	lambdam = atan2(cos(phig) * sin(lambdag - lambdaM) * cos(phiM), (sin(phiM) *
			  sin(phim) - sin(phig)));
	t = UTCToMJD(Year, Month, Day, Hours, Minutes, Seconds);
	Slon = M_PI * (1.0 - 2.0 * (t - floor(t)));
	phii = phim;
	lambdai = lambdam - atan2(sin(Slon - lambdaM), sin(phiM) * cos(Slon - lambdaM));
	//printf("phii=%lf lambdai=%lf, %lf", (phii * RadToDeg), (lambdai * RadToDeg), t);
   //	getch();
	//phii = 27.56 * DegToRad;
	//lambdai = -49.12 * DegToRad;
	for(i = 0; i < 9; i++)
	{
		if(mi[i] >= 0)
		{
			A[i + 1] = N(abs(ni[i]), abs(mi[i])) *
					   PSin(abs(ni[i]), abs(mi[i]), phii) * cos(mi[i] * lambdai);
		}
		else
		{
			A[i + 1] = N(abs(ni[i]), abs(mi[i])) *
					   PSin(abs(ni[i]), abs(mi[i]), phii) * sin(-mi[i] * lambdai);
		}
	}
	t = UTCToMJD(Year, Month, Day, Hours, Minutes, Seconds);
	if(!(Hours % 2))
	{
		tp1 = UTCToMJD(Year, Month, Day, Hours + 1, 0, 0.0);
		tp2 = UTCToMJD(Year, Month, Day, Hours - 1, 0, 0.0);
		if(fabs(t - tp1) <= fabs(t - tp2))
		{
			tp = tp1;
		}
		else
		{
			tp = tp2;
		}
	}
	else
	{
		tp = UTCToMJD(Year, Month, Day, Hours, 0, 0.0);
	}
	A[0] = 0.0;
	for(j = 0; j < 17; j++)
	{
		beta = a[0][j];
		for(k = 0; k < 12; k++)
		{
			omega = 2.0 * M_PI / T[k];
			beta += a[k + 1][j] * cos(omega * tp) + b[k][j] * sin(omega * tp);
		}
	   	//printf("\nbeta[%d] %lf", j, beta);
		if(mj[j] >= 0)
		{
			B[j] = N(abs(nj[j]), abs(mj[j])) *
				   PSin(abs(nj[j]), abs(mj[j]), phii) * cos(mj[j] * lambdai);
		}
		else
		{
			B[j] = N(abs(nj[j]), abs(mj[j])) *
				   PSin(abs(nj[j]), abs(mj[j]), phii) * sin(-mj[j] * lambdai);
		}
		A[0] += beta * B[j];
	}
	VTEC = A[0];
	//printf("\n%lf", A[0]);
	for(i = 0; i < 9; i++)
	{
		VTEC += Alpha[Hours / 2][i] * A[i + 1];
    }
	return VTEC;
}

// Модель ионосферной задержки GEMTEC
// (Global Empirical Model of Total Electron Content)
// LO - географическая долгота, °
// TIM - мировое время, ч
// LA - географическая широта, °
// F10.7 - индекс солнечной активности. По умолчанию можно задать равным 120.
// MES - номер месяца в году
// DAY - номер дня в месяце
// evt, evm, evl, evf, A - массивы эмпирических значений модели из файла aio.bin
// Возвращаемое значение - значение вертикального ПЭС, TECU (10^16 эл/м^(-2))

// Оригинальный заголовок В.Б. Иванова
// float GEMTEC(float LO, float TIM, float LA, float F107, int MES, int DAY,
//			  short evt[4][13], short evm[4][4][13], short evl[4][4][5][15],
//			  short evf[4][4][5][2][2], short A[4][4][5][2][13])

float GEMTEC(float LO, float TIM, float LA, float F107, int MES, int DAY,
			 short (*evt)[13], short (*evm)[4][13], short (*evl)[4][5][15],
			 short (*evf)[4][5][2][2], short (*A)[4][5][2][13])
{
	float dLO, dTIM, dLA, dMES,	XT,	XM,	XL, XF, R;
	int i, j, jj, k, kkk, r, kk, NLO, NTIM, NLA;
	if(LO >= 180.0)
	{
		LO -= 360.0;
	}
	NLO = ((int)(LO + 180.0)) / 30;
	dLO = (LO + 180.0) / 30.0 - NLO;
	NTIM = ((int)(TIM)) / 2;
	dTIM = TIM * 0.5 - NTIM;
	if(LA > 70.0)
	{
		LA = 70.0;
	}

	if(LA < -70.0)
	{
		LA = -70.0;
	}
	NLA = ((int)(70.0 - LA)) / 10;
	dLA = 7.0 - LA * 0.1 - NLA;
	dMES = DAY * 0.032258;
	for(R = 0.0, jj = 0; jj < 4; jj++)
	{
		XT = evt[jj][NTIM] + (evt[jj][NTIM + 1] - evt[jj][NTIM]) * dTIM;
		for(kkk = 0; kkk < 4; kkk++)
		{
			XM = evm[jj][kkk][MES - 1] + (evm[jj][kkk][MES] -
			     evm[jj][kkk][MES - 1]) * dMES;
			for(kk = 0; kk < 5; kk++)
			{
				XL = evl[jj][kkk][kk][NLA] +
					 (evl[jj][kkk][kk][NLA + 1] - evl[jj][kkk][kk][NLA]) * dLA;
				for(j = 0; j < 2; j++)
				{
					XF = -(F107 * (evf[jj][kkk][kk][j][1] -
						 evf[jj][kkk][kk][j][0]) +
						 evf[jj][kkk][kk][j][0] * 60 -
						 evf[jj][kkk][kk][j][1] * 240);
					R += XT * XM * XL * XF * (A[jj][kkk][kk][j][NLO] +
						 (A[jj][kkk][kk][j][NLO + 1] -
						 A[jj][kkk][kk][j][NLO]) * dLO);
				}
			}
		}
	}

	if(R < 0.0)
	{
		R = 0.0;
	}
	return R * 5.55555556e-20; // TECU
	//return 0.9E-20;          // метры, для частоты f1 GPS
}

// Функция отображения (пересчёт вертикального значения полного электронного
// содержания в наклонное)
// El - угол возвышения спутника над горизонтом, рад
// Re - радиус Земли, км
// hi - высота ионосферного слоя, км
// Возвращаемое значение - коэффициент пересчёта вертикального значения ПЭС в
//                         наклонное
double IonoMapping(double El, double Re, double hi)
{
	return 1.0 / sqrt(1.0 - sqr(Re * cos(El) / (Re + hi)));
}

// Функция отображения, применяемая в методике TayAbsTEC
//(пересчёт вертикального значения полного электронного
// содержания в наклонное)
// _B - географическая широта расчётной точки, °
// El - угол возвышения спутника над горизонтом, рад
// Re - радиус Земли, км
// hi - высота ионосферного слоя, км
// Возвращаемое значение - коэффициент пересчёта вертикального значения ПЭС в
//                         наклонное
double IonoMappingT(double _B, double El, double Re, double hi)
{
	float alpha = 1.0;
	if(_B < 15.0)
	{
		alpha = 0.87;
	}
	else
	{
		if(_B >= 15.0 && _B < 30)
		{
			alpha = 0.93;
		}
		else
		{
			if(_B >= 30.0 && _B < 65)
			{
				alpha = 0.97;
			}
			else
			{
				if(_B >= 65.0)
				{
					alpha = 0.94;
				}
			}
		}
	}
	return 1.0 / cos(asin(Re * cos(alpha * El) / (Re + hi)));
}

//Критерии такие:
//Если широта станции < 15 то alpha = 0.87
//Если от 15 до 30 градусов, то alpha = 0.93
//Если от 30 до 65 градусов, то alpha = 0.97
//Если от 65 до 90 градусов, то alpha = 0.94

