#include "utils.h"
#include "array.h"
#include "data.h"
#include "LinearRegression.h"
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "plot.h"
#define log_to_console false


void LogCallback(int logType, const char *text, va_list args) {
    FILE *file = fopen("../log/rayliblog.txt", "a");
    if (file != NULL) {
        char *logLevel;
        switch(logType) {
            case LOG_ALL: logLevel = "ALL"; break;
            case LOG_TRACE: logLevel = "TRACE"; break;
            case LOG_DEBUG: logLevel = "DEBUG"; break;
            case LOG_INFO: logLevel = "INFO"; break;
            case LOG_WARNING: logLevel = "WARNING"; break;
            case LOG_ERROR: logLevel = "ERROR"; break;
            case LOG_FATAL: logLevel = "FATAL"; break;
            case LOG_NONE: logLevel = "NONE"; break;
            default: logLevel = "UNKNOWN";
        }
        fprintf(file, "%s : ", logLevel);
        vfprintf(file, text, args);
        fprintf(file, "\n");
        fclose(file);
        // Also write to the console
        if(log_to_console){
            printf("%s : ", logLevel);
            vprintf(text, args);
            printf("\n");
        }



    }
}
/*typedef enum GameScreen { INTRO, PLOT, GAME } GameScreen;*/
typedef enum GameScreen {
    MAIN_MENU = 0,
    SCREEN_ONE,
    SCREEN_TWO,
    SCREEN_THREE
} GameScreen;

int main(){
    //    resets the Rayliblogfile
    FILE *file = fopen("../log/rayliblog.txt", "w");
    SetTraceLogLevel(LOG_INFO);
    SetTraceLogCallback(LogCallback);
    TraceLog(LOG_INFO, "Starting IN104");
    array* Y = subset(read_file("../data/height_weight.csv",","),500,5000);
    info(Y);
    //    printf("Y\n");
//    print(Y);
//    printf("%d lignes and %d colonnes\n",Y->shape[0], Y->shape[1]);
//    printf("%lf moyenne de la derniere colonne \n",mean(col_subset(Y,1,2)));

    LinearRegression* Model=LinearRegression_init(1);
    array* X= col_subset(Y,0,1);
    array* y =col_subset(Y,1,2);
    array* x = transpose(linspace(min_array(X), max_array(X), 1000));

    LinearRegression_fit(Model, X, y,10,0.0000000001 ,false, false);
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





    Figure* fig =Figureinit();
    if (file != NULL) {
        fclose(file);
    }
    int screenWidth = 960;
    int screenHeight = 480;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT );
    InitWindow(screenWidth, screenHeight, "IN104");

    Image icon = LoadImage("../assets/icon.png");
    SetWindowIcon(icon);
    UnloadImage(icon); // Unload image data
    SetTargetFPS(12);

//    HideCursor();
    // Main game loop
    bool isRendered = false;
    Texture2D texture;
    Texture2D rockTexture = LoadTexture("../assets/rock.png");
    Texture2D paperTexture = LoadTexture("../assets/paper.png");
    Texture2D scissorsTexture = LoadTexture("../assets/scissors.png");
    GameScreen currentScreen = MAIN_MENU;
    Rectangle playButton = { screenWidth/2 - 100, screenHeight/2 - 40, 200, 80 };
    Rectangle plotButton = { screenWidth/2 - 100, screenHeight/2 + 60, 200, 80 }; // New button below the play button

    /*while (!WindowShouldClose())    // Detect window close button or ESC key
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
                    LinearRegression_fit(Model, X, y,5,0.0000000001 ,false, false);
                    y_predictions = LinearRegression_predict(Model, x);
                    BeginDrawing();
                    fig->axis_set = false;
                    DrawTexture(texture, 0, 0, WHITE);
                    DrawScatterPlot(X, y, fig, 3, GREEN, 50);
                    fig->axis_set = true;
                    DrawLinePlot(x, y_predictions, fig,3, BLUE, 150);

                    EndDrawing();
                }



                break;
        }

    }*/
    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE);
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        // Keyboard event for 'b' key
        if (IsKeyPressed(KEY_B)) currentScreen = MAIN_MENU;

        switch (currentScreen) {
            case MAIN_MENU: {
                DrawText("MAIN MENU", 20, 20, 20, LIGHTGRAY);

                if (GuiButton((Rectangle) {screenWidth / 2 - 90, screenHeight / 2 - 30, 180, 60}, "Plot"))
                    currentScreen = SCREEN_ONE;
                if (GuiButton((Rectangle) {screenWidth / 2 - 90, screenHeight / 2 + 40, 180, 60},
                              "Linear Regression"))
                    currentScreen = SCREEN_TWO;
                if (GuiButton((Rectangle) {screenWidth / 2 - 90, screenHeight / 2 + 110, 180, 60},
                              "Neural Network"))
                    currentScreen = SCREEN_THREE;
                if (GuiButton((Rectangle) {screenWidth - 120, screenHeight - 60, 100, 40}, "GitHub"))
//                    SetWindowState(FLAG_WINDOW_UNFOCUSED); // Unfocus the window
                    OpenURL("https://github.com/dravenstud/IN104");
//                    SetWindowState(FLAG_FULLSCREEN_MODE); // Set the window back to fullscreen

            }
                break;
            case SCREEN_ONE: {
                DrawText("Plot", 20, 20, 20, LIGHTGRAY);

                if (GuiButton((Rectangle) {screenWidth / 2 - 60, screenHeight - 80, 120, 60}, "Back"))
                    currentScreen = MAIN_MENU;

            }
                break;
            case SCREEN_TWO: {
                DrawText("SCREEN TWO", 20, 20, 20, LIGHTGRAY);

                if (GuiButton((Rectangle) {screenWidth / 2 - 60, screenHeight - 80, 120, 60}, "Back"))
                    currentScreen = MAIN_MENU;

            }
                break;
            case SCREEN_THREE: {
                DrawText("SCREEN THREE", 20, 20, 20, LIGHTGRAY);

                if (GuiButton((Rectangle) {screenWidth / 2 - 60, screenHeight - 80, 120, 60}, "Back"))
                    currentScreen = MAIN_MENU;

            }
                break;
            default:
                break;
        }

        EndDrawing();
    }
// Unload the texture
//    Model->weights= array_init(1,1,3.08347645);
//    Model->bias = -82.57574306;
//    LOG_DEBUG("MSE : %lf \n",MSE(LinearRegression_predict(Model, X), y));

//    printf("MSE : %lf \n",MSE(LinearRegression_predict(Model, X), y));
    printf("a :%lf \n",Model->weights->values[0][0]);
    info(Model->weights);
    printf("b :%lf\n",Model->bias);
    printf("R2 : %lf\n",R2(LinearRegression_predict(Model,X),y));
    UnloadTexture(rockTexture);
    UnloadTexture(paperTexture);
    UnloadTexture(scissorsTexture);
    UnloadTexture(texture);
    CloseWindow();
    array_destroy(Y);
    array_destroy(X);
    array_destroy(y);
    LinearRegression_destroy(Model);
	return 0;
}