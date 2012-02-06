#define _USE_MATH_DEFINES

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <stdlib.h>
#include "hspvar_complex.h"
#include "calculate_complex.h"

/* NaN��Ԃ������̂�VC++���Ƃ��������֐���`����ĂȂ����Ăǂ��������Ƃł����[ */
const double NaN = ::sqrt(-1.0);

complex cmplx(const double Real, const double Imaginary) {
	complex val;
	val.Real = Real; val.Imaginary = Imaginary;
	return val;
}

/* ���Z�q�I�[�o�[���[�h�Ŏ������悤�Ƃ���ƃo�O���Ă��܂����̂Ŋ֐��Ŏ��� */
/* �|�[�����h�L�@������Ă����̂���[ �ނ����[ */
complex cxadd(const complex arg1, const complex arg2)
{
	// (a + bi) + (c + di) = (a + c) + (b + d)i
	return cmplx(
		arg1.Real + arg2.Real,
		arg1.Imaginary + arg2.Imaginary
		);
}
complex cxsub(const complex arg1, const complex arg2)
{
	// (a + bi) - (c + di) = (a - c) + (b - d)i
	return cmplx(
		arg1.Real - arg2.Real,
		arg1.Imaginary - arg2.Imaginary
		);
}
complex cxmul(const complex arg1, const complex arg2)
{
	// (a + bi) (c + di) = (ac - bd) + (bc + ad)i
	return cmplx(
		arg1.Real * arg2.Real - arg1.Imaginary * arg2.Imaginary,
		arg1.Imaginary * arg2.Real + arg1.Real * arg2.Imaginary
		);
}
complex cxdiv(const complex arg1, const complex arg2)
{
	// (a + bi) / (c + di) = (ac + bd) / (c^2 + d^2) + (bc - ad)i / (c^2 + d^2)
	return cmplx(
		(arg1.Real * arg2.Real + arg1.Imaginary * arg2.Imaginary) /
			(arg2.Real * arg2.Real + arg2.Imaginary * arg2.Imaginary),
		(arg1.Imaginary * arg2.Real - arg1.Real * arg2.Imaginary) /
			(arg2.Real * arg2.Real + arg2.Imaginary * arg2.Imaginary)
		);
}



/* �Ɍ`���𒼌��`���ɂ��� */
complex polar(const double modulus, const double argument)
{
	return cmplx(
		modulus * cos(argument),
		modulus * sin(argument)
		);
}

/* ��Βl */
double abscx(const complex arg)
{
	return sqrt(arg.Real * arg.Real + arg.Imaginary * arg.Imaginary);
}

/* �Ίp */
double argcx(const complex arg)
{
	return atan2(arg.Imaginary, arg.Real);
}

/* ���f���� */
complex conjg(const complex arg)
{
	return cmplx(
		arg.Real,
		-arg.Imaginary
		);
}



/* ���R�ΐ� */
complex logcx(const complex arg)
{
	return cmplx(
		log(abscx(arg)),
		argcx(arg)
		);
}

/* �ΐ��֐� */
complex logcx(const complex arg, const complex base)
{
	return cxdiv(logcx(arg), logcx(base));
}

/* ��p�ΐ� */
complex log10cx(const complex arg)
{
	return logcx(arg, cmplx(10,0));
}

/* �w���֐� */
complex expcx(const complex arg)
{
	// exp (x + iy) = exp x(cos y + i sin y) = exp x cos y + i exp x sin y
	return cmplx(
		exp(arg.Real) * cos(arg.Imaginary),
		exp(arg.Real) * sin(arg.Imaginary)
		);
}

