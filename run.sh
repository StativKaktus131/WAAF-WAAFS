gcc -o main src/main.c src/chunk.c src/instrhelp.c src/wii.c src/tinyexpr.c -Iinclude
./main
rm main