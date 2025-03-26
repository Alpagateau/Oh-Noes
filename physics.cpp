#include "physics.hpp"
#include <cmath>
#include <iostream>


PhysicWorld::PhysicWorld()
{
  std::cout << "[C++] Nopes, doesn't work" << std::endl;
}

PhysicWorld::PhysicWorld(sol::state &p_lua)
{
  bodies.clear();
  lua = &p_lua;
  gravity = {0, 0};
  for(int i = 0; i < MAX_LAYER * MAX_LAYER; i++)
  {
    collision_mask[i] = true;  
  }

  (*lua)["Physics"] = (*lua).create_table_with(
    "gravity", (*lua).create_table_with(
      "x", 0.0,
      "y", 0.0
    ),
    "bodies", (*lua).create_table_with()
  );
}

void PhysicWorld::Compile()
{
  float minx =  100000;
  float maxx = -100000;
  float miny =  100000;
  float maxy = -100000;
  
  for(int i = 0; i < bodies.size(); i++)
  {
    if(bodies[i].is_static && bodies[i].collider.shape == SHAPE_REC){
      if(bodies[i].position.x/16 < minx)
        minx = bodies[i].position.x/16 - 0.5;
      if(bodies[i].position.x/16 > maxx)
        maxx = bodies[i].position.x/16 - 0.5;
      if(bodies[i].position.y/16 < miny)
        miny = bodies[i].position.y/16 - 0.5;
      if(bodies[i].position.x/16 > maxy)
        maxy = bodies[i].position.y/16 - 0.5;
    }
  }
  int w = maxx - minx;
  int h = maxy - miny;
  pair cworld[w*h] = {0};
 
  for(int i = 0; i < bodies.size(); i++)
  {
    if(bodies[i].is_static && bodies[i].collider.shape == SHAPE_REC){
      int x = bodies[i].position.x/16 - minx;
      int y = bodies[i].position.y/16 - miny;
      cworld[x + w*y] = {i, bodies[i].is_active?1:0};
    }
  }

  std::cout << minx << "|" << miny << "|" << maxx << "|" << maxy << std::endl;
}

void PhysicWorld::Cpp2Lua()
{
  //Gravity 
  if(lua == nullptr)
    return;
  (*lua)["Physics"]["gravity"]["x"] = gravity.x;
  (*lua)["Physics"]["gravity"]["y"] = gravity.y;

  for(int i = 0; i < bodies.size(); i++)
  {
    (*lua)["Physics"]["bodies"][i+1]["pos"]["x"] = bodies[i].position.x;
    (*lua)["Physics"]["bodies"][i+1]["pos"]["y"] = bodies[i].position.y;

    (*lua)["Physics"]["bodies"][i+1]["vel"]["x"] = bodies[i].velocity.x;
    (*lua)["Physics"]["bodies"][i+1]["vel"]["y"] = bodies[i].velocity.y;

    (*lua)["Physics"]["bodies"][i+1]["mass"] = bodies[i].mass;
    (*lua)["Physics"]["bodies"][i+1]["Cr"] = bodies[i].Cr;
    (*lua)["Physics"]["bodies"][i+1]["static"] = bodies[i].is_static;

    (*lua)["Physics"]["bodies"][i+1]["f_acc"]["x"] = bodies[i].force_acc.x;
    (*lua)["Physics"]["bodies"][i+1]["f_acc"]["y"] = bodies[i].force_acc.y;

    (*lua)["Physics"]["bodies"][i+1]["entity"] = bodies[i].entity_type;
  } 
}

