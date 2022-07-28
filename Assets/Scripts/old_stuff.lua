if Raylib.isKeyDown(KEY.KEY_D) then
    self.velocity = WALK_FORCE
    self.sourceRec.x = 0.0
    -- within bounds of the map
    local max = self.map.mapWidth * self.map.tileWidth - self.sourceRec.width
    self.position.x = math.min(max, self.position.x)
elseif Raylib.isKeyDown(KEY.KEY_A) then
    self.velocity = -WALK_FORCE
    self.sourceRec.x = 250.0
    -- within bounds of the map
    self.position.x = math.max(0.0, self.position.x)
else
    self.velocity.x = 0.0 
end

if Raylib.isKeyDown(KEY.KEY_W) then

elseif Raylib.isKeyDown(KEY.KEY_S) then

end

self.position.x = self.position.x + self.velocity.x * dt

self.position.y = self.position.y + self.velocity.y * dt

if self.velocity.x < 0 then
    -- check left collision
    if self.map:checkCollision(self.map:tileAtPosition(self.position.x - 1, self.position.y + 1)) or
        self.map:checkCollision(self.map:tileAtPosition(self.position.x - 1, self.position.y + self.sourceRec.height - 1)) then
        self.velocity.x = 0.0
        local mod = (self.position.x - 1) % self.map.tileWidth
        local offset = self.map.tileWidth - mod
        self.position.x = math.floor(self.position.x - 1 + offset)
    end
elseif self.velocity.x > 0 then
    -- check right collision
    if self.map:checkCollision(self.map:tileAtPosition(self.position.x + self.sourceRec.width + 1, self.position.y + 1)) or
        self.map:checkCollision(self.map:tileAtPosition(self.position.x + self.sourceRec.width + 1, self.position.y + self.sourceRec.height - 1)) then
        self.velocity.x = 0.0
        local mod = (self.position.x + self.sourceRec.width + 1) % self.map.tileWidth
        self.position.x = math.floor(self.position.x + 1 - mod)
    end 
end

-- check down collision
if self.map:checkCollision(self.map:tileAtPosition(self.position.x, self.position.y + self.sourceRec.height + 1)) or
    self.map:checkCollision(self.map:tileAtPosition(self.position.x + self.sourceRec.width - 1, self.position.y + self.sourceRec.height + 1)) then
    self.velocity.y = 0.0
    self.isJumping = false
    local mod = (self.position.y + self.sourceRec.height + 1) % self.map.tileHeight
    self.position.y = self.position.y - mod
end

self.collisionBox.x = self.position.x
self.collisionBox.y = self.position.y 