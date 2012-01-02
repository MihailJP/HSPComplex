#include <math.h>
#include "hspvar_complex.h"
#include "calculate_complex.h"

/* 演算子オーバーロード */

#define A (Real)
#define B (Imaginary)
#define C (val.Real)
#define D (val.Imaginary)

complex complex::operator=(complex &val) {
	return complex(C, D);
}
complex complex::operator+(complex &val) {
	// (a + bi) + (c + di) = (a + c) + (b + d)i
	return complex(A + C, B + D);
}
complex complex::operator-(complex &val) {
	// (a + bi) - (c + di) = (a - c) + (b - d)i
	return complex(A - C, B - D);
}
complex complex::operator*(complex &val) {
	// (a + bi) (c + di) = (ac - bd) + (bc + ad)i
	return complex(
		A * C - B * D,
		B * C + A * D
	);
}
complex complex::operator/(complex &val) {
	// (a + bi) / (c + di) = (ac + bd) / (c^2 + d^2) + (bc - ad)i / (c^2 + d^2)
	return complex(
		(A * C + B * D) / (C * C + D * D),
		(B * C - A * D) / (C * C + D * D)
	);
}
complex complex::operator+=(complex &val) {
	return complex(A + C, B + D);
}
complex complex::operator-=(complex &val) {
	return complex(A - C, B - D);
}
complex complex::operator*=(complex &val) {
	return complex(
		A * C - B * D,
		B * C + A * D
	);
}
complex complex::operator/=(complex &val) {
	return complex(
		(A * C + B * D) / (C * C + D * D),
		(B * C - A * D) / (C * C + D * D)
	);
}
int complex::operator==(complex &val) {
	return ( (A==C)&&(B==D) );
}
int complex::operator!=(complex &val) {
	return ( (A!=C)||(B!=D) );
}

#undef A
#undef B
#undef C
#undef D




/* 関数 */

	double abscx(static complex arg)
{
	return sqrt(arg.Real * arg.Real + arg.Imaginary * arg.Imaginary);
}

double argcx(static complex arg)
{
	return atan2(arg.Imaginary, arg.Real);
}

complex conjg(static complex arg)
{
	complex ans;
	ans.Real = arg.Real; ans.Imaginary = -arg.Imaginary;
	return ans;
}

complex logcx(static complex arg)
{
	complex ans;
	ans.Real = log(abscx(arg));
	ans.Imaginary = argcx(arg);
	return ans;
}

complex logcx(static complex arg, static complex base)
{
	return logcx(arg) / logcx(base);
}

complex log10cx(static complex arg)
{
	return logcx(arg, complex(10,0));
}
