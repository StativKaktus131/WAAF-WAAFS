gcc -o main src/main.c src/chunk.c src/instrhelp.c src/wii.c src/tinyexpr.c src/strutils.c src/variable.c -Iinclude
./main $1 $2 $3
rm main