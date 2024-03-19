local function isMovementInputAllowed()
    local s = state()

    return s == "IDLE" or s == "RUNNING" or s == "PREPARING_ATTACK" or s == "ATTACKING"
end

local function isAttackInputAllowed()
    local s = state()

    return s == "IDLE" or s == "RUNNING"
end

local function needsStateChangeForMovement()
    local s = state()

    return s == "IDLE"
end

local function needsToResolveNewStateOnceAnimationEnds()
    local s = state()

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
    local s = state()

    if s == "DYING" then
        exile()
    elseif isGrounded() then
        setState("IDLE")
        setVelocityX(0)
    end
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
    local s = state()
    local st = stateTime()

    if s == "PREPARING_ATTACK" and st == 60 then
        setState("ATTACKING")
    elseif s == "ATTACKING" and st == 30 then
        local touchingEntityID = getNetworkUInt32("touchingEntityID")
        if isPlayer(touchingEntityID) then
            sendMessage(touchingEntityID, "MSG_DAMAGE")
        end
    end

    st = stateTime()
    if st >= animationNumFrames() and needsToResolveNewStateOnceAnimationEnds() then
        resolveNewState()
    end
end

function onMessage(message, fromEntityID)
    local s = state()

    if message == "MSG_DANGEROUS_TOUCH" then
        if (isPlayer(fromEntityID)) then
            setNetworkUInt32("touchingEntityID", fromEntityID)
        end
    elseif message == "MSG_NO_TOUCH" then
        if (isPlayer(fromEntityID)) then
            setNetworkUInt32("touchingEntityID", 0)
        end
    elseif message == "MSG_DAMAGE" and s ~= "DYING" then
        local health = getNetworkUInt8("health")
        health = health - 1
        setNetworkUInt8("health", health)
        if health == 0 then
            setState("DYING")
            resetVelocity()
        end
    end
end
