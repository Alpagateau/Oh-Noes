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


struct Rigidbody
{
  Vector2 position = {0,0};
  Vector2 velocity = {0,0};
  float mass = 1;
  float radius = 1;
  float Cr = 1;
  bool isStatic = false;
  Vector2 force_acc = {0,0};  
};

class PhysicWorld 
{
public:
  PhysicWorld();
  Vector2 gravity;
  std::deque<Rigidbody> bodies;
  
  Rigidbody* Subscribe(Rigidbody);
  void Update(float dt);
  void EulerInt(int idx, float dt);
  void CheckCollision(int a, int b);
  void BorderRepelent(int i);
};

#endif
