;-----------------------------------------------------------------------------
; HSPComplex プラグイン説明
;-----------------------------------------------------------------------------

%dll
HSPComplex

%ver
0.4a

%date
2012/02/06

%author
MihailJP

%type
拡張関数

%group
拡張入出力制御関数

%url
https://github.com/MihailJP/HSPComplex

%port
Win

;-----------------------------------------------------------------------------
%index
complex
複素数を代入する、複素数に変換する

%prm
(p1,p2)
p1=実数(0.0) : 実部
p2=実数(0.0) : 虚部

%inst
p1とp2で指定された値の複素数を返します。
p1には実部を、p2には虚部を指定します。
それぞれ省略された場合は0が指定されたものとみなされます。

返り値は本プラグインで追加される「複素数型("complex")」になります。
複素数型は、本プラグインの他の関数で使用します。

複素数を変数に代入するには、complex関数を使います。例えば、
	a = complex(3, 4)
とすると、変数 a が複素数型になり、 3 + 4i が代入されます。また、複素数の配列を作るには、
	dimtype a,vartype("complex"),20
のようにします。

四則計算は、整数や実数と同じようにできます。たとえば、
	print (complex(1,2)+complex(3,4))
と書けば、「4+6i」と表示されるはずです。なお、剰余(\)はエラーになります。

複素数型への暗黙の型変換は、実数型、整数型からの変換をサポートしています。例えば、
	print (complex(1,2)+3)
と書けば、「4+2i」と表示されるはずです。但し、文字列型から複素数型に変換しようとするとエラーになります。

複素数型からの暗黙の型変換は、実数型、整数型、文字列型への変換をサポートしています。文字列型への変換は、「3+4i」などの文字列に変換されます。
実数型または整数型に変換する場合は、元の複素数型変数で虚部が0である必要があります。虚数(虚部が非0)を変換しようとすると、実数型であればNaNが返りますが、整数型であればエラーになります。

比較演算子は複素数の性質上、「=」(等しい)と「!」(等しくない)のみをサポートしています(実部と虚部がともに等しい場合に、等号が成立します)。「>」や「<」などは定義されないためエラーになります。
また、論理演算やビットシフトはエラーになります。

%href
polar
creal
cimag

%note
hspcmplx.asをインクルードすること。

;-----------------------------------------------------------------------------
%index
cabs
複素数の絶対値を返す

%prm
(p1)
p1=複素数 : 絶対値に変換する複素数

%inst
p1の絶対値を実数で返します。

%href
carg
polar

%note
hspcmplx.asをインクルードすること。

;-----------------------------------------------------------------------------
%index
carg
複素数の偏角を返す

%prm
(p1)
p1=複素数 : 偏角を求める複素数

%inst
p1の偏角を実数で返します。
単位はラジアンで返ります。

%href
cabs
polar

%note
hspcmplx.asをインクルードすること。

;-----------------------------------------------------------------------------
%index
conj
共役複素数を返す

%prm
(p1)
p1=複素数 : 共役を求める複素数

%inst
p1の共役複素数を返します。
複素数 a+bi に対する共役複素数は、 a-bi です。

%href
complex

%note
hspcmplx.asをインクルードすること。

;-----------------------------------------------------------------------------
%index
polar
極形式で複素数に変換する

%prm
(p1)
p1=実数(0.0) : 絶対値
p2=実数(0.0) : 偏角

%inst
p1とp2で指定された値の複素数を返します。

p1には絶対値を実数で指定します。
p2には偏角をラジアンで指定します。

%href
complex
cabs
carg

%note
hspcmplx.asをインクルードすること。

;-----------------------------------------------------------------------------
%index
creal
複素数の実部を返す

%prm
(p1)
p1=複素数 : 実部を取り出す複素数

%inst
p1の実部を実数で返します。

%href
cimag
complex

%note
hspcmplx.asをインクルードすること。

;-----------------------------------------------------------------------------
%index
cimag
複素数の虚部を返す

%prm
(p1)
p1=複素数 : 虚部を取り出す複素数

%inst
p1の虚部を実数で返します。

%href
creal
complex

%note
hspcmplx.asをインクルードすること。

;-----------------------------------------------------------------------------
%index
clog
複素数の自然対数を返す

%prm
(p1)
p1=複素数 : 対数を求める複素数

%inst
p1の自然対数を複素数で返します。
p1に0を指定した場合は無限大(INF)を返します。

%href
clog10
cexp

%note
hspcmplx.asをインクルードすること。

;-----------------------------------------------------------------------------
%index
clog10
複素数の常用対数を返す

%prm
(p1)
p1=複素数 : 対数を求める複素数

%inst
p1の常用対数を複素数で返します。
p1に0を指定した場合は無限大(INF)を返します。

%href
clog

%note
hspcmplx.asをインクルードすること。

;-----------------------------------------------------------------------------
%index
cexp
複素数の指数を返す

%prm
(p1)
p1=複素数 : 対数を求める複素数

%inst
e の p1 乗を複素数で返します。
オーバーフローした場合は無限(INF)を返し、アンダーフローした場合は0を返します。

%href
clog
cpow

%note
hspcmplx.asをインクルードすること。

;-----------------------------------------------------------------------------
%index
cpow
複素数の冪乗を求める

