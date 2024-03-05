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
renderLight("behindPlayer", "behindPlayerNormals", 0.15)
    
bindFramebuffer("playerLights")
renderLight("player", "playerNormals", 0.1)
    
bindFramebuffer("inFrontOfPlayerLights")
renderLight("inFrontOfPlayer", "inFrontOfPlayerNormals", 0.05)
    
bindFramebuffer("main")
    
renderFramebuffer("behindPlayerLights", "framebuffer")
renderFramebuffer("playerLights", "framebuffer")
renderFramebuffer("inFrontOfPlayerLights", "framebuffer")
