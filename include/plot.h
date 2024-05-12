//
// Created by adamg on 4/26/2024.
//

#ifndef IN104_PLOT_H
#define IN104_PLOT_H
#include "utils.h"
#include "array.h"
#include "raylib.h"
typedef struct {
    double x_min;
    double x_max;
    double y_min;
    double y_max;
    bool axis_set;
} Figure;
Figure* Figureinit();
void FigureDestroy(Figure* fig);
void DrawScatterPlot(array* x_coords, array* y_coords, Figure* fig, float radius, Color color, int alpha);
void DrawLinePlot(array* x_coords, array* y_coords, Figure * fig,float thickness, Color color, int alpha);
#endif //IN104_PLOT_H
