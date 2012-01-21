
//
//		HSP複素数プラグイン（float型のサンプルから改造）
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

static complex ref_cval; static double ref_dval;		// 返値のための変数
static double dp1, dp2; static complex cp1, cp2;

static void *reffunc( int *type_res, int cmd )
{
	//		関数・システム変数の実行処理 (値の参照時に呼ばれます)
	//

	int answerType = 0;

	//			'('で始まるかを調べる
	//
	if ( *type != TYPE_MARK ) puterror( HSPERR_INVALID_FUNCPARAM );
	if ( *val != '(' ) puterror( HSPERR_INVALID_FUNCPARAM );
	code_next();


	switch( cmd ) {							// サブコマンドごとの分岐

	case 0x00:								// complex関数
		dp1 = code_getdd(0.0);				// 実数値を取得
		dp2 = code_getdd(0.0);
		ref_cval = cmplx(dp1, dp2);		// 返値を設定
		answerType = 1;						// 返り値は複素数
		break;

	case 0x01:								// cabs関数
		// r = sqrt(x^2 + y^2)
		cp1 = excomplex();					// 複素数値を取得
		ref_dval = abscx(cp1);
		answerType = 0;						// 返り値は実数数
		break;

	case 0x02:								// carg関数
		// theta = arctan(y / x)
		cp1 = excomplex();
		ref_dval = argcx(cp1);
		answerType = 0;
		break;

	case 0x03:								// conj関数
		// 共役複素数を返す
		cp1 = excomplex();
		ref_cval = conjg(cp1);
		answerType = 1;
		break;

	case 0x04:								// cpolar関数
		// 極形式で入力
		dp1 = code_getdd(0.0);
		dp2 = code_getdd(0.0);
		ref_cval = polar(dp1, dp2);
		answerType = 1;
		break;

	case 0x05:								// creal関数
		// 実部
		cp1 = excomplex();
		ref_dval = cp1.Real;
		answerType = 0;
		break;

	case 0x06:								// cimag関数
		// 虚部
		cp1 = excomplex();
		ref_dval = cp1.Imaginary;
		answerType = 0;
		break;

	case 0x10:								// clog関数
		// 複素自然対数関数
		cp1 = excomplex();
		ref_cval = logcx(cp1);
		answerType = 1;
		break;

	case 0x11:								// clog10関数
		// 複素常用対数関数
		cp1 = excomplex();
		ref_cval = log10cx(cp1);
		answerType = 1;
		break;

	case 0x14:								// cexp関数
		// 複素指数関数
		cp1 = excomplex();
		ref_cval = expcx(cp1);
		answerType = 1;
		break;

	case 0x15:								// cpow関数
		// 複素冪関数
		cp1 = excomplex();
		cp2 = excomplex();
		ref_cval = powcx(cp1, cp2);
		answerType = 1;
		break;

	case 0x16:								// csqrt関数
		// 複素平方根関数
		cp1 = excomplex();
		ref_cval = sqrtcx(cp1);
		answerType = 1;
		break;

	case 0x20:								// csin関数
		// 正弦
		cp1 = excomplex();
		ref_cval = sincx(cp1);
		answerType = 1;
		break;

	case 0x21:								// ccos関数
		// 余弦
		cp1 = excomplex();
		ref_cval = coscx(cp1);
		answerType = 1;
		break;

	case 0x22:								// ctan関数
		// 正接
		cp1 = excomplex();
		ref_cval = tancx(cp1);
		answerType = 1;
		break;

	case 0x28:								// casin関数
		// 逆正弦
		cp1 = excomplex();
		ref_cval = asincx(cp1);
		answerType = 1;
		break;

	case 0x29:								// cacos関数
		// 逆余弦
		cp1 = excomplex();
		ref_cval = acoscx(cp1);
		answerType = 1;
		break;

	case 0x2A:								// catan関数
		// 逆正接
		cp1 = excomplex();
		ref_cval = atancx(cp1);
		answerType = 1;
		break;

	case 0x30:								// csinh関数
		// 双曲線正弦
		cp1 = excomplex();
		ref_cval = sinhcx(cp1);
		answerType = 1;
		break;

	case 0x31:								// ccosh関数
		// 双曲線余弦
		cp1 = excomplex();
		ref_cval = coshcx(cp1);
		answerType = 1;
		break;

	case 0x32:								// ctanh関数
		// 双曲線正接
		cp1 = excomplex();
		ref_cval = tanhcx(cp1);
		answerType = 1;
		break;

	case 0x38:								// casinh関数
		// 双曲線逆正弦
		cp1 = excomplex();
		ref_cval = asinhcx(cp1);
		answerType = 1;
		break;

	case 0x39:								// cacosh関数
		// 双曲線逆余弦
		cp1 = excomplex();
		ref_cval = acoshcx(cp1);
		answerType = 1;
		break;

	case 0x3A:								// catanh関数
		// 双曲線逆正接
		cp1 = excomplex();
		ref_cval = atanhcx(cp1);
		answerType = 1;
		break;

	case 0x40:								// cproj関数
		cp1 = excomplex();
		ref_cval = projection(cp1);
		answerType = 1;
		break;

	case 0x41:								// cgamma関数
		cp1 = excomplex();
		ref_cval = gamma(cp1);
		answerType = 1;
		break;

	default:
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
	}

	//			'('で終わるかを調べる
	//
	if ( *type != TYPE_MARK ) puterror( HSPERR_INVALID_FUNCPARAM );
	if ( *val != ')' ) puterror( HSPERR_INVALID_FUNCPARAM );
	code_next();

	if (answerType) {
		*type_res = HspVarComplex_typeid();		// 返値のタイプを指定する
		return (void *)&ref_cval;
	} else {
		*type_res = HSPVAR_FLAG_DOUBLE;		// 返値のタイプを指定する
		return (void *)&ref_dval;
	}
}


/*------------------------------------------------------------*/

static int termfunc( int option )
{
	//		終了処理 (アプリケーション終了時に呼ばれます)
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
	//		DLLエントリー (何もする必要はありません)
	//
	return TRUE;
}


EXPORT void WINAPI hsp3cmdinit( HSP3TYPEINFO *info )
{
	//		プラグイン初期化 (実行・終了処理を登録します)
	//
	hsp3sdk_init( info );		// SDKの初期化(最初に行なって下さい)

	info->reffunc = reffunc;		// 参照関数(reffunc)の登録
	info->termfunc = termfunc;		// 終了関数(termfunc)の登録

	registvar( -1, HspVarComplex_Init );		// 新しい型の追加
}

/*------------------------------------------------------------*/

