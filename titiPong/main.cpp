#include "raylib.h"
#include <format>

enum class GameState
{
    PLAYING,
    FINISHED,
    RESET
};
struct Vector2d
{
    float x {};
    float y {};
};
struct RectanglePlayer
{
    float width{};
    float height{};
    float x{};
    float y{};
    float speed{};
    int score {};
};

struct Ball
{
    float centerX {};
    float centerY {};
    float radius {};
    Color color {};
    float vitesse {};
};

int screen_width = 2560;
int screen_height = 1600;
constexpr int max_score = 7;
void start_launch(RectanglePlayer&, RectanglePlayer&, Ball&, Vector2d&, GameState&, int);
void handle_input_event(RectanglePlayer&, RectanglePlayer&);
void draw_game(const RectanglePlayer&, const RectanglePlayer&, const Ball&, int, int);
void reset_game(RectanglePlayer&, RectanglePlayer&, Ball&, GameState&);

int main()
{
    float player_width { static_cast<float>(screen_height) / 50.0f };
    float player_height { static_cast<float>(screen_height) / 5.0f };

    InitWindow(screen_width, screen_height, "titiPong");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);


    RectanglePlayer first_player_rectangle {
        .width = player_width,
        .height = player_height,
        .x = static_cast<float>(screen_width) / 50.0f,
        .y = (static_cast<float>(screen_height) / 2.0f) - (player_height / 2.0f),
        .speed = static_cast<float>(screen_height)
    };

    RectanglePlayer second_player_rectangle {
        .width = player_width,
        .height = player_height,
        .x = static_cast<float>(screen_width) - (static_cast<float>(screen_width) / 50.0f) - player_width,
        .y = first_player_rectangle.y,
        .speed = static_cast<float>(screen_height)
    };

    Ball ball {
        static_cast<float>(screen_width) / 2.0f,
        static_cast<float>(screen_height) / 2.0f,
        static_cast<float>(screen_width + screen_height) / 250.0f,
        RED,
        static_cast<float>(screen_height)
    };

    Vector2d ball_direction { 1.0f, 0.2f };
    GameState gamestate = GameState::PLAYING;
    while (!WindowShouldClose())
    {

        //====================Window Resize Handling======================
        if (IsWindowResized() && !IsWindowFullscreen())
        {
            screen_width =  GetScreenWidth();
            screen_height = GetScreenHeight();
        }

        // check for alt + enter
        if (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)))
        {
            int display = GetCurrentMonitor();

            if (IsWindowFullscreen())
            {
                SetWindowSize(screen_width, screen_height);
            }
            else
            {
                SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));
            }
            ToggleFullscreen();
        }
        //===========================================

        BeginDrawing();
        ClearBackground(BLACK);
        if (gamestate == GameState::PLAYING)
        {
            draw_game(first_player_rectangle,
                second_player_rectangle,
                ball,
                first_player_rectangle.score,
                second_player_rectangle.score
                );
            start_launch(
                first_player_rectangle,
                second_player_rectangle,
                ball,
                ball_direction,
                gamestate,
                max_score);
            handle_input_event(first_player_rectangle, second_player_rectangle);
        }
        else if (gamestate == GameState::RESET)
        {
            gamestate = GameState::PLAYING;
            reset_game(first_player_rectangle, second_player_rectangle, ball, gamestate);
        }
        else if (gamestate == GameState::FINISHED)
        {
            if (IsKeyPressed(KEY_ESCAPE))
            {
                gamestate = GameState::PLAYING;
            }
        }

        EndDrawing();

    }

    CloseWindow();
    return 0;
}
bool is_bound(const RectanglePlayer& player,const Ball& ball)
{
    return ball.centerY > player.y && ball.centerY < (player.y + player.height);
}
void transform_direction_upon_hit(Vector2d direction)
{

}
bool still_playing(
    const Ball& ball,
    const RectanglePlayer& first_player_rectangle,
    const RectanglePlayer& second_player_rectangle,
    int max_score)
{
    return first_player_rectangle.score <= max_score && second_player_rectangle.score <= max_score;
}
void start_launch(
    RectanglePlayer& first_player,
    RectanglePlayer& second_player,
    Ball& ball,
    Vector2d& direction,
    GameState& gamestate,
    int max_score
    )
{
    //initial check if the game we are still below the max score
    if (!still_playing(ball, first_player, second_player, max_score))
    {
        gamestate = GameState::FINISHED;
        return;
    }
    if ((ball.centerX - ball.radius) < (first_player.x + first_player.width) && is_bound(first_player, ball))
    {
        direction.x = -direction.x;
    }
    if ((ball.centerX + ball.radius) > second_player.x && is_bound(second_player, ball))
    {
        direction.x = -direction.x;
    }
    if ((ball.centerY + ball.radius) > screen_height || (ball.centerY - ball.radius) < 0)
    {
        direction.y = -direction.y;
    }
    if ((ball.centerX - ball.radius) < 0)
    {
        second_player.score += 1;
        gamestate = GameState::RESET;
    }
    if ( (ball.centerX + ball.radius) > screen_width)
    {
        first_player.score += 1;
        gamestate = GameState::RESET;
    }
    ball.centerX += (ball.vitesse * GetFrameTime()) * direction.x;
    ball.centerY += (ball.vitesse * GetFrameTime()) * direction.y;
}

