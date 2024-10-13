#include "raylib.h"
#include <stdlib.h>
#include <math.h>

#define WIDTH 800
#define HEIGHT 800
#define GRID_SIZE 10
#define GRID_LENGTH 80
#define SPEED 0.04f
#define ROTATION_SPEED 0.1f
#define PLAYER_RADIUS 10
#define ANGLE_OF_VIEW 60
#define LINE_OF_VIEW 1000
#define DELTA 5
#define ANGLE_DELTA 1

int maps[10][10] = {
    {1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,1,1,1},
    {1,0,0,0,0,0,0,1,1,1},
    {1,0,0,0,0,0,0,1,1,1},
    {1,1,1,1,1,1,1,1,1,1},
};

typedef struct Line {
    float theta;
    float r;
} Line;

float Deg2Rad(float deg) {
    return (deg) * (PI / 180.0f);
}

float distanceBtw(Vector2 a, Vector2 b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

Vector2 Polar2Cart(Line line) {
    float theta = Deg2Rad(line.theta);
    return (Vector2){cosf(theta) * line.r, sinf(theta) * line.r};
}

int CastRay(Vector3 pos, float angle) {
    float rayDirX = cosf(Deg2Rad(angle));
    float rayDirY = sinf(Deg2Rad(angle));
    
    float wallDist = 0;
    int hit = 0;

    while (wallDist < LINE_OF_VIEW && !hit) {
        wallDist += 0.1f; 

        int mapX = (int)(pos.x + rayDirX * wallDist) / GRID_LENGTH;
        int mapY = (int)(pos.y + rayDirY * wallDist) / GRID_LENGTH;

        if (mapX < 0 || mapX >= GRID_SIZE || mapY < 0 || mapY >= GRID_SIZE) break; 

        if (maps[mapY][mapX] == 1) {
            hit = 1;
        }
    }

    return hit ? (int)wallDist : LINE_OF_VIEW;
}

void Draw3D(int* distances) {
    const int res = (int)(ANGLE_OF_VIEW / ANGLE_DELTA);
    for (int i = 0; i < res; i++) {
        int wallDistance = distances[i];
        if (wallDistance > 0 && wallDistance < LINE_OF_VIEW) {
            int wallHeight = (int)(HEIGHT / wallDistance);
            int wallWidth = (WIDTH / res);
            
            int xPosition = (i * wallWidth);
            int yPosition = (HEIGHT / 2) - (wallHeight / 2);

            if (yPosition < 0) yPosition = 0;
            if (yPosition + wallHeight > HEIGHT) wallHeight = HEIGHT - yPosition;

            DrawRectangle(xPosition, yPosition, wallWidth, wallHeight, (Color){139, 69, 19, 255}); 
        }
    }
}

void DrawScreen(Vector3 player) {
    int hght[(int)ANGLE_OF_VIEW / ANGLE_DELTA] = {0};
    int counter = 0;

    for (int i = -ANGLE_OF_VIEW / 2; i <= ANGLE_OF_VIEW / 2; i += ANGLE_DELTA) {
        hght[counter] = CastRay(player, player.z + i);
        counter++;
    }

    Draw3D(hght);
}

void UpdateControl(Vector3* player) {
    if (IsKeyDown(KEY_RIGHT)) player->z += ROTATION_SPEED;
    if (IsKeyDown(KEY_LEFT)) player->z -= ROTATION_SPEED;

    if (player->z < 0) player->z += 360;
    if (player->z > 360) player->z -= 360;

    if (IsKeyDown(KEY_UP)) {
        player->x += cosf(player->z * (PI / 180.0f)) * SPEED;
        player->y += sinf(player->z * (PI / 180.0f)) * SPEED;
    }
    if (IsKeyDown(KEY_DOWN)) {
        player->x -= cosf(player->z * (PI / 180.0f)) * SPEED;
        player->y -= sinf(player->z * (PI / 180.0f)) * SPEED;
    }

    if (player->x < 0) player->x = WIDTH;
    if (player->y < 0) player->y = HEIGHT;
    if (player->x > WIDTH) player->x = 0;
    if (player->y > HEIGHT) player->y = 0;
}

int main(void) {
    InitWindow(WIDTH, HEIGHT, "Doom-like Raycasting");
    Vector3 player = {GRID_LENGTH * 1.5f, GRID_LENGTH * 1.5f, 0}; 

    while (!WindowShouldClose()) {
        UpdateControl(&player);
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawScreen(player);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
