@echo off

if not exist build mkdir build
if not exist bin mkdir bin

rem Compile each source file separately
for %%f in (src\*.c) do (
    gcc -Wall -Iinclude -c %%f -o build\%%~nf.o
)

rem Link the object files into the executable
gcc -o bin\main build\*.o

echo Build complete. Executable generated at bin\main
