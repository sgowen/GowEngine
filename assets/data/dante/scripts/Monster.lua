local function isMovementInputAllowed()
    s = state()
            LOG(s)
        
    return s == "IDLE" or s == "RUNNING" or s == "PREPARING_ATTACK" or s == "ATTACKING"
end

local function isAttackInputAllowed()
    s = state()
        LOG(s)
        
    return s == "IDLE" or s == "RUNNING"
end

local function processMovementInput(inputState)
        LOG("processMovementInput")
end

local function processAttackInput(inputState)
        LOG("processAttackInput")
end

function onInit ()
        LOG("onInit")
end

function processInput (inputState)
    if isMovementInputAllowed() then
        processMovementInput(inputState)
    end
    
    if isAttackInputAllowed() then
        processAttackInput(inputState)
    end
end

function onUpdate (numMovesProcessed)
        LOG("onUpdate")
end

function onMessage (message, fromEntityID)
        LOG("onMessage")
end
