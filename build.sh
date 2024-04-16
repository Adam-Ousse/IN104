#!/bin/bash

mkdir -p build bin

gcc -Wall -Iinclude -c src/*.c -o build/
gcc -o bin/main build/*.o

echo "Build complete. Executable generated at bin/main"
