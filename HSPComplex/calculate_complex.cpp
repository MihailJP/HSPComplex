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

/* 極形式を直交形式にする */
complex polar(static double modulus, static double argument)
{
	return complex(
		modulus * cos(argument),
		modulus * sin(argument)
		);
}

/* 絶対値 */
double abscx(static complex arg)
{
	return sqrt(arg.Real * arg.Real + arg.Imaginary * arg.Imaginary);
}

/* 偏角 */
double argcx(static complex arg)
{
	return atan2(arg.Imaginary, arg.Real);
}

/* 複素共役 */
complex conjg(static complex arg)
{
	return complex(
		arg.Real,
		-arg.Imaginary
		);
}

/* 自然対数 */
complex logcx(static complex arg)
{
	return complex(
		log(abscx(arg)),
		argcx(arg)
		);
}

/* 対数関数 */
complex logcx(static complex arg, static complex base)
{
	return logcx(arg) / logcx(base);
}

/* 常用対数 */
complex log10cx(static complex arg)
{
	return logcx(arg, complex(10,0));
}

/* 指数関数 */
complex expcx(static complex arg)
{
	// exp (x + iy) = exp x(cos y + i sin y) = exp x cos y + i exp x sin y
	return complex(
		exp(arg.Real) * cos(arg.Imaginary),
		exp(arg.Real) * sin(arg.Imaginary)
		);
}

/* 冪乗 */
complex powcx(static complex arg, static complex exponent)
{
	// 誤差を極力抑えるために場合分け
	// a ^ z = e ^ (z ln a)
	if (exponent.Imaginary) /* 虚数乗の場合 */
		return expcx(exponent * logcx(arg));
	else if ((arg.Real < 0)&&(arg.Imaginary == 0)&&((exponent.Real * 2)==floor(exponent.Real * 2))) /* 負の実数の半整数乗 */
		switch ((int)floor(exponent.Real * 2) % 4) {
		case 0: // 実軸プラス
			return complex(pow(-arg.Real, exponent.Real), 0);
		case 1: // 虚軸プラス
			return complex(0, pow(-arg.Real, exponent.Real));
		case 2: // 実軸マイナス
			return complex(-pow(-arg.Real, exponent.Real), 0);
		case 3: // 虚軸マイナス
			return complex(0, -pow(-arg.Real, exponent.Real));
		}
	else if ((arg.Imaginary != 0)&&(arg.Real == 0)&&(exponent.Real == floor(exponent.Real))) /* 純虚数の整数乗 */
		switch ((int)floor(exponent.Real) % 4) {
		case 0: // 実軸プラス
			return complex(pow(arg.Imaginary, exponent.Real), 0);
		case 1: // 虚軸プラス
			return complex(0, pow(arg.Imaginary, exponent.Real));
		case 2: // 実軸マイナス
			return complex(-pow(arg.Imaginary, exponent.Real), 0);
		case 3: // 虚軸マイナス
			return complex(0, -pow(arg.Imaginary, exponent.Real));
		}
	else if (argcx(arg) != 0) /* 底が正の実数でない場合 */
		return polar(pow(abscx(arg), exponent.Real), argcx(arg) * exponent.Real);
	else /* 普通のライブラリ関数が使える場合 */
		return complex(pow(arg.Real, exponent.Real), 0);
}

/* 平方根 */
complex sqrtcx(static complex arg)
{
	return powcx(arg, complex(0.5, 0));
}