/* �p�� */
complex powcx(const complex arg, const complex exponent)
{
	// �덷���ɗ͗}���邽�߂ɏꍇ����
	// a ^ z = e ^ (z ln a)
	if (exponent.Imaginary) /* ������̏ꍇ */
		return expcx(cxmul(exponent, logcx(arg)));
	else if ((arg.Real < 0)&&(arg.Imaginary == 0)&&((exponent.Real * 2)==floor(exponent.Real * 2))) /* ���̎����̔������� */
		switch ((int)floor(exponent.Real * 2) % 4) {
		case 0: // �����v���X
			return cmplx(pow(-arg.Real, exponent.Real), 0);
		case 1: // �����v���X
			return cmplx(0, pow(-arg.Real, exponent.Real));
		case 2: // �����}�C�i�X
			return cmplx(-pow(-arg.Real, exponent.Real), 0);
		case 3: // �����}�C�i�X
			return cmplx(0, -pow(-arg.Real, exponent.Real));
		default: // ���s����Ȃ��͂��̃R�[�h
			return *((complex *)NULL);
		}
	else if ((arg.Imaginary != 0)&&(arg.Real == 0)&&(exponent.Real == floor(exponent.Real))) /* �������̐����� */
		switch ((int)floor(exponent.Real) % 4) {
		case 0: // �����v���X
			return cmplx(pow(arg.Imaginary, exponent.Real), 0);
		case 1: // �����v���X
			return cmplx(0, pow(arg.Imaginary, exponent.Real));
		case 2: // �����}�C�i�X
			return cmplx(-pow(arg.Imaginary, exponent.Real), 0);
		case 3: // �����}�C�i�X
			return cmplx(0, -pow(arg.Imaginary, exponent.Real));
		default: // ���s����Ȃ��͂��̃R�[�h
			return *((complex *)NULL);
		}
	else if (argcx(arg) != 0) /* �ꂪ���̎����łȂ��ꍇ */
		return polar(pow(abscx(arg), exponent.Real), argcx(arg) * exponent.Real);
	else /* ���ʂ̃��C�u�����֐����g����ꍇ */
		return cmplx(pow(arg.Real, exponent.Real), 0);
}

/* ������ */
complex sqrtcx(const complex arg)
{
	return powcx(arg, cmplx(0.5, 0));
}



/* ���� */
complex sincx(const complex arg)
{
	// sin (x + iy) = sin x cosh y + i cos x sinh y
	return cmplx(
		sin(arg.Real) * cosh(arg.Imaginary),
		cos(arg.Real) * sinh(arg.Imaginary)
		);
}

/* �]�� */
complex coscx(const complex arg)
{
	// cos (x + iy) = cos x cosh y - i sin x sinh y
	return cmplx(
		cos(arg.Real) * cosh(arg.Imaginary),
		-sin(arg.Real) * sinh(arg.Imaginary)
		);
}

/* ���� */
complex tancx(const complex arg)
{
	// tan x = sin x / cos x
	if (arg.Imaginary)
		return cxdiv(sincx(arg), coscx(arg));
	else /* �����̎��͕��ʂ̃��C�u�����֐����g�� */
		return cmplx(tan(arg.Real), 0);
}

/* �t���� */
complex asincx(const complex arg)
{
	// arcsin z = -i log (iz + sqrt(1 - z ^ 2))
	// [PN] *(-i, log(+(*(i, z), sqrt(-(1, *(z, z))))))
	return cxmul(cmplx(0, -1),
		logcx(cxadd(cxmul(cmplx(0,1), arg),sqrtcx(cxsub(cmplx(1,0), cxmul(arg, arg)))))
		);
}

/* �t�]�� */
complex acoscx(const complex arg)
{
	// arccos z = pi / 2 - arcsin(z)
	return cxsub(cmplx(M_PI / 2.0, 0), asincx(arg));
}

/* �t���� */
complex atancx(const complex arg)
{
	// arctan z = 0.5i log ((1 - iz) / (1 + iz))
	// [PN] *(0.5i, log(/(-(1, *(z, i)), +(1, *(z, i)))))
	return cxmul(
		cmplx(0,0.5),
		logcx(cxdiv(cxsub(cmplx(1,0), cxmul(arg, cmplx(0,1))),
		cxadd(cmplx(1,0), cxmul(arg, cmplx(0,1)))))
		);
}



/* �o�Ȑ����� */
complex sinhcx(const complex arg)
{
	// sinh x = (e ^ x - e ^ (-x)) / 2
	return cxdiv(cxsub(expcx(arg), expcx(cxmul(cmplx(-1,0),arg))), cmplx(2,0));
}