void PhysicWorld::Lua2Cpp()
{
  if(lua == nullptr)
    return;
  //Gravity 
  gravity.x = (*lua)["Physics"]["gravity"]["x"];
  gravity.y = (*lua)["Physics"]["gravity"]["y"];

  for(int i = 0; i < bodies.size(); i++)
  {
    bodies[i].position.x = (*lua)["Physics"]["bodies"][i+1]["pos"]["x"];
    bodies[i].position.y = (*lua)["Physics"]["bodies"][i+1]["pos"]["y"];

    bodies[i].velocity.x = (*lua)["Physics"]["bodies"][i+1]["vel"]["x"];
    bodies[i].velocity.y = (*lua)["Physics"]["bodies"][i+1]["vel"]["y"];

    bodies[i].mass = (*lua)["Physics"]["bodies"][i+1]["mass"];
    bodies[i].Cr = (*lua)["Physics"]["bodies"][i+1]["Cr"];
    bodies[i].is_static = (*lua)["Physics"]["bodies"][i+1]["static"];

    bodies[i].force_acc.x = (*lua)["Physics"]["bodies"][i+1]["f_acc"]["x"];
    bodies[i].force_acc.y = (*lua)["Physics"]["bodies"][i+1]["f_acc"]["y"];

    bodies[i].entity_type = (*lua)["Physics"]["bodies"][i+1]["entity"];
  }
}

Rigidbody* PhysicWorld::Subscribe(Rigidbody body)
{
  bodies.push_back(body);
  Cpp2Lua();
  return &bodies.back();
}

void PhysicWorld::Update(float adt)
{  
  int step_per_frame = SUB_FRAME_COUNT;
  float dt = adt/step_per_frame;
  sub_frame_cols.clear();
  Lua2Cpp();
  for(int t = 0; t <step_per_frame; t++)
  {
    double start_time = GetTime();
    double single_time = 0;
    for(int i = 0; i < bodies.size(); i++)
    {
      if(!bodies[i].is_static){
        //Integrate
        EulerInt(i, dt);
        //then remove force acc 
        //Fake borders rn
        //BorderRepelent(i);
      }
      bodies[i].force_acc = {0,0};
    }
    single_time = GetTime();
    for(int i = 0; i < bodies.size(); i++)
    {
      for(int j = i; j < bodies.size(); j++)
      {
        if(bodies[i].is_active && bodies[j].is_active){
          bool c = collision_mask[bodies[i].layer + bodies[j].layer * MAX_LAYER];
          if(c){
            if(bodies[i].is_static){bodies[i].velocity = {0,0};}
            if(bodies[j].is_static){bodies[j].velocity = {0,0};}
            if(i != j){CheckCollision(i, j);}
          }
        }
      }
    }
    double final = GetTime();
    double tot = final - start_time;
    double single_percent = (single_time - start_time)/tot;
    //std::cout << "Single Percent ; " << single_percent*100 << std::endl;
  }
  Cpp2Lua();

  for(int i = 0; i < sub_frame_cols.size(); i++)
  {
    int a = sub_frame_cols[i].a;
    int b = sub_frame_cols[i].b;

    if(bodies[a].entity_type != "")
    {
      (*lua)["Entities"][bodies[a].entity_type]["on_collide"](a+1,b+1);
    }
    if(bodies[b].entity_type != "")
    {
      (*lua)["Entities"][bodies[a].entity_type]["on_collide"](a+1,b+1);
    }
  }
}

void PhysicWorld::SetMask(int a, int b, bool v)
{
  collision_mask[a + MAX_LAYER*b] = v;
  collision_mask[b + MAX_LAYER*a] = v;
}

