#ifndef PHYSICS_HPP
#define PHYSICS_HPP
#include "raylib.h"
#include "raymath.h"
#include <sol/sol.hpp>
#include <vector>
#include <deque>
#include <string>

//-------------------------------
//         How It Works 
//-------------------------------
//-> PhysicWorld :
//  -> A list of constants 
//  -> A vector of Rigidbodies
//
//->  Rigidbody :
//  -> static / dynamic 
//  -> position 
//  -> mass 
//  -> collider (axis align box)

#define SHAPE_CIRCLE 0
#define SHAPE_REC    1

#define SUB_FRAME_COUNT 10
#define MAX_LAYER 65

struct Collider 
{
  int shape = SHAPE_CIRCLE;
  union{
    float r;
    Vector2 rec;
  }param;
};

struct Rigidbody
{
  Vector2 position = {0,0};
  Vector2 velocity = {0,0};
  float mass = 1;
  float friction = 0;
  int layer = 0;
  Collider collider;
  float Cr = 1;
  bool is_static = false;
  bool is_active = true;
  Vector2 force_acc = {0,0};  
  std::string entity_type = "";
};

struct pair 
{
  int a;
  int b;
};

class PhysicWorld 
{
public:
  PhysicWorld();
  PhysicWorld(sol::state &lua);
  
  sol::state* lua;
  Vector2 gravity;
  std::deque<Rigidbody> bodies;
  std::vector<pair> sub_frame_cols;
  bool collision_mask[MAX_LAYER*MAX_LAYER];
  Rigidbody* Subscribe(Rigidbody);
  Rigidbody* NewBall(Vector2 position, float r, float m, float Cr);
  Rigidbody* NewRec(Vector2 position, Vector2 r, float m, float Cr);
  
  void Lua2Cpp();
  void Cpp2Lua();
  void Update(float dt);
  void EulerInt(int idx, float dt);
  void CheckCollision(int a, int b);
  void BorderRepelent(int i);
  void SSCollision(int a, int b);
  void SRCollision(int a, int b);
  void RRCollision(int a, int b);
  void SetMask(int a, int b, bool v);
  bool Raycast(Vector2 o, Vector2 dir,int layer ,Vector2 &closest);
  bool RaycastLua(float x1, float y1, float x2, float y2, int layer);
  bool LineLine(Vector2 a, Vector2 b, Vector2 c, Vector2 d,Vector2 &closest);
};

#endif
