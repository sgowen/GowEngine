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
   local isInputRight = isInputFlagSet("MOVING_RIGHT")
   local speedX = getFloat("speedX")

   local idleState = "IDLE"
   local runningState = "RUNNING"
   local jumpingState = "JUMPING"

   if isVampire() then
      idleState = "VAMPIRE_IDLE"
      runningState = "VAMPIRE_RUNNING"
      jumpingState = "VAMPIRE_JUMPING"
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
   else
      local s = stateStr()
      if s ~= jumpingState and s ~= idleState and isGrounded() then
         setState(idleState)
      end

      setVelocityX(0)
   end
end

local function processJumpInput(inputState)
   local isInputJump = isInputFlagSet("JUMPING")
   local wasInputJump = wasInputFlagSet("JUMPING")

   local numJumpFrames = getUInt("numJumpFrames")
   local speedY = getFloat("speedY")

   local s = stateStr()
   local st = stateTime()

   if s == "JUMPING" then
      if isInputJump and st < numJumpFrames then
         setVelocityY(speedY)
      end
   elseif isGrounded() then
      if isInputJump and not wasInputJump then
         setState("JUMPING")
         setVelocityY(speedY)
      end
   end
end

local function processAttackInput(inputState)
   local isInputAttack = isInputFlagSet("ATTACKING")
   local wasInputAttack = wasInputFlagSet("ATTACKING")

   local s = stateStr()
   local st = stateTime()

   --TODO
end

local function processAbilityInput(inputState)
   local isInputAttack = isInputFlagSet("ATTACKING")
   local wasInputAttack = wasInputFlagSet("ATTACKING")

   local s = stateStr()
   local st = stateTime()

   --TODO
end

local function processWarmingUpInput(inputState)
   local isInputAttack = isInputFlagSet("ATTACKING")
   local wasInputAttack = wasInputFlagSet("ATTACKING")

   local s = stateStr()
   local st = stateTime()

   --TODO
end

local function processSpecialInput(inputState)
   local isInputAttack = isInputFlagSet("ATTACKING")
   local wasInputAttack = wasInputFlagSet("ATTACKING")

   local s = stateStr()
   local st = stateTime()

   --TODO
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
      setState("IDLE")
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

function onInit ()
   -- Empty
end

function processInput (inputState)
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

function onUpdate (numMovesProcessed)
   local s = stateStr()
   local st = stateTime()



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
