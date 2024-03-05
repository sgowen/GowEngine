local touchingEntityID = getNetworkUInt32("touchingEntityID")
setInput("ATTACKING", isPlayer(touchingEntityID))

local targetID = findTargetWithinDistance(16.0)

if targetID > 0 then
    setInput("MOVING_RIGHT", myPositionX() < entityPositionX(targetID))
    setInput("MOVING_LEFT", myPositionX() > entityPositionX(targetID))
else
    setInput("MOVING_RIGHT", false)
    setInput("MOVING_LEFT", false)
    setInput("ATTACKING", false)
end
