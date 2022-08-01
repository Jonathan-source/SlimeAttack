
require 'Keys'
require 'Color'
require 'Vector2'

--[[ Load engine variables ]]--
local m_host = nil
Raylib = {

    setHostReference = function(host)
        if m_host == nil then m_host = host
        else print("Host reference already set") end
    end,

    loadLevel = function(level) -- returns width, height, tiles = {}
        return _LoadLevel(m_host, level)
    end,

    isKeyDown = function(key)
        return _IsKeyDown(m_host, key)
    end,

    isKeyPressed = function(key)
        return _IsKeyPressed(m_host, key)
    end,

    isMouseButtonDown = function(button)
        return _IsMouseButtonDown(m_host, button)
    end,

    isMouseButtonPressed = function(button)
        return _IsMouseButtonPressed(m_host, button)
    end,

    getMousePosition = function()
        return _GetMousePosition(m_host)
    end,

    getMouseWorldPosition = function()
        return _GetMouseScreenToWorld2D(m_host)
    end,

    drawTextureRec = function(texture, sourceRec, position)
        _DrawTextureRec(m_host, texture,
            sourceRec.x, sourceRec.y, sourceRec.width, sourceRec.height,
            position.x, position.y)
    end,

    addToRenderBatch = function(texture, sourceRec, position)
        _AddToRenderBatch(m_host, texture,
            sourceRec.x, sourceRec.y, sourceRec.width, sourceRec.height,
            position.x, position.y)
    end,

    drawTexturePro = function(texture, sourceRec, position, scale, rotation)
        _DrawTexturePro(m_host, texture,
            sourceRec.x, sourceRec.y, sourceRec.width, sourceRec.height,
            position.x, position.y, scale, rotation)
    end,

    setCameraTarget = function(target)
        _SetCameraTarget(m_host, target.x, target.y)
    end,

    setCameraOffset = function(offset)
        _SetCameraOffset(m_host, offset.x, offset.y)
    end,

    setCameraZoom = function(zoom)
        _SetCameraZoom(m_host, zoom)
    end,

    setCameraRotation = function(rotation)
        _SetCameraRotation(m_host, rotation)
    end,

    drawRectangle = function(rectangle)
        _DrawRectangle(m_host, rectangle.x, rectangle.y, rectangle.width, rectangle.height)
    end,

    checkCollisionRect = function(rect1, rect2)
        return _CheckCollisionRect(m_host, 
            rect1.x, rect1.y, rect1.width, rect1.height,
            rect2.x, rect2.y, rect2.width, rect2.height)
    end,

    drawText = function(text, position, size, color)
        _DrawText(m_host, text, position.x, position.y, size, color)
    end,

    postRenderText = function(text, position, size, color)
        _PostRenderText(m_host, text, position.x, position.y, size, color)
    end,
}

Raylib = setmetatable({}, 
    { __index = Raylib,
    __newindex = function() 
        error("Raylib table is read-only")
    end 
})
