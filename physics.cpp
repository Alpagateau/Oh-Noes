#include "physics.hpp"
#include <cmath>
#include <iostream>


PhysicWorld::PhysicWorld()
{
  bodies.clear();
  gravity = {0, 0};
}

Rigidbody* PhysicWorld::Subscribe(Rigidbody body)
{
  bodies.push_back(body);
  return &bodies.back();
}

void PhysicWorld::Update(float adt)
{
  int step_per_frame = 50;
  float dt = adt/step_per_frame;
  for(int t = 0; t <step_per_frame; t++)
  {
    for(int i = 0; i < bodies.size(); i++)
    {
      if(!bodies[i].is_static){
        //Apply gravity
        bodies[i].force_acc = Vector2Add(bodies[i].force_acc, Vector2Scale(gravity, bodies[i].mass));
        //Integrate
        EulerInt(i, dt);
        //then remove force acc
        bodies[i].force_acc = {0,0};
        //Fake borders rn
        BorderRepelent(i);
      }
    }

    for(int i = 0; i < bodies.size(); i++)
    {
      for(int j = i; j < bodies.size(); j++)
      {
        if(bodies[i].is_static){bodies[i].velocity = {0,0};}
        if(bodies[j].is_static){bodies[j].velocity = {0,0};}
        if(i != j){CheckCollision(i, j);}
      }
    }
  }
}

void PhysicWorld::EulerInt(int idx, float dt)
{
  //double half integrater hehe
  bodies[idx].position = Vector2Add(
    bodies[idx].position,
    Vector2Scale(bodies[idx].velocity,dt*0.5)
  );
  bodies[idx].velocity = Vector2Add(
    bodies[idx].velocity,
    Vector2Scale(bodies[idx].force_acc, (1/bodies[idx].mass)*dt)
  );
  bodies[idx].position = Vector2Add(
    bodies[idx].position,
    Vector2Scale(bodies[idx].velocity, dt*0.5)
  );
}

void PhysicWorld::BorderRepelent(int i)
{
  if(bodies[i].collider.shape == SHAPE_CIRCLE){
    if(bodies[i].position.x > 160-bodies[i].collider.param.r)
    {
      bodies[i].velocity.x *= -bodies[i].Cr;
      bodies[i].position.x = 160-bodies[i].collider.param.r;
    }
    if(bodies[i].position.x < bodies[i].collider.param.r)
    {
      bodies[i].velocity.x *= -bodies[i].Cr;
      bodies[i].position.x = bodies[i].collider.param.r;
    }
    if(bodies[i].position.y > 144-bodies[i].collider.param.r)
    {
      bodies[i].velocity.y *= -bodies[i].Cr;
      bodies[i].position.y = 144-bodies[i].collider.param.r;
    }
    if(bodies[i].position.y < bodies[i].collider.param.r)
    {
      bodies[i].velocity.y *= -bodies[i].Cr;
      bodies[i].position.y = bodies[i].collider.param.r;
    }
  }
  else if(bodies[i].collider.shape == SHAPE_REC){
    if(bodies[i].position.x > 160-(bodies[i].collider.param.rec.x/2))
    {
      bodies[i].velocity.x *= -bodies[i].Cr;
      bodies[i].position.x = 160-(bodies[i].collider.param.rec.x/2);
    }
    if(bodies[i].position.x < (bodies[i].collider.param.rec.x/2))
    {
      bodies[i].velocity.x *= -bodies[i].Cr;
      bodies[i].position.x = (bodies[i].collider.param.rec.x/2);
    }
    if(bodies[i].position.y > 144-(bodies[i].collider.param.rec.y/2))
    {
      bodies[i].velocity.y *= -bodies[i].Cr;
      bodies[i].position.y = 144-(bodies[i].collider.param.rec.y/2);
    }
    if(bodies[i].position.y < (bodies[i].collider.param.rec.y/2))
    {
      bodies[i].velocity.y *= -bodies[i].Cr;
      bodies[i].position.y = (bodies[i].collider.param.rec.y/2);
    }
  }
}

