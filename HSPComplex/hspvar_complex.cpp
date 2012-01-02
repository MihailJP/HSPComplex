/*
	HSP Complex (仮称) - HSP複素数プラグイン
*/

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
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
	/* 複素数型への変換 */
	switch (flag) {
	case HSPVAR_FLAG_INT:
		conv = complex((double)(*(int *)buffer), 0);
		return &conv;
	case HSPVAR_FLAG_DOUBLE:
		conv = complex((double)(*(int *)buffer), 0);
		return &conv;
	case HSPVAR_FLAG_STR:
		/* 未実装 */
		/* FALLTHRU */
	default:
		throw HSPVAR_ERROR_TYPEMISS;
	}
	return (void *)buffer;
}

static void *HspVarComplex_CnvCustom(const void *buffer, int flag)
{
	/* 複素数型から変換 */
	complex p;
	p = *(complex *)buffer;
	char realstr[32] = ""; char imagstr[32] = ""; char imagstr_tmp[32]; int mantlen;
	switch (flag) {
	case HSPVAR_FLAG_INT:
		if (p.Imaginary == 0) *(int *)custom = (int)p.Real;
		else throw HSPVAR_ERROR_ILLEGALPRM; // 虚部が0でない場合はエラー
		break;
	case HSPVAR_FLAG_DOUBLE:
		*(double *)custom = (double)p;
		break;
	case HSPVAR_FLAG_STR:
		*(char *)custom = (char)p;
		break;
	default:
		throw HSPVAR_ERROR_TYPEMISS;
	}
	return custom;
}


static int GetVarSize( PVal *pval )
{
	//		PVALポインタの変数が必要とするサイズを取得する
	//		(sizeフィールドに設定される)
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
	//		PVALポインタの変数メモリを解放する
	//
	if ( pval->mode == HSPVAR_MODE_MALLOC ) { hspfree( pval->pt ); }
	pval->pt = NULL;
	pval->mode = HSPVAR_MODE_NONE;
}

static void HspVarComplex_Alloc( PVal *pval, const PVal *pval2 )
{
	//		pval変数が必要とするサイズを確保する。
	//		(pvalがすでに確保されているメモリ解放は呼び出し側が行なう)
	//		(flagの設定は呼び出し側が行なう)
	//		(pval2がNULLの場合は、新規データ)
	//		(pval2が指定されている場合は、pval2の内容を継承して再確保)
	//
	int i,size;
	char *pt;
	complex *fv;
	if ( pval->len[1] < 1 ) pval->len[1] = 1;		// 配列を最低1は確保する
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
	*((complex *)pval) += *((complex *)val);
	*aftertype = mytype;
}

// Sub
static void HspVarComplex_SubI( PDAT *pval, const void *val )
{
	*((complex *)pval) -= *((complex *)val);
	*aftertype = mytype;
}

// Mul
static void HspVarComplex_MulI( PDAT *pval, const void *val )
{
	*((complex *)pval) *= *((complex *)val);
	*aftertype = mytype;
}

// Div
static void HspVarComplex_DivI( PDAT *pval, const void *val )
{
	complex p = *((complex *)(val));
	if ((p.Real==0)&&(p.Imaginary==0)) throw( HSPVAR_ERROR_DIVZERO ); // 0除算したらエラー
	*((complex *)pval) /= *((complex *)val);
	*aftertype = mytype;
}

// Eq
static void HspVarComplex_EqI( PDAT *pval, const void *val )
{
	*((int *)pval) = ( *((complex *)pval) == *((complex *)val) );
	*aftertype = HSPVAR_FLAG_INT;
}

// Ne
static void HspVarComplex_NeI( PDAT *pval, const void *val )
{
	*((int *)pval) = ( *((complex *)pval) != *((complex *)val) );
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

	// 複素数では大小関係が定義されない
	p->GtI = HspVarComplex_Invalid;
	p->LtI = HspVarComplex_Invalid;
	p->GtEqI = HspVarComplex_Invalid;
	p->LtEqI = HspVarComplex_Invalid;

	p->RrI = HspVarComplex_Invalid;
	p->LrI = HspVarComplex_Invalid;

	p->vartype_name = "complex";				// タイプ名
	p->version = 0x001;					// 型タイプランタイムバージョン(0x100 = 1.0)
	p->support = HSPVAR_SUPPORT_STORAGE|HSPVAR_SUPPORT_FLEXARRAY;
										// サポート状況フラグ(HSPVAR_SUPPORT_*)
	p->basesize = sizeof(complex);		// １つのデータが使用するサイズ(byte) / 可変長の時は-1
	mytype = p->flag;
}

/*------------------------------------------------------------*/


// 複素数の引数を取得する
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
		throw 5; //引数が省略されてた時
	}
	return Ret;
}
