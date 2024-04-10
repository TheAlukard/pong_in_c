#include "include/raylib.h"
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

void Update(void);
void Draw(void);

#define screenWidth 500
#define screenHeight 500
#define ballRadius 15
#define rodHeight 100
#define rodWidth 20
float rodSpeed   = 6.f;
float ballXspeed = 7.f;
float ballYspeed = 7.f;
float gravity    = 2.5f;

int score = 0;
int lives = 3;

Vector2 ball     = {.x = screenWidth / 2, .y = screenHeight / 2};
Vector2 player   = {.x = screenWidth - (rodWidth / 2), .y = screenHeight / 2};
Vector2 computer = {.x = (rodWidth / 2), .y = screenHeight / 2};

typedef enum {
    rWall,
    lWall,
    uWall,
    dWall,
} CollType;

int main(void)
{
    InitWindow(screenWidth, screenHeight, "Pong in C");
    
    SetTargetFPS(60);

    while (!WindowShouldClose())    
    {
        Update();
        
        BeginDrawing();

        Draw();
        
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

void draw_string(char *text, int fontsize, Vector2 pos, Color color, bool half) 
{
    Font font = GetFontDefault();
    float fontspacing = 3;
    Vector2 measurement = half ? MeasureTextEx(font, text, fontsize, fontspacing) : (Vector2){0, 0};
    DrawTextEx(font, text, (Vector2){pos.x - (measurement.x / 2), pos.y - (measurement.y / 2)},
               fontsize, fontspacing, color);
}

void get_minmax(Vector2 pos, float width, float height, Vector2 *min, Vector2 *max) 
{
    *min = (Vector2){.x = pos.x - (width / 2), .y = pos.y - (height / 2)};
    *max = (Vector2){.x = pos.x + (width / 2), .y = pos.y + (height / 2)};
}

bool check_collision(Vector2 firstmin, Vector2 firstmax, Vector2 secondmin, Vector2 secondmax)
{
    if (firstmin.x > secondmax.x) return false;
    if (firstmax.x < secondmin.x) return false;
    if (firstmin.y > secondmax.y) return false;
    if (firstmax.y < secondmin.y) return false;

    return true;
}

void ball_coll()
{
    Vector2 ballmin;
    Vector2 ballmax;
    Vector2 playermin;
    Vector2 playermax;
    Vector2 compmin;
    Vector2 compmax;

    get_minmax(ball, ballRadius * 2, ballRadius * 2, &ballmin, &ballmax);
    get_minmax((Vector2){.x = player.x - 7.5f, .y = player.y}, rodWidth - 15, rodHeight, &playermin, &playermax);
    get_minmax((Vector2){.x = computer.x - 7.5f, .y = computer.y}, rodWidth - 15, rodHeight, &compmin, &compmax);

    if (check_collision(ballmin, ballmax, playermin, playermax)) {
        ballXspeed *= -1;
        ball.x = player.x - (rodWidth / 2);
        ball.x -= ballRadius;
    }
    else if (check_collision(ballmin, ballmax, compmin, compmax)) {
        ballXspeed *= -1;
        ball.x = computer.x + (rodWidth / 2);
        ball.x += ballRadius;
    }
    if (ball.y - (ballRadius) < 0) {
        ballYspeed *= -1;
    }
    if (ball.y + (ballRadius) >= screenHeight) {
        ballYspeed *= -1;
    }
    if (ball.x + (ballRadius) < 0) {
        score++;
        ball.x = screenWidth / 2;
        ball.y = ballRadius; 
        ballXspeed = ballXspeed > 0 ? ballXspeed * -1 : ballXspeed;
    }    
    if (ball.x - (ballRadius) >= screenWidth) {
        lives--;
        ball.x = screenWidth / 2;
        ball.y = ballRadius; 
        ballXspeed = ballXspeed > 0 ? ballXspeed : ballXspeed * -1;
    }
}

void rod_coll(Vector2 *pos, float width, float height)
{
    if (pos->x - (width / 2) < 0) {
        pos->x = width / 2;
    }
    if (pos->x + (width / 2) >= screenWidth) {
        pos->x = screenWidth - (width / 2);
    }
    if (pos->y - (height / 2) < 0) {
        pos->y = height / 2;
    }
    if (pos->y + (height / 2) >= screenHeight) {
        pos->y = screenHeight - (height / 2);
    }
}
  
void Update(void)
{
    if (lives <= 0) {
        score = 0;
        lives = 3;
        ball     = (Vector2){.x = screenWidth / 2, .y = screenHeight / 2};
        player   = (Vector2){.x = screenWidth - (rodWidth / 2), .y = screenHeight / 2};
        computer = (Vector2){.x = (rodWidth / 2), .y = screenHeight / 2};
        rodSpeed   = 4.5f;
        ballXspeed = 7.f;
        ballYspeed = 7.f;
        gravity    = 2.5f;
    }
    
    if (IsKeyDown(KEY_UP)) {
        player.y -= rodSpeed;
    }   
    if (IsKeyDown(KEY_DOWN)) { 
        player.y += rodSpeed;
    }   
    ball.x += ballXspeed;
    ball.y += ballYspeed;
    float error = 40.f;
    if (computer.y + error < ball.y) {
        computer.y += rodSpeed;
    }
    else if (computer.y - error > ball.y) {
        computer.y -= rodSpeed;
    }
    rod_coll(&player, rodWidth, rodHeight);
    rod_coll(&computer, rodWidth, rodHeight);
    ball_coll();
}

void Draw(void)
{
    ClearBackground(BLACK);
    char SCORE[100] = "SCORE: ";
    char s_score[50];
    char LIVES[100] = "Lives: ";
    char l_lives[50];
    itoa(score, s_score, 10);
    char *f_score = strcat(SCORE, s_score);
    itoa(lives, l_lives, 10);
    char *f_lives = strcat(LIVES, l_lives);
    DrawRectanglePro((Rectangle){.x = player.x, .y = player.y, .width = rodWidth, .height = rodHeight},
                        (Vector2){.x = rodWidth / 2, .y = rodHeight / 2}, 0.f, YELLOW);
    DrawRectanglePro((Rectangle){.x = computer.x, .y = computer.y, .width = rodWidth, .height = rodHeight},
                        (Vector2){.x = rodWidth / 2, .y = rodHeight / 2}, 0.f, YELLOW);
    DrawCircle(ball.x, ball.y, ballRadius, RED);
    draw_string(f_score, 30, (Vector2){10, 10}, (Color){BLUE.r, BLUE.g, BLUE.b, 150}, false);
    draw_string(f_lives, 30, (Vector2){screenWidth - 200, 10}, (Color){BLUE.r, BLUE.g, BLUE.b, 150}, false);
}

