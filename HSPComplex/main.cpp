
//
//		HSP���f���v���O�C���ifloat�^�̃T���v����������j
//

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hsp3plugin.h"

#include "calculate_complex.h"
#include "hspvar_complex.h"

 /*------------------------------------------------------------*/
/*
		controller
*/
/*------------------------------------------------------------*/

static complex ref_cval; static double ref_dval;		// �Ԓl�̂��߂̕ϐ�
static double dp1, dp2; static complex cp1, cp2;

static void *reffunc( int *type_res, int cmd )
{
	//		�֐��E�V�X�e���ϐ��̎��s���� (�l�̎Q�Ǝ��ɌĂ΂�܂�)
	//

	int answerType = 0;

	//			'('�Ŏn�܂邩�𒲂ׂ�
	//
	if ( *type != TYPE_MARK ) puterror( HSPERR_INVALID_FUNCPARAM );
	if ( *val != '(' ) puterror( HSPERR_INVALID_FUNCPARAM );
	code_next();


	switch( cmd ) {							// �T�u�R�}���h���Ƃ̕���

	case 0x00:								// complex�֐�
		dp1 = code_getdd(0.0);				// �����l���擾
		dp2 = code_getdd(0.0);
		ref_cval = cmplx(dp1, dp2);		// �Ԓl��ݒ�
		answerType = 1;						// �Ԃ�l�͕��f��
		break;

	case 0x01:								// cabs�֐�
		// r = sqrt(x^2 + y^2)
		cp1 = excomplex();					// ���f���l���擾
		ref_dval = abscx(cp1);
		answerType = 0;						// �Ԃ�l�͎�����
		break;

	case 0x02:								// carg�֐�
		// theta = arctan(y / x)
		cp1 = excomplex();
		ref_dval = argcx(cp1);
		answerType = 0;
		break;

	case 0x03:								// conj�֐�
		// ���𕡑f����Ԃ�
		cp1 = excomplex();
		ref_cval = conjg(cp1);
		answerType = 1;
		break;

	case 0x04:								// cpolar�֐�
		// �Ɍ`���œ���
		dp1 = code_getdd(0.0);
		dp2 = code_getdd(0.0);
		ref_cval = polar(dp1, dp2);
		answerType = 1;
		break;

	case 0x05:								// creal�֐�
		// ����
		cp1 = excomplex();
		ref_dval = cp1.Real;
		answerType = 0;
		break;

	case 0x06:								// cimag�֐�
		// ����
		cp1 = excomplex();
		ref_dval = cp1.Imaginary;
		answerType = 0;
		break;

	case 0x10:								// clog�֐�
		// ���f���R�ΐ��֐�
		cp1 = excomplex();
		ref_cval = logcx(cp1);
		answerType = 1;
		break;

	case 0x11:								// clog10�֐�
		// ���f��p�ΐ��֐�
		cp1 = excomplex();
		ref_cval = log10cx(cp1);
		answerType = 1;
		break;

	case 0x14:								// cexp�֐�
		// ���f�w���֐�
		cp1 = excomplex();
		ref_cval = expcx(cp1);
		answerType = 1;
		break;

	case 0x15:								// cpow�֐�
		// ���f�p�֐�
		cp1 = excomplex();
		cp2 = excomplex();
		ref_cval = powcx(cp1, cp2);
		answerType = 1;
		break;

	case 0x16:								// csqrt�֐�
		// ���f�������֐�
		cp1 = excomplex();
		ref_cval = sqrtcx(cp1);
		answerType = 1;
		break;

	case 0x20:								// csin�֐�
		// ����
		cp1 = excomplex();
		ref_cval = sincx(cp1);
		answerType = 1;
		break;

	case 0x21:								// ccos�֐�
		// �]��
		cp1 = excomplex();
		ref_cval = coscx(cp1);
		answerType = 1;
		break;

	case 0x22:								// ctan�֐�
		// ����
		cp1 = excomplex();
		ref_cval = tancx(cp1);
		answerType = 1;
		break;

	case 0x28:								// casin�֐�
		// �t����
		cp1 = excomplex();
		ref_cval = asincx(cp1);
		answerType = 1;
		break;

	case 0x29:								// cacos�֐�
		// �t�]��
		cp1 = excomplex();
		ref_cval = acoscx(cp1);
		answerType = 1;
		break;

	case 0x2A:								// catan�֐�
		// �t����
		cp1 = excomplex();
		ref_cval = atancx(cp1);
		answerType = 1;
		break;

	case 0x30:								// csinh�֐�
		// �o�Ȑ�����
		cp1 = excomplex();
		ref_cval = sinhcx(cp1);
		answerType = 1;
		break;

	case 0x31:								// ccosh�֐�
		// �o�Ȑ��]��
		cp1 = excomplex();
		ref_cval = coshcx(cp1);
		answerType = 1;
		break;

	case 0x32:								// ctanh�֐�
		// �o�Ȑ�����
		cp1 = excomplex();
		ref_cval = tanhcx(cp1);
		answerType = 1;
		break;

	case 0x38:								// casinh�֐�
		// �o�Ȑ��t����
		cp1 = excomplex();
		ref_cval = asinhcx(cp1);
		answerType = 1;
		break;

	case 0x39:								// cacosh�֐�
		// �o�Ȑ��t�]��
		cp1 = excomplex();
		ref_cval = acoshcx(cp1);
		answerType = 1;
		break;

	case 0x3A:								// catanh�֐�
		// �o�Ȑ��t����
		cp1 = excomplex();
		ref_cval = atanhcx(cp1);
		answerType = 1;
		break;

	case 0x40:								// cproj�֐�
		cp1 = excomplex();
		ref_cval = projection(cp1);
		answerType = 1;
		break;

	case 0x41:								// cgamma�֐�
		cp1 = excomplex();
		ref_cval = gamma(cp1);
		answerType = 1;
		break;

	default:
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
	}

	//			'('�ŏI��邩�𒲂ׂ�
	//
	if ( *type != TYPE_MARK ) puterror( HSPERR_INVALID_FUNCPARAM );
	if ( *val != ')' ) puterror( HSPERR_INVALID_FUNCPARAM );
	code_next();

	if (answerType) {
		*type_res = HspVarComplex_typeid();		// �Ԓl�̃^�C�v���w�肷��
		return (void *)&ref_cval;
	} else {
		*type_res = HSPVAR_FLAG_DOUBLE;		// �Ԓl�̃^�C�v���w�肷��
		return (void *)&ref_dval;
	}
}


/*------------------------------------------------------------*/

static int termfunc( int option )
{
	//		�I������ (�A�v���P�[�V�����I�����ɌĂ΂�܂�)
	//
	return 0;
}


/*------------------------------------------------------------*/
/*
		interface
*/
/*------------------------------------------------------------*/

int WINAPI DllMain (HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved)
{
	//		DLL�G���g���[ (��������K�v�͂���܂���)
	//
	return TRUE;
}


EXPORT void WINAPI hsp3cmdinit( HSP3TYPEINFO *info )
{
	//		�v���O�C�������� (���s�E�I��������o�^���܂�)
	//
	hsp3sdk_init( info );		// SDK�̏�����(�ŏ��ɍs�Ȃ��ĉ�����)

	info->reffunc = reffunc;		// �Q�Ɗ֐�(reffunc)�̓o�^
	info->termfunc = termfunc;		// �I���֐�(termfunc)�̓o�^

	registvar( -1, HspVarComplex_Init );		// �V�����^�̒ǉ�
}

/*------------------------------------------------------------*/

