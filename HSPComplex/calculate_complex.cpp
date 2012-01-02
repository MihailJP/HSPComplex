#include <math.h>
#include "hspvar_complex.h"
#include "calculate_complex.h"

/* NaN��Ԃ������̂�VC++���Ƃ��������֐���`����ĂȂ����Ăǂ��������Ƃł����[ */
const double NaN = ::sqrt(-1.0);

/* ���Z�q�I�[�o�[���[�h */

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
complex complex::operator+() {
	return complex(A, B);
}
complex complex::operator-() {
	return complex(-A, -B);
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

/* �L���X�g���Z�q�̃I�[�o�[���[�h */
complex::operator double() {
	if (Imaginary) return NaN;
	else return Real;
}




/* �֐� */

#define I (complex(0,1))

/* �Ɍ`���𒼌��`���ɂ��� */
complex polar(static double modulus, static double argument)
{
	return complex(
		modulus * cos(argument),
		modulus * sin(argument)
		);
}

/* ��Βl */
double abscx(static complex arg)
{
	return sqrt(arg.Real * arg.Real + arg.Imaginary * arg.Imaginary);
}

/* �Ίp */
double argcx(static complex arg)
{
	return atan2(arg.Imaginary, arg.Real);
}

/* ���f���� */
complex conjg(static complex arg)
{
	return complex(
		arg.Real,
		-arg.Imaginary
		);
}



/* ���R�ΐ� */
complex logcx(static complex arg)
{
	return complex(
		log(abscx(arg)),
		argcx(arg)
		);
}

/* �ΐ��֐� */
complex logcx(static complex arg, static complex base)
{
	return logcx(arg) / logcx(base);
}

/* ��p�ΐ� */
complex log10cx(static complex arg)
{
	return logcx(arg, complex(10,0));
}

/* �w���֐� */
complex expcx(static complex arg)
{
	// exp (x + iy) = exp x(cos y + i sin y) = exp x cos y + i exp x sin y
	return complex(
		exp(arg.Real) * cos(arg.Imaginary),
		exp(arg.Real) * sin(arg.Imaginary)
		);
}

/* �p�� */
complex powcx(static complex arg, static complex exponent)
{
	// �덷���ɗ͗}���邽�߂ɏꍇ����
	// a ^ z = e ^ (z ln a)
	if (exponent.Imaginary) /* ������̏ꍇ */
		return expcx(exponent * logcx(arg));
	else if ((arg.Real < 0)&&(arg.Imaginary == 0)&&((exponent.Real * 2)==floor(exponent.Real * 2))) /* ���̎����̔������� */
		switch ((int)floor(exponent.Real * 2) % 4) {
		case 0: // �����v���X
			return complex(pow(-arg.Real, exponent.Real), 0);
		case 1: // �����v���X
			return complex(0, pow(-arg.Real, exponent.Real));
		case 2: // �����}�C�i�X
			return complex(-pow(-arg.Real, exponent.Real), 0);
		case 3: // �����}�C�i�X
			return complex(0, -pow(-arg.Real, exponent.Real));
		}
	else if ((arg.Imaginary != 0)&&(arg.Real == 0)&&(exponent.Real == floor(exponent.Real))) /* �������̐����� */
		switch ((int)floor(exponent.Real) % 4) {
		case 0: // �����v���X
			return complex(pow(arg.Imaginary, exponent.Real), 0);
		case 1: // �����v���X
			return complex(0, pow(arg.Imaginary, exponent.Real));
		case 2: // �����}�C�i�X
			return complex(-pow(arg.Imaginary, exponent.Real), 0);
		case 3: // �����}�C�i�X
			return complex(0, -pow(arg.Imaginary, exponent.Real));
		}
	else if (argcx(arg) != 0) /* �ꂪ���̎����łȂ��ꍇ */
		return polar(pow(abscx(arg), exponent.Real), argcx(arg) * exponent.Real);
	else /* ���ʂ̃��C�u�����֐����g����ꍇ */
		return complex(pow(arg.Real, exponent.Real), 0);
}

/* ������ */
complex sqrtcx(static complex arg)
{
	return powcx(arg, complex(0.5, 0));
}



/* ���� */
complex sincx(static complex arg)
{
	// sin (x + iy) = sin x cosh y + i cos x sinh y
	return complex(
		sin(arg.Real) * cosh(arg.Imaginary),
		cos(arg.Real) * sinh(arg.Imaginary)
		);
}

/* �]�� */
complex coscx(static complex arg)
{
	// cos (x + iy) = cos x cosh y + i sin x sinh y
	return complex(
		cos(arg.Real) * cosh(arg.Imaginary),
		sin(arg.Real) * sinh(arg.Imaginary)
		);
}

/* ���� */
complex tancx(static complex arg)
{
	// tan x = sin x / cos x
	if (arg.Imaginary)
		return sincx(arg) / coscx(arg);
	else /* �����̎��͕��ʂ̃��C�u�����֐����g�� */
		return complex(tan(arg.Real), 0);
}

/* �t���� */
complex asincx(static complex arg)
{
	// arcsin z = -i log (iz + sqrt(1 - z ^ 2))
	return -I * logcx(I * arg + sqrtcx(1 - arg * arg));
}

/* �t�]�� */
complex acoscx(static complex arg)
{
	// arccos z = -i log (z + sqrt(z ^ 2 - 1))
	return -I * logcx(arg + sqrtcx(arg * arg - 1));
}

/* �t���� */
complex atancx(static complex arg)
{
	// arctan z = 0.5i log ((1 - iz) / (1 + iz))
	return -0.5 * I * logcx((1 - I * arg) / (1 + I * arg));
}



/* �o�Ȑ����� */
complex sinhcx(static complex arg)
{
	// sinh x = (e ^ x - e ^ (-x)) / 2
	return (expcx(arg) - expcx(-arg)) / 2;
}

/* �o�Ȑ��]�� */
complex coshcx(static complex arg)
{
	// cosh x = (e ^ x + e ^ (-x)) / 2
	return (expcx(arg) + expcx(-arg)) / 2;
}

/* �o�Ȑ����� */
complex tanhcx(static complex arg)
{
	// tanh �� = sinh �� / cosh ��
	if (arg.Imaginary)
		return sinhcx(arg) / coshcx(arg);
	else /* �����̎��͕��ʂ̃��C�u�����֐����g�� */
		return complex(tanh(arg.Real), 0);
}

/* �o�Ȑ��t���� */
complex asinhcx(static complex arg)
{
	// arsinh x = ln (x + sqrt(x ^ 2 + 1))
	return logcx(arg + sqrtcx(arg * arg + 1));
}

/* �o�Ȑ��t�]�� */
complex acoshcx(static complex arg)
{
	// arsinh x = ln (x + sqrt(x ^ 2 - 1)); x>= 1
	return logcx(arg + sqrtcx(arg * arg - 1));
}

/* �o�Ȑ��t���� */
complex atanhcx(static complex arg)
{
	// arctanh x = 0.5 ln ((1 + x) / (1 - x)); |x| < 1
	return 0.5 * logcx((1 + arg) / (1 - arg));
}
