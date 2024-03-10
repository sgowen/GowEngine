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

bindFramebuffer("behindPlayer")

updateMatrixCenteredOnPlayerForParallaxLayer(0.6)
renderLayersBoundToTexture("sb_001", "texture_001")

updateMatrixCenteredOnPlayerForParallaxLayer(0.8)
renderLayersBoundToTexture("sb_002", "texture_002")

updateMatrixCenteredOnPlayerForParallaxLayer(0.9)
renderLayersBoundToTexture("sb_003", "texture_003")

updateMatrixCenteredOnPlayer()
renderLayersBoundToTexture("sb_004", "texture_004")
renderDynamicEntitiesBoundToTexture("sb_005", "texture_005")

bindFramebuffer("behindPlayerNormals")

updateMatrixCenteredOnPlayerForParallaxLayer(0.6)
spriteBatcherEnd("sb_001", "n_texture_001")

updateMatrixCenteredOnPlayerForParallaxLayer(0.8)
spriteBatcherEnd("sb_002", "n_texture_002")

updateMatrixCenteredOnPlayerForParallaxLayer(0.9)
spriteBatcherEnd("sb_003", "n_texture_003")

updateMatrixCenteredOnPlayer()
spriteBatcherEnd("sb_004", "n_texture_004")
spriteBatcherEnd("sb_005", "n_texture_005")

bindFramebuffer("player")
renderPlayersBoundToTexture("sb_006", "texture_006")

bindFramebuffer("playerNormals")
spriteBatcherEnd("sb_006", "n_texture_006")

bindFramebuffer("inFrontOfPlayer")
renderDynamicEntitiesBoundToTexture("sb_007", "texture_007")
renderDynamicEntitiesBoundToTexture("sb_008", "texture_008")
renderStaticEntitiesBoundToTexture("sb_009", "texture_009")

bindFramebuffer("inFrontOfPlayerNormals")
spriteBatcherEnd("sb_007", "n_texture_007")
spriteBatcherEnd("sb_008", "n_texture_008")
spriteBatcherEnd("sb_009", "n_texture_009")

bindFramebuffer("behindPlayerLights")
renderPlayerLights("behindPlayer", "behindPlayerNormals", 0.15)

bindFramebuffer("playerLights")
renderPlayerLights("player", "playerNormals", 0.1)

bindFramebuffer("inFrontOfPlayerLights")
renderPlayerLights("inFrontOfPlayer", "inFrontOfPlayerNormals", 0.05)

bindFramebuffer("main")
renderFramebuffer("behindPlayerLights")
renderFramebuffer("playerLights")
renderFramebuffer("inFrontOfPlayerLights")

renderPhysicsIfEnabled()

renderGameInfo()

renderFramebufferToScreen("main")
