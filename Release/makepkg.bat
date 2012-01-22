rmdir /s /q HSPComplex

mkdir HSPComplex
mkdir HSPComplex\common
mkdir HSPComplex\hsphelp
mkdir HSPComplex\sample
mkdir HSPComplex\sample\HSPComplex

copy HSPComplex.dll HSPComplex
copy common\* HSPComplex\common
copy hsphelp\* HSPComplex\hsphelp
copy sample\HSPComplex\* HSPComplex\sample\HSPComplex
copy readme.txt HSPComplex
"C:\Program Files\7-Zip\7z.exe" a -mx=9 HSPComplex.zip HSPComplex
