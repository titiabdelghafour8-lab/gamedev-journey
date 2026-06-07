#include "raylib.h"
#include <vector>

void DrawBackground(int screenWidth, int screenHeight);
int cellCountY = 30;
int cellCountX = 40;
constexpr int cellSize = 40;
std::vector<Vector2> snake {};
Vector2 fruitPosition { 0, 1 };
constexpr float moveTime { 0.05 };
float timer { moveTime };
float speed { 4.0f };

void DrawCell(int posX, int posY, Color color)
{
    DrawRectangle(posX, posY, cellSize, cellSize, color);
}
void generateFruitRandom()
{
    while (true)
    {
        fruitPosition.x = GetRandomValue(0, cellCountX - 1);
        fruitPosition.y = GetRandomValue(0, cellCountY - 1);
        bool touching { false };
        for (auto& i : snake)
        {
            if (i.x == fruitPosition.x && i.y == fruitPosition.y)
            {
                touching = true;
                break;
            }
        }
        if (!touching)
        {
           break;
        }
    }

}
int main ()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(cellCountX * cellSize, cellCountY * cellSize, "Welcome to TITI Snake");
    SetTargetFPS(60);
    if (snake.empty()) snake.push_back( Vector2 {
        cellSize,  cellSize } );
    bool lost { false };
    Vector2 direction { 0, 1 };
    generateFruitRandom();
    while (!WindowShouldClose())
    {
        //====================Window Handling======================
        if (IsWindowResized() && !IsWindowFullscreen())
        {
            cellCountX = GetScreenWidth() / cellSize;
            cellCountY = GetScreenHeight() / cellSize;
        }
        // check for alt + enter
        if (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)))
        {
            int display = GetCurrentMonitor();

            if (IsWindowFullscreen())
            {
                SetWindowSize(cellCountX * cellSize, cellCountY * cellSize);
            }
            else
            {
                SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));
            }
            ToggleFullscreen();
        }
        //===========================================
        //draw
        int drawWidth = GetScreenWidth();
        int drawHeight = GetScreenHeight();

        BeginDrawing();
        timer -= GetFrameTime();
        if (timer <= 0)
        {
            timer += moveTime;
            for (int i = snake.size() - 1; i > 0; i--)
            {
                snake[i] = snake[i - 1];
            }
            snake[0].x += direction.x;
            snake[0].y += direction.y;

            bool isHead { true };
            for (auto& s : snake)
            {
                if (isHead)
                {
                    isHead = false;
                    continue;
                }
                else if (snake[0].x == s.x && snake[0].y == s.y)
                {
                    lost = true;
                    break;
                }

            }
        }

        //losing condition
        if (lost)
        {
            ClearBackground(Color{255, 255, 255, 255});
            DrawText(
                "You Lost! Press (ENTER) to continue.",
                (cellCountX * cellSize) / 2, (cellCountY * cellSize) / 2,
                30, Color{255, 0 , 0 , 255}
                );
            snake = { Vector2 { snake[0].x, snake[0].y } };
            if (IsKeyDown(KEY_ENTER))
                lost = false;
        }
        else
        {
            DrawBackground(drawWidth, drawHeight);
            DrawCell(fruitPosition.x * cellSize, fruitPosition.y * cellSize, RED);
            //handling I/O
            if (IsKeyDown(KEY_LEFT) && direction.x <= 0)  direction = {-1, 0};
            if (IsKeyDown(KEY_RIGHT) && direction.x >= 0) direction = {1, 0};
            if (IsKeyDown(KEY_UP) && direction.y <= 0) direction = {0, -1};
            if (IsKeyDown(KEY_DOWN) && direction.y >= 0)  direction = {0, 1};
            //making the game unbound
            for (auto& s : snake)
            {
                if (s.x < 0) s.x = (cellCountX - 1) ;
                if (s.x >= cellCountX ) s.x = 0;
                if (s.y < 0) s.y = (cellCountY - 1) ;
                if (s.y >= cellCountY ) s.y = 0;
            }
            //Drawing the snake
            int i = 0;
            for (auto& s : snake)
            {
                if (i == 0)
                {
                    DrawCell(snake[i].x * cellSize, snake[i].y * cellSize, Color {0, 100, 150, 255});
                    i++;
                }
                else
                    DrawCell(s.x * cellSize, s.y * cellSize, Color {0, 200, 150, 255});
            }
            //snake eats it gets bigger
            if (snake[0].x == fruitPosition.x && snake[0].y == fruitPosition.y)
            {
                generateFruitRandom();
                snake.push_back( Vector2 { snake[snake.size() - 1].x, snake[snake.size() - 1].y });
            }

        }

            EndDrawing();
    }
}

void DrawBackground(int screenWidth, int screenHeight)
{

    ClearBackground(Color{50, 50, 50, 255});
    for (int j = 0; j < screenHeight; j += cellSize)
    {
        for (int i = 0; i < screenWidth; i += cellSize)
        {
            if (((i / cellSize) + (j / cellSize)) % 2 == 0)
            {
                DrawRectangle(i, j,
                              cellSize, cellSize,
                              Color{70, 70, 70, 255});
            }
        }
    }
}
