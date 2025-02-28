#include "physics.hpp"
//pff
Rigidbody::Rigidbody()
{
  position = (Vector2){0,0};
  velocity = (Vector2){0,0};
  mass = 1;
  force_acc = (Vector2){0,0};
  for(int i = 0; i < 64; i++)
  {
    collision_mask[i] = true;
  }
}

void Rigidbody::AddForce(Vector2 force)
{
  force_acc = Vector2Add(force, force_acc);
}

bool Rigidbody::CheckCollisionStatic(Rectangle other)
{
  Rectangle world_space = (Rectangle)
  {
      position.x + collision.x,
      position.y + collision.y,
      collision.width,
      collision.height
  };
  return CheckCollisionRecs(world_space, other);
}

bool Rigidbody::CheckCollisionTilemap(tilemap& tm)
{
  bool isGood = true;
  Rectangle world_space = (Rectangle)
  {
      position.x + collision.x,
      position.y + collision.y,
      collision.width,
      collision.height
  };
  for(int i = 0; i < tm.tiles.size(); i++)
  {
    if(isGood)
    {
      if(collision_mask[tm.tiles[i].val])
      {
        Rectangle r = (Rectangle)
        {
          tm.tiles[i].posx * tm.scale,
          tm.tiles[i].posy * tm.scale,
          tm.scale,
          tm.scale
        };
        if(CheckCollisionRecs(world_space, r))
        {
          isGood = false;
        }
      }
    }
  }
  return !isGood;
}

void Rigidbody::Update(float dt, tilemap& tm)
{
  force_acc = Vector2Scale(force_acc, dt/mass);
  velocity = Vector2Add(velocity, force_acc);
  force_acc = (Vector2){0, 0};
  Vector2 scaled_speed = Vector2Scale(velocity, dt);
  position.x += scaled_speed.x;
  if(CheckCollisionTilemap(tm))
  {
    position.x -= scaled_speed.x;
    velocity.x = 0;
  }
  position.y += scaled_speed.y;
  if(CheckCollisionTilemap(tm))
  {
    position.y -= scaled_speed.y;
    velocity.y = 0;
  }
}

