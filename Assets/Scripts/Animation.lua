
Animation = {}
Animation.__index = Animation


---------------------
-- Animation methods
---------------------
function Animation:getCurrentFrame()
    return self.frames[self.currentFrame]
end

function Animation:restart()
    self.timer = 0.0
    self.currentFrame = 1
end

function Animation:update(dt)
    if #self.frames > 1 then
        self.timer = self.timer + dt

        if self.timer > self.interval then
            self.timer = self.timer % self.interval
            self.currentFrame = math.max(1, (self.currentFrame + 1) % (#self.frames + 1))
        end
    end
end


---------------------
-- Animation constructor
---------------------
function Animation:new(params)
    local t = {
        texture = params.texture,
        frames = params.frames or {},
        interval = params.interval or 0.05,
        timer = 0.0,
        currentFrame = 1
    }

    setmetatable(t, self)
    return t
end
