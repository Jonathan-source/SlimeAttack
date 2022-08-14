
require 'Map'
require 'Player'
require 'Enemy'
require 'Knife'
require 'Potion'

local gameMap
local player

local enemyList = {}
local knifeList = {}
local itemList = {}

local MAX_KNIFE_COUNT = 5
local slimesKilled = 0

local WAVE_START_TIME = 10.0
local waveTimer = WAVE_START_TIME

local gameOver = false

---------------------
-- Initialize
---------------------
function onInit()
    gameMap = Map:new()
    gameMap:loadLevel("level.eff")
    gameMap:addMapToRenderBatch()

    player = Player:new({ position = Vector2.new(200, 200) })
    player.map = gameMap

    spawnWave()
    Raylib.playMusic("overworld.ogg")

    Raylib.setCameraTarget(player.position)
    Raylib.setCameraOffset({x = WINDOW_WIDTH / 2, y = WINDOW_HEIGHT / 2})
    Raylib.setCameraZoom(2.0)
    Raylib.setCameraRotation(0.0)
end


---------------------
-- Update
---------------------
function onUpdate(dt)
    if gameOver then return end

    waveTimer = waveTimer - dt

    player:update(dt)

    -- update enemies
    for i, enemy in ipairs(enemyList) do
        if waveTimer < 0 then 
            enemy.state = 'chasing' 
        end
        enemy:update(dt)

        if Raylib.checkCollisionRect(enemy.boundingBox, player.boundingBox) then
            player.health = player.health - math.random(enemy.minAttackPower, enemy.maxAttackPower)
            if  player.health < 0 then  player.health = 0 end
            Raylib.playSound("life_lost.ogg")
            table.remove(enemyList, i)
            if player.health <= 0 then -- game over
                Raylib.stopMusic("overworld.ogg")
                Raylib.playSound("game_over.ogg")
                gameOver = true
            end
        end
    end

    -- spawn next wave
    if waveTimer < 0 then 
        spawnWave()
        waveTimer = WAVE_START_TIME
    end

    -- update items
    for i, item in ipairs(itemList) do
        item:update(dt)
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
                    if math.random(1, 10) == 10 then
                        itemList[#itemList + 1] = Potion:new({ position = enemy.position})
                    end
                    table.remove(enemyList, j)
                    slimesKilled = slimesKilled + 1
                end
            end

        end
    end

    -- update player-item collisions
    for i, item in ipairs(itemList) do

        if Raylib.checkCollisionRect(item.boundingBox, player.boundingBox) then
            player.health = player.health + math.random(item.minHealthGain, item.maxHealthGain)
            if player.health > 100 then player.health = 100 end
            Raylib.playSound("item_pickup.ogg")
            table.remove(itemList, i)
        end

    end

    -- throw knives
    if Raylib.isMouseButtonPressed(MOUSE.BUTTON_LEFT) then
       
        if #knifeList < MAX_KNIFE_COUNT then
            local mouse = Vector2.new(Raylib.getMouseWorldPosition()) 
            local knifeDir = mouse - player.position
            knifeDir:normalize()

            knifeList[#knifeList + 1] = Knife:new({
                position = Vector2.new(
                    player.position.x + (player.sourceRec.width / 2),
                    player.position.y + (player.sourceRec.height / 2)
                ),
                direction = knifeDir,
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

    for i = 1, #itemList do
        itemList[i]:render()
    end

    --gameMap:drawLayers(gameMap:layers['Ground'])
    player:render()

    for i = 1, #enemyList do
        enemyList[i]:render()
    end

    for i = 1, #knifeList do
        knifeList[i]:render()
    end

    local textPosition = { x = 10, y = 10 }
    local text = "Slimes killed: " .. slimesKilled
    Raylib.postRenderText(text, textPosition, 25, COLOR.WHITE)

    textPosition.x = 280
    text = "Next wave in: " .. math.ceil(waveTimer)
    Raylib.postRenderText(text, textPosition, 25, COLOR.WHITE)

    -- GAME OVER
    if gameOver then
        -- update player health an extra time
        player.healthBar.width = 0
        Raylib.drawRectangle(player.missingHealthBar, COLOR.RED)
        Raylib.drawRectangle(player.healthBar, COLOR.GREEN)

        -- print game over info
        Raylib.postRenderText("GAME OVER", { x = 350, y = 250 }, 25, COLOR.WHITE)
        Raylib.postRenderText("ESC TO QUIT", { x = 350, y = 370 }, 25, COLOR.WHITE)
    end
end




---------------------
-- Spawn new wave
---------------------
function spawnWave()
    for i, spawner in ipairs(gameMap.spawners) do

        if spawner.id == 6 then
            enemyList[#enemyList + 1] = Enemy:new({map = gameMap, player = player,
            texture = "enemy_slime.png", position = Vector2.new((spawner.x - 1) * gameMap.tileWidth, (spawner.y - 1) * gameMap.tileHeight)})

        elseif spawner.id == 7 then
            enemyList[#enemyList + 1] = Enemy:new({map = gameMap, player = player, chaseSpeed = 80,
            texture = "enemy_slime_red.png", position = Vector2.new((spawner.x - 1) * gameMap.tileWidth, (spawner.y - 1) * gameMap.tileHeight)})
        end

    end 
end
