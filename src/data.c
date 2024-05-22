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
        Log(LOG_ERROR, "Could not open file %s", filename);
//        TraceLog(LOG_ERROR, "Could not open file [%s:%d]", __FILE__, __LINE__);
        return NULL;
    }
    Log(LOG_INFO, "Opened file %s", filename);
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
    Log(LOG_INFO, "Closing file %s", filename);
    return data;
}



void write_to_csv(const char* filename, array* data) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        Log(LOG_ERROR, "Could not open file %s", filename);
        return;
    }

    for (int i = 0; i < data->shape[0]; i++) {
        for (int j = 0; j < data->shape[1]; j++) {
            fprintf(file, "%f", data->values[i][j]);
            if (j < data->shape[1] - 1) {
                fprintf(file, ",");
            }
        }
        fprintf(file, "\n");
    }

    fclose(file);
    Log(LOG_INFO, "Data written to file %s", filename);
}