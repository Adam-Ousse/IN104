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
#undef RAYGUI_IMPLEMENTATION
#define GUI_WINDOW_FILE_DIALOG_IMPLEMENTATION
#include "ANN.h"
#define MAX_INPUT_CHARS     10
//for the length input box
#include "gui_window_file_dialog.h"

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
    array* Y = subset(read_file("../data/x_6.9_3.2.txt",","),0,15);
    info(Y);
    LinearRegression* Model=LinearRegression_init(1);
    array* X= col_subset(Y,0,1);
    array* y =col_subset(Y,1,2);
    array* x = transpose(linspace(min_array(X), max_array(X), 1000));

    LinearRegression_fit(Model, X, y,10,0.0000000001 ,false,false);
    array* y_predictions = LinearRegression_predict(Model, x);
    info(y_predictions);
    Color my_red = CLITERAL(Color){ 243,102,102,255 };
    Color my_hover_red = CLITERAL(Color){ 252, 124, 143,255};
    Color my_grey = CLITERAL(Color){ 89,98,111,255};
    Color my_bleu = CLITERAL(Color){ 102, 175, 243,255};
    Color my_hover_bleu = CLITERAL(Color){ 118, 202, 245,255};
    Figure* fig_screen_one =Figureinit();
    Figure* fig_screen_two =Figureinit();
    Figure* fig_screen_three =Figureinit();

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
    GuiWindowFileDialogState fileDialogState = InitGuiWindowFileDialog("../");
    char dataPointsStr[MAX_INPUT_CHARS + 1] = "\0";  // Input box text
    int dataPoints = 0;  // Number of data points

    //ANN Model
    int layer_sizes[3]={1,20,1};
    ANN* ann = ANN_init(3,layer_sizes);
    ann->weights[0] = transpose(read_file("../data/square_weights/w_1.txt",","));
    info(ann->weights[0]);
    ann->weights[1] = read_file("../data/square_weights/w_2.txt",",");
    info(ann->weights[1]);
    ann->biases[0]= transpose(read_file("../data/square_weights/b_1.txt",","));
    info(ann->biases[0]);
    ann->biases[1]= read_file("../data/square_weights/b_2.txt",",");
    info(ann->biases[1]);
    array* x_ann = linspace(-5,5,100);
