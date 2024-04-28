#include "utils.h"
#include "array.h"
#include "data.h"
#include "linearmodels.h"
#include "raylib.h"
#include "plot.h"

void LogCallback(int logType, const char *text, va_list args) {
    FILE *file = fopen("../log/rayliblog.txt", "a");
    if (file != NULL) {
        vfprintf(file, text, args);
        fprintf(file, "\n");
        fclose(file);
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
    array* Y = subset(read_file("../data/x_6.9_3.2.txt","\t"),0,18);
    info(Y);
    //    printf("Y\n");
//    print(Y);
//    printf("%d lignes and %d colonnes\n",Y->shape[0], Y->shape[1]);
//    printf("%lf moyenne de la derniere colonne \n",mean(col_subset(Y,1,2)));

    LinearRegression* Model=LinearRegression_init(1);
    array* X= col_subset(Y,0,1);
    array* y =col_subset(Y,1,2);
    array* x = transpose(linspace(min_array(X), max_array(X), 1000));

    LinearRegression_fit(Model, X, y,1000,0.0000000001 ,false, false);
    printf("MSE : %lf \n",MSE(LinearRegression_predict(Model, X), y));
    printf("a :%lf \n",Model->weights->values[0][0]);
    info(Model->weights);
    printf("b :%lf\n",Model->bias);
// area_price csv : w = 5.67314105, b= 123719.24203732
// heigh_weight csv : w = 3.08347645, b= -82.57574306
//    Model->weights= array_init(1,1,3.08347645);
//    Model->bias = -82.57574306;
    array* y_predictions = LinearRegression_predict(Model, x);
//
    info(y_predictions);
    LinearRegression_destroy(Model);


//    resets the Rayliblogfile
    FILE *file = fopen("../log/rayliblog.txt", "w");
    Figure* fig =Figureinit();
    if (file != NULL) {
        fclose(file);
    }
    const int screenWidth = 1200;
    const int screenHeight = 800;
//    SetTraceLogLevel(LOG_WARNING);
    SetTraceLogCallback(LogCallback);
    InitWindow(screenWidth, screenHeight, "IN104");
    Image icon = LoadImage("../assets/icon.png");
    SetWindowIcon(icon);
    UnloadImage(icon); // Unload image data
    SetTargetFPS(24);
//    HideCursor();
    // Main game loop
    bool isRendered = false;
    Texture2D texture;

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        if (!isRendered) {
            // Draw
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawScatterPlot(X, y, fig, 3, GREEN, 100);
            info(y_predictions);
            DrawLinePlot(x, y_predictions, fig,3, BLUE, 100);
            fig->axis_set = true;

            // Capture the screen to an image and convert it to a texture
            Image screenshot = LoadImageFromScreen();
            texture = LoadTextureFromImage(screenshot);
            UnloadImage(screenshot);  // Unload the image data

            isRendered = true;
            EndDrawing();
        }else{
            BeginDrawing();
            fig->axis_set = false;
            DrawTexture(texture, 0, 0, WHITE);
            DrawScatterPlot(X, y, fig, 3, GREEN, 100);
            fig->axis_set = true;
            DrawLinePlot(x, y_predictions, fig,3, BLUE, 100);

            EndDrawing();
        }


        if(GetCharPressed()==(int)'e'){
            break;
        }
    }

// Unload the texture
    UnloadTexture(texture);
    CloseWindow();
    array_destroy(Y);
    array_destroy(X);
    array_destroy(y);
	return 0;
}