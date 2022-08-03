
---------------------
-- Map attributes
---------------------
Map = {
    texture = "ground.png",
    tileWidth = 16,
    tileHeight = 16,
    numTilesX = 16, -- screen resolution.x / tileWidth 
    numTilesY = 12, -- screen resolution.y / tileHeight
    mapWidth = 0,
    mapHeight = 0,
    gravity = 1500,
    tiles = {}
}


---------------------
-- Map methods
---------------------
function Map:getTile(x, y)
    return self.tiles[(y - 1) * self.mapWidth + x]
end

function Map:tileAtPosition(x, y)
    return self:getTile(math.floor(x / self.tileWidth) + 1, math.floor(y / self.tileWidth) + 1)
end

function Map:checkCollision(tileID)
    local collidableTileIDs = {
        0, 1, 2,
    }
    for _, v in ipairs(collidableTileIDs) do
        if tileID == v then
            return true
        end
    end

    return false
end

function Map:loadLevel(level)
    print("\n\nLoading '" .. level .. "'")
    self.mapWidth, self.mapHeight, self.tiles = Raylib.loadLevel(level)

    print("width: " .. self.mapWidth .. " height: " .. self.mapHeight)
    for y = 1, self.mapHeight do
        for x = 1, self.mapWidth do
            io.write(self.tiles[(y - 1) * self.mapWidth + x])
            io.write(" ")
        end
        print()
    end
end


function Map:createLevel(width, height)
    self.mapWidth = width
    self.mapHeight = height
    for i = 1, (width * height) do
        if math.random(0, 1) == 1 then
            self.tiles[#self.tiles + 1] = 4 -- grass
        else
            self.tiles[#self.tiles + 1] = math.random(0, 3) -- grass with deco
        end
    end
end

function Map:render()
    -- todo: only render what the camera see
    for h = 1, self.mapHeight do
        for w = 1, self.mapWidth do

            local tileID = self:getTile(w, h)
            local tileX = tileID % 5
            local tileY = 0 --math.floor(tileID / 7)
   
            sourceRec = { 
                x = tonumber(tileX * self.tileWidth), 
                y = tonumber(tileY * self.tileHeight), 
                width = tonumber(self.tileWidth), 
                height = tonumber(self.tileHeight) 
            }

            position = { 
                x = tonumber((w - 1) * self.tileWidth),
                y = tonumber((h - 1) * self.tileHeight)
            } 

            Raylib.drawTextureRec(self.texture, sourceRec, position)
        end
    end
end

-- Call once to let C++ render map each frame
function Map:addMapToRenderBatch()
    -- todo: only render what the camera see
    for h = 1, self.mapHeight do
        for w = 1, self.mapWidth do

            local tileID = self:getTile(w, h)
            local tileX = tileID % 5
            local tileY = 0--math.floor(tileID / 7)
   
            sourceRec = { 
                x = tileX * self.tileWidth, 
                y = tileY * self.tileHeight, 
                width = self.tileWidth, 
                height = self.tileHeight 
            }

            position = { 
                x = (w - 1) * self.tileWidth,
                y = (h - 1) * self.tileHeight
            } 

            Raylib.addToRenderBatch(self.texture, sourceRec, position)
        end
    end
end


---------------------
-- Map constructor
---------------------
function Map:new(t)
    t = t or {}
    setmetatable(t, self)
    self.__index = self
    return t
end