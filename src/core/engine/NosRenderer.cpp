//
//  NosRenderer.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/1/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

// TODO, figure out what this would have to look like in Lua
static void renderJon(Renderer& r, Entity* jon, Color colorFactor, std::string framebufferKey, std::string framebufferToRenderKey)
{
    bool isReleasingShockwave = false;
    bool isVampire = false;
    uint16_t shockwaveStateTime = 0;
    
    if (jon != nullptr &&
        !jon->isExiled())
    {
        std::string textureRegionKey = jon->renderController()->getTextureMapping();
        std::string textureForRegionKey = ASSETS_MGR.textureForRegionKey(textureRegionKey);
        
        r.spriteBatcherBegin("main");
        r.spriteBatcherAddEntity(jon);
        r.configSpriteColorFactor(colorFactor);
        r.spriteBatcherEnd("main", textureForRegionKey);
        
        JonController* jonC = jon->controller<JonController>();
        isReleasingShockwave = jonC->isReleasingShockwave();
        isVampire = jonC->isVampire();
        shockwaveStateTime = jonC->shockwaveStateTime();
    }
    
    r.bindFramebuffer(framebufferKey);
    if (isReleasingShockwave)
    {
        r.renderFramebufferWithShockwave(framebufferToRenderKey, jon->position()._x, jon->position()._y, shockwaveStateTime, isVampire);
    }
    else
    {
        r.renderFramebuffer(framebufferToRenderKey, isVampire ? "framebufferWithTint" : "framebuffer");
    }
}

void NosRenderer::render(Renderer& r, World& w)
{
    r.updateMatrixCenteredOnPlayer();
    
    r.bindFramebuffer("main");
    
    r.renderNosParallaxLayers("main", "background_upper", w.getLayers());
    r.renderNosParallaxLayers("main", "background_mid", w.getLayers());
    r.renderNosParallaxLayers("main", "background_lower", w.getLayers());
    
    r.renderEntitiesBoundToTexture("main", "ground", w.getStaticEntities());
    r.renderEntitiesBoundToTexture("main", "platforming_1", w.getStaticEntities());
    
    renderJon(r, w.getPlayer(4), Color::BLUE, "player4", "main");
    renderJon(r, w.getPlayer(3), Color::GREEN, "player3", "player4");
    renderJon(r, w.getPlayer(2), Color::RED, "player2", "player3");
    renderJon(r, w.getPlayer(1), Color::ONE, "player1", "player2");
}
