
//
//	hspvar_complex.cpp header
//
#ifndef __hspvar_complex_h
#define __hspvar_complex_h

#include "hspvar_core.h"

#ifdef __cplusplus
extern "C" {
#endif

struct complex
{
	double Real;
	double Imaginary;
};

EXPORT void HspVarComplex_Init( HspVarProc *p );
EXPORT int HspVarComplex_typeid( void );
complex excomplex(void);

#ifdef __cplusplus
}
#endif


#endif


