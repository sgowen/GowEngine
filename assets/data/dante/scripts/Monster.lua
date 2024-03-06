local function isMovementInputAllowed()
    s = stateStr()
    
    return s == "IDLE" or s == "RUNNING" or s == "PREPARING_ATTACK" or s == "ATTACKING"
end

local function isAttackInputAllowed()
    s = stateStr()
    
    return s == "IDLE" or s == "RUNNING"
end

local function needsStateChangeForMovement()
    s = stateStr()
    
    return s == "IDLE"
end

local function needsToResolveNewStateOnceAnimationEnds()
    s = stateStr()
    
    return s == "DYING" or s == "ATTACKING"
end

local function processMovementInput(inputState)
    local isInputLeft = isInputFlagSet("MOVING_LEFT")
    local isInputRight = isInputFlagSet("MOVING_RIGHT")
    local speedX = getFloat("speedX")
    
    if isInputLeft then
        if needsStateChangeForMovement() then
            setState("RUNNING")
        end
        
        setVelocityX(-speedX)
        setXFlipped(true)
    elseif isInputRight then
        if needsStateChangeForMovement() then
            setState("RUNNING")
        end
        
        setVelocityX(speedX)
        setXFlipped(false)
    else
        if isGrounded() then
            setState("IDLE")
        end
        
        setVelocityX(0)
    end
end

local function processAttackInput(inputState)
    local isInputAttack = isInputFlagSet("ATTACKING")
    local wasInputAttack = wasInputFlagSet("ATTACKING")
            
    if isInputAttack and not wasInputAttack then
        setState("PREPARING_ATTACK")
    end
end

local function resolveNewState()
    s = stateStr()
        
    if s == "DYING" then
        exile()
    elseif isGrounded() then
       setState("IDLE")
       setVelocityX(0)
    end
end

function onInit()
    setNetworkUInt8("health", 7)
end

function processInput(inputState)
    if isMovementInputAllowed() then
        processMovementInput(inputState)
    end
    
    if isAttackInputAllowed() then
        processAttackInput(inputState)
    end
end

function onUpdate(numMovesProcessed)
    s = stateStr()
    st = stateTime()
    
    if s == "PREPARING_ATTACK" and st == 60 then
        setState("ATTACKING")
    elseif s == "ATTACKING" and st == 30 then
        local touchingEntityID = getNetworkUInt32("touchingEntityID")
        if isPlayer(touchingEntityID) then
            sendMsgDamage(touchingEntityID)
        end
    end
    
    st = stateTime()
    if st >= animationNumFrames() and needsToResolveNewStateOnceAnimationEnds() then
        resolveNewState()
    end
end

function onMessage(message, fromEntityID)
    s = stateStr()
    
    if isMsgDangerousTouch() then
        if (isPlayer(fromEntityID)) then
            setNetworkUInt32("touchingEntityID", fromEntityID)
        end
    elseif isMsgNoTouch() then
        if (isPlayer(fromEntityID)) then
            setNetworkUInt32("touchingEntityID", 0)
        end
    elseif isMsgDamage() and s ~= "DYING" then
        health = getNetworkUInt8("health")
        health = health - 1
        setNetworkUInt8("health", health)
        if health == 0 then
            setState("DYING")
            resetVelocity()
        end
    end
end
