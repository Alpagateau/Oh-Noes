#ifndef PHYSICS_HPP
#define PHYSICS_HPP
#include "raylib.h"
#include "tiles.hpp"
#include "raymath.h"
#include <vector>

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

class Rigidbody 
{
public:
  Vector2 position;
  Vector2 velocity;
  Rectangle collision;
  float mass;
  bool collision_mask[64];
  bool static = false;
  Vector2 force_acc;
  int collision_layer;

  Rigidbody();
  void AddForce(Vector2 force);
  //void Update(float dt, tilemap& tm);
  Rectangle GetWorldCollision();
};

class PhysicWorld 
{
public:
  Vector2 gravity;
  std::vector<Rigidbody*> bodies;

  void Update(float dt);
  bool CheckCollision(Rigidbody* a, Rigidbody* b);
  //bool CheckCollisionTilemap(Rigidbody a, tilemap& tm);
};

#endif
