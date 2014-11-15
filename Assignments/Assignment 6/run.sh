set -x

lex token.l
yacc -d parser.y
gcc lex.yy.c y.tab.c -ll -o threeCodeAddress.exe