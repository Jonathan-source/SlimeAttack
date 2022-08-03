
require 'Animation'

Enemy = {}
Enemy.__index = Enemy

---------------------
-- Enemy methods
---------------------
function Enemy:update(dt)
    self.behaviours[self.state](dt)
    self.animation:update(dt)
    self.sourceRec.x = self.animation:getCurrentFrame().x

    self.direction:normalize()
    self.position = self.position + self.direction * self.speed * dt

    self.boundingBox.x = self.position.x + 3
    self.boundingBox.y = self.position.y + 2

    self.healthBar.x = self.position.x
    self.healthBar.y = self.position.y - 5
    self.healthBar.width = 25.0/100 * self.health

    self.missingHealthBar.x = self.healthBar.x
    self.missingHealthBar.y = self.healthBar.y
end

function Enemy:render()

    if self.direction.x < 0 then
        self.sourceRec.y = 16.0 
    else
        self.sourceRec.y = 0.0 
    end

    Raylib.drawTexturePro(self.texture, self.sourceRec, self.position, self.scale, 0.0)
    Raylib.drawRectangle(self.missingHealthBar, COLOR.RED)
    Raylib.drawRectangle(self.healthBar, COLOR.GREEN)
end

---------------------
-- Enemy constructor
---------------------
function Enemy:new(params)
    local this = {
        map = params.map, -- reference to map

        player = params.player, -- reference to player
    
        texture = params.texture,
    
        sourceRec = {x = 0.0, y = 0.0, width = 16.0,  height = 16.0},
    
        position = params.position or Vector2.new(0.0, 0.0),
    
        direction = Vector2.new(0, 0),

        scale = 1.5,

        speed = params.speed or 10,
    
        state = params.state or 'patrolling',
    
        boundingBox = {x = 0.0, y = 0.0, width = 15.0,  height = 20.0},

        healthBar = {x = 0.0, y = 0.0, width = 25.0,  height = 5.0},
        missingHealthBar = {x = 0.0, y = 0.0, width = 25.0,  height = 5.0},
    
        animation = nil,
    
        timer = 0.0,

        health = 100,

        minAttackPower = 5,

        maxAttackPower = 15,
    }

    ---------------------
    -- Enemy behaviours
    ---------------------
    this.behaviours = {
        ['patrolling'] = function(dt)
            this.timer = this.timer + dt
            if this.timer > 1 then
                this.direction.x = math.random(-1, 1)
                this.direction.y = math.random(-1, 1)
                this.timer = 0.0
            end

            local len = this.player.position - this.position
            if len:length() < 100 then
                this.state = 'chasing'
            end

        end,

        ['chasing'] = function(dt)
            this.direction = this.player.position - this.position
            this.speed = 40
        end,
    }

    ---------------------
    -- Enemy animations
    ---------------------
    this.animations = Animation:new({
        texture = this.texture,
        frames = {           
            [1] = {x = 0.0, y = 0.0, width = 16.0,  height = 16.0},
            [2] = {x = 16.0, y = 0.0, width = 16.0,  height = 16.0},
            [3] = {x = 32.0, y = 0.0, width = 16.0,  height = 16.0},
            [4] = {x = 48.0, y = 0.0, width = 16.0,  height = 16.0},
        },
        interval = 0.2
    })

    this.animation = this.animations -- only one animation for slime

    setmetatable(this, self)
    return this
end
