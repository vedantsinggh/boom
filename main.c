#include "raylib.h"
#include <math.h>

#define WIDTH 800
#define HEIGHT 800
#define GRID_SIZE 10
#define SPEED 0.02f
#define ROTATION_SPEED 0.1f
#define PLAYER_RADIUS 10
#define ANGLE_OF_VIEW 45
#define LINE_OF_VIEW  100

int maps[10][10] =  {
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

void Grid(){
	for(int i=0; i < GRID_SIZE; i++){
		for(int j=0; j < GRID_SIZE; j++){
			if (maps[i][j] != 0){
				DrawRectangle(HEIGHT/GRID_SIZE * j, HEIGHT/GRID_SIZE * i, HEIGHT/GRID_SIZE , WIDTH/GRID_SIZE, BLUE);
			}
		}
	}
	for (int i=0; i <= WIDTH; i+= WIDTH/GRID_SIZE){
		 DrawLine(i, 0,i,HEIGHT, BLACK );
	}
	for (int i=0; i <= HEIGHT; i+= HEIGHT/GRID_SIZE){
		 DrawLine(0, i,WIDTH,i, BLACK );
	}
}

void DrawPlayer(Vector3 pos){
	DrawCircle(pos.x,pos.y, PLAYER_RADIUS, RED);
	DrawCircleSector((Vector2){pos.x, pos.y},PLAYER_RADIUS, pos.z - ANGLE_OF_VIEW/2,pos.z  + ANGLE_OF_VIEW/2, 5, YELLOW);
	

	//here we draw player FOV
	float theta = pos.z * (PI / 180.0f);
	float front_x = pos.x + LINE_OF_VIEW * cosf(theta);
	float front_y = pos.y + LINE_OF_VIEW * sinf(theta);
	DrawCircle(front_x, front_y, 2, GREEN);

	DrawLine(pos.x, pos.y, front_x, front_y, GREEN);
}

void Update(Vector3* player){
	if (IsKeyDown(KEY_RIGHT)) player->z += ROTATION_SPEED;
	if (IsKeyDown(KEY_LEFT)) player->z -= ROTATION_SPEED;
	if (IsKeyDown(KEY_UP)) {
		player->x += cosf(player->z * (PI / 180.0f)) * SPEED;
		player->y += sinf(player->z * (PI / 180.0f)) * SPEED;
	};
	if (IsKeyDown(KEY_DOWN)) {
		player->x -= cosf(player->z * (PI / 180.0f)) * SPEED;
		player->y -= sinf(player->z * (PI / 180.0f)) * SPEED;
	};
}

int main(void)
{
    InitWindow(WIDTH, HEIGHT, "BOOM!");

	Vector3 player = {0}; 

    while (!WindowShouldClose())
    {
		Update(&player);
        BeginDrawing();
            ClearBackground(RAYWHITE);
			Grid();
			DrawPlayer(player);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
