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
    SCREEN_THREE,
    SCREEN_FOUR,
} GameScreen;

int main(){
    //    resets the Rayliblogfile
    FILE *file = fopen("../log/rayliblog.txt", "w");
    FILE *file_temp = fopen("../loss.csv", "w");
    fclose(file_temp);


    SetTraceLogLevel(LOG_INFO);
    SetTraceLogCallback(LogCallback);
    TraceLog(LOG_INFO, "Starting IN104");
    if(TEST){
        main_test();
    }
    array* Y = read_file("../data/height_weight.csv",",");
    LinearRegression* Model=LinearRegression_init(1);
    array* X= col_subset(Y,0,1);
    array* y =col_subset(Y,1,2);
    array* x = transpose(linspace(min_array(X), max_array(X), 1000));//for plotting
    //partially fitting the model.
    LinearRegression_fit(Model, X, y,10,0.0000000001 ,false,false);
    //predicted values
    array* y_predictions = LinearRegression_predict(Model, x);
    //colors :
    Color my_red = CLITERAL(Color){ 243,102,102,255 };
    Color my_hover_red = CLITERAL(Color){ 252, 124, 143,255};
    Color my_grey = CLITERAL(Color){ 89,98,111,255};
    Color my_bleu = CLITERAL(Color){ 102, 175, 243,255};
    Color my_hover_bleu = CLITERAL(Color){ 118, 202, 245,255};
    // Screen sizes:
    int screenWidth = 960;
    int screenHeight = 480;
    //Figures
    Figure* fig_screen_one =Figureinit();
    Figure* fig_screen_two =Figureinit();
    Figure* fig_screen_three =Figureinit();
    Figure*  fig_screen_four=Figureinit();
    static Texture2D scatterPlotTexture;
    static bool isScatterPlotSaved = false;
    static int lastScreenWidth = 960;
    static int lastScreenHeight = 480;
    static RenderTexture2D target;


    //screen config :
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT );
    InitWindow(screenWidth, screenHeight, "IN104");
    Font globalFont = LoadFontEx("../assets/Roboto-Medium.ttf", (int)(5*screenWidth/100.0), 0, 0);
    GuiSetFont(globalFont);
    Image icon = LoadImage("../assets/icon.png");
    SetWindowIcon(icon);
    UnloadImage(icon); // Unload image data
    SetTargetFPS(20);
    if (file != NULL) {
        fclose(file);
        Log(LOG_ERROR,"Failed to initialize raylog");
    }
    GuiWindowFileDialogState fileDialogState = InitGuiWindowFileDialog("../");
    char dataPointsStr_screen_two[MAX_INPUT_CHARS + 1] = "\0";  // Input box text
    char dataPointsStr_screen_one[MAX_INPUT_CHARS + 1] = "\0";  // Input box text*
    char dataPointsStr_screen_three[MAX_INPUT_CHARS + 1] = "\0";  // Input box text
    int dataPoints_screen_two = 0;  // Number of data points
    int dataPoints_screen_one = 100000; // Number of data points screen one
    int dataPoints_screen_three = 0;  // Number of data points screen three
    //ANN Model
    int layer_sizes[6]={1,12,12,32,12,1};
    //no activation function for the input
    ACTIVATION_FUNCTION activation_functions[5]={RELU,RELU,SIGMOID,RELU,IDENTITY};
    ANN* ann = ANN_init(6,layer_sizes,activation_functions);
//    for(int i=0;i<4;i++){
//
//        char weight_file[50];
//        char bias_file[50];
//        sprintf(weight_file, "../data/square_weights/layer_%d_weights.csv", i);
//
//        sprintf(bias_file, "../data/square_weights/layer_%d_biases.csv", i);
//        array_destroy(ann->weights[i]);
//        array_destroy(ann->biases[i]);
//        ann->weights[i] = read_file(weight_file, ",");
//        printf("weights of layer %d\n",i+1);
//        info(ann->weights[i]);
//        printf("biases of layer %d\n",i+1);
//        ann->biases[i] = read_file(bias_file, ",");
//        info(ann->biases[i]);
//    }

    array* x_ann = transpose(linspace(-10,10,500));
    int samples = 100;
    array* x_ann_sample = sample(x_ann,samples);

    array* x_ann_2 = sum(elementwise_product(prodc(transform(prodc(x_ann,2),sin),10),transform(divisionc(x_ann,5),square)),prodc(transform(prodc(x_ann,6.0/10),cos),10));
