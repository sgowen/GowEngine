function renderJon (playerID, colorFactorKey, framebufferKey, framebufferToRenderKey)
    local isReleasingShockwave = isReleasingShockwave(playerID)
    local isVampire = isVampire(playerID)
    local shockwaveStateTime = shockwaveStateTime(playerID)
    
    local textureName = textureNameForPlayer(playerID)
    if textureName ~= "" then
        spriteBatcherBegin("main")
        spriteBatcherAddPlayer(playerID)
        configSpriteColorFactor(colorFactorKey)
        spriteBatcherEnd("main", textureName)
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
    
bindFramebuffer("main")
    
renderNosParallaxLayers("main", "background_upper")
renderNosParallaxLayers("main", "background_mid")
renderNosParallaxLayers("main", "background_lower")
    
renderStaticEntitiesBoundToTexture("main", "ground")
renderStaticEntitiesBoundToTexture("main", "platforming_1")

renderJon(4, "BLUE", "player4", "main")
renderJon(3, "GREEN", "player3", "player4")
renderJon(2, "RED", "player2", "player3")
renderJon(1, "ONE", "player1", "player2")
