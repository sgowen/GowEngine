local function isMovementInputAllowed()
    local s = state()

    return s == "IDLE" or s == "RUNNING" or s == "JUMPING"
end

local function isAttackInputAllowed()
    local s = state()

    return s == "IDLE" or s == "RUNNING" or s == "JUMPING" or s == "PUNCH_1" or s == "PUNCH_2"
end

local function isJumpInputAllowed()
    local s = state()

    return s == "IDLE" or s == "RUNNING" or s == "JUMPING"
end

local function needsStateChangeForMovement()
    local s = state()

    return s == "IDLE"
end

local function needsToResolveNewStateOnceAnimationEnds()
    local s = state()

    return s == "JUMPING" or s == "PUNCH_1" or s == "PUNCH_2" or s == "PUNCH_3"
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
        local s = state()
        if s ~= "JUMPING" and s ~= "IDLE" and isGrounded() then
            setState("IDLE")
        end

        setVelocityX(0)
    end
end

local function processJumpInput(inputState)
    local isInputJump = isInputFlagSet("JUMPING")
    local wasInputJump = wasInputFlagSet("JUMPING")

    local numJumpFrames = getUInt("numJumpFrames")
    local speedY = getFloat("speedY")

    local s = state()
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

    local s = state()
    local st = stateTime()

    if isInputAttack and not wasInputAttack then
        if s == "PUNCH_2" and st >= 12 then
            setState("PUNCH_3")
        elseif s == "PUNCH_1" and st >= 12 then
            setState("PUNCH_2")
        elseif s ~= "PUNCH_1" then
            setState("PUNCH_1")
        end
    end
end

local function resolveNewState()
    local s = state()

    if isGrounded() then
        setState("IDLE")
        setVelocityX(0)
    end
end

function onInit()
    -- Empty
end

function processInput(inputState)
    if isMovementInputAllowed() then
        processMovementInput(inputState)
    end

    if isJumpInputAllowed() then
        processJumpInput(inputState)
    end

    if isAttackInputAllowed() then
        processAttackInput(inputState)
    end
end

function onUpdate(numMovesProcessed)
    local s = state()
    local st = stateTime()

    local isDeliveringDmgPunch1 = s == "PUNCH_1" and st == 6
    local isDeliveringDmgPunch2 = s == "PUNCH_2" and st == 6
    local isDeliveringDmgPunch3 = s == "PUNCH_3" and st == 6

    if isDeliveringDmgPunch1 or isDeliveringDmgPunch2 or isDeliveringDmgPunch3 then
        local touchingEntityID = getNetworkUInt32("touchingEntityID")
        sendMsgDamage(touchingEntityID)
        if isDeliveringDmgPunch3 then
            sendMsgDamage(touchingEntityID)
            sendMsgDamage(touchingEntityID)
            sendMsgDamage(touchingEntityID)
            sendMsgDamage(touchingEntityID)
        end
    end

    st = stateTime()
    if st >= animationNumFrames() and needsToResolveNewStateOnceAnimationEnds() then
        resolveNewState()
    end
end

function onMessage(message, fromEntityID)
    local s = state()

    if isMsgDangerousTouch() then
        setNetworkUInt32("touchingEntityID", fromEntityID)
    elseif isMsgNoTouch() then
        setNetworkUInt32("touchingEntityID", 0)
    elseif isMsgDamage() then
        exile()
    end
end
