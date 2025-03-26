

function love.load()

  love.graphics.setDefaultFilter("nearest", "nearest")
  sti = require 'libs/sti'
  gameMap = sti('SecondTry.lua')
  camera_lib  = require('libs/camera')
  wf = require('libs/windfield/windfield') 

-- Prepare physics world with horizontal and vertical gravity
	World = wf.newWorld(0,9)

  Player = {}
  Player.collider = World:newBSGRectangleCollider(0, 0, 16, 32, 3)
  Player.collider:setFixedRotation(true)
  Player.x = 0
  Player.y = 0
  Player.speed = 1
  Player.sprite = love.graphics.newImage("res/Prota.png")

  camera = camera_lib()
  camera.scale = 2
end

function love.update(dt)
  local isMoving = false

  local vx = 0
  local vy = 9

  if love.keyboard.isDown("right") then
    vx = Player.speed
  --player.anim = player.animations.right
    isMoving = true
  end

  if love.keyboard.isDown("left") then
    vx = - Player.speed
  --player.anim = player.animations.left
    isMoving = true
  end

  if love.keyboard.isDown("down") then
    vy = Player.speed
  --player.anim = player.animations.down
    isMoving = true
  end

  if love.keyboard.isDown("up") then
    vy =  -Player.speed
    isMoving = true
  end
  Player.collider:setLinearVelocity(vx, vy)

  World:update(dt)

  camera:lookAt(Player.x, Player.y)
end

function love.draw()
  camera:attach()
  gameMap:drawLayer(gameMap.layers["Tile Layer 1"])
  love.graphics.draw(Player.sprite, Player.x, Player.y)
  World:draw()
  camera:detach()
end