%prm
(p1,p2)
p1=複素数 : 底
p2=複素数 : 指数

%inst
p1 の p2 乗を複素数で返します。

%href
clog
cpow

%note
hspcmplx.asをインクルードすること。

;-----------------------------------------------------------------------------
%index
csqrt
複素数の平方根を返す

%prm
(p1)
p1=複素数 : 平方根を求める値

%inst
p1 の平方根を複素数で返します。

%href
cpow

%note
hspcmplx.asをインクルードすること。

;-----------------------------------------------------------------------------
%index
csin
複素数の正弦を返す

%prm
(p1)
p1=複素数 : 角度(ラジアン)

%inst
p1 の正弦を複素数で返します。
p1 で指定する単位はラジアンになります。

%href
ccos
ctan

%note
hspcmplx.asをインクルードすること。

;-----------------------------------------------------------------------------
%index
ccos
複素数の余弦を返す

%prm
(p1)
p1=複素数 : 角度(ラジアン)

%inst
p1 の余弦を複素数で返します。
p1 で指定する単位はラジアンになります。

%href
csin
ctan

%note
hspcmplx.asをインクルードすること。

;-----------------------------------------------------------------------------
%index
ctan
複素数の正接を返す

%prm
(p1)
p1=複素数 : 角度(ラジアン)

%inst
p1 の正接を複素数で返します。
p1 で指定する単位はラジアンになります。

%href
csin
ccos

%note
hspcmplx.asをインクルードすること。

;-----------------------------------------------------------------------------
%index
casin
複素数の逆正弦を返す

%prm
(p1)
p1=複素数 : 逆正弦を求める値

%inst
p1 の逆正弦を複素数で返します。
返り値の単位はラジアンになります。

%href
cacos
catan

%note
hspcmplx.asをインクルードすること。

;-----------------------------------------------------------------------------
%index
cacos
複素数の逆余弦を返す

%prm
(p1)
p1=複素数 : 逆余弦を求める値

%inst
p1 の逆余弦を複素数で返します。
返り値の単位はラジアンになります。

%href
casin
catan

%note
hspcmplx.asをインクルードすること。

;-----------------------------------------------------------------------------
%index
catan
複素数の逆正接を返す

%prm
(p1)
p1=複素数 : 逆正接を求める値

%inst
p1 の逆正接を複素数で返します。
返り値の単位はラジアンになります。

%href
casin
cacos

%note
hspcmplx.asをインクルードすること。

;-----------------------------------------------------------------------------
%index
csinh
複素数の双曲線正弦を返す

%prm
(p1)
p1=複素数 : 双曲線正弦を求める値

%inst
p1 の双曲線正弦を複素数で返します。

%href
ccosh
ctanh

%note
hspcmplx.asをインクルードすること。

;-----------------------------------------------------------------------------
%index
ccosh
複素数の双曲線余弦を返す

%prm
(p1)
p1=複素数 : 双曲線余弦を求める値

%inst
p1 の双曲線余弦を複素数で返します。

%href
csinh
ctanh

%note
hspcmplx.asをインクルードすること。

;-----------------------------------------------------------------------------
%index
ctanh
複素数の双曲線正接を返す

%prm
(p1)
p1=複素数 : 双曲線正接を求める値

%inst
p1 の双曲線正接を複素数で返します。

%href
csinh
ccosh

%note
hspcmplx.asをインクルードすること。

;-----------------------------------------------------------------------------
%index
casinh
複素数の双曲線逆正弦を返す

%prm
(p1)
p1=複素数 : 双曲線逆正弦を求める値

%inst
p1 の双曲線逆正弦を複素数で返します。

%href
cacosh
catanh

%note
hspcmplx.asをインクルードすること。

;-----------------------------------------------------------------------------
%index
cacosh
複素数の双曲線逆余弦を返す

%prm
(p1)
p1=複素数 : 双曲線逆余弦を求める値

%inst
p1 の双曲線逆余弦を複素数で返します。

%href
casinh
catanh

%note
hspcmplx.asをインクルードすること。

;-----------------------------------------------------------------------------
%index
catanh
複素数の双曲線逆正接を返す

%prm
(p1)
p1=複素数 : 双曲線逆正接を求める値

%inst
p1 の双曲線逆正接を複素数で返します。

%href
casinh
cacosh

%note
hspcmplx.asをインクルードすること。

;-----------------------------------------------------------------------------
%index
cproj
リーマン球面上への投射を返す

%prm
(p1)
p1=複素数 : 投射を求める値

%inst
p1 のリーマン球面上への投射を複素数で返します。
p1 の実部、虚部の少なくとも片方が正または負の無限大である場合、(+∞ +0i) を返します。
それ以外の場合、引数をそのまま返します。

%href
cabs

%note
hspcmplx.asをインクルードすること。

;-----------------------------------------------------------------------------
%index
cgamma
ガンマ関数の値を返す

%prm
(p1)
p1=複素数 : ガンマ関数の引数

%inst
p1 を引数としたガンマ関数の解を複素数で返します。
ガンマ関数とは階乗を一般化した関数で、複素数を引数にとることができます。
p1 が実軸上の整数の場合は、階乗の計算法と同様の計算方法を行います。
それ以外の場合は、スターリングの公式の変形により計算しています。

%href
complex

%note
hspcmplx.asをインクルードすること。
