#ifndef TILES_HPP 
#define TILES_HPP

#include <iostream>
#include <raylib.h>
#include <vector>
#include <string>

struct tile
{
  int posx = 0;
  int posy = 0;
  int val = 0;
  Color c = WHITE;
};

struct tileset 
{
  Texture2D sheet;
  float twidth;
  float theight;
  int width;
  int height;
  Color asAlpha = MAGENTA;
};

struct tilemap 
{
  int posx = 0;
  int posy = 0;
  float scale = 1;
  bool transparent;
  std::vector<tile> tiles;
  int maxLine = -1;
  int margin = 0;
  int squish = 0;
};

int loadTilesetCR(std::string,tileset&,int,int);
int base64CharToInt(char c);
namespace tiles {
  int draw(tilemap &tm, tileset &ts, int maxX, int maxY);
}

#endif