void PhysicWorld::EulerInt(int idx, float dt)
{
  //double half integrater hehe 
  bodies[idx].position += bodies[idx].velocity * dt * 0.5;
  bodies[idx].velocity += bodies[idx].force_acc * 3000 * (dt/bodies[idx].mass);
  bodies[idx].velocity += gravity * dt;
  /*
  if(bodies[idx].force_acc != (Vector2){0,0})
  {
    std::cout << "[C++] Diff for " << idx << " [" << bodies[idx].force_acc.x << ";" << bodies[idx].force_acc.y << "]\n";
    std::cout << "[C++] Velocity " << idx << " [" << bodies[idx].velocity.x << ";" << bodies[idx].velocity.y << "]\n";
  } 
  */
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
  if(bodies[a].is_static && bodies[b].is_static)
    return;
  if(Vector2Length(bodies[a].position - bodies[b].position) > causality)
    return;
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
    sub_frame_cols.push_back({a,b});
    //Collision happens here
    //First, we need a new base, Un and Ut for u normal and u tangent 
    Vector2 Un = bodies[b].position - bodies[a].position;
    Un = Vector2Normalize(Un);
    //From Ut, simply rotate 90 degrees
    Vector2 Ut = {Un.y, -Un.x};

    //Now, let's project our speeds in the new base
    float ua = Vector2DotProduct(Un, bodies[a].velocity);
    float ub = Vector2DotProduct(Un, bodies[b].velocity);
    float ea = Vector2DotProduct(Ut, bodies[a].velocity);
    float eb = Vector2DotProduct(Ut, bodies[b].velocity);
    

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
    
    float friction_force = (eb-ea) * bodies[a].friction * bodies[b].friction / M;

    //Now put the new vectors back. 
    bodies[a].velocity = Un*va + Ut*ea + Ut*friction_force * bodies[b].mass;
    bodies[b].velocity = Un*vb + Ut*eb - Ut*friction_force * bodies[a].mass;
    
    //Now, move the two circles based on weight
    float Dr = (bodies[a].collider.param.r + bodies[b].collider.param.r)-Vector2Length(off);
    if(!bodies[a].is_static)
      bodies[a].position -= Un * (bodies[b].mass / M);
    if(!bodies[b].is_static)
      bodies[b].position += Un * (bodies[a].mass / M); 
  }
}


//TODO A optimiser

