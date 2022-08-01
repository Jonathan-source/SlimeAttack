
require 'Map'
require 'Player'
require 'Enemy'
require 'Knife'

local gameMap
local player

local enemyList = {}
local knifeList = {}

local MAX_KNIFE_COUNT = 2

local slimesKilled = 0

---------------------
-- Initialize
---------------------
function onInit()
    gameMap = Map:new()
    --gameMap:loadLevel("test.txt")
    gameMap:createLevel(25, 25)
    gameMap:addMapToRenderBatch()

    player = Player:new()
    player.map = gameMap

    for i = 1, 20 do
        enemyList[#enemyList + 1] = Enemy:new({map = gameMap, player = player,
        texture = "enemy_slime_red.png", position = Vector2.new(math.random(0, 400), math.random(0, 400))})
    end 

    Raylib.setCameraTarget(player.position)
    Raylib.setCameraOffset({x = WINDOW_WIDTH / 2, y = WINDOW_HEIGHT / 2})
    Raylib.setCameraZoom(2.0)
    Raylib.setCameraRotation(0.0)
end


---------------------
-- Update
---------------------
function onUpdate(dt)
    player:update(dt)

    -- update enemies
    for i, enemy in ipairs(enemyList) do
        enemy:update(dt)

        if Raylib.checkCollisionRect(enemy.boundingBox, player.boundingBox) then
            player.health = player.health - math.random(enemy.minAttackPower, enemy.maxAttackPower)
            table.remove(enemyList, i)
            if player.health <= 0 then
                print("GAME OVER")
            end
        end

    end

    -- update knives
    for i, knife in ipairs(knifeList) do
        knife:update(dt)
        if knife.timer > 0.5 then -- despawn after 0.5s
            table.remove(knifeList, i)
        end
    end

    -- update knife-enemy collisions
    for i, knife in ipairs(knifeList) do
        for j, enemy in ipairs(enemyList) do

            if Raylib.checkCollisionRect(knife.boundingBox, enemy.boundingBox) then
                table.remove(knifeList, i)
                enemy.health = enemy.health - math.random(knife.minAttackPower, knife.maxAttackPower)
                enemy.state = 'chasing'
                if enemy.health <= 0 then
                    table.remove(enemyList, j)
                    slimesKilled = slimesKilled + 1
                    print(slimesKilled)
                end
            end

        end
    end

    -- throw knives
    if Raylib.isMouseButtonPressed(MOUSE.BUTTON_LEFT) then
       
        if #knifeList < MAX_KNIFE_COUNT then
            local mouse = Vector2.new(Raylib.getMouseWorldPosition()) 
            local knifeDir = mouse - player.position
            knifeDir:normalize()

            local knifePos = Vector2.new(
                player.position.x + (player.sourceRec.width / 2),
                player.position.y + (player.sourceRec.height / 2)
            )

            knifeList[#knifeList + 1] = Knife:new({
                position = knifePos, direction = knifeDir,
                rotation = calculateRotation(knifeDir)
            })
        end
    end

    -- update camera
    Raylib.setCameraTarget(player.position)
end


---------------------
-- Render
---------------------
function onRender()
    --gameMap:drawLayers(gameMap:layers['Ground'])
    player:render()
    --Raylib.drawRectangle(player.boundingBox); -- for debug

    for i = 1, #enemyList do
        enemyList[i]:render()
        --Raylib.drawRectangle(enemyList[i].boundingBox); -- for debug
    end

    for i = 1, #knifeList do
        knifeList[i]:render()
    end

    local position = { x = 10, y = 10 }
    local text = "Slimes killed: " .. slimesKilled
    Raylib.postRenderText(text, position, 25, COLOR.WHITE)
end