void handle_input_event(RectanglePlayer& first_player_rectangle, RectanglePlayer& second_player_rectangle)
{
    auto current_screen_height = screen_height;

    if (IsKeyDown(KEY_UP) && first_player_rectangle.y > 0.0f)
    {
        first_player_rectangle.y -= first_player_rectangle.speed * GetFrameTime();
    }
    else if (IsKeyDown(KEY_DOWN) && first_player_rectangle.y < (current_screen_height - first_player_rectangle.height))
    {
        first_player_rectangle.y += first_player_rectangle.speed * GetFrameTime();
    }

    if (IsKeyDown(KEY_W) && second_player_rectangle.y > 0.0f)
    {
        second_player_rectangle.y -= second_player_rectangle.speed * GetFrameTime();
    }
    else if (IsKeyDown(KEY_S) && second_player_rectangle.y < (current_screen_height - second_player_rectangle.height))
    {
        second_player_rectangle.y += second_player_rectangle.speed * GetFrameTime();
    }
}

void draw_game(
    const RectanglePlayer& first_player_rectangle,
    const RectanglePlayer& second_player_rectangle,
    const Ball& ball,
    int first_player_score,
    int second_player_score)
{
    DrawRectangle(
        static_cast<int>(first_player_rectangle.x),
        static_cast<int>(first_player_rectangle.y),
        static_cast<int>(first_player_rectangle.width),
        static_cast<int>(first_player_rectangle.height),
        WHITE
    );
    DrawRectangle(
        static_cast<int>(second_player_rectangle.x),
        static_cast<int>(second_player_rectangle.y),
        static_cast<int>(second_player_rectangle.width),
        static_cast<int>(second_player_rectangle.height),
        WHITE
    );
    DrawCircle(
        static_cast<int>(ball.centerX),
        static_cast<int>(ball.centerY),
        ball.radius,
        ball.color
    );
    std::string formatted_string = std::format("{} : {}", first_player_score, second_player_score);
    const char* score_text = formatted_string.c_str();
    int font_size { 200 };
    DrawText(score_text ,screen_width / 2 - font_size, 0 , font_size, WHITE);
}
void reset_game(RectanglePlayer& first_player, RectanglePlayer& second_player, Ball& ball, GameState& gamestate)
{

    first_player.x = static_cast<float>(screen_width) / 50.0f;
    first_player.y = (static_cast<float>(screen_height) / 2.0f) - (first_player.height / 2.0f);

    second_player.x = static_cast<float>(screen_width) - (static_cast<float>(screen_width) / 50.0f) - second_player.width;
    second_player.y = first_player.y;

    ball = {
        static_cast<float>(screen_width) / 2.0f,
        static_cast<float>(screen_height) / 2.0f,
        static_cast<float>(screen_width + screen_height) / 250.0f,
        RED,
        static_cast<float>(screen_height)
    };
    if (gamestate == GameState::FINISHED)
    {
        first_player.score = 0;
        second_player.score = 0;
        gamestate = GameState::PLAYING;
    }
}
