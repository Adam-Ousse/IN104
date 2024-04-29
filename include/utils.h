#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "mathextra.h"
#include <string.h>
#include <unistd.h>// for getcwd
#include <stdbool.h>
#include "raylib.h"
/*
 * Log function that writes to a file and the console it outputs the message in printf-style the file directory and line
 */
#define Log(logLevel, message, ...) TraceLog(logLevel, message " [%s:%d]", ##__VA_ARGS__, __FILE__, __LINE__)
#endif