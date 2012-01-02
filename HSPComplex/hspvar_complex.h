
//
//	hspvar_complex.cpp header
//

#ifndef __hspvar_complex_h
#define __hspvar_complex_h

#include "hsp3plugin.h"
#include "hspvar_core.h"
#include "calculate_complex.h"

#ifdef __cplusplus
extern "C" {
#endif

EXPORT void HspVarComplex_Init( HspVarProc *p );
EXPORT int HspVarComplex_typeid( void );
complex excomplex(void);

#ifdef __cplusplus
}
#endif


#endif