//    array* y_ann = ForwardPass(ann,x_ann);
    
    // Main game loop
    bool isRendered = false;
    Texture2D texture;
    GameScreen currentScreen = MAIN_MENU;
    Rectangle playButton = { screenWidth/2 - 100, screenHeight/2 - 40, 200, 80 };
    Rectangle plotButton = { screenWidth/2 - 100, screenHeight/2 + 60, 200, 80 }; // New button below the play button
    bool SCREEN_TWO_isPaused = false;
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
                DrawText("Plot", screenWidth/2, 20, 20, my_grey);
                if (GuiButton((Rectangle) {screenWidth / 2 - 60, screenHeight - 80, 120, 60}, "Back"))
                    currentScreen = MAIN_MENU;
                fig_screen_two->axis_set = false;
                DrawScatterPlot(X, y, fig_screen_two, 3, my_bleu, 150);
                fig_screen_two->axis_set = true;
                if (GuiButton((Rectangle) {screenWidth - 300, screenHeight /10, 120, 60}, "Open File")){
                    fileDialogState.windowActive = true;
                }
                if (fileDialogState.windowActive)
                {
                    GuiWindowFileDialog(&fileDialogState);

                    if (!fileDialogState.windowActive && fileDialogState.SelectFilePressed)
                    {
                        char filePath[1024];
                        strcpy(filePath, TextFormat("%s" PATH_SEPERATOR "%s", fileDialogState.dirPathText, fileDialogState.fileNameText));
//                        sprintf(filePath, "%s/%s", fileDialogState.dirPathText, fileDialogState.fileNameText);
                        Log(LOG_INFO,"%s",filePath);
                        array_destroy(Y);
                        array_destroy(X);
                        array_destroy(y);
                        array_destroy(x);
                        Y = read_file(filePath, ","), 0, (int)max(dataPoints,15);
                        X= col_subset(Y,0,1);
                        y =col_subset(Y,1,2);
                        x = transpose(linspace(min_array(X), max_array(X), 1000));
                        reset(Model);
                    }
                }

            }
                break;
            case SCREEN_TWO: {
                DrawText("Linear Regression", screenWidth/2, 20, 20, my_grey);
                if(!SCREEN_TWO_isPaused){
                    LinearRegression_fit(Model, X, y,5,0.0000000001 ,false,false);
                }
                array_destroy(y_predictions);

                y_predictions = LinearRegression_predict(Model, X);

                fig_screen_two->axis_set = false;
//                DrawTexture(texture, 0, 0, WHITE);
                DrawScatterPlot(X, y, fig_screen_two, 3, my_bleu, 150);
                fig_screen_two->axis_set = true;
                DrawLinePlot(X, y_predictions, fig_screen_two, 3, my_red, 150);
                char weightText[64];
                double mse_value = MSE(y_predictions, y);
                if (mse_value > 10000) {
                    sprintf(weightText, "MSE: %.2e", mse_value);
                } else {
                    sprintf(weightText, "MSE: %.2lf", mse_value);
                }
                DrawText(weightText, screenWidth/2, 60, 20, my_grey);
                if (GuiTextBox((Rectangle){ screenWidth - 280, screenHeight /10 +120 , 125, 30 }, dataPointsStr, MAX_INPUT_CHARS, true))
                {
                    dataPoints = atoi(dataPointsStr);  // Convert string to int
                }
                if (GuiButton((Rectangle) {screenWidth / 2 - 60, screenHeight - 80, 120, 60}, "Back"))
                    currentScreen = MAIN_MENU;
                if (GuiButton((Rectangle) {screenWidth / 2 + 60, screenHeight - 80, 120, 60}, "Reset")){
                    reset(Model);
                }
                if (GuiButton((Rectangle) {screenWidth / 2 + 180, screenHeight - 80, 120, 60}, "Stop")){
                    SCREEN_TWO_isPaused=true;
                }
                if (GuiButton((Rectangle) {screenWidth / 2 + 300, screenHeight - 80, 120, 60}, "Resume")){
                    SCREEN_TWO_isPaused=false;
                }
                if (GuiButton((Rectangle) {screenWidth - 300, screenHeight /10, 120, 60}, "Open File")){
                    fileDialogState.windowActive = true;
                }
                if (fileDialogState.windowActive)
                {
                    GuiWindowFileDialog(&fileDialogState);

                    if (!fileDialogState.windowActive && fileDialogState.SelectFilePressed)
                    {
                        char filePath[1024];
                        strcpy(filePath, TextFormat("%s" PATH_SEPERATOR "%s", fileDialogState.dirPathText, fileDialogState.fileNameText));
//                        sprintf(filePath, "%s/%s", fileDialogState.dirPathText, fileDialogState.fileNameText);
                        Log(LOG_INFO,"%s",filePath);
                        array_destroy(Y);
                        array_destroy(X);
                        array_destroy(y);
                        array_destroy(x);
                        Y = subset(read_file(filePath, ","), 0, (int)max(dataPoints,15));
                        X= col_subset(Y,0,1);
                        y =col_subset(Y,1,2);
                        x = transpose(linspace(min_array(X), max_array(X), 1000));
                        reset(Model);
                    }
                }

            }
                break;
            case SCREEN_THREE: {
                DrawText("AN", screenWidth/2, 20, 20, my_grey);
                if (GuiButton((Rectangle) {screenWidth / 2 - 60, screenHeight - 80, 120, 60}, "Back"))
                    currentScreen = MAIN_MENU;
                fig_screen_two->axis_set = false;
//                DrawScatterPlot(transpose(x_ann), transpose(), fig_screen_two, 3, my_bleu, 150);
                fig_screen_two->axis_set = true;

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
    FigureDestroy(fig_screen_one);
    FigureDestroy(fig_screen_two);
    FigureDestroy(fig_screen_three);
    LinearRegression_destroy(Model);
	return 0;
}