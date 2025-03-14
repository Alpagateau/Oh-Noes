#ifndef PHYSICS_HPP
#define PHYSICS_HPP
#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <deque>

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
  Collider collider;
  float Cr = 1;
  bool is_static = false;
  Vector2 force_acc = {0,0};  
};

class PhysicWorld 
{
public:
  PhysicWorld();
  Vector2 gravity;
  std::deque<Rigidbody> bodies;
  
  Rigidbody* Subscribe(Rigidbody);
  Rigidbody* NewBall(Vector2 position, float r, float m, float Cr);
  Rigidbody* NewRec(Vector2 position, Vector2 r, float m, float Cr);

  void Update(float dt);
  void EulerInt(int idx, float dt);
  void CheckCollision(int a, int b);
  void BorderRepelent(int i);
  void SSCollision(int a, int b);
  void SRCollision(int a, int b);
  void RRCollision(int a, int b);

};

#endif
