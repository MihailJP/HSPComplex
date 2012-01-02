#ifndef __calculate_complex_h
#define __calculate_complex_h

struct complex
{
	double Real;
	double Imaginary;
};

complex cmplx(const double Real, const double Imaginary);
complex cxadd(const complex arg1, const complex arg2);
complex cxsub(const complex arg1, const complex arg2);
complex cxmul(const complex arg1, const complex arg2);
complex cxdiv(const complex arg1, const complex arg2);

complex polar(const double modulus, const double argument);
double abscx(const complex arg);
double argcx(const complex arg);
complex conjg(const complex arg);

complex logcx(const complex arg);
complex logcx(const complex arg, const complex base);
complex log10cx(const complex arg);
complex expcx(const complex arg);
complex powcx(const complex arg, const complex exponent);
complex sqrtcx(const complex arg);

complex sincx(const complex arg);
complex coscx(const complex arg);
complex tancx(const complex arg);
complex asincx(const complex arg);
complex acoscx(const complex arg);
complex atancx(const complex arg);

complex sinhcx(const complex arg);
complex coshcx(const complex arg);
complex tanhcx(const complex arg);
complex asinhcx(const complex arg);
complex acoshcx(const complex arg);
complex atanhcx(const complex arg);

#endif
