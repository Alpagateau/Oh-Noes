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

  Rigidbody block;
  block.position.x = 50;
  block.position.y = 10;
  block.velocity.x = 6;
  block.velocity.y = -7;
  block.collision = (Rectangle){-8, -8, 16, 16};

  while(!WindowShouldClose())
  {
    float dt = GetFrameTime();
    worldSpaceCamera.target = (Vector2){ cameraX, cameraY };
    
    //block.AddForce((Vector2){0, 9});
    //block.Update(dt, tm);

    BeginTextureMode(target);
      ClearBackground(RAYWHITE);

      BeginMode2D(worldSpaceCamera);
        // Draw The Actual World 
        //DrawCircle(block.position.x,block.position.y,8,RED);
        tiles::draw(tm, ts, vW, vH);   
      EndMode2D();
    EndTextureMode();

    //UPSCALES THE LOW RES IMAGE
    BeginDrawing();
      ClearBackground(RED);
 
      DrawTexturePro(target.texture, sourceRec, destRec, origin, 0.0f, WHITE);
      DrawFPS(GetScreenWidth() - 95, 10);
    EndDrawing();
  }
}
