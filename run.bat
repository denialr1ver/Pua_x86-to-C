nasm -f win64 func.asm
gcc -c main.c -o main.obj -m64
gcc main.obj func.obj -o main.exe -m64
main.exe
pause