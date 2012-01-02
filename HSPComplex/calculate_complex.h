#ifndef __calculate_complex_h
#define __calculate_complex_h

struct complex
{
	double Real;
	double Imaginary;

	complex(double real, double imaginary): Real(real), Imaginary(imaginary) {};
	complex(): Real(0.0), Imaginary(0.0) {};
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

#endif
