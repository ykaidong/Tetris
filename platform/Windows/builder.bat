
gcc -Idep -I..\..\src -c main.c
gcc -Idep -c ui.c
gcc -Idep -c ..\..\src\tetris.c
gcc -c dep\pcc32.c
gcc -o tetris.exe pcc32.o ui.o tetris.o main.o

@del *.o
@pause
