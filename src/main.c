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
typedef enum GameScreen { INTRO, PLOT, GAME } GameScreen;


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
    array* Y = subset(read_file("../data/height_weight.csv",","),0,5000);
    info(Y);
    //    printf("Y\n");
//    print(Y);
//    printf("%d lignes and %d colonnes\n",Y->shape[0], Y->shape[1]);
//    printf("%lf moyenne de la derniere colonne \n",mean(col_subset(Y,1,2)));

    LinearRegression* Model=LinearRegression_init(1);
    array* X= col_subset(Y,0,1);
    array* y =col_subset(Y,1,2);
    array* x = transpose(linspace(min_array(X), max_array(X), 1000));

    LinearRegression_fit(Model, X, y,15000,0.0000000001 ,false, false);
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
    const int screenWidth = 1920;
    const int screenHeight = 1080;

//    SetTraceLogLevel(LOG_WARNING);
    SetTraceLogCallback(LogCallback);
    InitWindow(screenWidth, screenHeight, "IN104");

    Image icon = LoadImage("../assets/icon.png");
    SetWindowIcon(icon);
    UnloadImage(icon); // Unload image data
    SetTargetFPS(24);
    ToggleFullscreen();
//    HideCursor();
    // Main game loop
    bool isRendered = false;
    Texture2D texture;
    Texture2D rockTexture = LoadTexture("../assets/rock.png");
    Texture2D paperTexture = LoadTexture("../assets/paper.png");
    Texture2D scissorsTexture = LoadTexture("../assets/scissors.png");
    GameScreen screen = INTRO;
    Rectangle playButton = { screenWidth/2 - 100, screenHeight/2 - 40, 200, 80 };
    Rectangle plotButton = { screenWidth/2 - 100, screenHeight/2 + 60, 200, 80 }; // New button below the play button

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        if(GetCharPressed()==(int)'e'){
            break;
        }
        switch(screen){
            case INTRO:
                BeginDrawing();
                ClearBackground(RAYWHITE);

                // Draw the button
                DrawRectangleRec(playButton, GRAY);
                DrawText("Play", playButton.x + 40, playButton.y + 20, 40, BLACK); // Doubled the size of the text

                DrawRectangleRec(plotButton, GRAY);
                DrawText("Plot", plotButton.x + 40, plotButton.y + 20, 40, BLACK);

                // Check for button click
                if (CheckCollisionPointRec(GetMousePosition(), playButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    screen = GAME;
                }
                if (CheckCollisionPointRec(GetMousePosition(), plotButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    screen = PLOT;
                }
                EndDrawing();
                break;
            case GAME:
                BeginDrawing();
                ClearBackground(RAYWHITE);
                float scale = 0.3;
                Rectangle rockRec = { screenWidth/2 - 200, screenHeight/2 - 200, rockTexture.width*scale, rockTexture.height*scale};
                Rectangle paperRec = { screenWidth/2, screenHeight/2 - 200, paperTexture.width*scale, paperTexture.height*scale };
                Rectangle scissorsRec = { screenWidth/2 + 200, screenHeight/2 - 200, scissorsTexture.width*scale, scissorsTexture.height*scale };

                DrawTextureEx(rockTexture, (Vector2){ screenWidth/2 - 200, screenHeight/2 - 200 }, 0.0f, scale, WHITE);
                DrawTextureEx(paperTexture, (Vector2){ screenWidth/2, screenHeight/2 - 200 }, 0.0f, scale, WHITE);
                DrawTextureEx(scissorsTexture, (Vector2){ screenWidth/2 + 200, screenHeight/2 - 200 }, 0.0f, scale, WHITE);
                if (CheckCollisionPointRec(GetMousePosition(), rockRec) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    printf("Rock\n");
                }
                if (CheckCollisionPointRec(GetMousePosition(), paperRec) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    printf("Paper\n");
                }
                if (CheckCollisionPointRec(GetMousePosition(), scissorsRec) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    printf("Scissors\n");
                }
                EndDrawing();
                break;
            case PLOT:
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



                break;
        }

    }

// Unload the texture
    UnloadTexture(rockTexture);
    UnloadTexture(paperTexture);
    UnloadTexture(scissorsTexture);
    UnloadTexture(texture);
    CloseWindow();
    array_destroy(Y);
    array_destroy(X);
    array_destroy(y);
	return 0;
}