void PhysicWorld::CheckCollision(int a, int b)
{
  if(bodies[a].collider.shape == SHAPE_CIRCLE && bodies[b].collider.shape == SHAPE_CIRCLE)
  {
    SSCollision(a,b);
  }
  else if(bodies[a].collider.shape == SHAPE_REC || bodies[b].collider.shape == SHAPE_REC)
  { 
    if(bodies[a].collider.shape != bodies[b].collider.shape)
    {
      SRCollision(a,b);
    }
    else 
    {
      RRCollision(a,b);
    }
  }
}

void PhysicWorld::SSCollision(int a, int b)
{
  Vector2 off = Vector2Subtract(bodies[a].position, bodies[b].position);  
  
  if(Vector2LengthSqr(off)<(bodies[a].collider.param.r + bodies[b].collider.param.r)*(bodies[a].collider.param.r + bodies[b].collider.param.r))
  {
    //Collision happens here
    //First, we need a new base, Un and Ut for u normal and u tangent 
    Vector2 Un = bodies[b].position - bodies[a].position;
    Un = Vector2Normalize(Un);
    //From Ut, simply rotate 90 degrees
    Vector2 Ut = Vector2Rotate(Un, PI/2);

    //Now, let's project our speeds in the new base
    float ua = Vector2DotProduct(Un, bodies[a].velocity);
    float ub = Vector2DotProduct(Un, bodies[b].velocity);
    float ea = Vector2DotProduct(Ut, bodies[a].velocity);
    float eb = Vector2DotProduct(Ut, bodies[b].velocity);
    std::cout << ua << "||" << ub <<std::endl;

    //Finally, calculate the constants
    float M = bodies[a].mass + bodies[b].mass;
    float ms = bodies[a].mass * ua + bodies[b].mass * ub;
    float Delta = ub - ua;
    float Cr = (bodies[a].Cr + bodies[b].Cr)/2;

    //we can now calculate the new speeds 
    float va = bodies[b].mass * Cr * Delta / M;
    va += ms/M;
    float vb = bodies[a].mass * Cr * Delta / M;
    vb += ms/M;

    //Now put the new vectors back. 
    bodies[a].velocity = Un*va + Ut*ea;
    bodies[b].velocity = Un*vb + Ut*eb;
    
    //Now, move the two circles based on weight
    float Dr = (bodies[a].collider.param.r + bodies[b].collider.param.r)-Vector2Length(off);
    if(!bodies[a].is_static)
      bodies[a].position -= Un * (bodies[b].mass / M);
    if(!bodies[b].is_static)
      bodies[b].position += Un * (bodies[a].mass / M); 
  }
}

void PhysicWorld::SRCollision(int a, int b)
{
  if(bodies[a].collider.shape == SHAPE_REC)
  {
    int c = a;
    a = b;
    b = c;
  }
    // Step 1: Calculate the closest point on the rectangle to the circle
  Vector2 closestPoint;
  closestPoint.x = std::max(bodies[b].position.x - bodies[b].collider.param.rec.x / 2, 
                  std::min(bodies[a].position.x, bodies[b].position.x + bodies[b].collider.param.rec.x / 2));
  closestPoint.y = std::max(bodies[b].position.y - bodies[b].collider.param.rec.y / 2, 
                  std::min(bodies[a].position.y, bodies[b].position.y + bodies[b].collider.param.rec.y / 2));

  // Step 2: Calculate the distance from the circle center to the closest point on the rectangle
  Vector2 distVec = bodies[a].position - closestPoint;
  float distance = Vector2Length(distVec);

  // Step 3: Check if the distance is less than or equal to the circle's radius
  if (distance <= bodies[a].collider.param.r) {
    // Step 4: Calculate the normal vector and the relative velocity between the two bodies
    Vector2 normal = Vector2Normalize(distVec);
    float relativeVelocity = Vector2DotProduct((bodies[a].velocity - bodies[b].velocity),normal);

    // Step 5: If they are moving towards each other, resolve the collision
    if (relativeVelocity < 0) {
      // Step 6: Apply the coefficient of restitution to calculate new velocities
      float impulseMagnitude = (-(1 + bodies[a].Cr) * relativeVelocity) /
                              (1 / bodies[a].mass + 1 / bodies[b].mass);

      // Step 7: Update the velocities of both bodies
      Vector2 impulse = normal * impulseMagnitude;
      bodies[a].velocity = bodies[a].velocity + (impulse * (1 / bodies[a].mass));
      bodies[b].velocity = bodies[b].velocity - (impulse * (1 / bodies[b].mass)); 
    }
    if(distance < bodies[a].collider.param.r)
    {
      Vector2 ndist = Vector2Normalize(distVec);
      Vector2 move_vec = ndist * (bodies[a].collider.param.r - distance);
      if(!bodies[a].is_static)
        bodies[a].position += move_vec * (bodies[b].mass / (bodies[b].mass + bodies[a].mass) );
      if(!bodies[b].is_static)
        bodies[b].position -= move_vec * (bodies[a].mass / (bodies[b].mass + bodies[a].mass) );
    }
  } 
}

