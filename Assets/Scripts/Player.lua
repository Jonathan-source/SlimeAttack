
require 'Animation'

---------------------
-- Player attributes
---------------------
Player = {
    map = nil, -- reference to map

    texture = "player_knight.png",

    sourceRec = {x = 0.0, y = 0.0, width = 15.0,  height = 22.0},

    position = Vector2.new(0.0, 0.0),

    direction = Vector2.new(0, 0),

    speed = 100,

    state = 'idle',

    boundingBox = {x = 0.0, y = 0.0, width = 15.0,  height = 20.0},

    animation = nil,

    health = 100,
}

---------------------
-- Player behaviours
---------------------
Player.behaviours = {
    ['idle'] = function(dt)
        Player.update_direction()
        
        if not Player.direction:isZero() then
            Player.set_state_and_animation('walking')
        end

    end,

    ['walking'] = function(dt)   
        Player.update_direction()

        if Player.direction:isZero() then
            Player.set_state_and_animation('idle')
        end

    end,
}


---------------------
-- Player animations
---------------------
Player.animations = {
    ['idle'] = Animation:new({
        texture = Player.texture,
        frames = {           
            [1] = { x = 0.0, y = 0.0, width = 15.0, height = 22.0 },
        }
    }),
    ['walking'] = Animation:new({
        texture = Player.texture,
        frames = {       
            [1] = { x = 0.0, y = 0.0, width = 16.0, height = 22.0 },    
            [2] = { x = 16.0, y = 0.0, width = 16.0, height = 22.0 },
            [3] = { x = 32.0, y = 0.0, width = 16.0, height = 22.0 },
            [4] = { x = 48.0, y = 0.0, width = 16.0, height = 22.0 }
        },
    }),
}

Player.animation = Player.animations['idle']

---------------------
-- Player methods
---------------------
function Player:update(dt)
    self.behaviours[self.state](dt)
    self.animation:update(dt)
    self.sourceRec.x = self.animation:getCurrentFrame().x

    self.direction:normalize()
    self.position = self.position + self.direction * self.speed * dt

    self.boundingBox.x = self.position.x + 6
    self.boundingBox.y = self.position.y + 9
end

function Player:render()
    --Raylib.drawTextureRec(self.texture, self.sourceRec, self.position)
    Raylib.drawTexturePro(self.texture, self.sourceRec, self.position, 1.5, 0.0)
end



function Player.update_direction()
    -- right & left
    if Raylib.isKeyDown(KEY.KEY_D) then 
        Player.direction.x = 1
        Player.sourceRec.y = 0.0
    elseif Raylib.isKeyDown(KEY.KEY_A) then 
        Player.direction.x = -1
        Player.sourceRec.y = 22.0    
    else Player.direction.x = 0 end
    
    -- up & down
    if Raylib.isKeyDown(KEY.KEY_W) then Player.direction.y = -1
    elseif Raylib.isKeyDown(KEY.KEY_S) then Player.direction.y = 1
    else Player.direction.y = 0 end
end

function Player.set_state_and_animation(state)
    Player.state = state
    Player.animation = Player.animations[state]
    Player.animations[state]:restart()
end


---------------------
-- Player constructor
---------------------
function Player:new(t)
    t = t or {}
    setmetatable(t, self)
    self.__index = self
    return t
end
