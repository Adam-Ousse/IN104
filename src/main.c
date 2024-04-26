#include "utils.h"
#include "array.h"
#include "data.h"
#include "linearmodels.h"
#include "raylib.h"

void LogCallback(int logType, const char *text, va_list args) {
    FILE *file = fopen("../log/rayliblog.txt", "a");
    if (file != NULL) {
        vfprintf(file, text, args);
        fprintf(file, "\n");
        fclose(file);
    }
}


void DrawScatterPlot(array* x_coords, array* y_coords, float radius, Color color, int alpha) {
    // Ensure the arrays have the same size
    assert(x_coords->shape[0] == y_coords->shape[0]);
    double x_coef = GetScreenWidth()/ max_array(x_coords);
    double y_coef = GetScreenHeight()/ max_array(y_coords);
    double x_min = min_array(x_coords);
    // Adjust the alpha value of the color
    color.a = alpha;
    // Iterate over the arrays
    for (int i = 0; i < x_coords->shape[0]; i++) {
        // Create a Vector2 for the position of the point
        Vector2 position = { x_coef * x_coords->values[i][0], GetScreenHeight() - y_coef * y_coords->values[i][0] };

        // Draw a circle at the position with the specified color and alpha value
        DrawCircleV(position, radius, color);

        // Draw a black contour around the circle with the specified alpha value
        Color contourColor = BLACK;
        contourColor.a = alpha;
        DrawCircleLines(position.x, position.y, radius, contourColor);
    }
}
int main(){
	/*array* A = array_init(4,3,0);
	A->values[0][0] = 6;
	A->values[1][2]= 9;
	A->values[1][1] = 10;
	array* B = array_init(3,6,9);
	B->values[2][2] = 20;
	printf("A : \n");
	print(A);
	printf("B : \n");
	print(B);
	printf("A*B\n");
	print(prod(A,B));
	printf("A+1\n");
	print(sum(A,array_init(4,3,1)));
	print(sumc(A,1));
	printf("A * 2\n");
	print(prodc(A,2));
	array* T= prodc(eye(4,4),2);
	array* V = vector_col_init(4,1);
	printf("T : \n");
	print(T);
	printf("V \n");
	print(V);
	printf("T.V\n");
	print(dot_product(T,V));
	array_destroy(A);
	array_destroy(B);
	array_destroy(T);
	array_destroy(V);
    array* X = linspace(0,10,100);
    printf("X : \n");
    print(X);
    array_destroy(X);
    *//*char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("Current working directory: %s\n", cwd);*//*
    printf("data.csv : \n");
    array* Y =read_file("../data/data.csv",";");
    print(Y);
    array_destroy(Y);*/
    array* Y = subset(read_file("../data/area_price.csv",","),0,1454);
//    printf("Y\n");
//    print(Y);
//    printf("%d lignes and %d colonnes\n",Y->shape[0], Y->shape[1]);
//    printf("%lf moyenne de la derniere colonne \n",mean(col_subset(Y,1,2)));

//    LinearRegression* Model=LinearRegression_init(1);
    array* X= col_subset(Y,0,1);
    array* y =col_subset(Y,1,2);
//    LinearRegression_fit(Model, X, y,10000,0.0000000001 ,false, false);
//    printf("MSE : %lf \n",MSE(LinearRegression_predict(Model, X), y));
//    printf("a :%lf \n",Model->weights->values[0][0]);
//    info(Model->weights);
//    printf("b :%lf\n",Model->bias);


//    LinearRegression_destroy(Model);

//    print(X);
//    print(LinearRegression_predict(Model, X));
    //resets the Rayliblogfile
    FILE *file = fopen("../log/rayliblog.txt", "w");
    if (file != NULL) {
        fclose(file);
    }
    const int screenWidth = 1000;
    const int screenHeight = 1000;
//    SetTraceLogLevel(LOG_WARNING);
    SetTraceLogCallback(LogCallback);
    InitWindow(screenWidth, screenHeight, "IN104");
    Image icon = LoadImage("../assets/icon.png");
    SetWindowIcon(icon);
    UnloadImage(icon); // Unload image data
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawScatterPlot(X,y, 3, GREEN, 100);
        EndDrawing();

    }
    CloseWindow();
    array_destroy(Y);
    array_destroy(X);
    array_destroy(y);
	return 0;
}