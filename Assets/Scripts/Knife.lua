
Knife = {}
Knife.__index = Knife

---------------------
-- Knife methods
---------------------
function Knife:update(dt)
    self.direction:normalize()
    self.position = self.position + self.direction * self.speed * dt

    self.boundingBox.x = self.position.x
    self.boundingBox.y = self.position.y

    self.timer = self.timer + dt
end

function Knife:render()
    Raylib.drawTexturePro(self.texture, self.sourceRec, self.position, 1.0, self.rotation)
end


---------------------
-- Knife constructor
---------------------
function Knife:new(params)
    local this = {
        texture = "weapon_knife.png",
    
        sourceRec = {x = 0.0, y = 0.0, width = 13.0,  height = 6.0},

        rotation = params.rotation or 0.0,
    
        position = params.position,
    
        direction = params.direction,

        speed = 500,

        minAttackPower = 10,

        maxAttackPower = 50,

        timer = 0.0,
  
        boundingBox = {x = 0.0, y = 0.0, width = 13.0,  height = 6.0},
    }

    setmetatable(this, self)
    return this
end
