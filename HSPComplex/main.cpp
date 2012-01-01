/*
	HSP Complex (����) - HSP���f���v���O�C��
*/

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>
#include <string.h>

#include "hsp3plugin.h"
#include "hspvar_core.h"


// Core

struct complex
{
	double Real;
	double Imaginary;
};
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
		conv.Imaginary = 0;
		return &conv;
	case HSPVAR_FLAG_DOUBLE:
		conv.Real = (double)(*(double *)buffer);
		conv.Imaginary = 0;
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
		if (p.Imaginary == 0) *(double *)custom = (double)p.Real;
		else *(long long *)custom = 0x7fffffffffffffff; // �����NaN�ɂȂ�͂��c�c
		break;
	case HSPVAR_FLAG_STR:
		if (p.Real) sprintf(realstr, "%g", p.Real);
		if (p.Imaginary) {
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
		strcat(custom, realstr); strcat(custom, imagstr);
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

#define A (((complex *)pval)->Real)
#define B (((complex *)pval)->Imaginary)
#define C (((complex *)val)->Real)
#define D (((complex *)val)->Imaginary)

// Add
static void HspVarComplex_AddI( PDAT *pval, const void *val )
{
	// (a + bi) + (c + di) = (a + c) + (b + d)i
	A += C;
	B += D;
	*aftertype = mytype;
}

// Sub
static void HspVarComplex_SubI( PDAT *pval, const void *val )
{
	// (a + bi) - (c + di) = (a - c) + (b - d)i
	A -= C;
	B -= D;
	*aftertype = mytype;
}

// Mul
static void HspVarComplex_MulI( PDAT *pval, const void *val )
{
	// (a + bi) (c + di) = (ac - bd) + (bc + ad)i
	A = A * C - B * D;
	B = B * C + A * D;
	*aftertype = mytype;
}

// Div
static void HspVarComplex_DivI( PDAT *pval, const void *val )
{
	// (a + bi) / (c + di) = (ac + bd) / (c^2 + d^2) + (bc - ad)i / (c^2 + d^2)
	complex p = *((complex *)(val));
	if ((p.Real==0)&&(p.Imaginary==0)) throw( HSPVAR_ERROR_DIVZERO ); // 0���Z������G���[
	A = (A * C + B * D) / (C * C + D * D);
	B = (B * C - A * D) / (C * C + D * D);
	*aftertype = mytype;
}
