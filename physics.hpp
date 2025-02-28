#ifndef PHYSICS_HPP
#define PHYSICS_HPP
#include "raylib.h"
#include "tiles.hpp"
#include "raymath.h"

//-------------------------------
//         How It Works 
//-------------------------------
//-> Every physics object added to a pool 
//

class Rigidbody 
{
public:
  Vector2 position;
  Vector2 velocity;
  Rectangle collision;
  float mass;
  bool collision_mask[64];
  Vector2 force_acc;

  Rigidbody();
  void AddForce(Vector2 force);
  void Update(float dt, tilemap& tm);
  bool CheckCollisionStatic(Rectangle other);
  bool CheckCollisionTilemap(tilemap& tm);
};

#endif
