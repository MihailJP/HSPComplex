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

double abscx(static complex arg);
double argcx(static complex arg);
complex conjg(static complex arg);

#endif
