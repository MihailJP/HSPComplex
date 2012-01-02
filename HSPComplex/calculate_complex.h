#ifndef __calculate_complex_h
#define __calculate_complex_h

struct complex
{
	double Real;
	double Imaginary;

	complex(double real, double imaginary): Real(real), Imaginary(imaginary) {};
	complex(double real): Real(real), Imaginary(0.0) {};
	complex(): Real(0.0), Imaginary(0.0) {};
	complex operator+();
	complex operator-();
	complex operator=(complex &val);
	complex operator+(complex &val);
	complex operator-(complex &val);
	complex operator*(complex &val);
	complex operator/(complex &val);
	complex operator+=(complex &val);
	complex operator-=(complex &val);
	complex operator*=(complex &val);
	complex operator/=(complex &val);
	int operator==(complex &val);
	int operator!=(complex &val);
	operator double();
};

complex polar(static double modulus, static double argument);
double abscx(static complex arg);
double argcx(static complex arg);
complex conjg(static complex arg);
complex logcx(static complex arg);
complex logcx(static complex arg, static complex base);
complex log10cx(static complex arg);
complex expcx(static complex arg);
complex powcx(static complex arg, static complex exponent);
complex sqrtcx(static complex arg);

complex sincx(static complex arg);
complex coscx(static complex arg);
complex tancx(static complex arg);
complex asincx(static complex arg);
complex acoscx(static complex arg);
complex atancx(static complex arg);

complex sinhcx(static complex arg);
complex coshcx(static complex arg);
complex tanhcx(static complex arg);
complex asinhcx(static complex arg);
complex acoshcx(static complex arg);
complex atanhcx(static complex arg);

#endif
