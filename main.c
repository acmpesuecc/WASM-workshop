#include "raylib/include/raylib.h"
#include <emscripten/emscripten.h>

#define MAX_PARTICLES 200

// Particle structure with basic data
typedef struct {
  Vector2 position;
  Color color;
  float alpha;
  float size;
  float rotation;
  bool active; // NOTE: Use it to activate/deactive particle
} Particle;

Particle mouseTail[MAX_PARTICLES] = {0};
int blending = BLEND_ALPHA;
float gravity = 3.0f;
Texture2D smoke;

const int screenWidth = 800;
const int screenHeight = 450;

void draw(void) {
  // Activate one particle every frame and Update active particles
  for (int i = 0; i < MAX_PARTICLES; i++) {
    if (!mouseTail[i].active) {
      mouseTail[i].active = true;
      mouseTail[i].alpha = 1.0f;
      mouseTail[i].position = GetMousePosition();
      i = MAX_PARTICLES;
    }
  }

  for (int i = 0; i < MAX_PARTICLES; i++) {
    if (mouseTail[i].active) {
      mouseTail[i].position.y += gravity / 2;
      mouseTail[i].alpha -= 0.005f;

      if (mouseTail[i].alpha <= 0.0f)
        mouseTail[i].active = false;

      mouseTail[i].rotation += 2.0f;
    }
  }

  if (IsKeyPressed(KEY_SPACE)) {
    if (blending == BLEND_ALPHA)
      blending = BLEND_ADDITIVE;
    else
      blending = BLEND_ALPHA;
  }

  // Draw
  BeginDrawing();

  ClearBackground(DARKGRAY);

  BeginBlendMode(blending);

  // Draw active particles
  for (int i = 0; i < MAX_PARTICLES; i++) {
    if (mouseTail[i].active)
      DrawTexturePro(
          smoke,
          (Rectangle){0.0f, 0.0f, (float)smoke.width, (float)smoke.height},
          (Rectangle){mouseTail[i].position.x, mouseTail[i].position.y,
                      smoke.width * mouseTail[i].size,
                      smoke.height * mouseTail[i].size},
          (Vector2){(float)(smoke.width * mouseTail[i].size / 2.0f),
                    (float)(smoke.height * mouseTail[i].size / 2.0f)},
          mouseTail[i].rotation, Fade(mouseTail[i].color, mouseTail[i].alpha));
  }

  EndBlendMode();

  DrawText("PRESS SPACE to CHANGE BLENDING MODE", 180, 20, 20, BLACK);

  if (blending == BLEND_ALPHA)
    DrawText("ALPHA BLENDING", 290, screenHeight - 40, 20, BLACK);
  else
    DrawText("ADDITIVE BLENDING", 280, screenHeight - 40, 20, RAYWHITE);

  EndDrawing();
}

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
  // Initialization
  //--------------------------------------------------------------------------------------

  InitWindow(screenWidth, screenHeight,
             "raylib [textures] example - particles blending");

  for (int i = 0; i < MAX_PARTICLES; i++) {
    mouseTail[i].position = (Vector2){0, 0};
    mouseTail[i].color = (Color){GetRandomValue(0, 255), GetRandomValue(0, 255),
                                 GetRandomValue(0, 255), 255};
    mouseTail[i].alpha = 1.0f;
    mouseTail[i].size = (float)GetRandomValue(1, 30) / 20.0f;
    mouseTail[i].rotation = (float)GetRandomValue(0, 360);
    mouseTail[i].active = false;
  }

  smoke = LoadTexture("./idk.png");

  SetTargetFPS(60);

  // Main game loop
  emscripten_set_main_loop(draw, 0, 1);

  // De-Initialization
  UnloadTexture(smoke);

  CloseWindow(); // Close window and OpenGL context
  return 0;
}
