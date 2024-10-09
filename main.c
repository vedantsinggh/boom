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
#define LINE_OF_VIEW 100
#define DELTA 5
#define ANGLE_DELTA 1

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

typedef struct Line {
    float theta;
    float r;
} Line;

float Deg2Rad(float deg) {
    return (deg) * (PI / 180.0f);
}

float distanceBtw(Vector2 a, Vector2 b){
	return sqrt(pow(a.x - b.x,2) + pow(a.y - b.y,2));
}

Vector2 Polar2Cart(Line line) {
    float theta = Deg2Rad(line.theta);
    return (Vector2){cosf(theta) * line.r, sinf(theta) * line.r};
}

void Grid() {
    for(int i = 0; i < GRID_SIZE; i++) {
        for(int j = 0; j < GRID_SIZE; j++) {
            if (maps[i][j] != 0) {
                DrawRectangle(HEIGHT / GRID_SIZE * j, HEIGHT / GRID_SIZE * i, HEIGHT / GRID_SIZE , WIDTH / GRID_SIZE, BLUE);
            }
        }
    }
    for (int i = 0; i <= WIDTH; i += WIDTH / GRID_SIZE) {
        DrawLine(i, 0, i, HEIGHT, BLACK);
    }
    for (int i = 0; i <= HEIGHT; i += HEIGHT / GRID_SIZE) {
        DrawLine(0, i, WIDTH, i, BLACK);
    }
}

Vector2 GetNearestSnappedPoint(Vector3 pos){
    Vector2 snappedPointX;
	if(pos.z < 90 || pos.z > 270) snappedPointX.x = ceil(pos.x/GRID_LENGTH)*GRID_LENGTH;
	else snappedPointX.x = floor(pos.x/GRID_LENGTH)*GRID_LENGTH;

	float dist = abs(pos.x - snappedPointX.x);
	float mag = abs(tanf(pos.z * (PI/180.0f)) * dist);
    if(pos.z > 0 && pos.z < 90) snappedPointX.y = pos.y + mag;
	else if(pos.z > 90 && pos.z < 180) snappedPointX.y = pos.y + mag;
	else if(pos.z > 180 && pos.z < 270) snappedPointX.y = pos.y - mag;
	else if(pos.z > 270 && pos.z < 360) snappedPointX.y = pos.y - mag;
	else snappedPointX.y = pos.y;

    Vector2 snappedPointY;
	if(pos.z < 180) snappedPointY.y = ceil(pos.y/GRID_LENGTH)*GRID_LENGTH;
	else snappedPointY.y = floor(pos.y/GRID_LENGTH)*GRID_LENGTH;
	snappedPointY.x = pos.x;

	dist = abs(pos.y - snappedPointY.y);
	mag = abs(tanf((90 - pos.z) * (PI/180.0f)) * dist);
    if(pos.z > 0 && pos.z < 90) snappedPointY.x = pos.x + mag;
	else if(pos.z > 90 && pos.z < 180) snappedPointY.x = pos.x -  mag;
	else if(pos.z > 180 && pos.z < 270) snappedPointY.x = pos.x - mag;
	else if(pos.z > 270 && pos.z < 360) snappedPointY.x = pos.x + mag;
	else snappedPointY.x = pos.x;
    
    if (distanceBtw((Vector2){pos.x, pos.y}, snappedPointX) < 
			distanceBtw((Vector2){pos.x, pos.y}, snappedPointY))
		return snappedPointX;
	else 
		return snappedPointY;
}

void CreateRay(Vector3 pos, float angle){
	pos.z += angle;
    Vector2 angleRay = Polar2Cart((Line){pos.z, LINE_OF_VIEW});
    Vector2 rayEnd = {angleRay.x + pos.x, angleRay.y + pos.y};

    //DrawCircle(rayEnd.x, rayEnd.y, 2, GREEN);

	Vector3 lastPoint = pos;
	Vector2 lastNearestPoint;
	float dist = 0;
	bool hit = false;
	for(int i=0; i <= LINE_OF_VIEW; i += dist){
		lastNearestPoint = GetNearestSnappedPoint(lastPoint);
		dist = distanceBtw((Vector2){lastPoint.x, lastPoint.y}, lastNearestPoint);
		if(dist + i > LINE_OF_VIEW) break;

		Vector2 nextRay = Polar2Cart((Line){lastPoint.z, dist + DELTA});
		if (maps[(int)((lastPoint.y + nextRay.y)/GRID_LENGTH)][(int)((lastPoint.x + nextRay.x)/GRID_LENGTH)]){
			DrawCircleV(lastNearestPoint, 4, PINK);
			hit = true;
			break;
		}
		lastPoint.x = nextRay.x + lastPoint.x;
		lastPoint.y = nextRay.y + lastPoint.y;
	}
    DrawLine(pos.x, pos.y, hit ? lastNearestPoint.x : rayEnd.x, hit ? lastNearestPoint.y : rayEnd.y, GREEN);
}

void DrawPlayer(Vector3 pos) {
    DrawCircle(pos.x, pos.y, PLAYER_RADIUS, RED);
    DrawCircleSector((Vector2){pos.x, pos.y}, PLAYER_RADIUS, pos.z - ANGLE_OF_VIEW / 2, pos.z + ANGLE_OF_VIEW / 2, 5, YELLOW);
    
	for(int i= -1 * ANGLE_OF_VIEW/2; i <= ANGLE_OF_VIEW/2; i+= ANGLE_DELTA){
		CreateRay(pos, i);
	}
}

void Update(Vector3* player) {
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
    InitWindow(WIDTH, HEIGHT, "BOOM");

    Vector3 player = {GRID_LENGTH * 1.5f, GRID_LENGTH * 1.5f, 0}; 

    while (!WindowShouldClose()) {
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
