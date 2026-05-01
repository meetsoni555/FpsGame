
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include <vector>

struct Enemy {
    Vector3 position;
    bool active;
};

void SpawnWave(std::vector<Enemy> &enemies, int count, Vector3 playerPos) {
    enemies.clear();
    for (int i = 0; i < count; i++) {
        float angle = (float)GetRandomValue(0, 360) * DEG2RAD;
        float dist = (float)GetRandomValue(20, 35); 
        Vector3 pos = { playerPos.x + cosf(angle) * dist, 1.0f, playerPos.z + sinf(angle) * dist };
        enemies.push_back({ pos, true });
    }
}

int main() {
    InitWindow(980, 720, "Raylib FPS - Gun Calibration");
    SetExitKey(KEY_NULL);

    Model gunModel = LoadModel("gun.glb"); 

    // ************************************************************
    // AREA 1: ADJUST POSITION AND SCALE
    // Change these numbers to move the gun on screen
    // ************************************************************
    float gunScale = 0.5f;          // Scale: Larger or smaller
    float offsetForward = 0.7f;     // Position: In/Out
    float offsetRight = 0.35f;      // Position: Left/Right
    float offsetDown = -0.35f;      // Position: Up/Down
    // ************************************************************

    // ************************************************************
    // AREA 2: ROTATE THE MODEL BASE
    // Change the 0 or 90 to adjust how the model sits in your hand
    // ************************************************************
    Matrix fixTransform = MatrixMultiply(
        MatrixScale(gunScale, gunScale, gunScale),
        MatrixMultiply(
            MatrixRotateX(0 * DEG2RAD),  // Tilt up/down
            MatrixRotateY(90 * DEG2RAD)  // Turn left/right
        )
    );
    // ************************************************************

    Camera3D camera = { (Vector3){ 0.0f, 2.0f, 10.0f }, (Vector3){ 0.0f, 2.0f, 0.0f }, (Vector3){ 0.0f, 1.0f, 0.0f }, 90.0f, 0 };
    std::vector<Enemy> enemies;
    int waveSize = 5; int score = 0; bool isGameOver = false;
    float verticalVelocity = 0.0f; bool isGrounded = true; const float gravity = 0.8f; bool cursorLocked = true;

    SpawnWave(enemies, waveSize, camera.position);
    DisableCursor();
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (isGameOver) {
            if (IsKeyPressed(KEY_R)) {
                score = 0; waveSize = 5; isGameOver = false;
                camera.position = (Vector3){ 0.0f, 2.0f, 10.0f };
                camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };
                SpawnWave(enemies, waveSize, camera.position);
                DisableCursor(); cursorLocked = true;
            }
        } else {
            if (IsKeyPressed(KEY_ESCAPE)) {
                cursorLocked = !cursorLocked;
                if (cursorLocked) DisableCursor(); else EnableCursor();
            }

            if (cursorLocked) {
                // Save position before internal update to calculate extra speed
                Vector3 oldPos = camera.position;
                UpdateCamera(&camera, CAMERA_FIRST_PERSON);

                // ************************************************************
                // AREA 3: PLAYER SPEED BOOST
                // Increase 'extraSpeed' to move even faster (0.5 is a 50% boost)
                // ************************************************************
                float extraSpeed = 0.5f; 
                Vector3 moveDiff = Vector3Subtract(camera.position, oldPos);
                moveDiff.y = 0; // Don't speed up gravity or jumping
                camera.position = Vector3Add(camera.position, Vector3Scale(moveDiff, extraSpeed));
                camera.target = Vector3Add(camera.target, Vector3Scale(moveDiff, extraSpeed));
                // ************************************************************

                if (isGrounded && IsKeyPressed(KEY_SPACE)) { verticalVelocity = 0.15f; isGrounded = false; }
                if (!isGrounded) {
                    verticalVelocity -= gravity * GetFrameTime();
                    camera.position.y += verticalVelocity;
                    camera.target.y += verticalVelocity;
                }
                if (camera.position.y <= 2.0f) { camera.position.y = 2.0f; isGrounded = true; }

                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    Ray ray = { camera.position, Vector3Normalize(Vector3Subtract(camera.target, camera.position)) };
                    for (auto& enemy : enemies) {
                        if (enemy.active) {
                            BoundingBox box = { { enemy.position.x - 0.5f, 0.0f, enemy.position.z - 0.5f }, { enemy.position.x + 0.5f, 2.0f, enemy.position.z + 0.5f } };
                            if (GetRayCollisionBox(ray, box).hit) { enemy.active = false; score++; }
                        }
                    }
                }

                bool allDead = true;
                for (auto& enemy : enemies) {
                    if (enemy.active) {
                        allDead = false;
                        Vector3 dir = Vector3Normalize(Vector3Subtract(camera.position, enemy.position));
                        enemy.position.x += dir.x * 3.5f * GetFrameTime(); 
                        enemy.position.z += dir.z * 3.5f * GetFrameTime();
                        float dist = Vector2Distance((Vector2){camera.position.x, camera.position.z}, (Vector2){enemy.position.x, enemy.position.z});
                        if (dist < 1.3f) { isGameOver = true; EnableCursor(); }
                    }
                }
                if (allDead) { waveSize++; SpawnWave(enemies, waveSize, camera.position); }
            }
        }

        BeginDrawing();
            ClearBackground(SKYBLUE);
            if (!isGameOver) {
                BeginMode3D(camera);
                    DrawGrid(50, 1.0f);
                    DrawPlane((Vector3){0,0,0}, (Vector2){100, 100}, LIGHTGRAY);
                    for (auto& e : enemies) if (e.active) { DrawCube(e.position, 1, 2, 1, MAROON); DrawCubeWires(e.position, 1, 2, 1, BLACK); }
                EndMode3D();

                Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
                Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, camera.up));
                float yaw = atan2f(forward.x, forward.z);
                float pitch = asinf(forward.y);
                
                Matrix camRotation = MatrixRotateXYZ((Vector3){ -pitch, yaw, 0 });
                gunModel.transform = MatrixMultiply(fixTransform, camRotation);
                
                Vector3 gunPos = Vector3Add(camera.position, 
                                 Vector3Add(Vector3Scale(forward, offsetForward), 
                                 Vector3Add(Vector3Scale(right, offsetRight), 
                                 Vector3Scale(camera.up, offsetDown))));

                rlDisableDepthTest();
                    BeginMode3D(camera);
                        DrawModel(gunModel, gunPos, 1.0f, WHITE); 
                    EndMode3D();
                rlEnableDepthTest();

                DrawCircle(GetScreenWidth()/2, GetScreenHeight()/2, 4, GREEN);
                DrawText(TextFormat("SCORE: %i", score), 20, 20, 20, BLACK);
            } else {
                ClearBackground(BLACK);
                DrawText("GAME OVER", GetScreenWidth()/2 - 120, 250, 40, RED);
                DrawText(TextFormat("FINAL SCORE: %i", score), GetScreenWidth()/2 - 100, 320, 30, WHITE);
                DrawText("PRESS 'R' TO RESTART", GetScreenWidth()/2 - 140, 400, 20, GRAY);
            }
        EndDrawing();
    }

    UnloadModel(gunModel);
    CloseWindow();
    return 0;
}
