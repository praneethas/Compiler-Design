cd src
flex assign2c.l 
mv lex.yy.c lex.yy.cpp
g++ assign2.cpp lex.yy.cpp -lfl -o ../bin/assign2.exe
cd ../bin
./assign2.exe <../src/test.c >../output.txt

