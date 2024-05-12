#include "utils.h"
#include "array.h"
#include "data.h"
#include "LinearRegression.h"
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "plot.h"
#define log_to_console false
#define TEST true
#include "array_test.h"
#include "metrics.h"
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
    if(TEST){
        main_test();
    }
    array* Y = subset(read_file("../data/height_weight.csv",","),0,10000);
    info(Y);

    LinearRegression* Model=LinearRegression_init(1);
    array* X= col_subset(Y,0,1);
    array* y =col_subset(Y,1,2);
    array* x = transpose(linspace(min_array(X), max_array(X), 1000));

    LinearRegression_fit(Model, X, y,10,0.0000000001 ,false,false);
//    printf("MSE : %lf \n",MSE(LinearRegression_predict(Model, X), y));
//    printf("a :%lf \n",Model->weights->values[0][0]);
//    info(Model->weights);
//    printf("b :%lf\n",Model->bias);
// area_price csv : w = 5.67314105, b= 123719.24203732
// heigh_weight csv : w = 3.08347645, b= -82.57574306
//    Model->weights= array_init(1,1,3.08347645);
//    Model->bias = -82.57574306;
    array* y_predictions = LinearRegression_predict(Model, x);
//
    info(y_predictions);
    Color my_red = CLITERAL(Color){ 243,102,102,255 };
    Color my_hover_red = CLITERAL(Color){ 252, 124, 143,255};
    Color my_grey = CLITERAL(Color){ 89,98,111,255};
    Color my_bleu = CLITERAL(Color){ 102, 175, 243,255};
    Color my_hover_bleu = CLITERAL(Color){ 118, 202, 245,255};
    Figure* fig =Figureinit();
    if (file != NULL) {
        fclose(file);
    }
    int screenWidth = 960;
    int screenHeight = 480;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT );
    InitWindow(screenWidth, screenHeight, "IN104");
    Font globalFont = LoadFont("../assets/Roboto-Medium.ttf");
    GuiSetFont(globalFont);
    Image icon = LoadImage("../assets/icon.png");
    SetWindowIcon(icon);
    UnloadImage(icon); // Unload image data
    SetTargetFPS(20);


    // Main game loop
    bool isRendered = false;
    Texture2D texture;
    GameScreen currentScreen = MAIN_MENU;
    Rectangle playButton = { screenWidth/2 - 100, screenHeight/2 - 40, 200, 80 };
    Rectangle plotButton = { screenWidth/2 - 100, screenHeight/2 + 60, 200, 80 }; // New button below the play button
    bool SCREEN_ONE_isPaused = false;
    bool close = false;
    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE);
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        // Keyboard event for 'b' key
        if (IsKeyPressed(KEY_B)) currentScreen = MAIN_MENU;

        switch (currentScreen) {
            case MAIN_MENU: {
                GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(my_bleu));
                GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, ColorToInt(my_hover_bleu)); // Color when hovered
                GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, ColorToInt(my_hover_bleu)); // Color when pressed
                GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
                GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, ColorToInt(WHITE));
                GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, ColorToInt(WHITE));
                GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED, ColorToInt(my_bleu));
                //                DrawText("MAIN MENU", 20, 20, 20, LIGHTGRAY);
                DrawTextEx(globalFont, "Projet IN104", (Vector2) {20, 20}, globalFont.baseSize, 0,my_grey);
                if (GuiButton((Rectangle) {screenWidth / 2 - 90, screenHeight / 2 - 100, 180, 60}, "Plot"))
                    currentScreen = SCREEN_ONE;
                if (GuiButton((Rectangle) {screenWidth / 2 - 90, screenHeight / 2 -30, 180, 60},
                              "Linear Regression"))
                    currentScreen = SCREEN_TWO;
                if (GuiButton((Rectangle) {screenWidth / 2 - 90, screenHeight / 2 + 40, 180, 60},
                              "Neural Network"))
                    currentScreen = SCREEN_THREE;
                if (GuiButton((Rectangle) {screenWidth - 120, screenHeight - 60, 100, 40}, "GitHub"))
//                    SetWindowState(FLAG_WINDOW_UNFOCUSED); // Unfocus the window
                    OpenURL("https://github.com/dravenstud/IN104");
//                    SetWindowState(FLAG_FULLSCREEN_MODE); // Set the window back to fullscreen
                GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(my_red));
                GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, ColorToInt(my_hover_red)); // Color when hovered
                GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, ColorToInt(my_hover_red)); // Color when pressed
                GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
                GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, ColorToInt(WHITE));
                GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, ColorToInt(WHITE));
                GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED, ColorToInt(my_red));

                if (GuiButton((Rectangle) {screenWidth /2 -90,  screenHeight / 2 + 110, 180, 60}, "Exit")) {
                    close=true;
                    break;
                }
            }
                break;
            case SCREEN_ONE: {
                DrawText("Plot", screenWidth/2, 20, 20, LIGHTGRAY);
                if(!SCREEN_ONE_isPaused){
                    LinearRegression_fit(Model, X, y,5,0.0000000001 ,false,false);
                }
                array_destroy(y_predictions);

                y_predictions = LinearRegression_predict(Model, X);

                fig->axis_set = false;
//                DrawTexture(texture, 0, 0, WHITE);
                DrawScatterPlot(X, y, fig, 3, my_bleu, 50);
                fig->axis_set = true;
                DrawLinePlot(X, y_predictions, fig,3, my_red, 150);
                char weightText[64];
                sprintf(weightText, "MSE: %.2lf", MSE(y_predictions, y));
                DrawText(weightText, screenWidth/2, 60, 20, my_grey);

                if (GuiButton((Rectangle) {screenWidth / 2 - 60, screenHeight - 80, 120, 60}, "Back"))
                    currentScreen = MAIN_MENU;
                if (GuiButton((Rectangle) {screenWidth / 2 + 60, screenHeight - 80, 120, 60}, "Reset")){
                    reset(Model);
                }
                if (GuiButton((Rectangle) {screenWidth / 2 + 180, screenHeight - 80, 120, 60}, "Stop")){
                    SCREEN_ONE_isPaused=true;
                }
                if (GuiButton((Rectangle) {screenWidth / 2 + 300, screenHeight - 80, 120, 60}, "Resume")){
                    SCREEN_ONE_isPaused=false;
                }
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
        if(close){
            break;
        }
    }

    printf("a :%lf \n",Model->weights->values[0][0]);
    info(Model->weights);
    printf("b :%lf\n",Model->bias);
    printf("R2 : %lf\n",R2(LinearRegression_predict(Model,X),y));
    UnloadTexture(texture);
    UnloadFont(globalFont);
    CloseWindow();
    array_destroy(Y);
    array_destroy(X);
    array_destroy(y);
    array_destroy(x);
    array_destroy(y_predictions);
    LinearRegression_destroy(Model);
	return 0;
}