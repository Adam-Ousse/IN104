//
// Created by adamg on 4/26/2024.
//

#include "plot.h"
#define percentage 0.05

Figure* Figureinit(){
    Figure*  fig = malloc(sizeof(Figure));
    fig->axis_set = false;
    return fig;
}

void FigureDestroy(Figure* fig){
    free(fig);
}
void DrawScatterPlot(array* x_coords, array* y_coords,Figure * fig,float radius, Color color, int alpha) {

    double height = GetScreenHeight();
    double width = GetScreenWidth();
    // Ensure the arrays have the same size
    assert(x_coords->shape[0] == y_coords->shape[0]);
    if(fig->axis_set == false){
        fig->x_min = min_array(x_coords);
        fig->x_max = max_array(x_coords);
        fig->y_min = min_array(y_coords);
        fig->y_max = max_array(y_coords);

    }
    double x_min = fig->x_min;
    double x_max = fig->x_max;
    double y_min = fig->y_min;
    double y_max = fig->y_max;
//    double x_coef = (GetScreenWidth()-x_min)/(x_max-x_min);
//    double y_coef = (GetScreenHeight()-y_min)/(y_max-y_min);
    // Adjust the alpha value of the color
    color.a = alpha;

    //screen coords :
    double x_start = percentage * width;
    double y_start = (1-percentage) * height;
    double x_end = (1-percentage) * width;
    double y_end = percentage * height;
    if(fig->axis_set==false){
        // Draw x-axis and y-axis
        DrawLine(x_start, y_start,x_end,y_start, BLACK);
        DrawLine(x_start,y_start,x_start,y_end, BLACK);
        for (int i = 0; i <= 10; i++) {
            // X-axis ticks
            DrawLine(x_start + i * (x_end - x_start) / 10, y_start, x_start + i * (x_end - x_start) / 10, y_start + 10, BLACK);
            // X-axis labels
            DrawText(TextFormat("%0.2f", x_min + i * (x_max - x_min) / 10), x_start + i * (x_end - x_start) / 10, y_start + 20, 10, BLACK);
            // Y-axis ticks
            DrawLine(x_start, y_start - i * (y_start - y_end) / 10, x_start - 10, y_start - i * (y_start - y_end) / 10, BLACK);
            // Y-axis labels
            DrawText(TextFormat("%0.2f", y_min + i * (y_max - y_min) / 10), x_start - 50, y_start - i * (y_start - y_end) / 10, 10, BLACK);
        }
        fig->axis_set = true;
    }

    // Iterate over the arrays
    for (int i = 0; i < x_coords->shape[0]; i++) {
        // Create a Vector2 for the position of the point
        Vector2 position = { (x_coords->values[i][0]-x_min)/(x_max-x_min)*(width*(1-2*percentage))+percentage*width, height - ((y_coords->values[i][0]-y_min)/(y_max-y_min)*(height*(1-2*percentage))+percentage*height) };

        // Draw a circle at the position with the specified color and alpha value
        DrawCircleV(position, radius, color);

        // Draw a black contour around the circle with the specified alpha value
        Color contourColor = BLACK;
        contourColor.a = alpha;
        DrawCircleLines(position.x, position.y, radius, contourColor);
    }
}




void DrawLinePlot(array* x_coords, array* y_coords, Figure * fig,float thickness, Color color, int alpha) {
    double height = GetScreenHeight();
    double width = GetScreenWidth();
    // Ensure the arrays have the same size
    assert(x_coords->shape[0] == y_coords->shape[0]);
    if(fig->axis_set == false){
        fig->x_min = min_array(x_coords);
        fig->x_max = max_array(x_coords);
        fig->y_min = min_array(y_coords);
        fig->y_max = max_array(y_coords);

    }
    double x_min = fig->x_min;
    double x_max = fig->x_max;
    double y_min = fig->y_min;
    double y_max = fig->y_max;

    // Adjust the alpha value of the color
    color.a = alpha;

    //screen coords :
    double x_start = percentage * width;
    double y_start = (1-percentage) * height;
    double x_end = (1-percentage) * width;
    double y_end = percentage * height;
    if(fig->axis_set==false){
        // Draw x-axis and y-axis
        DrawLine(x_start, y_start,x_end,y_start, BLACK);
        DrawLine(x_start,y_start,x_start,y_end, BLACK);
        for (int i = 0; i <= 10; i++) {
            // X-axis ticks
            DrawLine(x_start + i * (x_end - x_start) / 10, y_start, x_start + i * (x_end - x_start) / 10, y_start + 10, BLACK);
            // X-axis labels
            DrawText(TextFormat("%0.2f", x_min + i * (x_max - x_min) / 10), x_start + i * (x_end - x_start) / 10, y_start + 20, 10, BLACK);
            // Y-axis ticks
            DrawLine(x_start, y_start - i * (y_start - y_end) / 10, x_start - 10, y_start - i * (y_start - y_end) / 10, BLACK);
            // Y-axis labels
            DrawText(TextFormat("%0.2f", y_min + i * (y_max - y_min) / 10), x_start - 50, y_start - i * (y_start - y_end) / 10, 10, BLACK);
        }
        fig->axis_set = true;
    }
    // Iterate over the arrays
    for (int i = 0; i < x_coords->shape[0] - 1; i++) {
        // Create a Vector2 for the position of the current point
        Vector2 position1 = { (x_coords->values[i][0]-x_min)/(x_max-x_min)*(width*(1-2*percentage))+percentage*width, height - ((y_coords->values[i][0]-y_min)/(y_max-y_min)*(height*(1-2*percentage))+percentage*height) };
        // Create a Vector2 for the position of the next point
        Vector2 position2 = { (x_coords->values[i+1][0]-x_min)/(x_max-x_min)*(width*(1-2*percentage))+percentage*width, height - ((y_coords->values[i+1][0]-y_min)/(y_max-y_min)*(height*(1-2*percentage))+percentage*height) };

        // Draw a line between the current point and the next point
        DrawLineEx(position1, position2, thickness, color);
    }
}