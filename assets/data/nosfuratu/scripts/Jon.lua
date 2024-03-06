local function isRabbit()
   local s = stateStr()

   return s == "IDLE" or
   s == "RUNNING" or
   s == "JUMPING" or
   s == "FALLING" or
   s == "DOUBLE_JUMPING" or
   s == "LANDING" or
   s == "DRILL_STOMPING" or
   s == "LANDING_ROLL" or
   s == "WARMING_UP" or
   s == "PUNCHING" or
   s == "DRILLING" or
   s == "ATTEMPTING_TO_TRANSFORM" or
   s == "CANCELING_ATTEMPT_TO_TRANSFORM" or
   s == "REVERTING" or
   s == "RABBIT_DEATH"
end

local function isVampire()
   local s = stateStr()

   return s == "TRANSFORMING" or s == "VAMPIRE_IDLE" or s == "VAMPIRE_RUNNING" or s == "VAMPIRE_JUMPING" or s == "VAMPIRE_DASHING" or s == "VAMPIRE_DOUBLE_JUMPING" or s == "VAMPIRE_GLIDING" or s == "VAMPIRE_FALLING" or s == "VAMPIRE_LANDING" or s == "ATTEMPTING_TO_REVERT" or s == "CANCELING_ATTEMPT_TO_REVERT" or s == "VAMPIRE_DASH_COMPLETED" or s == "VAMPIRE_DEATH"
end

local function isMovementInputAllowed()
   local s = stateStr()

   return s == "IDLE" or
   s == "RUNNING" or
   s == "JUMPING" or
   s == "FALLING" or
   s == "DOUBLE_JUMPING" or
   s == "LANDING" or
   s == "WARMING_UP" or
   s == "VAMPIRE_IDLE" or
   s == "VAMPIRE_RUNNING" or
   s == "VAMPIRE_DOUBLE_JUMPING" or
   s == "VAMPIRE_GLIDING" or
   s == "VAMPIRE_FALLING" or
   s == "VAMPIRE_LANDING"
end

local function isAttackInputAllowed()
   local s = stateStr()

   return s == "IDLE" or
   s == "RUNNING" or
   s == "JUMPING" or
   s == "FALLING" or
   s == "DOUBLE_JUMPING" or
   s == "WARMING_UP"
end

local function isJumpInputAllowed()
   local s = stateStr()

   return s == "IDLE" or
   s == "RUNNING" or
   s == "JUMPING" or
   s == "FALLING" or
   s == "DOUBLE_JUMPING" or
   s == "LANDING" or
   s == "DRILL_STOMPING" or
   s == "LANDING_ROLL" or
   s == "WARMING_UP" or
   s == "VAMPIRE_IDLE" or
   s == "VAMPIRE_RUNNING" or
   s == "VAMPIRE_JUMPING" or
   s == "VAMPIRE_DOUBLE_JUMPING" or
   s == "VAMPIRE_GLIDING" or
   s == "VAMPIRE_FALLING" or
   s == "VAMPIRE_LANDING"
end

local function isAbilityInputAllowed()
   local s = stateStr()

   return s == "IDLE" or
   s == "RUNNING" or
   s == "JUMPING" or
   s == "FALLING" or
   s == "DOUBLE_JUMPING" or
   s == "LANDING" or
   s == "WARMING_UP" or
   s == "VAMPIRE_IDLE" or
   s == "VAMPIRE_RUNNING" or
   s == "VAMPIRE_JUMPING" or
   s == "VAMPIRE_DOUBLE_JUMPING" or
   s == "VAMPIRE_GLIDING" or
   s == "VAMPIRE_FALLING" or
   s == "VAMPIRE_LANDING"
end

local function isWarmingUpInputAllowed()
   local s = stateStr()

   return s == "IDLE"
end

local function needsStateChangeForMovement()
   local s = stateStr()

   return s == "IDLE" or s == "WARMING_UP" or s == "VAMPIRE_IDLE"
end

local function canCurrentStateBeInterruptedByFalling()
   local s = stateStr()

   return s == "IDLE" or
   s == "RUNNING" or
   s == "JUMPING" or
   s == "DOUBLE_JUMPING" or
   s == "LANDING" or
   s == "LANDING_ROLL" or
   s == "WARMING_UP" or
   s == "DRILLING" or
   s == "VAMPIRE_IDLE" or
   s == "VAMPIRE_RUNNING" or
   s == "VAMPIRE_JUMPING" or
   s == "VAMPIRE_DOUBLE_JUMPING" or
   s == "VAMPIRE_LANDING" or
   s == "VAMPIRE_DASH_COMPLETED"
