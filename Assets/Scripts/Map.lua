
---------------------
-- Map attributes
---------------------
Map = {
    texture = "tileset.png",
    tileWidth = 16,
    tileHeight = 16,
    mapWidth = 0,
    mapHeight = 0,
    tiles = {},
    spawners = {}
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

function Map:loadLevel(level)
    print("\n\nLoading '" .. level .. "'")
    self.mapWidth, self.mapHeight, self.tiles = Raylib.loadLevel(level)

    print("width: " .. self.mapWidth .. " height: " .. self.mapHeight)
    for y = 1, self.mapHeight do
        for x = 1, self.mapWidth do
            local tileID = self.tiles[(y - 1) * self.mapWidth + x]
            io.write(tileID)
            io.write(" ")
            if tileID == 6 or tileID == 7 then
                self.spawners[#self.spawners + 1] = { x = x, y = y, id = tileID }
            end
        end
        print()
    end

    for _, v in ipairs(self.spawners) do
        print(v.x .. " " .. v.y .. " " .. v.id)
    end
end

function Map:isPlayerWithinBoarders(player)
    local xMax = 0 
    local yMax = 0 
    local xMax = self.tileWidth * self.mapWidth
    local yMax = self.tileWidth * self.mapWidth


    return true
end

function Map:render()
    for h = 1, self.mapHeight do
        for w = 1, self.mapWidth do

            local tileID = self:getTile(w, h)
            local tileX = tileID % 8
            local tileY = 0
   
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
    for h = 1, self.mapHeight do
        for w = 1, self.mapWidth do

            local tileID = self:getTile(w, h)
            local tileX = tileID % 8
            local tileY = 0
   
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