
gcc -Wall -I dep -I..\..\src -c main.c
gcc -Wall -I dep -c ui.c
gcc -Wall -I dep -c ..\..\src\tetris.c
REM gcc -Wall -D TETRIS_USE_COLOR -I dep -c ..\..\src\tetris.c
REM gcc -Wall -D UI_USE_COLOR -I dep -c ui.c
gcc -Wall -c dep\pcc32.c
gcc -Wall -o tetris.exe pcc32.o ui.o tetris.o main.o

@del *.o
@pause
