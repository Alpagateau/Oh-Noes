Entities = {
  Red = require("scripts.red"),
  Player = require("scripts.player")
}

function StartEntities()
  for k,v in pairs(Physics.bodies) do
    if Entities[v["entity"]] ~= nil then
      Entities[v["entity"]].start(k)
    end
  end
end

function UpdateEntities(dt)
  for k,v in pairs(Physics.bodies) do
    if Entities[v["entity"]] ~= nil then
      Entities[v["entity"]].update(k, dt)
    end
  end
end
