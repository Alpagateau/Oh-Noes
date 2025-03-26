-- Heavely based on Game Maker's toolkit 
Player = {
  maxSpeed = 50,
  acc = 1
}

function Player.start(me)
end

function Player.update(me, dt)
  local dir = 0
  local b = 0
  if IsKeyDown(KeyCodes.KEY_RIGHT) then
    dir = dir + 1
  end

  if IsKeyDown(KeyCodes.KEY_LEFT) then
    dir = dir - 1
  end

  local v = Physics.bodies[me].vel.x
  local v2 = v * ((Player.acc )- 1) + ( dir * Player.maxSpeed ) * (Player.acc)
  Physics.bodies[me].vel.x = v2

  if IsKeyPressed(KeyCodes.KEY_SPACE) then
    Physics.bodies[me].f_acc.y = -100
  end
end

function Player.on_collide(me, other)
end

return Player
