//
// Created by adamg on 4/20/2024.
//

#ifndef IN104_DATA_H
#define IN104_DATA_H
#include "array.h"
char *strsep(char **stringp, const char *delim);
array* read_file(const char* filename,const char* delimiter);
#endif //IN104_DATA_H