end

local function canCurrentStateBeInterruptedByLanding()
   local s = stateStr()

   return s == "FALLING" or
   s == "VAMPIRE_GLIDING" or
   s == "VAMPIRE_FALLING"
end

local function isSpecialInputAllowed()
   local s = stateStr()

   return s ~= "TRANSFORMING" and s ~= "REVERTING"
end

local function needsToResolveNewStateOnceAnimationEnds()
   local s = stateStr()

   return s == "JUMPING" or
   s == "DOUBLE_JUMPING" or
   s == "LANDING" or
   s == "LANDING_ROLL" or
   s == "PUNCHING" or
   s == "DRILLING" or
   s == "TRANSFORMING" or
   s == "CANCELING_ATTEMPT_TO_TRANSFORM" or
   s == "DRILLING" or
   s == "VAMPIRE_JUMPING" or
   s == "VAMPIRE_LANDING" or
   s == "REVERTING" or
   s == "CANCELING_ATTEMPT_TO_REVERT" or
   s == "VAMPIRE_DASH_COMPLETED" or
   s == "RABBIT_DEATH" or
   s == "VAMPIRE_DEATH"
end

local function isVampire()
   local s = stateStr()

   return s == "TRANSFORMING" or
   s == "VAMPIRE_IDLE" or
   s == "VAMPIRE_RUNNING" or
   s == "VAMPIRE_JUMPING" or
   s == "VAMPIRE_DASHING" or
   s == "VAMPIRE_DOUBLE_JUMPING" or
   s == "VAMPIRE_GLIDING" or
   s == "VAMPIRE_FALLING" or
   s == "VAMPIRE_LANDING" or
   s == "ATTEMPTING_TO_REVERT" or
   s == "CANCELING_ATTEMPT_TO_REVERT" or
   s == "VAMPIRE_DASH_COMPLETED" or
   s == "VAMPIRE_DEATH"
end

local function processMovementInput(inputState)
   local isInputLeft = isInputFlagSet("MOVING_LEFT")
   local wasInputLeft = wasInputFlagSet("MOVING_LEFT")
   local isInputRight = isInputFlagSet("MOVING_RIGHT")
   local wasInputRight = wasInputFlagSet("MOVING_RIGHT")
   local speedX = getFloat("speedX")
   if isVampire() then
          speedX = getFloat("speedVampireX")
   end

   local idleState = "IDLE"
   local runningState = "RUNNING"
   local jumpingState = "JUMPING"
   local landingState = "LANDING"

   if isVampire() then
      idleState = "VAMPIRE_IDLE"
      runningState = "VAMPIRE_RUNNING"
      jumpingState = "VAMPIRE_JUMPING"
      landingState = "VAMPIRE_LANDING"
   end

   if isInputLeft then
      if needsStateChangeForMovement() then
         setState(runningState)
      end

      setVelocityX(-speedX)
      setXFlipped(true)
   elseif isInputRight then
      if needsStateChangeForMovement() then
         setState(runningState)
      end

      setVelocityX(speedX)
      setXFlipped(false)
   elseif wasInputLeft or wasInputRight then
      local s = stateStr()
      if isGrounded() then
         setState(idleState)
      end

      setVelocityX(0)
   end
end

