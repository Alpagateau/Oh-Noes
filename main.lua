
function love.load()
  love.graphics.setDefaultFilter("nearest", "nearest")
  --Importing all libraries
  sti = require 'libs/sti'
  gameMap = sti('res/level_one.lua')
  camera_lib  = require('libs/camera')
  love.graphics.setBackgroundColor(99/255, 124/255, 143/255)
  wf = require('libs/windfield/windfield')

-- Prepare physics world with horizontal and vertical gravity
	World = wf.newWorld(0,200)

  require("scripts.player")
  camera = camera_lib()
  camera.scale = 3

  --Adding collisions
  if gameMap.layers["Ground"] then
    for i,obj in pairs(gameMap.layers["Ground"].objects) do
      local grnd = World:newRectangleCollider(obj.x, obj.y, obj.width, obj.height)
      grnd:setType('static')
    end
  end
end

function love.update(dt)
  World:update(dt)
  Player.update(dt)
  camera:lookAt(Player.x, Player.y)
end

function love.draw()

  camera:attach()

  gameMap:drawLayer(gameMap.layers["Parralax"])
  gameMap:drawLayer(gameMap.layers["Tile Layer 1"])
  --gameMap:draw()
  Player.draw()
  World:draw()
  camera:detach()
  love.graphics.print("Grounded: " .. tostring(Player.isGrounded), 10, 10)
end

function love.keypressed(key)
  if key == "up" then
    Player.jump()
  end
end
