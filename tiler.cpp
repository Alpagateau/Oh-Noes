#include "iostream"
#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <string>
#include <vector>
#include "tiles.hpp"
#include "tiler.hpp"
#define TILE_SIZE 16

int main()
{
  InitWindow(1024, 640, "Tiler Editor");

  tileset ts;
  tilemap tm;
  tilemap select_map;
  tm.scale = TILE_SIZE;
  tm.transparent = true;
  std::vector<Param> data; 
  std::string til = "./res/tilemap0.1.png";
  loadTilesetCR(til, ts, 8, 8);
  select_map.posy = 18;
  select_map.scale = 32;

  for(int i = 0; i < 64; i++)
  {
    int x = i;
    int y = 0;
    if(x >= 32)
    {
      x = i-32;
      y = 1;
    }
    select_map.tiles.push_back((tile){x, y, i, WHITE});
  }
  
  int selectedTile = 0;
  int px = 0;
  int py = 0;
  
  bool paint = true;
  bool erase = false;
  while(!WindowShouldClose())
  {
    //Select tile by hand
    if(IsMouseButtonPressed(0))
    {
      int mx = GetMouseX();
      int my = GetMouseY();
      if(my >= 576)
      {
        my -= 576;
        mx = (int)(mx/32);
        my = (int)(my/32);
        selectedTile = mx + my*32;
        std::cout << "Selected tile #" << selectedTile << std::endl;
      }
      else if(my > 50) 
      {
        if(erase)
        {
          int idx = -1;
          for(int i = 0; i < tm.tiles.size(); i++)
          {
            if(idx == -1)
            {
              if(tm.tiles[i].posx == px && tm.tiles[i].posy == py)
              {
                idx = i;
              }
            }
          }
          if(idx != -1)
            tm.tiles.erase(tm.tiles.begin()+idx);
        }
        else
        {
          if(paint)
          {
            tm.tiles.push_back((tile){px, py, selectedTile, WHITE});
            data.push_back( (Param){px, py, true, true});
          }
          else 
          {
            int idx = -1;
            for(int i = 0; i < data.size(); i++)
            {
              if(data[i].x == px && data[i].y == py)
              {
                data[i].is_active = !data[i].is_active;
              }
            }
          }
        }
      }
    }

    int mx = GetMouseX();
    int my = GetMouseY();
    if(my < 576)
    {
      px = (int)(mx/16);
      py = (int)(my/16);
    }

    BeginDrawing();
      ClearBackground(WHITE);
      tiles::draw(select_map, ts, 1024, 664, 0, 0); 
      tiles::draw(tm, ts, 1024, 576, 0, 0);

      if(!paint)
      {
        for(int i = 0; i < data.size(); i++)
        {
          if(data[i].is_active)
            DrawRectangle(data[i].x*16, data[i].y*16, 16, 16, (Color){0,255,0,127});
        }
      }

      int sx = selectedTile;
      int sy = 0;
      if(sx >= 32)
      {
        sx -= 32;
        sy = 1;
      }
      DrawRectangleLines(sx*32, 576+32*sy, 32, 32, RED);
      DrawRectangleLines(px*16, 16*py, 16, 16, paint?BLUE:GREEN);
      //GUI
      if(GuiButton( (Rectangle){5, 5, 100, 40}, "Load Map" ) )
      {
        std::cout << "Load Map" << std::endl; 
      }      
      if(GuiButton( (Rectangle){110, 5, 100, 40}, "Save Map" ) )
      {
        std::cout << "Save Map" << std::endl;  
      }
      if(GuiButton( (Rectangle){215, 5, 100, 40}, (paint?"Edit":"Paint") ) )
      {
        paint = !paint;
        std::cout << "paint : " << paint << std::endl;  
      }
      if(GuiButton( (Rectangle){320, 5, 100, 40}, (erase?"Dont Erase":"Erase") ) )
      {
        erase = !erase;
        std::cout << "paint : " << paint << std::endl;  
      }

    EndDrawing();
  }

  CloseWindow();
}


std::string serParam(Param p)
{
  return "{pos={" 
    + std::to_string(p.x) + "," + std::to_string(p.y) + 
    "}, is_active="+std::to_string(p.is_active)+
    "is_static=" + std::to_string(p.is_static) + "}";
}
