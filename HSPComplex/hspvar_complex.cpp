/*
	HSP Complex (����) - HSP���f���v���O�C��
*/

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "hsp3plugin.h"
#include "calculate_complex.h"
#include "hspvar_core.h"
#include "hspvar_complex.h"


// Core

static complex conv;
static char custom[64];
static short *aftertype;
static int mytype;

static PDAT *HspVarComplex_GetPtr(PVal *pval)
{
	return (PDAT *)(( (complex *)(pval->pt))+pval->offset);
}

static void *HspVarComplex_Cnv(const void *buffer, int flag)
{
	/* ���f���^�ւ̕ϊ� */
	switch (flag) {
	case HSPVAR_FLAG_INT:
		conv.Real = (double)(*(int *)buffer);
		conv.Imaginary = 0.0;
		return &conv;
	case HSPVAR_FLAG_DOUBLE:
		conv.Real = (double)(*(double *)buffer);
		conv.Imaginary = 0.0;
		return &conv;
	case HSPVAR_FLAG_STR:
		/* ������ */
		/* FALLTHRU */
	default:
		throw HSPVAR_ERROR_TYPEMISS;
	}
	return (void *)buffer;
}

static void *HspVarComplex_CnvCustom(const void *buffer, int flag)
{
	/* ���f���^����ϊ� */
	complex p;
	p = *(complex *)buffer;
	char realstr[32] = ""; char imagstr[32] = ""; char imagstr_tmp[32]; int mantlen;
	switch (flag) {
	case HSPVAR_FLAG_INT:
		if (p.Imaginary == 0) *(int *)custom = (int)p.Real;
		else throw HSPVAR_ERROR_ILLEGALPRM; // ������0�łȂ��ꍇ�̓G���[
		break;
	case HSPVAR_FLAG_DOUBLE:
		if (p.Imaginary) *(double *)custom = sqrt(-1.0); // ������������NaN��Ԃ�
		else *(double *)custom = p.Real;
		break;
	case HSPVAR_FLAG_STR:
		//*(char *)custom = *"aaaaa";
		if (p.Real != 0) sprintf(realstr, "%g", p.Real);
		if (p.Imaginary != 0) {
			memset(imagstr_tmp, 0, sizeof(imagstr_tmp)); //��Ɨp�o�b�t�@������������0�Ŗ��߂�
			sprintf(imagstr_tmp, "%g", p.Imaginary);
			mantlen = (int)strcspn(imagstr_tmp, "e");
			imagstr_tmp[mantlen] = (char)0; // e���������炻���ŕ���
			char* imagstr_exp = imagstr_tmp + mantlen + 1; //�w�����ւ̃|�C���^
			if((p.Real != 0)&&(p.Imaginary > 0)) strcat(imagstr, "+"); // �������ŋ������v���X�Ȃ畄��������
			strcat(imagstr, imagstr_tmp); strcat(imagstr, "i");
			if (*imagstr_exp != '\0') { // �������w���\�L��������
				strcat(imagstr, "e"); strcat(imagstr, imagstr_exp);
			}
		}
		if ((p.Real == 0)&&(p.Imaginary == 0)) sprintf(custom, "%g", 0.0);
		else sprintf(custom, "%s%s", realstr, imagstr);
		break;
	default:
		throw HSPVAR_ERROR_TYPEMISS;
	}
	return custom;
}


static int GetVarSize( PVal *pval )
{
	//		PVAL�|�C���^�̕ϐ����K�v�Ƃ���T�C�Y���擾����
	//		(size�t�B�[���h�ɐݒ肳���)
	//
	int size;
	size = pval->len[1];
	if ( pval->len[2] ) size*=pval->len[2];
	if ( pval->len[3] ) size*=pval->len[3];
	if ( pval->len[4] ) size*=pval->len[4];
	size *= sizeof(complex);
	return size;
}

static void HspVarComplex_Free( PVal *pval )
{
	//		PVAL�|�C���^�̕ϐ����������������
	//
	if ( pval->mode == HSPVAR_MODE_MALLOC ) { hspfree( pval->pt ); }
	pval->pt = NULL;
	pval->mode = HSPVAR_MODE_NONE;
}

static void HspVarComplex_Alloc( PVal *pval, const PVal *pval2 )
{
	//		pval�ϐ����K�v�Ƃ���T�C�Y���m�ۂ���B
	//		(pval�����łɊm�ۂ���Ă��郁��������͌Ăяo�������s�Ȃ�)
	//		(flag�̐ݒ�͌Ăяo�������s�Ȃ�)
	//		(pval2��NULL�̏ꍇ�́A�V�K�f�[�^)
	//		(pval2���w�肳��Ă���ꍇ�́Apval2�̓��e���p�����čĊm��)
	//
	int i,size;
	char *pt;
	complex *fv;
	if ( pval->len[1] < 1 ) pval->len[1] = 1;		// �z����Œ�1�͊m�ۂ���
	size = GetVarSize( pval );
	pval->mode = HSPVAR_MODE_MALLOC;
	pt = hspmalloc( size );
	fv = (complex *)pt;
	for(i=0;i<(int)(size/sizeof(complex));i++) { fv[i].Real=0.0; fv[i].Imaginary=0.0; }
	if ( pval2 != NULL ) {
		memcpy( pt, pval->pt, pval->size );
		hspfree( pval->pt );
	}
	pval->pt = pt;
	pval->size = size;
}


