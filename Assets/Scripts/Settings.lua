--[[ Path to all scripts ]]--
local pathToScripts = ';C:/Dev/SlimeAttack/Assets/Scripts/?.lua'
package.path = package.path .. pathToScripts

print("LUA: path to lua scripts: " .. pathToScripts)

math.randomseed(os.time())

require 'Raylib'

-- Globals
WINDOW_WIDTH = 800
WINDOW_HEIGHT = 600

--[[ EnginePreload ]]--
function preload(host)
    Raylib.setHostReference(host)

    title = "Engine"
    windowWidth = WINDOW_WIDTH
    windowHeight = WINDOW_HEIGHT
    targetFPS = 60
    mainScript = "Game.lua"
    
    _InitializeEngine(host, title, windowWidth, windowHeight, targetFPS, mainScript)
end
