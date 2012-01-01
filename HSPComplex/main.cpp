
//
//		HSP���f���v���O�C���ifloat�^�̃T���v����������j
//

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hsp3plugin.h"

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

	int answerType = 0; int prmType;

	//			'('�Ŏn�܂邩�𒲂ׂ�
	//
	if ( *type != TYPE_MARK ) puterror( HSPERR_INVALID_FUNCPARAM );
	if ( *val != '(' ) puterror( HSPERR_INVALID_FUNCPARAM );
	code_next();


	switch( cmd ) {							// �T�u�R�}���h���Ƃ̕���

	case 0x00:								// complex�֐�
		dp1 = code_getdd(0);				// �����l���擾
		dp2 = code_getdd(0);
		ref_cval.Real = dp1;				// �Ԓl��ݒ�
		ref_cval.Imaginary = dp2;
		answerType = 1;						// �Ԃ�l�͕��f��
		break;

	case 0x01:								// abscx�֐�
		// r = sqrt(x^2 + y^2)
		cp1 = excomplex();					// ���f���l���擾
		ref_dval = sqrt(cp1.Real * cp1.Real + cp1.Imaginary * cp1.Imaginary);
		answerType = 0;						// �Ԃ�l�͎�����
		break;

	case 0x02:								// argcx�֐�
		// theta = arctan(y / x)
		cp1 = excomplex();
		ref_dval = atan2(cp1.Imaginary, cp1.Real);
		answerType = 0;
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

