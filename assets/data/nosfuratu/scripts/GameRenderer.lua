local function updatePlayerInfoText(playerID, textKey)
    if doesPlayerExist(playerID) then
        if playerID ~= 1 and networkIsPlayerIDLocal(playerID) and networkIsPlayerIDLocal(1) then
            setText(textKey, string.format("%d|%s [.] to drop", playerID, playerName(playerID)))
        else
            setText(textKey, string.format("%d|%s", playerID, playerName(playerID)))
        end
    elseif playerID == 1 then
        setText("player1Info", "1|Joining...")
    elseif playerID == 2 then
        setText(textKey, "2|Connect gamepad or use arrow keys...")
    else
        setText(textKey, string.format("%d|Maybe someone will join...?", playerID))
    end
end

local function renderGameInfo()
    setText("fps", string.format("FPS %d", fps()))
    setText("rtt", string.format("RTT %d ms", networkAvgRoundTripTime()))
    setText("rollbackFrames", string.format("Rollback frames %d", numRollbackFrames()))
    setText("inBps", string.format(" In %d Bps", networkBytesReceivedPerSecond()))
    setText("outBps", string.format("Out %d Bps", networkBytesSentPerSecond()))

    updatePlayerInfoText(4, "player4Info")
    updatePlayerInfoText(3, "player3Info")
    updatePlayerInfoText(2, "player2Info")
    updatePlayerInfoText(1, "player1Info")

    useMatrix("hud")
    configShader("sprite")
    configTextColorFactor("RED")
    renderTextViews()
end

local function isVampire(playerID)
    local s = playerState(playerID)

    return s == "TRANSFORMING" or s == "VAMPIRE_IDLE" or s == "VAMPIRE_RUNNING" or s == "VAMPIRE_JUMPING" or s == "VAMPIRE_DASHING" or s == "VAMPIRE_DOUBLE_JUMPING" or s == "VAMPIRE_GLIDING" or s == "VAMPIRE_FALLING" or s == "VAMPIRE_LANDING" or s == "ATTEMPTING_TO_REVERT" or s == "CANCELING_ATTEMPT_TO_REVERT" or s == "VAMPIRE_DASH_COMPLETED" or s == "VAMPIRE_DEATH"
end

local function renderJon(playerID, colorFactorKey, framebufferKey, framebufferToRenderKey)
    local isReleasingShockwave = playerNetworkBool(playerID, "isReleasingShockwave")
    local isVampire = isVampire(playerID)
    local shockwaveStateTime = playerNetworkUInt16(playerID, "shockwaveStateTime")

    if doesPlayerExist(playerID) then
        local spriteBatcher = "main"

        spriteBatcherBegin(spriteBatcher)
        spriteBatcherAddPlayer(playerID)

        local textureName = textureNameForPlayer(playerID)
        configSpriteColorFactor(colorFactorKey)
        spriteBatcherEnd(spriteBatcher, textureName)
    end

    bindFramebuffer(framebufferKey)

    if isReleasingShockwave then
        renderFramebufferWithShockwave(framebufferToRenderKey, playerPositionX(playerID), playerPositionY(playerID), shockwaveStateTime, isVampire)
    else
        if isVampire then
            renderFramebuffer(framebufferToRenderKey, "framebufferWithTint")
        else
            renderFramebuffer(framebufferToRenderKey, "framebuffer")
        end
    end
end

updateMatrixCenteredOnPlayer()

local framebuffer = "main"
local spriteBatcher = "main"

bindFramebuffer(framebuffer)

renderParallaxLayersBoundToRepeatingTexture(spriteBatcher, "background_upper")
renderParallaxLayersBoundToRepeatingTexture(spriteBatcher, "background_mid")
renderParallaxLayersBoundToRepeatingTexture(spriteBatcher, "background_lower")

renderStaticEntitiesBoundToTexture(spriteBatcher, "ground")
renderStaticEntitiesBoundToTexture(spriteBatcher, "platforming_1")

local framebufferP4 = "player4"
local framebufferP3 = "player3"
local framebufferP2 = "player2"
local framebufferP1 = "player1"

local colorFactorP4 = "BLUE"
local colorFactorP3 = "GREEN"
local colorFactorP2 = "RED"
local colorFactorP1 = "ONE"

renderJon(4, colorFactorP4, framebufferP4, framebuffer)
renderJon(3, colorFactorP3, framebufferP3, framebufferP4)
renderJon(2, colorFactorP2, framebufferP2, framebufferP3)
renderJon(1, colorFactorP1, framebufferP1, framebufferP2)

renderPhysicsIfEnabled()
renderGameInfo()

renderToScreen()
