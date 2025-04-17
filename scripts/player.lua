require("scripts.utils")

-- Heavely based on Game Maker's toolkit 
Player = {}

Player.collider = World:newBSGRectangleCollider(0, 0, 16, 32, 3)
Player.collider:setFixedRotation(true)

-- Player Position
Player.x = 0
Player.y = 0

-- Shush
Player.canMove = true
Player.useAcceleration = true
Player.isGrounded = false
Player.maxSpeedChange = 100

-- Calculations
Player.directionX = 0
Player.desiredVelocity = {0, 0}
Player.velocity = {0, 0}
Player.acceleration = 1
Player.deceleration = 1
Player.turnSpeed = 1

-- Settings
Player.maxSpeed = 120
Player.maxAcceleration = 340
Player.maxDeceleration = 450
Player.maxTurnSpeed = 500
Player.maxAirAcceleration = 34
Player.maxAirDeceleration = 45
Player.maxAirTurnSpeed = 50
Player.friction = 0

-- Jump Settings
Player.jumpForce = 100

-- Gfx
Player.sprite = love.graphics.newImage("res/Prota.png")

function Player.update(dt)

  --check if grounded 
  Player.isGrounded = false
  for _, contact in ipairs(Player.collider:getContacts()) do
    local _, ny = contact:getNormal()
    if ny == 1 then
      Player.isGrounded = true
    end
  end

  Player.directionX = 0
  -- read inputs and set direction
  if love.keyboard.isDown("right") then
    Player.directionX = 1
  end

  if love.keyboard.isDown("left") then
    Player.directionX = -1
  end

  Player.desiredVelocity = {
    Player.directionX * math.max(Player.maxSpeed - Player.friction, 0),
    0
  }

  Player.velocity[1], Player.velocity[2] = Player.collider:getLinearVelocity()

  if Player.useAcceleration then
    Player.runWithAcceleration(dt)
  else
    if Player.isGrounded then
      Player.runWithoutAcceleration(dt)
    else
      Player.runWithAcceleration(dt)
    end
  end

  -- Set speed and move player
  --local speed = (cur_speed + (Player.speed * dir) )/2
  --Player.collider:setLinearVelocity(speed, vy)

  --Move the plater to the correct position
  Player.x = Player.collider:getX() - 8
  Player.y = Player.collider:getY() - 16
end

function Player.runWithAcceleration(dt)
  if Player.isGrounded then
    Player.acceleration = Player.maxAcceleration
    Player.deceleration = Player.maxDeceleration
    Player.turnSpeed = Player.maxTurnSpeed
  else
    Player.acceleration = Player.maxAirAcceleration
    Player.deceleration = Player.maxAirDeceleration
    Player.turnSpeed = Player.maxAirTurnSpeed
  end

  if sign(Player.directionX) ~= sign(Player.velocity[1]) then
    Player.maxSpeedChange = Player.turnSpeed * dt
  else
    Player.maxSpeedChange = Player.acceleration * dt
  end

  Player.velocity[1] = MoveTowards( Player.velocity[1], Player.desiredVelocity[1], Player.maxSpeedChange)

  Player.collider:setLinearVelocity(Player.velocity[1], Player.velocity[2])
end

function Player.runWithoutAcceleration(dt)
  Player.velocity[1] = Player.desiredVelocity[1]
  Player.collider:setLinearVelocity(Player.velocity[1], Player.velocity[2])
end

function Player.jump()
  if Player.isGrounded then
    Player.collider:applyLinearImpulse(0, -Player.jumpForce)
  end
end


-- Rendering
function Player.draw()
  love.graphics.draw(Player.sprite, Player.x, Player.y)
end

return Player
