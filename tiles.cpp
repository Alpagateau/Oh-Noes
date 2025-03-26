#include <iostream>
#include <raylib.h>
#include "tiles.hpp"
#include <unordered_map>

int base64CharToInt(char c) {
    // Base64 index table (mapping characters to their respective values)
  std::unordered_map<char, int> base64_map = {
    {'A', 0}, {'B', 1}, {'C', 2}, {'D', 3}, {'E', 4}, {'F', 5}, {'G', 6}, {'H', 7},
    {'I', 8}, {'J', 9}, {'K', 10}, {'L', 11}, {'M', 12}, {'N', 13}, {'O', 14}, {'P', 15},
    {'Q', 16}, {'R', 17}, {'S', 18}, {'T', 19}, {'U', 20}, {'V', 21}, {'W', 22}, {'X', 23},
    {'Y', 24}, {'Z', 25},
    {'a', 26}, {'b', 27}, {'c', 28}, {'d', 29}, {'e', 30}, {'f', 31}, {'g', 32}, {'h', 33},
    {'i', 34}, {'j', 35}, {'k', 36}, {'l', 37}, {'m', 38}, {'n', 39}, {'o', 40}, {'p', 41},
    {'q', 42}, {'r', 43}, {'s', 44}, {'t', 45}, {'u', 46}, {'v', 47}, {'w', 48}, {'x', 49},
    {'y', 50}, {'z', 51},
    {'0', 52}, {'1', 53}, {'2', 54}, {'3', 55}, {'4', 56}, {'5', 57}, {'6', 58}, {'7', 59},
    {'8', 60}, {'9', 61},
    {'+', 62}, {'/', 63}
    };
    
    // Look up the character in the map and return its corresponding integer value
  if (base64_map.find(c) != base64_map.end()) {
    return base64_map[c];
  } else {
    // Return -1 if the character is not valid in Base64 encoding
    return -1;
  }
}

bool EqCol(Color a, Color b)
{
  return (a.r == b.r) && (a.g == b.g) && (a.b == b.b) && (a.a == b.a);
}

int loadTilesetCR(std::string path, tileset& out, int c, int r)
{
  if(EqCol(out.asAlpha,BLANK)){
    out.sheet = LoadTexture(path.c_str());
  }else{
    Image img = LoadImage(path.c_str()); 
    ImageColorReplace(&img, out.asAlpha, BLANK);
    out.sheet = LoadTextureFromImage(img); 
    UnloadImage(img);
  }
  GenTextureMipmaps(&out.sheet);
  out.width = out.sheet.width;
  out.height = out.sheet.height;
  out.twidth = (float)out.sheet.width/c;
  out.theight = (float)out.sheet.height/r;
  return 0;
}


int tiles::draw(tilemap &tm, tileset &ts, int maxX, int maxY, int minX, int minY)
{
  for(int i = 0; i < tm.tiles.size(); i++)
  {
    tile t = tm.tiles[i];
    float nposx = (t.posx + tm.posx)*tm.scale;
    float nposy = (t.posy + tm.posy)*tm.scale;
    float s = tm.scale;

    if(nposx >= maxX || nposy >= maxY)
      continue; 
    if((nposx+s) <= minX || (nposy+s) <= minY)
      continue;

    int rat = (int)(ts.width/ts.twidth);
    int tposx = (int)t.val % rat;
    int tposy = (int)(t.val - tposx) / rat;
    if(!tm.transparent)
      DrawRectangle(nposx, nposy, s, s, BLACK);
     
    Rectangle frec = {
      tposx*ts.twidth,
      tposy*ts.theight,
      ts.twidth, 
      ts.theight};
    Rectangle fsize = {
      (float)nposx + tm.margin,
      (float)nposy + tm.margin,
      s - 2*tm.margin - tm.squish,
      s - 2*tm.margin
    };
    DrawTexturePro(
      ts.sheet,
      frec, 
      fsize,
      (Vector2){0, 0}, 
      0.0f,
      t.c
      );
  }
  return 0;
}
