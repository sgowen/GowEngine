//
//  DanteRenderer.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/1/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

void DanteRenderer::render(Renderer& r, World& w)
{
    r.bindFramebuffer("behindPlayer");
    
    r.updateMatrixCenteredOnPlayerForParallaxLayer(0.6f);
    r.renderEntitiesBoundToTexture("sb_001", "texture_001", w.getLayers());
    
    r.updateMatrixCenteredOnPlayerForParallaxLayer(0.8f);
    r.renderEntitiesBoundToTexture("sb_002", "texture_002", w.getLayers());
    
    r.updateMatrixCenteredOnPlayerForParallaxLayer(0.9f);
    r.renderEntitiesBoundToTexture("sb_003", "texture_003", w.getLayers());
    
    r.updateMatrixCenteredOnPlayer();
    r.renderEntitiesBoundToTexture("sb_004", "texture_004", w.getLayers());
    r.renderEntitiesBoundToTexture("sb_005", "texture_005", w.getDynamicEntities());
    
    r.bindFramebuffer("behindPlayerNormals");
    
    r.updateMatrixCenteredOnPlayerForParallaxLayer(0.6f);
    r.spriteBatcherEnd("sb_001", "n_texture_001");
    
    r.updateMatrixCenteredOnPlayerForParallaxLayer(0.8f);
    r.spriteBatcherEnd("sb_002", "n_texture_002");
    
    r.updateMatrixCenteredOnPlayerForParallaxLayer(0.9f);
    r.spriteBatcherEnd("sb_003", "n_texture_003");
    
    r.updateMatrixCenteredOnPlayer();
    
    r.spriteBatcherEnd("sb_004", "n_texture_004");
    r.spriteBatcherEnd("sb_005", "n_texture_005");
    
    r.bindFramebuffer("player");
    r.renderEntitiesBoundToTexture("sb_006", "texture_006", w.getPlayers());
    
    r.bindFramebuffer("playerNormals");
    r.spriteBatcherEnd("sb_006", "n_texture_006");
    
    r.bindFramebuffer("inFrontOfPlayer");
    r.renderEntitiesBoundToTexture("sb_007", "texture_007", w.getDynamicEntities());
    r.renderEntitiesBoundToTexture("sb_008", "texture_008", w.getDynamicEntities());
    r.renderEntitiesBoundToTexture("sb_009", "texture_009", w.getStaticEntities());
    
    r.bindFramebuffer("inFrontOfPlayerNormals");
    r.spriteBatcherEnd("sb_007", "n_texture_007");
    r.spriteBatcherEnd("sb_008", "n_texture_008");
    r.spriteBatcherEnd("sb_009", "n_texture_009");
    
    r.bindFramebuffer("behindPlayerLights");
    r.renderLight("behindPlayer", "behindPlayerNormals", 0.15f, w.getPlayers());
    
    r.bindFramebuffer("playerLights");
    r.renderLight("player", "playerNormals", 0.1f, w.getPlayers());
    
    r.bindFramebuffer("inFrontOfPlayerLights");
    r.renderLight("inFrontOfPlayer", "inFrontOfPlayerNormals", 0.05f, w.getPlayers());
    
    r.bindFramebuffer("main");
    
    r.renderFramebuffer("behindPlayerLights");
    r.renderFramebuffer("playerLights");
    r.renderFramebuffer("inFrontOfPlayerLights");
}