/* �o�Ȑ��]�� */
complex coshcx(const complex arg)
{
	// cosh x = (e ^ x + e ^ (-x)) / 2
	return cxdiv(cxadd(expcx(arg), expcx(cxmul(cmplx(-1,0),arg))), cmplx(2,0));
}

/* �o�Ȑ����� */
complex tanhcx(const complex arg)
{
	// tanh �� = sinh �� / cosh ��
	if (arg.Imaginary)
		return cxdiv(sinhcx(arg), coshcx(arg));
	else /* �����̎��͕��ʂ̃��C�u�����֐����g�� */
		return cmplx(tanh(arg.Real), 0);
}

/* �o�Ȑ��t���� */
complex asinhcx(const complex arg)
{
	// arsinh x = ln (x + sqrt(x ^ 2 + 1))
	return logcx(cxadd(arg, sqrtcx(cxadd(cxmul(arg, arg), cmplx(1,0)))));
}

/* �o�Ȑ��t�]�� */
complex acoshcx(const complex arg)
{
	// arcosh x = ln (x + sqrt(x + 1) * sqrt(x - 1))
	return logcx(cxadd(arg, cxmul(sqrtcx(cxadd(arg, cmplx(1,0))), sqrtcx(cxsub(arg, cmplx(1,0))))));
}

/* �o�Ȑ��t���� */
complex atanhcx(const complex arg)
{
	// arctanh x = 0.5 ln ((1 + x) / (1 - x)); |x| < 1
	return cxmul(cmplx(0.5,0), logcx(cxdiv(cxadd(cmplx(1,0), arg), cxsub(cmplx(1,0), arg))));
}

/* Ver0.3�ǉ� */
/* C99��cproj�݂����Ȃ��� */
complex projection(const complex arg)
{
	if ((!_finite(arg.Real))&&(!_isnan(arg.Real))&&(!_finite(arg.Imaginary))&&(!_isnan(arg.Imaginary)))
	{
		return cmplx(DBL_MAX*1000, 0);
	}
	else {
		return arg;
	}
}

/* Ver0.3�ǉ� */
/* �K���}�֐� */
complex gamma(const complex arg)
{
	int k; double x;
	if ((arg.Imaginary == 0)&&(arg.Real == floor(arg.Real))) {
		/* �����̏ꍇ */
		if (arg.Real >= 170) {
			return cmplx(DBL_MAX*1000, 0); /* Overflow if >= 170 */
		} else if (arg.Real > 0) {
			/* ���������̎� */
			x = 1.0;
			for (k = 1; k < arg.Real; k++) x *= (double)k;
			return cmplx(x, 0);
		} else if ((int)floor(arg.Real) % 2) {
			/* ���������Ŋ�̎� */
			return cmplx(-DBL_MAX*1000, 0);
		} else {
			/* ���������ŋ����̎� */
			return cmplx(DBL_MAX*1000, 0);
		}
	} else if (arg.Real < 0.5) {
		/* �Ίp���傫���ꍇ */
		/* �I�C���[�̔��ˌ����ɂ��덷���C������ */
		/* ��(z)��(1-z) = pi / sin(pi * z) */
		/* ��(z) = (pi / sin(pi * z)) / ��(1-z) */
		return cxdiv(
			cxdiv(cmplx(M_PI,0),sincx(cxmul(cmplx(M_PI,0),arg))),
			gamma(cxsub(cmplx(1,0),arg))
			);
	} else {
		/* ��ʉ� */
		/* �X�^�[�����O�̌����̉��p���g�� */
		/* ��(z) �� sqrt(2 * pi / z) * (z / e * sqrt(z * sinh(1 / z) + 1 / (810 * z^6)))^z */
		return cxmul(
			sqrtcx(cxdiv(cmplx(2.0 * M_PI,0), arg)),
			powcx(cxmul(
				cxdiv(arg, cmplx(M_E, 0)),
				sqrtcx(cxadd(
					cxmul(arg, sinhcx(cxdiv(cmplx(1,0),arg))),
					cxdiv(cmplx(1,0), cxmul(cmplx(810,0),powcx(arg,cmplx(6,0)))))
					)
				),
			arg)
			);

	}
}