// Size
static int HspVarComplex_GetSize( const PDAT *pval )
{
	return sizeof(complex);
}

// Set
static void HspVarComplex_Set( PVal *pval, PDAT *pdat, const void *in )
{
	*((complex *)pdat) = *((complex *)(in));
}

// Add
static void HspVarComplex_AddI( PDAT *pval, const void *val )
{
	*((complex *)pval) = cxadd(*((complex *)pval), *((complex *)val));
	*aftertype = mytype;
}

// Sub
static void HspVarComplex_SubI( PDAT *pval, const void *val )
{
	*((complex *)pval) = cxsub(*((complex *)pval), *((complex *)val));
	*aftertype = mytype;
}

// Mul
static void HspVarComplex_MulI( PDAT *pval, const void *val )
{
	*((complex *)pval) = cxmul(*((complex *)pval), *((complex *)val));
	*aftertype = mytype;
}

// Div
static void HspVarComplex_DivI( PDAT *pval, const void *val )
{
	complex p = *((complex *)(val));
	if ((p.Real==0)&&(p.Imaginary==0)) throw( HSPVAR_ERROR_DIVZERO ); // 0���Z������G���[
	*((complex *)pval) = cxdiv(*((complex *)pval), *((complex *)val));
	*aftertype = mytype;
}

// Eq
static void HspVarComplex_EqI( PDAT *pval, const void *val )
{
	*((int *)pval) = (
		(((complex *)pval)->Real == ((complex *)val)->Real)
		&&(((complex *)pval)->Imaginary == ((complex *)val)->Imaginary)
		);
	*aftertype = HSPVAR_FLAG_INT;
}

// Ne
static void HspVarComplex_NeI( PDAT *pval, const void *val )
{
	*((int *)pval) = (
		(((complex *)pval)->Real != ((complex *)val)->Real)
		||(((complex *)pval)->Imaginary != ((complex *)val)->Imaginary)
		);
	*aftertype = HSPVAR_FLAG_INT;
}

// INVALID
static void HspVarComplex_Invalid( PDAT *pval, const void *val )
{
	throw( HSPVAR_ERROR_INVALID );
}

static void *GetBlockSize( PVal *pval, PDAT *pdat, int *size )
{
	*size = pval->size - ( ((char *)pdat) - pval->pt );
	return (pdat);
}

static void AllocBlock( PVal *pval, PDAT *pdat, int size )
{
}

/*------------------------------------------------------------*/

EXPORT int HspVarComplex_typeid( void )
{
	return mytype;
}

EXPORT void HspVarComplex_Init( HspVarProc *p )
{
	aftertype = &p->aftertype;

	p->Set = HspVarComplex_Set;
	p->Cnv = HspVarComplex_Cnv;
	p->GetPtr = HspVarComplex_GetPtr;
	p->CnvCustom = HspVarComplex_CnvCustom;
	p->GetSize = HspVarComplex_GetSize;
	p->GetBlockSize = GetBlockSize;
	p->AllocBlock = AllocBlock;

//	p->ArrayObject = HspVarFloat_ArrayObject;
	p->Alloc = HspVarComplex_Alloc;
	p->Free = HspVarComplex_Free;

	p->AddI = HspVarComplex_AddI;
	p->SubI = HspVarComplex_SubI;
	p->MulI = HspVarComplex_MulI;
	p->DivI = HspVarComplex_DivI;
	p->ModI = HspVarComplex_Invalid;

	p->AndI = HspVarComplex_Invalid;
	p->OrI  = HspVarComplex_Invalid;
	p->XorI = HspVarComplex_Invalid;

	p->EqI = HspVarComplex_EqI;
	p->NeI = HspVarComplex_NeI;

	// ���f���ł͑召�֌W����`����Ȃ�
	p->GtI = HspVarComplex_Invalid;
	p->LtI = HspVarComplex_Invalid;
	p->GtEqI = HspVarComplex_Invalid;
	p->LtEqI = HspVarComplex_Invalid;

	p->RrI = HspVarComplex_Invalid;
	p->LrI = HspVarComplex_Invalid;

	p->vartype_name = "complex";				// �^�C�v��
	p->version = 0x001;					// �^�^�C�v�����^�C���o�[�W����(0x100 = 1.0)
	p->support = HSPVAR_SUPPORT_STORAGE|HSPVAR_SUPPORT_FLEXARRAY;
										// �T�|�[�g�󋵃t���O(HSPVAR_SUPPORT_*)
	p->basesize = sizeof(complex);		// �P�̃f�[�^���g�p����T�C�Y(byte) / �ϒ��̎���-1
	mytype = p->flag;
}

/*------------------------------------------------------------*/


// ���f���̈������擾����
complex excomplex(void)
{
	complex Ret; Ret.Real = 0; Ret.Imaginary = 0;
	complex *Tmp;
	int prmStat = code_getprm();
	switch (prmStat) {
	case PARAM_OK: case PARAM_SPLIT:
		if (mpval->flag != HspVarComplex_typeid()) throw HSPVAR_ERROR_TYPEMISS;
		else {
			Tmp = (complex *)mpval->pt;
			Ret.Real = Tmp->Real; Ret.Imaginary = Tmp->Imaginary;
		}
		break;
	default:
		throw 5; //�������ȗ�����Ă���
	}
	return Ret;
}