void PhysicWorld::SRCollision(int a, int b)
{
  if(bodies[a].collider.shape == SHAPE_REC)
  {
    int c = a;
    a = b;
    b = c;
  }
  Rectangle rec2 = {
      bodies[b].position.x - bodies[b].collider.param.rec.x/2,
      bodies[b].position.y - bodies[b].collider.param.rec.y/2,
      bodies[b].collider.param.rec.x,
      bodies[b].collider.param.rec.y
    };
  
  if(!CheckCollisionCircleRec(bodies[a].position, bodies[a].collider.param.r, rec2))
  {
    return;
  }
  
  sub_frame_cols.push_back({a,b});

  Rectangle rec1 = {
    bodies[a].position.x - bodies[a].collider.param.r,
    bodies[a].position.y - bodies[a].collider.param.r,
    bodies[a].collider.param.r * 2,
    bodies[a].collider.param.r * 2
  };
  //Easy case, similar to rect
  if(
    (bodies[a].position.x <= rec2.x + rec2.width && bodies[a].position.x >= rec2.x)  || 
    (bodies[a].position.y <= rec2.y + rec2.height && bodies[a].position.y >= rec2.y)
    )
  {
    //Rect Collision 
    Vector2 off = bodies[b].position - bodies[a].position;
    Vector2 normal, tangent;
    Rectangle overlap = GetCollisionRec(rec2 ,rec1);
    //std::cout << "Overlap : " << overlap.width << " " << overlap.height << std::endl;
    if(overlap.width < overlap.height)
    { normal = {1, 0}; tangent = {0, 1}; }
    else
    { normal = {0, 1}; tangent = {1, 0}; }
    
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
    
    float friction_force = (t_b - t_a)*bodies[a].friction * bodies[b].friction / M;
    bodies[a].velocity = normal * va + tangent * t_a + tangent * friction_force * bodies[b].mass;
    bodies[b].velocity = normal * vb + tangent * t_b - tangent * friction_force * bodies[a].mass;

    //Now move the two Bodies 
    //std::cout << "[C++] normal : {" << normal.x << ";" << normal.y << "}" << std::endl; 
    Vector2 move;
    if(normal == (Vector2){1, 0})
    {  
      if(bodies[a].position.x < bodies[b].position.x){move = normal * -overlap.width;}
      else {move = normal * overlap.width; }
    }
    else 
    {
      if(bodies[a].position.y < bodies[b].position.y){move = normal * -overlap.height; } 
      else {move = normal * overlap.height;}
    }
    if(!bodies[a].is_static)
      bodies[a].position += move * (bodies[b].mass / M);
    if(!bodies[b].is_static)
      bodies[b].position -= move * (bodies[a].mass / M);
  } 
  else 
  {
    //Angular 
    Vector2 cCorner = {rec2.x, rec2.y};
    float d = Vector2Length( cCorner - bodies[a].position );
    if(d > Vector2Length( bodies[a].position - (Vector2){rec2.x + rec2.width, rec2.y}) )
    {
      d = Vector2Length( bodies[a].position - (Vector2){rec2.x + rec2.width, rec2.y});
      cCorner = {rec2.x + rec2.width, rec2.y};
    }
    if(d > Vector2Length( bodies[a].position - (Vector2){rec2.x + rec2.width, rec2.y + rec2.height} ))
    {
      d = Vector2Length( bodies[a].position - (Vector2){rec2.x + rec2.width, rec2.y + rec2.height});
      cCorner = {rec2.x + rec2.width, rec2.y + rec2.height};
    }
    if(d > Vector2Length( bodies[a].position - (Vector2){rec2.x, rec2.y+rec2.height} ))
    {
      d = Vector2Length( bodies[a].position - (Vector2){rec2.x, rec2.y+rec2.height});
      cCorner = {rec2.x, rec2.y + rec2.height};
    }
    Vector2 off = cCorner - bodies[a].position; 
    //Vector2 Un = cCorner - bodies[a].position;
    Vector2 Un = Vector2Normalize(off);
    //From Ut, simply rotate 90 degrees
    Vector2 Ut = {Un.y, -Un.x};

    //Now, let's project our speeds in the new base
    float ua = Vector2DotProduct(Un, bodies[a].velocity);
    float ub = Vector2DotProduct(Un, bodies[b].velocity);
    float ea = Vector2DotProduct(Ut, bodies[a].velocity);
    float eb = Vector2DotProduct(Ut, bodies[b].velocity);
    

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
    
    float friction_force = (eb-ea) * bodies[a].friction * bodies[b].friction / M;

    //Now put the new vectors back. 
    bodies[a].velocity = Un*va + Ut*ea + Ut*friction_force * bodies[b].mass;
    bodies[b].velocity = Un*vb + Ut*eb - Ut*friction_force * bodies[a].mass;
    
    //Now, move the two circles based on weight
    float Dr = bodies[a].collider.param.r-Vector2Length(off);
    if(!bodies[a].is_static)
      bodies[a].position -= Un * (bodies[b].mass / M);
    if(!bodies[b].is_static)
      bodies[b].position += Un * (bodies[a].mass / M); 
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
  sub_frame_cols.push_back({a,b});
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
  
  float friction_force = (t_b - t_a)*bodies[a].friction * bodies[b].friction / M;
  bodies[a].velocity = normal * va + tangent * t_a + tangent * friction_force * bodies[b].mass;
  bodies[b].velocity = normal * vb + tangent * t_b - tangent * friction_force * bodies[a].mass;

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
  Rigidbody rb;
  rb.position = pos;
  rb.velocity = {0, 0};
  rb.mass = m,
  rb.collider = {
                  SHAPE_CIRCLE,
                  r
                };
  rb.Cr = Cr;
  rb.is_static = false;
  rb.force_acc = {0, 0};


  (*lua).script(
  R"(
Physics.bodies[#Physics.bodies+1] = {
      pos = {x=0,y=0},
      vel = {x=0,y=0},
      mass = 0,
      Cr = 0,
      static = false,
      f_acc = {x=0, y=0},
      entity = ""
    }
  )");
  return Subscribe(rb);
}

Rigidbody* PhysicWorld::NewRec(Vector2 pos, Vector2 re, float m, float Cr)
{
  Collider c;
  c.shape = SHAPE_REC;
  c.param.rec = re;
  Rigidbody rb;
  rb.position = pos;
  rb.velocity = {0, 0};
  rb.mass = m;
  rb.collider = c,
  rb.Cr = Cr,
  rb.is_static = false;
  rb.force_acc = {0, 0};
  
  (*lua).script(
  R"(
Physics.bodies[#Physics.bodies+1] = {
      pos = {x=0,y=0},
      vel = {x=0,y=0},
      mass = 0,
      Cr = 0,
      static = false,
      f_acc = {x=0, y=0},
      entity = ""
    }
  )"); 
  return Subscribe(rb);
}

bool PhysicWorld::LineLine(Vector2 a, Vector2 b, Vector2 c, Vector2 d,Vector2 &closest)
{
  float uA = ((d.x-c.x)*(a.y-c.y) - (d.y-c.y)*(a.x-c.x)) / ((d.y-c.y)*(b.x-a.x) - (d.x-c.x)*(b.y-a.y));
  float uB = ((b.x-a.x)*(a.y-c.y) - (b.y-a.y)*(a.x-c.x)) / ((d.y-c.y)*(b.x-a.x) - (d.x-c.x)*(b.y-a.y));

  // if uA and uB are between 0-1, lines are colliding
  if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {

    // optionally, draw a circle where the lines meet
    float intersectionX = a.x + (uA * (b.x-a.x));
    float intersectionY = a.y + (uA * (b.y-a.y));
    
    closest = {intersectionX,intersectionY};

    return true;
  }
  return false;
}
bool PhysicWorld::RaycastLua(float x1, float y1, float x2, float y2, int layer)
{
  Vector2 temp;
  return Raycast({x1, y1}, {x2, y2}, layer, temp);
}
bool PhysicWorld::Raycast(Vector2 o, Vector2 dir,int layer ,Vector2 &closest)
{
  Vector2 c = {-1000000,-10000000};
  bool as_found = false;
  for(int i = 0; i<bodies.size(); i++)
  {
    
    if(bodies[i].collider.shape == SHAPE_REC)
    {
      //Create four lines, for the corners 
      Vector2 A = {
        bodies[i].position.x - bodies[i].collider.param.rec.x/2,
        bodies[i].position.y - bodies[i].collider.param.rec.y/2,
      };
      Vector2 B = {
        bodies[i].position.x + bodies[i].collider.param.rec.x/2,
        bodies[i].position.y - bodies[i].collider.param.rec.y/2,
      };
      Vector2 C = {
        bodies[i].position.x + bodies[i].collider.param.rec.x/2,
        bodies[i].position.y + bodies[i].collider.param.rec.y/2,
      };
      Vector2 D = {
        bodies[i].position.x - bodies[i].collider.param.rec.x/2,
        bodies[i].position.y + bodies[i].collider.param.rec.y/2,
      };
      Vector2 temp;
      if(LineLine(A,B,o,o+dir,temp))
      {
        if(Vector2LengthSqr(o-temp) < Vector2LengthSqr(o-c))
          c = temp;
      }
      if(LineLine(B,C,o,o+dir,temp))
      {
        if(Vector2LengthSqr(o-temp) < Vector2LengthSqr(o-c))
          c = temp;
      }
      if(LineLine(C,D,o,o+dir,temp))
      {
        if(Vector2LengthSqr(o-temp) < Vector2LengthSqr(o-c))
          c = temp;
      }
      if(LineLine(D,A,o,o+dir,temp))
      {
        if(Vector2LengthSqr(o-temp) < Vector2LengthSqr(o-c))
          c = temp;
      }
    }
  }
  if(c != (Vector2){-1000000,-10000000})
  {
    closest = c;
    return true;
  }
  return false;
}



