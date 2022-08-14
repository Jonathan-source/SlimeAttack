
Potion = {}
Potion.__index = Potion

---------------------
-- Potion methods
---------------------
function Potion:update(dt)
    self.boundingBox.x = self.position.x
    self.boundingBox.y = self.position.y

    self.timer = self.timer + dt
end

function Potion:render()
    Raylib.drawTexturePro(self.texture, self.sourceRec, self.position, 1.5, self.rotation)
end


---------------------
-- Potion constructor
---------------------
function Potion:new(params)
    local this = {
        texture = "item_health_potion.png",
    
        sourceRec = {x = 0.0, y = 0.0, width = 10.0,  height = 11.0},

        rotation = params.rotation or 0.0,
    
        position = params.position,
    
        minHealthGain = 15,

        maxHealthGain = 35,

        timer = 0.0,
  
        boundingBox = {x = 0.0, y = 0.0, width = 13.0,  height = 6.0},
    }

    setmetatable(this, self)
    return this
end
