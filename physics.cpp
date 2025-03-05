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

Rectangle Rigidbody::GetWorldCollision()
{
  Rectangle world_space_a = (Rectangle)
  {
      position.x + collision.x,
      position.y + collision.y,
      collision.width,
      collision.height
  };

  return world_space_a;
}

PhysicWorld::PhysicWorld()
{
  bodies.clear();
  gravity = {0, 0};
}

bool PhysicWorld::CheckCollision(Rigidbody* a, Rigidbody* b)
{
  Rectangle world_space_a = (Rectangle)
  {
      a->position.x + a->collision.x,
      a->bposition.y + a->collision.y,
      a->collision.width,
      a->collision.height
  };

  Rectangle world_space_b = (Rectangle)
  {
      b->position.x + b->collision.x,
      b->position.y + b->collision.y,
      b->collision.width,
      b->collision.height
  };

  return CheckCollisionRecs(world_space_a, world_space_b);
}
/*
bool PhysicWorld::CheckCollisionTilemap(Rigidbody a, tilemap& tm)
{
  bool isGood = true;
  Rectangle world_space = (Rectangle)
  {
      a->osition.x + a->collision.x,
      a->position.y + a->collision.y,
      a->collision.width,
      a->collision.height
  };
  for(int i = 0; i < tm.tiles.size(); i++)
  {
    if(isGood)
    {
      if(a->collision_mask[tm.tiles[i].val])
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
*/

void PhysicsWorldUpdate(float dt)
{
  //First Apply Collisions
  for(int i = 0; i < bodies.size(); i++)
  { 
    for(int j = i; j < bodies.size(); j++)
    {
      if(i!=j)
      {
        if(bodies[i]->static && bodies[j]->static)
          continue;
        if(CheckCollision(bodies[i], bodies[j]))
        {
          Rectangle col_rec = GetCollisionRec(bodies[i]->GetWorldCollision(), bodies[j]->GetWorldCollision());
          float ratio = (bodies[i]->mass)/(bodies[i]->mass + bodies[j]->mass);
          
          //multipliers depending on the quadrant of the collision;
          int x_mul = 1;
          int y_mul = 1;
          //Check quadrant
          
          Vector2 delta = 

          //Apply collision
          if(bodies[i]->static)
          {
            ratio =1;
          }
          if(bodies[j]->static)
          {
            ratio = 0;
          } 
          
          bodies[i]->position.x -= (1-ratio)*x_mul*col_rec.width;
          bodies[i]->position.y -= (1-ratio)*y_mul*col_rec.height;
          
          bodies[j]->position.x += ratio*x_mul*col_rec.width;
          bodies[j]->position.y += ratio*y_mul*col_rec.height;
        }
      }
    }
  }
}



