#ifndef CORECONSTANTS_H
#define CORECONSTANTS_H

// типы выода даных
#define TO_SCREEN      0
#define TO_FILE        1
#define TO_SCREEN_FILE 2

// максимальное число символов в строке
#define BYTES_OF_STRING 512

// число существующих спутниковых навигационных систем (СНС)
#define TYPES_OF_SYSTEMS 7
// максимальное число СНС, которые могут участвовать в многосистемном решении
#define MAX_OF_SYSTEMS 4
// максимальное число неизвестных в системе нелинейных уравнений
#define MAX_OF_UNKNOWNS 3 + MAX_OF_SYSTEMS
// максимальное число наблюдаемых спутников
#define MAX_OF_VISIBLE_SATS 16 * MAX_OF_SYSTEMS
// максимальное число типов наблюдений для одной СНС
#define MAX_TYPES_OF_OBSERV 32

// порядок интерполяционного полинома Невилла
#define INTERPOLATION_ORDER 10

// скорость света, м/с
extern const double c;
// радиус Земли, км
extern const double RE_WGS84;
// радиус Земли в BeiDou, км
extern const double RE_BDS;
 // коэффициент перевода рад в °
extern const double RadToDeg;
// коэффициент перевода ° в рад
extern const double DegToRad;

// коэффициент -2sqrt(mu_WGS_84)/с^2
extern const double C;
// частота f1 (L1) GPS, МГц
extern const double fL1_GPS;
// частота f2 (L2) GPS, МГц
extern const double fL2_GPS;
// квадрат отношения значений частот f1 и f2 GPS
extern const double gamma_GPS;
// геоцентрическая константа гравитационного поля Земли с учётом атмосферы в системе WGS-84, м^3/с^2
extern const double mu_WGS84;
// большая (экваториальная) полуось общеземного эллипсоида в системе WGS-84, м
extern const double ae_WGS84;
// средняя угловая скорость вращения Земли относительно точки весеннего
// равноденствия в системе WGS-84, рад/с
extern const double OMEGAi_WGS84;

// частота f1 (G1) ГЛОНАСС, МГц
extern const double fG1_GLO;
// частота f2 (G2) ГЛОНАСС, МГц
extern const double fG2_GLO;
// разность значений частот между соседними каналами ГЛОНАСС в диапазоне G1
extern const double dfG1_GLO;
// разность значений частот между соседними каналами ГЛОНАСС в диапазоне G2
extern const double dfG2_GLO;
// квадрат отношения значений частот f1 и f2 ГЛОНАСС
extern const double gamma_GLO;
// геоцентрическая константа гравитационного поля Земли с учётом атмосферы в
// системе ПЗ-90.02, м^3/с^2
extern const double mu_PZ90;
// зональный гармонический коэффициент второй степени
extern const double J20_PZ90;
// большая (экваториальная) полуось общеземного эллипсоида в системе ПЗ-90.02
extern const double ae_PZ90;
// средняя угловая скорость вращения Земли относительно точки весеннего
// равноденствия в системе ПЗ-90.02, рад/с
extern const double omega_PZ90;
// частота f1 (E1) GALILEO, МГц
extern const double fE1_GAL;
// частота f2 (E5a) GALILEO, МГц
extern const double fE5a_GAL;
// частота f2 (E5b) GALILEO, МГц
extern const double fE5b_GAL;
// квадрат отношения значений частот E1 и E5a GALILEO
extern const double gamma_E1_E5a_GAL;
// квадрат отношения значений частот E1 и E5b GALILEO
extern const double gamma_E1_E5b_GAL;

// частота f1 (B1I) BeiDou, МГц
extern const double fB1_BDS;
// частота f2 (B2I) BeiDou, МГц
extern const double fB2_BDS;
// квадрат отношения значений частот B1 и B2 BeiDou
extern const double gamma_B1_B2_BDS;
// шаг интегрирования для метода Рунге-Кутты 4 порядка, с
extern const double GLOStep;

// высота ионосферного слоя для модели Клобучара для NTCM G, км
extern const double hI_NTCM_G;
// высота ионосферного слоя для модели Клобучара для BDS, км
extern const double hI_BDSK;
// высота ионосферного слоя для модели BDGIM, км
extern const double hI_BDGIM;
// высота ионосферного слоя для модели GEMEC, км
extern const double hI_GEMTEC;
// высота ионосферного слоя для методики TayAbsTEC, км
extern const double hI_T;


// географическая широта северного магнитного полюса, рад
extern const double phiM;
// географическая долгота северного магнитного полюса, рад
extern const double lambdaM;

 // полное давление на уровне моря, мбар
extern const double p0;
// температура на уровне моря, °C
extern const double t0;
// относительная влажность воздуха на уровне моря, %
extern const double r0;
// скорость падения температуры в тропосфере, К/км
extern const double Beta;
// высота тропопаузы, км
extern const double ht;

#endif
