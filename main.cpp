#include "raylib.h"
#include <vector>
#include <cmath>

#define WORLD_SIZE 20000
#define CELL_SIZE 20
#define VIEW_DISTANCE 150

enum BiomeType { FOREST, DESERT, TUNDRA, MOUNTAINS, PLAINS };

float GetHeight(float x, float z, BiomeType biome) {
    switch (biome) {
        case FOREST:    return sinf(x*0.002f) * cosf(z*0.002f) * 12.0f;
        case DESERT:    return sinf(x*0.0008f) * cosf(z*0.0008f) * 4.0f;
        case TUNDRA:    return sinf(x*0.0012f) * cosf(z*0.0012f) * 3.0f;
        case MOUNTAINS: return sinf(x*0.003f) * cosf(z*0.003f) * 40.0f;
        case PLAINS:    return sinf(x*0.0015f) * cosf(z*0.0015f) * 5.0f;
        default:        return 0.0f;
    }
}

BiomeType GetBiome(float x, float z) {
    if (x < 5000) return FOREST;
    if (x < 10000) return DESERT;
    if (x < 15000) return MOUNTAINS;
    if (z < 10000) return TUNDRA;
    return PLAINS;
}

Color GetBiomeColor(BiomeType biome) {
    switch (biome) {
        case FOREST:    return (Color){ 60, 100, 60, 255 };
        case DESERT:    return (Color){ 194, 178, 128, 255 };
        case TUNDRA:    return (Color){ 190, 190, 210, 255 };
        case MOUNTAINS: return GRAY;
        case PLAINS:    return (Color){ 110, 140, 60, 255 };
        default:        return DARKGRAY;
    }
}

int main() {
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "New Era: World Biomes");
    SetTargetFPS(60);

    Camera3D camera = { 0 };
    camera.position = (Vector3){ 100.0f, 20.0f, 100.0f };
    camera.target = (Vector3){ 101.0f, 20.0f, 100.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 65.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    SetCameraMode(camera, CAMERA_FIRST_PERSON);

    while (!WindowShouldClose()) {
        UpdateCamera(&camera);

        BeginDrawing();
        ClearBackground(SKYBLUE);
        BeginMode3D(camera);

        float px = camera.position.x;
        float pz = camera.position.z;

        for (float x = px - VIEW_DISTANCE; x < px + VIEW_DISTANCE; x += CELL_SIZE) {
            for (float z = pz - VIEW_DISTANCE; z < pz + VIEW_DISTANCE; z += CELL_SIZE) {
                if (x < 0 || z < 0 || x > WORLD_SIZE || z > WORLD_SIZE) continue;

                BiomeType biome = GetBiome(x, z);

                float h1 = GetHeight(x, z, biome);
                float h2 = GetHeight(x + CELL_SIZE, z, biome);
                float h3 = GetHeight(x, z + CELL_SIZE, biome);
                float h4 = GetHeight(x + CELL_SIZE, z + CELL_SIZE, biome);

                Vector3 v1 = { x, h1, z };
                Vector3 v2 = { x + CELL_SIZE, h2, z };
                Vector3 v3 = { x, h3, z + CELL_SIZE };
                Vector3 v4 = { x + CELL_SIZE, h4, z + CELL_SIZE };

                Color col = GetBiomeColor(biome);
                DrawTriangle3D(v1, v3, v4, col);
                DrawTriangle3D(v1, v4, v2, col);

                // Ближайшие деревья
                if (biome == FOREST && ((int)x + (int)z) % 300 == 0)
                    DrawCube((Vector3){x + 5, h1 + 8, z + 5}, 2, 16, 2, DARKGREEN);
                if (biome == MOUNTAINS && ((int)x + (int)z) % 500 == 0)
                    DrawCube((Vector3){x + 5, h1 + 12, z + 5}, 1.5f, 24, 1.5f, GRAY);
                if (biome == PLAINS && ((int)x + (int)z) % 400 == 0)
                    DrawCube((Vector3){x + 5, h1 + 4, z + 5}, 1, 8, 1, GREEN);
                if (biome == DESERT && ((int)x + (int)z) % 600 == 0)
                    DrawCube((Vector3){x + 5, h1 + 3, z + 5}, 1, 6, 1, BEIGE);
                if (biome == TUNDRA && ((int)x + (int)z) % 700 == 0)
                    DrawCube((Vector3){x + 5, h1 + 2, z + 5}, 1, 4, 1, LIGHTGRAY);
            }
        }

        EndMode3D();
        DrawText(TextFormat("X: %.1f, Z: %.1f", px, pz), 20, 20, 20, BLACK);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}