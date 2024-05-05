
#include "raylib.h"
typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, ENDING } GameScreen;
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    GameScreen currentScreen = LOGO;
    int framesCounter = 0;
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    ToggleFullscreen();
    //the start buttom position
    int RectX=(screenWidth-600)/2;
    int RectY=(screenHeight-230)/2;
    int startX=(RectX+600)/2;
    int startY=(RectY+230)/2;
    //Colors
    Color ARCTICBLUE={144, 224, 247, 255};
    Color PASTELBLUE={175,213,240,255};
    Color CARRIBEANCURRENT={22,105,122,255};
    Color MOONSTONE={72,159,181,255};
    Color ISABELINE={237,231,227,255};
    Color ORANGEPEEL={255,166,43,255};
    //Textures
    Texture2D rock = LoadTexture("C:/Users/DELL/Downloads/rock.png");
    Texture2D paper = LoadTexture("C:/Users/DELL/Downloads/paper.png");
    Texture2D scissors = LoadTexture("C:/Users/DELL/Downloads/scissors.png");
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        switch(currentScreen)
        {
            case LOGO:
            {

                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)){
                    currentScreen = TITLE;
                }
            } break;
            case TITLE:
            {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            // Check if the left mouse button is pressed
            Vector2 mousePosition = GetMousePosition();

            // Check if the mouse is within the button area
            if (CheckCollisionPointRec(mousePosition, (Rectangle){ RectX, RectY, 600, 230 }))
            {
                // If the mouse is within the button area, switch screens
                currentScreen = GAMEPLAY;
            }
        }
            } break;
            case GAMEPLAY:
            {
                // TODO: Update GAMEPLAY screen variables here!

                // Press enter to change to ENDING screen
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    currentScreen = ENDING;
                }
            } break;
            case ENDING:
            {
                // TODO: Update ENDING screen variables here!

                // Press enter to return to TITLE screen
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    currentScreen = TITLE;
                }
            } break;
            default: break;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            switch(currentScreen)
            {
                case LOGO:
                {
                    //TITLE
                    int rockX=screenWidth/2-MeasureText("rock",200)+200;
                    int paperX=screenWidth/2-MeasureText("paper",200)+245;
                    int scissorsX=screenWidth/2-MeasureText("scissors",120)+10;
                    DrawCircleGradient(1920/2, 1080/2, 1200, ARCTICBLUE, CARRIBEANCURRENT);
                    DrawText("ROCK",rockX-5,(screenHeight/2)-5-200-20,200,MAROON);
                    DrawText("ROCK",rockX,(screenHeight/2)-200-20,200,ORANGEPEEL);
                    DrawText("PAPER",paperX-5,(screenHeight/2)-5-20,200,MAROON);
                    DrawText("PAPER",paperX,(screenHeight/2)-20,200,ORANGEPEEL);
                    DrawText("SCISSORS",scissorsX-5,(screenHeight/2)+200-5-20,200,MAROON);
                    DrawText("SCISSORS",scissorsX,(screenHeight/2)+200-20,200,ORANGEPEEL);
                    //PICTURES
                    DrawTexture(rock, (screenWidth/2)-500-100 , 60, WHITE);
                    DrawTexture(paper, (screenWidth/2)-100 , 60, WHITE);
                    DrawTexture(scissors, (screenWidth/2)+500-100, 60, WHITE);

                } break;
                case TITLE:
                {
                    // BACKGROUND
                    DrawCircleGradient(1920/2, 1080/2, 1200, ARCTICBLUE, CARRIBEANCURRENT);
                    DrawText("Main Menu", 10, 10, 20, DARKGRAY);
                    DrawRectangle(RectX, RectY, 600, 230, WHITE);
                    DrawRectangle(RectX+15,RectY+15,569,199,ISABELINE);
                    //TEXT
                    DrawText("Start", RectX+160-5, RectY+60-5, 100, MAROON);
                    DrawText("Start", RectX+160, RectY+60, 100, ORANGEPEEL);

                } break;
                case GAMEPLAY:
                {
                    //BACKGROUND
                    DrawCircleGradient(1920/2, 1080/2, 1200, ARCTICBLUE, CARRIBEANCURRENT);
                    //CIRCLES
                    DrawCircle(1920/2+600,1080/2+100,250,WHITE);
                    DrawCircle(1920/2+600,1080/2+100,240,ISABELINE);
                    DrawCircle(1920/2,1080/2+100,250,WHITE);
                    DrawCircle(1920/2,1080/2+100,240,ISABELINE);
                    DrawCircle(1920/2-600,1080/2+100,250,WHITE);
                    DrawCircle(1920/2-600,1080/2+100,240,ISABELINE);
                    //PICTURES
                    DrawTexture(rock, 1920/2-700 , 1080/2-20, WHITE);
                    DrawTexture(paper, 1920/2-100 , 1080/2-20, WHITE);
                    DrawTexture(scissors, 1920/2+520, 1080/2-20, WHITE);
                    //TEXT
                    DrawText("CHOOSE",1920/2-400-5,1080/2-450-5,200,MAROON);
                    DrawText("CHOOSE",1920/2-400,1080/2-450,200,ORANGEPEEL);




                } break;
                case ENDING:
                {
                    // TODO: Draw ENDING screen here!
                    DrawRectangle(0, 0, screenWidth, screenHeight, BLUE);
                    DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
                    DrawText("PRESS ENTER or TAP to RETURN to TITLE SCREEN", 120, 220, 20, DARKBLUE);

                } break;
                default: break;
            }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------

    // TODO: Unload all loaded data (textures, fonts, audio) here!
    UnloadTexture(rock);
    UnloadTexture(paper);
    UnloadTexture(scissors);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}