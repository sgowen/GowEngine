//
//  DanteRenderer.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/1/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

void DanteRenderer::render(Renderer& r, World& w, Entity* controlledPlayer, float scale)
{
    // TODO, in the proceeding rendering code, consider using multiple matrices instead of changing
    // the "main" one over and over again
    
    r.bindFramebuffer("behindPlayer");
    
    r.updateMatrixCenteredOnEntityForParallaxLayer(controlledPlayer, 0.6f,  w.rightEdge(), w.topEdge(), scale);
    r.renderEntitiesBoundToTexture(w.getLayers(), "texture_001", "sb_001");
    
    r.updateMatrixCenteredOnEntityForParallaxLayer(controlledPlayer, 0.8f,  w.rightEdge(), w.topEdge(), scale);
    r.renderEntitiesBoundToTexture(w.getLayers(), "texture_002", "sb_002");
    
    r.updateMatrixCenteredOnEntityForParallaxLayer(controlledPlayer, 0.9f,  w.rightEdge(), w.topEdge(), scale);
    r.renderEntitiesBoundToTexture(w.getLayers(), "texture_003", "sb_003");
    
    r.updateMatrixCenteredOnEntity(controlledPlayer, w.rightEdge(), w.topEdge(), scale);
    
    r.renderEntitiesBoundToTexture(w.getLayers(), "texture_004", "sb_004");
    r.renderEntitiesBoundToTexture(w.getDynamicEntities(), "texture_005", "sb_005");
    
    r.bindFramebuffer("behindPlayerNormals");
    
    r.updateMatrixCenteredOnEntityForParallaxLayer(controlledPlayer, 0.6f,  w.rightEdge(), w.topEdge(), scale);
    r.spriteBatcherEnd("n_texture_001", "main", "sprite", "sb_001");
    
    r.updateMatrixCenteredOnEntityForParallaxLayer(controlledPlayer, 0.8f,  w.rightEdge(), w.topEdge(), scale);
    r.spriteBatcherEnd("n_texture_002", "main", "sprite", "sb_002");
    
    r.updateMatrixCenteredOnEntityForParallaxLayer(controlledPlayer, 0.9f,  w.rightEdge(), w.topEdge(), scale);
    r.spriteBatcherEnd("n_texture_003", "main", "sprite", "sb_003");
    
    r.updateMatrixCenteredOnEntity(controlledPlayer, w.rightEdge(), w.topEdge(), scale);
    
    r.spriteBatcherEnd("n_texture_004", "main", "sprite", "sb_004");
    r.spriteBatcherEnd("n_texture_005", "main", "sprite", "sb_005");
    
    r.bindFramebuffer("player");
    r.renderEntitiesBoundToTexture(w.getPlayers(), "texture_006", "sb_006");
    
    r.bindFramebuffer("playerNormals");
    r.spriteBatcherEnd("n_texture_006", "main", "sprite", "sb_006");
    
    r.bindFramebuffer("inFrontOfPlayer");
    r.renderEntitiesBoundToTexture(w.getDynamicEntities(), "texture_007", "sb_007");
    r.renderEntitiesBoundToTexture(w.getDynamicEntities(), "texture_008", "sb_008");
    r.renderEntitiesBoundToTexture(w.getStaticEntities(), "texture_009", "sb_009");
    
    r.bindFramebuffer("inFrontOfPlayerNormals");
    r.spriteBatcherEnd("n_texture_007", "main", "sprite", "sb_007");
    r.spriteBatcherEnd("n_texture_008", "main", "sprite", "sb_008");
    r.spriteBatcherEnd("n_texture_009", "main", "sprite", "sb_009");
    
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
