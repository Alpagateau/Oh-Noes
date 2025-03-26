#include <iostream>
#include <raylib.h>
#include <math.h>
#include "tiles.hpp"
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include "physics.hpp"
#include "lua_init.hpp"

#define TILE_SIZE 16

bool debug;
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
        Rigidbody* ab = PWorld.NewRec({x*16+8, y*16+8}, {16, 16}, 0.1, 1);
        ab->is_static = true;
      }
    }
    x++;
  }
  PWorld.gravity = {0, 80};
  Rigidbody* player = PWorld.NewBall({80, 32}, 8, 5, 0);
  player->friction = 1;
  player->entity_type = "Player";

  Texture2D prota_tex = LoadTexture("./res/Prota.png");
  Vector2 prota_offset = {-8, -24};
  //PWorld.Compile();
  PWorld.Cpp2Lua();
  lua["StartEntities"]();
  double start_frame, full_frame, physics_frame, lua_frame;
  float lua_frac, physics_frac;
  while(!WindowShouldClose())
  {
    if(IsKeyPressed(KEY_F3))
    {
      debug = !debug;
      std::cout << "Debug mode : " << debug << std::endl;
    }
    if(debug)
      start_frame = GetTime();
     

    float dt = GetFrameTime();
    worldSpaceCamera.target = player->position + cam_offset; 
    
    lua["UpdateEntities"](dt);
    if(debug)
      lua_frame = GetTime();
    PWorld.Update(dt);
    if(debug)
      physics_frame = GetTime();

    Color col = BLACK;
    Vector2 temp; 
    //Check keys
    PWorld.Cpp2Lua();
      
    BeginTextureMode(target);
      ClearBackground(RAYWHITE);

      BeginMode2D(worldSpaceCamera);
        //DrawRectangleLines(player->position.x - 8, player->position.y - 8, 16, 16, RED);
        //DrawCircleV(player->position, player->collider.param.r, col); 
        DrawTextureV(prota_tex, player->position+prota_offset , WHITE);
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
      if(debug){
        DrawRectangle(0, 0, 50, H, BLACK);
        int lua_he = (int)(lua_frac * H);
        int physics_he = (int)(physics_frac * H);
        DrawRectangle(0, 0, 50, lua_he, BLUE);
        DrawRectangle(0, lua_he, 50, physics_he, RED);
      }
    EndDrawing();
    if(debug)
      full_frame = GetTime();
    if(debug)
    {
      double tot = (full_frame - start_frame);
      lua_frac = (float)( (lua_frame - start_frame) / tot);
      physics_frac = (float)( (physics_frame - lua_frame) / tot);
//      std::cout << (int)(lua_frac*100) << "% LUA | " << (int)(physics_frac*100)<< "% Phys\n";
    }
  }

  CloseWindow();
}
