#include <math.h>
#include <stdio.h>
#include <string.h>
#include "hspvar_complex.h"
#include "calculate_complex.h"

/* NaNを返したいのにVC++だとそういう関数定義されてないってどういうことですかー */
const double NaN = ::sqrt(-1.0);

complex cmplx(double Real, double Imaginary) {
	complex val;
	val.Real = Real; val.Imaginary = Imaginary;
	return val;
}

#ifndef MINIMAL_TEST
/* 演算子オーバーロード */

#define A (Real)
#define B (Imaginary)
#define C (val.Real)
#define D (val.Imaginary)

complex complex::operator+() const{
	// (a + bi) + (c + di) = (a + c) + (b + d)i
	return cmplx(A, B);
}
complex complex::operator-() const{
	// (a + bi) - (c + di) = (a - c) + (b - d)i
	return cmplx(-A, -B);
}
complex complex::operator=(complex &val) {
	return cmplx(C, D);
}
complex complex::operator+(const complex &val) const{
	// (a + bi) + (c + di) = (a + c) + (b + d)i
	return cmplx(A + C, B + D);
}
complex complex::operator-(const complex &val) const{
	// (a + bi) - (c + di) = (a - c) + (b - d)i
	return cmplx(A - C, B - D);
}
complex complex::operator*(const complex &val) const{
	// (a + bi) (c + di) = (ac - bd) + (bc + ad)i
	return cmplx(
		A * C - B * D,
		B * C + A * D
	);
}
complex complex::operator/(const complex &val) const{
	// (a + bi) / (c + di) = (ac + bd) / (c^2 + d^2) + (bc - ad)i / (c^2 + d^2)
	return cmplx(
		(A * C + B * D) / (C * C + D * D),
		(B * C - A * D) / (C * C + D * D)
	);
}
complex complex::operator+=(complex &val) {
	return cmplx(A + C, B + D);
}
complex complex::operator-=(complex &val) {
	return cmplx(A - C, B - D);
}
complex complex::operator*=(complex &val) {
	return cmplx(
		A * C - B * D,
		B * C + A * D
	);
}
complex complex::operator/=(complex &val) {
	return cmplx(
		(A * C + B * D) / (C * C + D * D),
		(B * C - A * D) / (C * C + D * D)
	);
}
bool complex::operator==(complex &val) {
	return ( (A==C)&&(B==D) );
}
bool complex::operator!=(complex &val) {
	return ( (A!=C)||(B!=D) );
}

#undef A
#undef B
#undef C
#undef D

/* キャスト演算子のオーバーロード */
complex::operator double() {
	if (Imaginary) return NaN;
	else return Real;
}



/* 関数 */

#define I (cmplx(0,1))

#endif
/* 文字列に変換 */
char *complex2str(const complex val) {
	static char result[64] = "";
	char realstr[32] = ""; char imagstr[32] = ""; char imagstr_tmp[32]; int mantlen;
	if (val.Real) sprintf(realstr, "%g", val.Real);
	if (val.Imaginary) {
		memset(imagstr_tmp, 0, sizeof(imagstr_tmp)); //作業用バッファを初期化して0で埋める
		sprintf(imagstr_tmp, "%g", val.Imaginary);
		mantlen = (int)strcspn(imagstr_tmp, "e");
		imagstr_tmp[mantlen] = (char)0; // eがあったらそこで分割
		char* imagstr_exp = imagstr_tmp + mantlen + 1; //指数部へのポインタ
		if((val.Real != 0)&&(val.Imaginary > 0)) strcat(imagstr, "+"); // 混虚数で虚部がプラスなら符号をつける
		strcat(imagstr, imagstr_tmp); strcat(imagstr, "i");
		if (*imagstr_exp != '\0') { // 虚部が指数表記だったら
			strcat(imagstr, "e"); strcat(imagstr, imagstr_exp);
		}
	}
	strcat(result, realstr); strcat(result, imagstr);
	if ((val.Real == 0)&&(val.Imaginary == 0)) sprintf(result, "%g", 0.0);
	return result;
}

#ifndef MINIMAL_TEST
/* 極形式を直交形式にする */
complex polar(const double modulus, const double argument)
{
	return cmplx(
		modulus * cos(argument),
		modulus * sin(argument)
		);
}

/* 絶対値 */
double abscx(const complex arg)
{
	return sqrt(arg.Real * arg.Real + arg.Imaginary * arg.Imaginary);
}

/* 偏角 */
double argcx(const complex arg)
{
	return atan2(arg.Imaginary, arg.Real);
}

/* 複素共役 */
complex conjg(const complex arg)
{
	return cmplx(
		arg.Real,
		-arg.Imaginary
		);
}



/* 自然対数 */
complex logcx(const complex arg)
{
	return cmplx(
		log(abscx(arg)),
		argcx(arg)
		);
}

/* 対数関数 */
complex logcx(const complex arg, const complex base)
{
	return logcx(arg) / logcx(base);
}

/* 常用対数 */
complex log10cx(const complex arg)
{
	return logcx(arg, cmplx(10,0));
}

/* 指数関数 */
complex expcx(const complex arg)
{
	// exp (x + iy) = exp x(cos y + i sin y) = exp x cos y + i exp x sin y
	return cmplx(
		exp(arg.Real) * cos(arg.Imaginary),
		exp(arg.Real) * sin(arg.Imaginary)
		);
}

