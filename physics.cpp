#include "physics.hpp"
#include <iostream>
//pff
//

#define PI 3.14159265358979323846

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
      //Apply gravity
      bodies[i].force_acc = Vector2Add(bodies[i].force_acc, Vector2Scale(gravity, bodies[i].mass));
      //Integrate
      EulerInt(i, dt);
      //then remove force acc
      bodies[i].force_acc = {0,0};
      //Fake borders rn
      BorderRepelent(i);
    }

    for(int i = 0; i < bodies.size(); i++)
    {
      for(int j = i; j < bodies.size(); j++)
      {
        if(i != j)
        {
          CheckCollision(i, j);
        }
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
  if(bodies[i].position.x > 160-bodies[i].radius)
  {
    bodies[i].velocity.x *= -bodies[i].Cr;
    bodies[i].position.x = 160-bodies[i].radius;
  }
  if(bodies[i].position.x < bodies[i].radius)
  {
    bodies[i].velocity.x *= -bodies[i].Cr;
    bodies[i].position.x = bodies[i].radius;
  }
  if(bodies[i].position.y > 144-bodies[i].radius)
  {
    bodies[i].velocity.y *= -bodies[i].Cr;
    bodies[i].position.y = 144-bodies[i].radius;
  }
  if(bodies[i].position.y < bodies[i].radius)
  {
    bodies[i].velocity.y *= -bodies[i].Cr;
    bodies[i].position.y = bodies[i].radius;
  }
}

void PhysicWorld::CheckCollision(int a, int b)
{
  Vector2 off = Vector2Subtract(bodies[a].position, bodies[b].position);  
  if(Vector2LengthSqr(off) < (bodies[a].radius + bodies[b].radius)*(bodies[a].radius + bodies[b].radius))
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
    float Dr = (bodies[a].radius + bodies[b].radius)-Vector2Length(off);
    bodies[a].position -= Un * (bodies[b].mass / M);
    bodies[b].position += Un * (bodies[a].mass / M); 
  }
}
