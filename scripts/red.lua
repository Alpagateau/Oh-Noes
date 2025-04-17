Red = {}

function Red.start(me)
  print("[LUA] Redball start")
end

function Red.update(me, dt)
  print("[Lua] Redball Update")
end

function Red.on_collide(me, other)
  --print("[LUA] Red "..me.." Collided-with "..other)
  --local a = Physics.bodies[me].vel.x
  --print("[LUA] My speed is "..a)
end

return Red