/* 冪乗 */
complex powcx(const complex arg, const complex exponent)
{
	// 誤差を極力抑えるために場合分け
	// a ^ z = e ^ (z ln a)
	if (exponent.Imaginary) /* 虚数乗の場合 */
		return expcx(exponent * logcx(arg));
	else if ((arg.Real < 0)&&(arg.Imaginary == 0)&&((exponent.Real * 2)==floor(exponent.Real * 2))) /* 負の実数の半整数乗 */
		switch ((int)floor(exponent.Real * 2) % 4) {
		case 0: // 実軸プラス
			return cmplx(pow(-arg.Real, exponent.Real), 0);
		case 1: // 虚軸プラス
			return cmplx(0, pow(-arg.Real, exponent.Real));
		case 2: // 実軸マイナス
			return cmplx(-pow(-arg.Real, exponent.Real), 0);
		case 3: // 虚軸マイナス
			return cmplx(0, -pow(-arg.Real, exponent.Real));
		}
	else if ((arg.Imaginary != 0)&&(arg.Real == 0)&&(exponent.Real == floor(exponent.Real))) /* 純虚数の整数乗 */
		switch ((int)floor(exponent.Real) % 4) {
		case 0: // 実軸プラス
			return cmplx(pow(arg.Imaginary, exponent.Real), 0);
		case 1: // 虚軸プラス
			return cmplx(0, pow(arg.Imaginary, exponent.Real));
		case 2: // 実軸マイナス
			return cmplx(-pow(arg.Imaginary, exponent.Real), 0);
		case 3: // 虚軸マイナス
			return cmplx(0, -pow(arg.Imaginary, exponent.Real));
		}
	else if (argcx(arg) != 0) /* 底が正の実数でない場合 */
		return polar(pow(abscx(arg), exponent.Real), argcx(arg) * exponent.Real);
	else /* 普通のライブラリ関数が使える場合 */
		return cmplx(pow(arg.Real, exponent.Real), 0);
}

/* 平方根 */
complex sqrtcx(const complex arg)
{
	return powcx(arg, cmplx(0.5, 0));
}



/* 正弦 */
complex sincx(const complex arg)
{
	// sin (x + iy) = sin x cosh y + i cos x sinh y
	return cmplx(
		sin(arg.Real) * cosh(arg.Imaginary),
		cos(arg.Real) * sinh(arg.Imaginary)
		);
}

/* 余弦 */
complex coscx(const complex arg)
{
	// cos (x + iy) = cos x cosh y + i sin x sinh y
	return cmplx(
		cos(arg.Real) * cosh(arg.Imaginary),
		sin(arg.Real) * sinh(arg.Imaginary)
		);
}

/* 正接 */
complex tancx(const complex arg)
{
	// tan x = sin x / cos x
	if (arg.Imaginary)
		return sincx(arg) / coscx(arg);
	else /* 実数の時は普通のライブラリ関数を使う */
		return cmplx(tan(arg.Real), 0);
}

/* 逆正弦 */
complex asincx(const complex arg)
{
	// arcsin z = -i log (iz + sqrt(1 - z ^ 2))
	return -I * logcx(I * arg + sqrtcx(cmplx(1,0) - arg * arg));
}

/* 逆余弦 */
complex acoscx(const complex arg)
{
	// arccos z = -i log (z + sqrt(z ^ 2 - 1))
	return -I * logcx(arg + sqrtcx(arg * arg - cmplx(1,0)));
}

/* 逆正接 */
complex atancx(const complex arg)
{
	// arctan z = 0.5i log ((1 - iz) / (1 + iz))
	return cmplx(-0.5,0) * I * logcx((cmplx(1,0) - I * arg) / (cmplx(1,0) + I * arg));
}



/* 双曲線正弦 */
complex sinhcx(const complex arg)
{
	// sinh x = (e ^ x - e ^ (-x)) / 2
	return (expcx(arg) - expcx(-arg)) / cmplx(2,0);
}

/* 双曲線余弦 */
complex coshcx(const complex arg)
{
	// cosh x = (e ^ x + e ^ (-x)) / 2
	return (expcx(arg) + expcx(-arg)) / cmplx(2,0);
}

/* 双曲線正接 */
complex tanhcx(const complex arg)
{
	// tanh θ = sinh θ / cosh θ
	if (arg.Imaginary)
		return sinhcx(arg) / coshcx(arg);
	else /* 実数の時は普通のライブラリ関数を使う */
		return cmplx(tanh(arg.Real), 0);
}

/* 双曲線逆正弦 */
complex asinhcx(const complex arg)
{
	// arsinh x = ln (x + sqrt(x ^ 2 + 1))
	return logcx(arg + sqrtcx(arg * arg + cmplx(1,0)));
}

/* 双曲線逆余弦 */
complex acoshcx(const complex arg)
{
	// arsinh x = ln (x + sqrt(x ^ 2 - 1)); x>= 1
	return logcx(arg + sqrtcx(arg * arg - cmplx(1,0)));
}

/* 双曲線逆正接 */
complex atanhcx(const complex arg)
{
	// arctanh x = 0.5 ln ((1 + x) / (1 - x)); |x| < 1
	return cmplx(0.5,0) * logcx((cmplx(1,0) + arg) / (cmplx(1,0) - arg));
}
#endif
