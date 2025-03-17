#include <iostream>
#include <raylib.h>
#include <math.h>
#include "tiles.hpp"
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include "physics.hpp"
#include "lua_init.hpp"

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

  lua.open_libraries(
    sol::lib::base, 
    sol::lib::package,
    sol::lib::string,
    sol::lib::math,
    sol::lib::table
  ); 

  LuaSetup(&lua); 

  lua.script_file("scripts/level.lua");
  lua.script_file("scripts/utils.lua");
  lua.script_file("scripts/entities.lua");
 
  std::string til = lua["Tilemap"].get<std::string>(); //Load Tilemap sprite

  loadTilesetCR(til, ts, 8, 8);

  Rectangle sourceRec = { 0.0f, 0.0f, (float)target.texture.width, -(float)target.texture.height };
  Rectangle destRec = { -virtualRatio, -virtualRatio, W + (virtualRatio*2), H + (virtualRatio*2) };

  Vector2 origin = {0.0f, 0.0f};
  Vector2 cam_offset = {-80,-72-32};

  //SetTargetFPS(60);

  float cameraX = 0;
  float cameraY = 0;
  PhysicWorld PWorld(lua);
  //Load Level
  std::string lvl = lua["GetLevel"]();
  int x = 1;
  int y = 0;
  for(int i = 0; i < lvl.size(); i++)
  {
    if(lvl[i] == '\n')
    { y++; x = 0; }
    else 
    {
      int a = base64CharToInt(lvl[i]);
      if(a >= 0)
      {
        tm.tiles.push_back((tile){x, y, a, WHITE});
        Rigidbody* a = PWorld.NewRec({x*16+8, y*16+8}, {16, 16}, 0.1, 1);
        a->is_static = true;
      }
    }
    x++;
  }
  PWorld.gravity = {0, 40};
  Rigidbody* player = PWorld.NewBall({80, 32}, 8, 5, 0);
  player->friction = 1;
  player->entity_type = "Player";

  PWorld.Cpp2Lua();
  lua["StartEntities"]();
  while(!WindowShouldClose())
  {
    float dt = GetFrameTime();
    worldSpaceCamera.target = player->position + cam_offset; 
    PWorld.Update(dt);
    Color col = BLACK;
    Vector2 temp; 
    //Check keys
    PWorld.Cpp2Lua();
    lua["UpdateEntities"](dt);  
    BeginTextureMode(target);
      ClearBackground(RAYWHITE);

      BeginMode2D(worldSpaceCamera);
        //DrawRectangleLines(player->position.x - 8, player->position.y - 8, 16, 16, RED);
        DrawCircleV(player->position, player->collider.param.r, col); 
        tiles::draw(tm, ts, 
                    player->position.x + vW/2, 
                    player->position.y + vH/2 + 32,
                    player->position.x - vW/2, 
                    player->position.y - vH/2 - 32
                    ); 
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