local function processJumpInput(inputState)
   local isInputJump = isInputFlagSet("JUMPING")
   local wasInputJump = wasInputFlagSet("JUMPING")

   local numJumpFrames = getUInt("numJumpFrames")
   local numDoubleJumpFrames = getUInt("numDoubleJumpFrames")
   local speedRabbitY = getFloat("speedY")
   local speedVampireY = getFloat("speedVampireY")
   local speedY = speedRabbitY
   if isVampire() then
      speedY = speedVampireY
   end

   local jumpingState = "JUMPING"
   local doubleJumpingState = "DOUBLE_JUMPING"
   local fallingState = "FALLING"

   if isVampire() then
      jumpingState = "VAMPIRE_JUMPING"
      doubleJumpingState = "VAMPIRE_DOUBLE_JUMPING"
      fallingState = "VAMPIRE_FALLING"
   end

   local s = stateStr()
   local st = stateTime()

   if s == "VAMPIRE_GLIDING" then
      if isInputJump == false then
         setState("VAMPIRE_FALLING")
      elseif isInputJump and wasInputJump then
         setVelocityY(-16)
      end
   elseif s == doubleJumpingState then
      if isInputJump and wasInputJump and st < numDoubleJumpFrames then
         -- TODO, this allows the player to hold, let go, and hold again
         setVelocityY(speedY)
      end
   elseif s == jumpingState then
      if isInputJump then
         if wasInputJump == false then
            setState(doubleJumpingState)
            setStateFlag("FALLING_FLAG_CAN_JUMP_AGAIN", false)
            setVelocityY(speedY)
         elseif st < numJumpFrames then
            setVelocityY(speedY)
         end
      end
   elseif s == fallingState then
      if isInputJump and not wasInputJump and isStateFlagSet("FALLING_FLAG_CAN_JUMP_AGAIN") then
         setState(doubleJumpingState)
         setStateFlag("FALLING_FLAG_CAN_JUMP_AGAIN", false)
         setVelocityY(speedY)
      end
   elseif isGrounded() then
      if isInputJump and not wasInputJump then
         setState(jumpingState)
         setStateFlag("FALLING_FLAG_CAN_JUMP_AGAIN", true)
         setVelocityY(speedY)

         if isVampire() then
            setVelocityX(0)
         end
      end
   end
end

local function processAttackInput(inputState)
   local isInputAttack = isInputFlagSet("ATTACKING")
   local wasInputAttack = wasInputFlagSet("ATTACKING")

   local s = stateStr()
   local st = stateTime()

   if isRabbit() and
   state ~= "PUNCHING" and
   isInputAttack and
   not wasInputAttack then
      setState("PUNCHING")
   end
end

local function processAbilityInput(inputState)
   local isInputAbility = isInputFlagSet("ABILITY")
   local wasInputAbility = wasInputFlagSet("ABILITY")

   local s = stateStr()
   local st = stateTime()

   if isRabbit() then
      if isGrounded() then
         if s ~= "DRILLING" and
         isInputAbility and
         not wasInputAbility then
            setState("DRILLING")
         end
      else
         if s ~= "DRILL_STOMPING" and
         isInputAbility and
         not wasInputAbility then
            setState("DRILL_STOMPING")
            setZeroGravity(true)
            resetVelocity()
         end
      end
   end

   if isVampire() then
      if s ~= "VAMPIRE_DASHING" and
      isInputAbility and
      not wasInputAbility then
         local speedX = getFloat("speedVampireX")

         setState("VAMPIRE_DASHING")
         setZeroGravity(true)
         local dashSpeedX = speedX
         if isXFlipped() then
            dashSpeedX = -speedX
         end
         setVelocityX(dashSpeedX * 2)
         setVelocityY(0)
      end
   end
end

local function processWarmingUpInput(inputState)
   local isInputWarmingUp = isInputFlagSet("WARMING_UP")
   local wasInputWarmingUp = wasInputFlagSet("WARMING_UP")

   local s = stateStr()
   local st = stateTime()

   if isRabbit() and
   state ~= "WARMING_UP" and
   isInputWarmingUp and
   not wasInputWarmingUp then
      setState("WARMING_UP")
   end
end

local function processSpecialInput(inputState)
   local isInputSpecial = isInputFlagSet("SPECIAL")
   local wasInputSpecial = wasInputFlagSet("SPECIAL")

   local s = stateStr()
   local st = stateTime()

   local attemptingToTransformState = "ATTEMPTING_TO_TRANSFORM"
   local cancelingAttemptState = "CANCELING_ATTEMPT_TO_TRANSFORM"

   if isVampire() then
      attemptingToTransformState = "ATTEMPTING_TO_REVERT"
      cancelingAttemptState = "CANCELING_ATTEMPT_TO_REVERT"
   end

   if s == attemptingToTransformState then
      if isInputSpecial == false then
         setState(cancelingAttemptState)
         setSlowed(false)
         setZeroGravity(false)
      end
   else
      if isInputSpecial and
      not wasInputSpecial then
         setState(attemptingToTransformState)
         setSlowed(true)
      end
   end
end

local function resolveNewState()
   local s = stateStr()

   local deathState = "RABBIT_DEATH"
   local idleState = "IDLE"
   local doubleJumpingState = "DOUBLE_JUMPING"
   local fallingState = "FALLING"

   if isVampire() then
      deathState = "VAMPIRE_DEATH"
      idleState = "VAMPIRE_IDLE"
      doubleJumpingState = "VAMPIRE_DOUBLE_JUMPING"
      fallingState = "VAMPIRE_FALLING"
   end

   if s == deathState then
      exile()
   elseif isGrounded() then
      setState(idleState)
      setVelocityX(0)
   elseif getVelocityY() < 0 then
      local canJumpAgain = true
      if s == doubleJumpingState then
         canJumpAgain = false
      end
      setState(fallingState)
      setStateFlag("FALLING_FLAG_CAN_JUMP_AGAIN", canJumpAgain)
   end