void PhysicWorld::RRCollision(int a, int b)
{
  if(bodies[a].collider.shape != SHAPE_REC || bodies[a].collider.shape != SHAPE_REC)
  {
    std::cout << "[C++] Physics : R-R Collision with non REC shapes" << std::endl;
    return;
  }
  
  Rectangle rec1 = {
    bodies[a].position.x - bodies[a].collider.param.rec.x/2,
    bodies[a].position.y - bodies[a].collider.param.rec.y/2,
    bodies[a].collider.param.rec.x,
    bodies[a].collider.param.rec.y
  };

  Rectangle rec2 = {
    bodies[b].position.x - bodies[b].collider.param.rec.x/2,
    bodies[b].position.y - bodies[b].collider.param.rec.y/2,
    bodies[b].collider.param.rec.x,
    bodies[b].collider.param.rec.y
  };

  if(!CheckCollisionRecs(rec1, rec2))
  {
    return;
  }

  Vector2 off = bodies[b].position - bodies[a].position;
  Vector2 normal;
  Vector2 tangent;
  Rectangle overlap = GetCollisionRec(rec2 ,rec1);
  if(overlap.width < overlap.height)
  {
    normal = {1, 0};
    tangent = {0, 1};
  }
  else
  {
    normal = {0, 1};
    tangent = {1, 0};
  }
  
  float u_a = Vector2DotProduct(normal, bodies[a].velocity);
  float u_b = Vector2DotProduct(normal, bodies[b].velocity);
  float t_a = Vector2DotProduct(tangent, bodies[a].velocity);
  float t_b = Vector2DotProduct(tangent, bodies[b].velocity);

  float Delta = u_b - u_a;
  float Cr = (bodies[a].Cr + bodies[b].Cr)/2;
  float M = bodies[a].mass + bodies[b].mass;
  float ms = bodies[a].mass*u_a + bodies[b].mass*u_b;
  float va = Cr * Delta * bodies[b].mass / M;
  float vb = -Cr * Delta * bodies[a].mass / M;
  float S = ms/M;
  va += S;
  vb += S;
  
  bodies[a].velocity = normal * va + tangent * t_a;
  bodies[b].velocity = normal * vb + tangent * t_b;

  //Now move the two Bodies 
  //std::cout << "[C++] normal : {" << normal.x << ";" << normal.y << "}" << std::endl; 
  Vector2 move;
  if(normal == (Vector2){1, 0})
  {
    
    if(bodies[a].position.x < bodies[b].position.x)
    {
      move = normal * -overlap.width;
    } 
    else 
    {
      move = normal * overlap.width; 
    }
  }
  else 
  {
    if(bodies[a].position.y < bodies[b].position.y)
    {
      move = normal * -overlap.height; 
    } 
    else 
    {
      move = normal * overlap.height;
    }
  }
  if(!bodies[a].is_static)
    bodies[a].position += move * (bodies[b].mass / M);
  if(!bodies[b].is_static)
    bodies[b].position -= move * (bodies[a].mass / M);
}

Rigidbody* PhysicWorld::NewBall(Vector2 pos, float r, float m, float Cr)
{
  Rigidbody rb = {
    pos,
    {0, 0},
    m,
    {
      SHAPE_CIRCLE,
      r
    },
    Cr,
    false,
    {0, 0}
  };
  return Subscribe(rb);
}

Rigidbody* PhysicWorld::NewRec(Vector2 pos, Vector2 re, float m, float Cr)
{
  Collider c;
  c.shape = SHAPE_REC;
  c.param.rec = re;
  Rigidbody rb = {
    pos,
    {0, 0},
    m,
    c,
    Cr,
    false,
    {0, 0}
  };
  return Subscribe(rb);
}
