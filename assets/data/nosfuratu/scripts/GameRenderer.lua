local function isVampire(playerID)
      local s = stateStr(playerID)

   return s == "TRANSFORMING" or s == "VAMPIRE_IDLE" or s == "VAMPIRE_RUNNING" or s == "VAMPIRE_JUMPING" or s == "VAMPIRE_DASHING" or s == "VAMPIRE_DOUBLE_JUMPING" or s == "VAMPIRE_GLIDING" or s == "VAMPIRE_FALLING" or s == "VAMPIRE_LANDING" or s == "ATTEMPTING_TO_REVERT" or s == "CANCELING_ATTEMPT_TO_REVERT" or s == "VAMPIRE_DASH_COMPLETED" or s == "VAMPIRE_DEATH"
end

local function renderJon(playerID, colorFactorKey, framebufferKey, framebufferToRenderKey)
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

renderRepeatingTextureParallaxLayers("main", "background_upper")
renderRepeatingTextureParallaxLayers("main", "background_mid")
renderRepeatingTextureParallaxLayers("main", "background_lower")

renderStaticEntitiesBoundToTexture("main", "ground")
renderStaticEntitiesBoundToTexture("main", "platforming_1")

renderJon(4, "BLUE", "player4", "main")
renderJon(3, "GREEN", "player3", "player4")
renderJon(2, "RED", "player2", "player3")
renderJon(1, "ONE", "player1", "player2")