end

function onInit()
   -- Empty
end

function processInput(inputState)
   if isMovementInputAllowed() then
      processMovementInput(inputState)
   end

   if isAttackInputAllowed() then
      processAttackInput(inputState)
   end

   if isAbilityInputAllowed() then
      processAbilityInput(inputState)
   end

   if isWarmingUpInputAllowed() then
      processWarmingUpInput(inputState)
   end

   if isSpecialInputAllowed() then
      processSpecialInput(inputState)
   end

   if isJumpInputAllowed() then
      processJumpInput(inputState)
   end
end

function onUpdate(numMovesProcessed)
   local s = stateStr()
   local st = stateTime()

   local shockwaveStartTime = getNetworkUInt16("shockwaveStartTime")
   local isReleasingShockwave = getNetworkBool("isReleasingShockwave")

   local shockwaveStateTime = numMovesProcessed - shockwaveStartTime
   setNetworkUInt16("shockwaveStateTime", shockwaveStateTime)

   if isReleasingShockwave and shockwaveStateTime > 120 then
      setNetworkBool("isReleasingShockwave", false)
   end

   if s == "DRILL_STOMPING" then
      if isGrounded() then
         local speedX = getFloat("speedX")

         setState("LANDING_ROLL")
         local landingRollSpeedX = speedX
         if isXFlipped() then
            landingRollSpeedX = -speedX
         end
         setVelocityX(landingRollSpeedX * 2)
      elseif st == animationNumFramesBeforeFirstLoopingFrame() then
         local gravity = getFloat("gravity")
         setVelocityY(gravity * 2)
         setZeroGravity(false)
      end
   end

   if s == "ATTEMPTING_TO_TRANSFORM" and
   st == animationNumFrames() then
      setState("TRANSFORMING")
      setNetworkBool("isReleasingShockwave", true)
      setNetworkUInt16("shockwaveStartTime", numMovesProcessed)
      setSlowed(false)
      setZeroGravity(false)
   end

   -- Vampire
   if s == "VAMPIRE_DOUBLE_JUMPING" and
   st == animationNumFrames() then
      setState("VAMPIRE_GLIDING")
   end

   if s == "VAMPIRE_DASHING" and
   st == animationNumFrames() then
      setState("VAMPIRE_DASH_COMPLETED")
      setVelocityX(0)
      setZeroGravity(false)
   end

   if s == "ATTEMPTING_TO_REVERT" and
   st == animationNumFrames() then
      setState("REVERTING")
      setNetworkBool("isReleasingShockwave", true)
      setNetworkUInt16("shockwaveStartTime", numMovesProcessed)
      setSlowed(false)
      setZeroGravity(false)
   end

   -- Both Rabbit and Vampire
   if canCurrentStateBeInterruptedByFalling() and
   not isGrounded() and
   getVelocityY() < 0 then
      local doubleJumpingState = "DOUBLE_JUMPING"
      local fallingState = "FALLING"

      if isVampire() then
         doubleJumpingState = "VAMPIRE_DOUBLE_JUMPING"
         fallingState = "VAMPIRE_FALLING"
      end

      local canJumpAgain = true
      if s == doubleJumpingState then
         canJumpAgain = false
      end
      setState(fallingState)
      setStateFlag("FALLING_FLAG_CAN_JUMP_AGAIN", canJumpAgain)
   end

   if canCurrentStateBeInterruptedByLanding() and isGrounded() then
      local landingState = "LANDING"

      if isVampire() then
         landingState = "VAMPIRE_LANDING"
      end

      setState(landingState)
      setStateFlag("FALLING_FLAG_CAN_JUMP_AGAIN", true)
   end

   st = stateTime()
   if st >= animationNumFrames() and needsToResolveNewStateOnceAnimationEnds() then
      resolveNewState()
   end
end

function onMessage(message, fromEntityID)
   local s = stateStr()

   local deathState = "RABBIT_DEATH"

   if isVampire() then
      deathState = "VAMPIRE_DEATH"
   end

   if isMsgDamage() and s ~= deathState then
      setState(deathState)
      resetVelocity()
   end
end