//    array* x_ann_2 = transform(sumc(transform(transform(transform(x_ann,exp),sin),relu),1),log);
    array* x_ann_sample_2 = sample(x_ann_2,samples);
    bool reseted_ann = false;
    double learning_rate =0.01;
    array* loss= train(ann,x_ann,x_ann_2,1, learning_rate,0.25);
    printf("finished trainig");
    array* y_ann = forward(ann,x_ann);

    // Main game loop
    bool isRendered = false;
    Texture2D texture;
    GameScreen currentScreen = MAIN_MENU;
    Rectangle playButton = { screenWidth/2 - 100, screenHeight/2 - 40, 200, 80 };
    Rectangle plotButton = { screenWidth/2 - 100, screenHeight/2 + 60, 200, 80 }; // New button below the play button
    bool SCREEN_TWO_isPaused = false;
    char inputXStr[MAX_INPUT_CHARS] = "\0";
    double inputx=0;
    double predictedY = 0.0;
    bool SCREEN_THREE_isPaused = false;
    bool close = false;
    float buttom_mainW= screenWidth/5.39;
    float buttom_mainH= screenHeight/8.78;
    float buttom_screenW= screenWidth/8.2;
    float buttom_screenH= screenHeight/9.08;
    float open_file_x,open_file_y ;
    float back_button_x,back_button_y;
    array* loss_values;
    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE);
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        //font:
        GuiSetStyle(DEFAULT, TEXT_SIZE, (int)(2*screenWidth/100.0 )); // Adjust the size value as needed
        //Buttons widths and heights
        buttom_mainW= screenWidth/5.39;
        buttom_mainH= screenHeight/8.78;
        buttom_screenW= screenWidth/8.2;
        buttom_screenH= screenHeight/9.08;
        open_file_x= screenWidth /4.471;
        open_file_y = screenHeight /15.1;
        back_button_x=screenWidth / 11.77;
        back_button_y=screenHeight /15.1;
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
                if (GuiButton((Rectangle) {screenWidth / 2 - screenWidth/13.72, (screenHeight / 2 - screenHeight/6.64)-(screenHeight/22.15), buttom_mainW, buttom_mainH}, "Plot"))
                    currentScreen = SCREEN_ONE;
                if (GuiButton((Rectangle) {screenWidth / 2 - screenWidth/13.72, (screenHeight / 2 - screenHeight/22.15)-(screenHeight/33.23), buttom_mainW, buttom_mainH},
                              "Linear Regression"))
                    currentScreen = SCREEN_TWO;
                if (GuiButton((Rectangle) {screenWidth / 2 - screenWidth/13.72 , (screenHeight / 2 + screenHeight/16.61)-(screenHeight/66.46), buttom_mainW, buttom_mainH},
                              "Neural Network"))
                    currentScreen = SCREEN_THREE;
                if (GuiButton((Rectangle) {screenWidth - screenWidth/10.29, screenHeight - screenHeight/11.07, screenWidth/12.349, screenHeight/16.61}, "GitHub"))
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

                if (GuiButton((Rectangle) {screenWidth / 2 - screenWidth/13.72,  screenHeight / 2 + screenHeight/6.04, buttom_mainW, buttom_mainH}, "Exit")) {
                    close=true;
                    break;
                }
            }
                break;
            case SCREEN_ONE: {
                //set axis as not set
                fig_screen_two->axis_set = false;
                //resaves the plot
                if (screenWidth != lastScreenWidth || screenHeight != lastScreenHeight) {
                    lastScreenWidth = screenWidth;
                    lastScreenHeight = screenHeight;
                    isScatterPlotSaved = false;
                }
                if (!isScatterPlotSaved) {
                    UnloadRenderTexture(target);
                    target = LoadRenderTexture(screenWidth, screenHeight);
                    BeginTextureMode(target);
                    ClearBackground(RAYWHITE);
                    DrawScatterPlot(X, y, fig_screen_two, 3, my_bleu, 230);
                    EndTextureMode();
                    scatterPlotTexture = target.texture;
                    isScatterPlotSaved = true;
                }
                Rectangle sourceRect = { 0, 0, (float)scatterPlotTexture.width, (float)-scatterPlotTexture.height }; // Flip the height by using a negative value
                Rectangle destRect = { 0, 0, (float)screenWidth, (float)screenHeight };
                Vector2 origin = { 0, 0 };
                // Draw the texture correctly oriented
                DrawTexturePro(scatterPlotTexture, sourceRect, destRect, origin, 0.0f, WHITE);


                DrawText("Plot", screenWidth/2, 20, 20, my_grey);
                if (GuiButton((Rectangle) {back_button_x, back_button_y, buttom_screenW, buttom_screenH}, "Back")) {
                    currentScreen = MAIN_MENU;
                    isScatterPlotSaved = false;
                }

                DrawText("Number of points", screenWidth/8.5, screenHeight /5, screenWidth/77.1, my_grey);
                //input button for the number of points
                if (GuiTextBox((Rectangle){ screenWidth/8.87, screenHeight /4.03 , screenWidth/7.71, screenHeight/17.26 }, dataPointsStr_screen_one, MAX_INPUT_CHARS, true))
                {
                    dataPoints_screen_one = atoi(dataPointsStr_screen_one);  // Convert string to int
                }
                // set the axis as true so that the scale doesnt change
                fig_screen_two->axis_set = true;
                //button to open file
                if (GuiButton((Rectangle) {open_file_x, open_file_y, buttom_screenW, buttom_screenH}, "Open File")){
                    fileDialogState.windowActive = true;

                }
                if(!fileDialogState.windowActive){
                    GuiSetStyle(DEFAULT, TEXT_SIZE, (int)(2*screenWidth/100.0 ));
                }
                if (fileDialogState.windowActive)
                {
                    GuiSetStyle(DEFAULT, TEXT_SIZE, (int)(20 ));
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
                        Log(LOG_INFO,"%d",dataPoints_screen_one);
                        Y = subset(read_file(filePath, ","), 0, (int)max(dataPoints_screen_one, 15));
                        X= col_subset(Y,0,1);
                        y =col_subset(Y,1,2);
                        x = transpose(linspace(min_array(X), max_array(X), 1000));
                        isScatterPlotSaved = false;
                    }
                }

            }
                break;
            case SCREEN_TWO: {

                //DrawTexture(texture, 0, 0, WHITE);
                //resaves the plot
                fig_screen_two->axis_set = false;
                if (screenWidth != lastScreenWidth || screenHeight != lastScreenHeight) {
                    lastScreenWidth = screenWidth;
                    lastScreenHeight = screenHeight;
                    isScatterPlotSaved = false;
                }
                if (!isScatterPlotSaved) {
                    UnloadRenderTexture(target);
                    target = LoadRenderTexture(screenWidth, screenHeight);
                    BeginTextureMode(target);
                    ClearBackground(RAYWHITE);
                    DrawScatterPlot(X, y, fig_screen_two, 3, my_bleu, 230);
                    EndTextureMode();
                    scatterPlotTexture = target.texture;
                    isScatterPlotSaved = true;
                }
                Rectangle sourceRect = { 0, 0, (float)scatterPlotTexture.width, (float)-scatterPlotTexture.height }; // Flip the height by using a negative value
                Rectangle destRect = { 0, 0, (float)screenWidth, (float)screenHeight };
                Vector2 origin = { 0, 0 };
                // Draw the texture correctly oriented
                DrawTexturePro(scatterPlotTexture, sourceRect, destRect, origin, 0.0f, WHITE);
                DrawText("Linear Regression", screenWidth/2, 20, 20, my_grey);
                if(!SCREEN_TWO_isPaused){
                    LinearRegression_fit(Model, X, y,5,0.0000000001 ,false,false);
                }

                array_destroy(y_predictions);
                y_predictions = LinearRegression_predict(Model, X);
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
                DrawText("Input x", screenWidth/8.5, screenHeight /5, screenWidth/70, my_grey);
                if (GuiTextBox((Rectangle){ screenWidth/8.87, screenHeight /4.03 , screenWidth/7.71, screenHeight/17.26 }, inputXStr, MAX_INPUT_CHARS, true))
                {
                    inputx = atof(inputXStr);  // Convert string to float
                    if (!isnan(inputx)) {

                        array* inputXArray = array_init(1, 1,inputx);
                        array* y_pred = LinearRegression_predict(Model, inputXArray);
                        predictedY = y_pred->values[0][0];
                        array_destroy(y_pred);
                        array_destroy(inputXArray);
                    }
                }
                char y_pred_text[64];

                if (predictedY > 1000000) {
                    sprintf(y_pred_text, "y: %.2e", predictedY);
                } else {
                    sprintf(y_pred_text, "y: %.2lf", predictedY);
                }
                DrawText(y_pred_text, screenWidth/4, screenHeight /3.85, screenWidth/70, my_grey);

                if (GuiButton((Rectangle) {back_button_x, back_button_y, buttom_screenW, buttom_screenH}, "Back")){
                    currentScreen = MAIN_MENU;
                    isScatterPlotSaved = false;}
                if (GuiButton((Rectangle) {screenWidth / 1.757, screenHeight - (screenHeight/6.0), buttom_screenW, buttom_screenH}, "Reset")){
                    reset(Model);
                }
                if (GuiButton((Rectangle) {screenWidth / 1.43, screenHeight - (screenHeight/6.0), buttom_screenW, buttom_screenH}, "Stop")){
                    SCREEN_TWO_isPaused=true;
                }
                if (GuiButton((Rectangle) {screenWidth / 1.2, screenHeight - (screenHeight/6.0), buttom_screenW, buttom_screenH}, "Resume")){
                    SCREEN_TWO_isPaused=false;
                }
                if (GuiButton((Rectangle) {open_file_x, open_file_y, buttom_screenW, buttom_screenH}, "Open File")){
                    fileDialogState.windowActive = true;
                }
                if(!fileDialogState.windowActive){
                    GuiSetStyle(DEFAULT, TEXT_SIZE, (int)(2*screenWidth/100.0 ));
                }
                if (fileDialogState.windowActive)
                {
                    GuiSetStyle(DEFAULT, TEXT_SIZE, (int)(20 ));
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
                        Y = read_file(filePath, ",");
                        X= col_subset(Y,0,1);
                        y =col_subset(Y,1,2);
                        x = transpose(linspace(min_array(X), max_array(X), 1000));
                        isScatterPlotSaved = false;
                        reset(Model);
                    }
                }

            }
                break;
            case SCREEN_THREE: {
                DrawText("ANN approximation of x^2", screenWidth/2, 20, 20, my_grey);

                fig_screen_two->axis_set = false;

                DrawScatterPlot(x_ann_sample,x_ann_sample_2,fig_screen_two,3,my_red,150);

                DrawLinePlot(x_ann, y_ann, fig_screen_two, 3, my_bleu, 150);
                fig_screen_two->axis_set = true;

                if(!SCREEN_THREE_isPaused){
                    reseted_ann=false;
                    loss=train(ann,x_ann,x_ann_2,3, learning_rate,0.25);
                    write_to_csv("../loss.csv",loss,"a");
                    array_destroy(loss);
                    array_destroy(y_ann);
                    y_ann = forward(ann,x_ann);
                }
                if (GuiButton((Rectangle) {screenWidth / 2.277, screenHeight - (screenHeight/6.0), buttom_screenW, buttom_screenH}, "Plot Loss")){
                    // Read the loss values from "loss.csv" into an array
                     loss_values = read_file("../loss.csv", ",");

                    // Create a new Figure instance for the new window


                    // Plot the loss values
//                    DrawLinePlot(linspace(0, loss_values->shape[0], loss_values->shape[0]), loss_values, loss_figure, 3, my_red, 150);
                    currentScreen = SCREEN_FOUR;
                }

                if (GuiButton((Rectangle) {back_button_x, back_button_y, buttom_screenW, buttom_screenH}, "Back")) {
                    currentScreen = MAIN_MENU;
                }
                if (GuiButton((Rectangle) {screenWidth / 1.757, screenHeight - (screenHeight/6.0), buttom_screenW, buttom_screenH}, "Reset")){
                    if(!reseted_ann){
                        reset_ann(ann);
                        reseted_ann=true;
                        FILE *file_temp = fopen("../loss.csv", "w");
                        fclose(file_temp);
                    }

                }
                if (GuiButton((Rectangle) {screenWidth / 1.43, screenHeight - (screenHeight/6.0), buttom_screenW, buttom_screenH}, "Stop")){
                    SCREEN_THREE_isPaused=true;
                }
                if (GuiButton((Rectangle) {screenWidth / 1.2, screenHeight - (screenHeight/6.0), buttom_screenW, buttom_screenH}, "Resume")){
                    SCREEN_THREE_isPaused=false;
                }
                if (GuiButton((Rectangle) {open_file_x, open_file_y, buttom_screenW, buttom_screenH}, "Open File")){
                    fileDialogState.windowActive = true;
                }
                if(!fileDialogState.windowActive){
                    GuiSetStyle(DEFAULT, TEXT_SIZE, (int)(2*screenWidth/100.0 ));
                }
                if (fileDialogState.windowActive)
                {
                    GuiSetStyle(DEFAULT, TEXT_SIZE, (int)(20 ));
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
                        Y = subset(read_file(filePath, ","), 0, (int)max(dataPoints_screen_two, 15));
                        X= col_subset(Y,0,1);
                        y =col_subset(Y,1,2);
                        x = transpose(linspace(min_array(X), max_array(X), 1000));
                        isScatterPlotSaved = false;
                        reset(Model);
                    }
                }


            }
                break;
            case SCREEN_FOUR: {
                //set axis as not set
                fig_screen_four->axis_set = false;

                //resaves the plot
                if (screenWidth != lastScreenWidth || screenHeight != lastScreenHeight) {
                    lastScreenWidth = screenWidth;
                    lastScreenHeight = screenHeight;
                    isScatterPlotSaved = false;
                }
                if (!isScatterPlotSaved) {
                    UnloadRenderTexture(target);
                    target = LoadRenderTexture(screenWidth, screenHeight);
                    BeginTextureMode(target);
                    ClearBackground(RAYWHITE);
                    info(loss_values);

                    DrawLinePlot(transpose(linspace(0,loss_values->shape[0],loss_values->shape[0])), loss_values, fig_screen_four, 3, my_bleu, 230);
                    EndTextureMode();
                    scatterPlotTexture = target.texture;
                    isScatterPlotSaved = true;
                }
                Rectangle sourceRect = { 0, 0, (float)scatterPlotTexture.width, (float)-scatterPlotTexture.height }; // Flip the height by using a negative value
                Rectangle destRect = { 0, 0, (float)screenWidth, (float)screenHeight };
                Vector2 origin = { 0, 0 };
                // Draw the texture correctly oriented
                DrawTexturePro(scatterPlotTexture, sourceRect, destRect, origin, 0.0f, WHITE);
                fig_screen_four->axis_set = true;

                DrawText("Loss Function", screenWidth/2, 20, 20, my_grey);
                if (GuiButton((Rectangle) {back_button_x, back_button_y, buttom_screenW, buttom_screenH}, "Back")) {
                    currentScreen = MAIN_MENU;
                    isScatterPlotSaved = false;
                }


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