local touchingEntityID = networkUInt32Field("touchingEntityID")
setInput("EXECUTING_ATTACK", isPlayer(touchingEntityID))

local targetID = findTargetWithinDistance(16.0)

if targetID > 0 then
    setInput("MOVING_RIGHT", entityPositionX(targetID) > myPositionX())
    setInput("MOVING_LEFT", entityPositionX(targetID) < myPositionX())
else
    setInput("MOVING_RIGHT", false)
    setInput("MOVING_LEFT", false)
    setInput("EXECUTING_ATTACK", false)
end
