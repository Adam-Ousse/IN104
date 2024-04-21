//
// Created by adamg on 4/20/2024.
//
#include "data.h"
#include "utils.h"
#include "array.h"
char *strsep(char **stringp, const char *delim) {
    char *rv = *stringp;
    if (rv) {
        *stringp += strcspn(*stringp, delim);
        if (**stringp)
            *(*stringp)++ = '\0';
        else
            *stringp = 0; }
    return rv;
}
array* read_file(const char* filename,const char* delimiter){
    FILE* file = fopen(filename, "r+");
    if (file == NULL) {
        printf("Could not open file %s\n", filename);
        return NULL;
    }

    char line[1024];
    int num_lines = 0;
    int max_cols = 0;
    while (fgets(line, sizeof(line), file)) {
        num_lines++;
        int num_cols = 0;
        char* temp = strdup(line);
        char* token = strsep(&temp, delimiter);
        while (token != NULL) {
            num_cols++;
            token = strsep(&temp, delimiter);
        }
        free(temp);
        if (num_cols > max_cols) {
            max_cols = num_cols;
        }
    }

    // Reset file pointer to the beginning of the file
    fseek(file, 0, SEEK_SET);

    array* data = array_init(num_lines, max_cols, NAN); // Initialize array with NaN
    int i = 0;
    while (fgets(line, sizeof(line), file)) {
        int j = 0;
        char* temp = strdup(line);
        char* token = strsep(&temp, delimiter);
        while (token != NULL) {
            data->values[i][j] = *token ? atof(token) : NAN;
            j++;
            token = strsep(&temp, delimiter);
        }
        free(temp);
        i++;
    }

    fclose(file);
    return data;
}