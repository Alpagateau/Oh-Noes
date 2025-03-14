#include <iostream>
#include <raylib.h>
#include <math.h>
#include "tiles.hpp"
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include "physics.hpp"

#define TILE_SIZE 16

int main()
{
  //Real Screen Size
  const int W = 1000;
  const int H = 900;
  
  const int vW = 160;
  const int vH = 144; 

  const float virtualRatio = (float)W/(float)vW;

  Camera2D worldSpaceCamera = { 0 };  // Game world camera
  worldSpaceCamera.zoom = 1.0f;

  tileset ts;
  tilemap tm;
  tm.scale = TILE_SIZE;
  tm.transparent = true;

  InitWindow(W, H, "Oh Noes : The Game");

  RenderTexture2D target = LoadRenderTexture(vW, vH);

  sol::state lua;

  lua.script_file("scripts/level.lua");
 
  std::string til = lua["Tilemap"].get<std::string>(); //Load Tilemap sprite

  loadTilesetCR(til, ts, 8, 8);

  Rectangle sourceRec = { 0.0f, 0.0f, (float)target.texture.width, -(float)target.texture.height };
  Rectangle destRec = { -virtualRatio, -virtualRatio, W + (virtualRatio*2), H + (virtualRatio*2) };

  Vector2 origin = {0.0f, 0.0f};
  Vector2 cam_offset = {-80, -72};
  SetTargetFPS(60);

  float cameraX = 0;
  float cameraY = 0;
  PhysicWorld PWorld;
  //Load Level
  std::string lvl = lua["GetLevel"]();
  int x = 0;
  int y = 0;
  for(int i = 0; i < lvl.size(); i++)
  {
    if(lvl[i] == '\n')
    {
      y++;
      x = 0;
    }
    else 
    {
      int a = base64CharToInt(lvl[i]);
      if(a >= 0)
      {
        tm.tiles.push_back((tile){x, y, a, WHITE});
      }
    }
    x++;
  }
  PWorld.gravity = {0, 11}; 
  //Rigidbody* block = PWorld.Subscribe((Rigidbody){{15,15}, {3,0}, 1, 8, 0.5, false, {0,0}});
  //Rigidbody* block2 = PWorld.Subscribe((Rigidbody){{15,45}, {-10,-3}, 4, 10, 0.9, false, {0,0}});
  Rigidbody* block = PWorld.NewBall({15, 15}, 8, 1, 1);
  block->velocity = {4,0};
  Rigidbody* block2 = PWorld.NewBall({12, 45}, 10, 4, 0.9);
  block2->velocity = {-10,-3};
  block2->is_static = true;
  Rigidbody* block3 = PWorld.NewRec({50, 12}, {16, 32}, 3, 1);
  block3->velocity = {10, 2};
  Rigidbody* block4 = PWorld.NewRec({100, 100}, {64, 5}, 2000, 1);
  block4->velocity = {10, 2};
  block4->is_static = true;

  while(!WindowShouldClose())
  {
    float dt = GetFrameTime();
    worldSpaceCamera.target = (Vector2){ cameraX, cameraY };
     
    PWorld.Update(dt);
    BeginTextureMode(target);
      ClearBackground(RAYWHITE);

      BeginMode2D(worldSpaceCamera);
        // Draw The Actual World 
        DrawCircleV(block->position,block->collider.param.r,RED);
        DrawCircleV(block2->position,block2->collider.param.r,BLUE);
        DrawRectangle(
          block3->position.x - block3->collider.param.rec.x/2,
          block3->position.y - block3->collider.param.rec.y/2,
          block3->collider.param.rec.x,
          block3->collider.param.rec.y,
          GREEN
        );
        DrawRectangle(
          block4->position.x - block4->collider.param.rec.x/2,
          block4->position.y - block4->collider.param.rec.y/2,
          block4->collider.param.rec.x,
          block4->collider.param.rec.y,
          PURPLE
        );
        //tiles::draw(tm, ts, vW, vH);
      EndMode2D();
    EndTextureMode();

    //UPSCALES THE LOW RES IMAGE
    BeginDrawing();
      ClearBackground(RED);
 
      DrawTexturePro(target.texture, sourceRec, destRec, origin, 0.0f, WHITE);
      DrawFPS(GetScreenWidth() - 95, 10);
    EndDrawing();
  }

  CloseWindow();
}
