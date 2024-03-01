//
//  NosRenderer.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/1/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

void NosRenderer::render(Renderer& r, World& w, Entity* controlledPlayer, float scale)
{
    if (controlledPlayer != nullptr)
    {
        r.updateMatrixCenteredOnEntity(controlledPlayer, w.rightEdge(), w.topEdge(), scale);
    }
    
    r.bindFramebuffer();
    
    r.renderNosParallaxLayers(w.getLayers(), "background_upper");
    r.renderNosParallaxLayers(w.getLayers(), "background_mid");
    r.renderNosParallaxLayers(w.getLayers(), "background_lower");
    
    std::vector<Entity*> platformingEntities;
    r.spriteBatcherBegin();
    for (Entity* e : w.getStaticEntities())
    {
        // TODO, this isn't the best code you know
        std::string name = e->entityDef()._keyName;
        bool isPlatformingEntity = name == "P001" || name == "P002" || name == "P003"|| name == "P004";
        if (isPlatformingEntity)
        {
            platformingEntities.push_back(e);
        }
        else
        {
            r.spriteBatcherAddEntity(e);
        }
    }
    r.spriteBatcherEnd("ground");
    
    r.spriteBatcherBegin();
    for (Entity* e : platformingEntities)
    {
        r.spriteBatcherAddEntity(e);
    }
    r.spriteBatcherEnd("platforming_1");
    
    static float frameRate = static_cast<float>(ENGINE_CFG.frameRate());
    
    bool isReleasingShockwavePlayer4 = false;
    bool isVampirePlayer4 = false;
    uint16_t shockwaveStateTimePlayer4 = 0;
    Entity* player4 = w.getPlayer(4);
    if (player4 != nullptr &&
        !player4->isExiled())
    {
        r.spriteBatcherBegin();
        r.spriteBatcherAddEntity(player4);
        std::string textureRegionKey = player4->renderController()->getTextureMapping();
        std::string textureForRegionKey = ASSETS_MGR.textureForRegionKey(textureRegionKey);
        r.spriteBatcherEnd(textureForRegionKey, "main", "sprite", "main", Color::BLUE);
        
        JonController* player = player4->controller<JonController>();
        isReleasingShockwavePlayer4 = player->isReleasingShockwave();
        isVampirePlayer4 = player->isVampire();
        shockwaveStateTimePlayer4 = player->shockwaveStateTime();
    }
    
    r.bindFramebuffer("player4");
    if (isReleasingShockwavePlayer4)
    {
        r.renderFramebufferWithShockwave("main", player4->position()._x, player4->position()._y, shockwaveStateTimePlayer4 * frameRate, isVampirePlayer4);
    }
    else
    {
        r.renderFramebuffer("main", isVampirePlayer4 ? "framebufferWithTint" : "framebuffer");
    }
    
    bool isReleasingShockwavePlayer3 = false;
    bool isVampirePlayer3 = false;
    uint16_t shockwaveStateTimePlayer3 = 0;
    Entity* player3 = w.getPlayer(3);
    if (player3 != nullptr &&
        !player3->isExiled())
    {
        r.spriteBatcherBegin();
        r.spriteBatcherAddEntity(player3);
        std::string textureRegionKey = player3->renderController()->getTextureMapping();
        std::string textureForRegionKey = ASSETS_MGR.textureForRegionKey(textureRegionKey);
        r.spriteBatcherEnd(textureForRegionKey, "main", "sprite", "main", Color::GREEN);
        
        JonController* player = player3->controller<JonController>();
        isReleasingShockwavePlayer3 = player->isReleasingShockwave();
        isVampirePlayer3 = player->isVampire();
        shockwaveStateTimePlayer3 = player->shockwaveStateTime();
    }
    
    r.bindFramebuffer("player3");
    if (isReleasingShockwavePlayer3)
    {
        r.renderFramebufferWithShockwave("player4", player3->position()._x, player3->position()._y, shockwaveStateTimePlayer3 * frameRate, isVampirePlayer3);
    }
    else
    {
        r.renderFramebuffer("player4", isVampirePlayer3 ? "framebufferWithTint" : "framebuffer");
    }
    
    bool isReleasingShockwavePlayer2 = false;
    bool isVampirePlayer2 = false;
    uint16_t shockwaveStateTimePlayer2 = 0;
    Entity* player2 = w.getPlayer(2);
    if (player2 != nullptr &&
        !player2->isExiled())
    {
        r.spriteBatcherBegin();
        r.spriteBatcherAddEntity(player2);
        std::string textureRegionKey = player2->renderController()->getTextureMapping();
        std::string textureForRegionKey = ASSETS_MGR.textureForRegionKey(textureRegionKey);
        r.spriteBatcherEnd(textureForRegionKey, "main", "sprite", "main", Color::RED);
        
        JonController* player = player2->controller<JonController>();
        isReleasingShockwavePlayer2 = player->isReleasingShockwave();
        isVampirePlayer2 = player->isVampire();
        shockwaveStateTimePlayer2 = player->shockwaveStateTime();
    }
    
    r.bindFramebuffer("player2");
    if (isReleasingShockwavePlayer2)
    {
        r.renderFramebufferWithShockwave("player3", player2->position()._x, player2->position()._y, shockwaveStateTimePlayer2 * frameRate, isVampirePlayer2);
    }
    else
    {
        r.renderFramebuffer("player3", isVampirePlayer2 ? "framebufferWithTint" : "framebuffer");
    }
    
    bool isReleasingShockwavePlayer1 = false;
    bool isVampirePlayer1 = false;
    uint16_t shockwaveStateTimePlayer1 = 0;
    Entity* player1 = w.getPlayer(1);
    if (player1 != nullptr &&
        !player1->isExiled())
    {
        r.spriteBatcherBegin();
        r.spriteBatcherAddEntity(player1);
        std::string textureRegionKey = player1->renderController()->getTextureMapping();
        std::string textureForRegionKey = ASSETS_MGR.textureForRegionKey(textureRegionKey);
        r.spriteBatcherEnd(textureForRegionKey);
        
        JonController* player = player1->controller<JonController>();
        isReleasingShockwavePlayer1 = player->isReleasingShockwave();
        isVampirePlayer1 = player->isVampire();
        shockwaveStateTimePlayer1 = player->shockwaveStateTime();
    }
    
    r.bindFramebuffer("player1");
    if (isReleasingShockwavePlayer1)
    {
        r.renderFramebufferWithShockwave("player2", player1->position()._x, player1->position()._y, shockwaveStateTimePlayer1 * frameRate, isVampirePlayer1);
    }
    else
    {
        r.renderFramebuffer("player2", isVampirePlayer1 ? "framebufferWithTint" : "framebuffer");
    }
}
