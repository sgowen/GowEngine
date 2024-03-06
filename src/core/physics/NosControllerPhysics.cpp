//
//  NosControllerPhysics.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/04/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

IMPL_RTTI(NosControllerPhysics, EntityControllerPhysics)

NosControllerPhysics::NosControllerPhysics(Entity* e) : EntityControllerPhysics(e),
_velocityCache(e->velocity()),
_positionCache(e->position()),
_isBodyFacingLeft(false)
{
    createFixtures();
}

NosControllerPhysics::~NosControllerPhysics()
{
    destroyFixtures();
}

void NosControllerPhysics::updateBodyFromPose()
{
    if (_isBodyFacingLeft != _entity->isXFlipped())
    {
        destroyFixtures();
        createFixtures();
    }
    
    Entity::Pose& p =_entity->pose();
    
    for (Bounds& b : _bounds)
    {
        b.updateForPosition(p._position);
    }
}

void NosControllerPhysics::step(float deltaTime)
{
    if (_entity->isExiled())
    {
        return;
    }
    
    Entity::Pose& p =_entity->pose();
    
    if (p._isSlowed)
    {
        static float slowedDeltaTime = deltaTime / 8;
        deltaTime = slowedDeltaTime;
    }
    
    if (!p._isZeroGravity)
    {
        float gravity = _entity->data().getFloat("gravity");
        p._velocity.add(0, gravity * deltaTime);
    }
    _tolerance = fabsf(p._velocity._y * deltaTime * 2);
    p._position.add(p._velocity._x * deltaTime, p._velocity._y * deltaTime);
    for (Bounds& b : _bounds)
    {
        b.updateForPosition(p._position);
    }
    
    p._numGroundContacts = 0;
    
    if (ENGINE_CFG.physicsLoggingEnabled())
    {
        LOG("_velocity: %f, %f", p._velocity._x, p._velocity._y);
        LOG("_position: %f, %f", p._position._x, p._position._y);
    }
}

void NosControllerPhysics::extrapolate(float extrapolation)
{
    Entity::Pose& p =_entity->pose();
    
    _velocityCache = p._velocity;
    _positionCache = p._position;
    
    float gravity = _entity->data().getFloat("gravity");
    p._velocity.add(0, gravity * extrapolation);
    p._position.add(p._velocity._x * extrapolation, p._velocity._y * extrapolation);
}

void NosControllerPhysics::endExtrapolation()
{
    Entity::Pose& p =_entity->pose();
    
    p._velocity = _velocityCache;
    p._position = _positionCache;
}

void NosControllerPhysics::processCollisions(std::vector<Entity*>& entities)
{
    if (_entity->isExiled())
    {
        return;
    }
    
    Entity::Pose& p =_entity->pose();
    
    for (Entity* e : entities)
    {
        if (_entity == e)
        {
            // Don't collide with myself
            continue;
        }
        
        if (e->isExiled())
        {
            // Can't interact with something in exile
            continue;
        }
        
        NosControllerPhysics* epc = e->controllerPhysics<NosControllerPhysics>();
        
        bool t = false;
        bool b = false;
        bool r = false;
        bool l = false;
        
        for (Bounds& myBounds : _bounds)
        {
            if (IS_BIT_SET(myBounds._flags, NFIXF_GROUND_SENSOR))
            {
                continue;
            }
            
            Rektangle& myBoundingBox = myBounds._boundingBox;
            for (Bounds& yourBounds : epc->_bounds)
            {
                Rektangle& yourBoundingBox = yourBounds._boundingBox;
                uint32_t yourFlags = yourBounds._flags;
                if (OverlapTester::doRektanglesOverlap(myBoundingBox, yourBoundingBox))
                {
                    {
                        float mt = myBoundingBox.top();
                        float mb = myBoundingBox.bottom();
                        
                        float mr = myBoundingBox.right();
                        float ml = myBoundingBox.left();
                        
                        float yt = yourBoundingBox.top();
                        float yb = yourBoundingBox.bottom();
                        
                        float yr = yourBoundingBox.right();
                        float yl = yourBoundingBox.left();
                        
                        t = !IS_BIT_SET(yourFlags, NFIXF_IGNORE_BOTTOM) &&
                        crossesBottomEdge(yb, mt, _tolerance) &&
                        mb < yb && (mr > yl || ml < yr);
                        
                        b = p._velocity._y < 0 &&
                        crossesTopEdge(yt, mb, _tolerance) &&
                        mt > yt && (mr > yl || ml < yr);
                        
                        if (t)
                        {
                            p._position.sub(0, mt - yb);
                            p._velocity._y = 0;
                            myBounds.updateForPosition(p._position);
                            if (ENGINE_CFG.physicsLoggingEnabled())
                            {
                                LOG("top");
                            }
                        }
                        else if (b)
                        {
                            p._position.add(0, yt - mb);
                            p._velocity._y = 0;
                            myBounds.updateForPosition(p._position);
                            if (IS_BIT_SET(yourBounds._flags, NFIXF_DAMAGE_TOP))
                            {
                                _entity->message(MSG_DAMAGE, nullptr);
                            }
                            
                            if (ENGINE_CFG.physicsLoggingEnabled())
                            {
                                LOG("bottom");
                            }
                        }
                    }
                    
                    {
                        float mt = myBoundingBox.top();
                        float mb = myBoundingBox.bottom();
                        
                        float mr = myBoundingBox.right();
                        float ml = myBoundingBox.left();
                        
                        float yt = yourBoundingBox.top();
                        float yb = yourBoundingBox.bottom();
                        
                        float yr = yourBoundingBox.right();
                        float yl = yourBoundingBox.left();
                        
                        bool i = isInside(yb, yt, mb, mt);
                        if (ENGINE_CFG.physicsLoggingEnabled())
                        {
                            LOG("isInside: %d", i);
                        }
                        
                        r = !IS_BIT_SET(yourFlags, NFIXF_IGNORE_LEFT) &&
                        mr > yl && ml < yl && i;
                        
                        l = !IS_BIT_SET(yourFlags, NFIXF_IGNORE_RIGHT) &&
                        ml < yr && mr > yr && i;
                        
                        if (r)
                        {
                            p._position.sub(mr - yl, 0);
                            p._velocity._x = 0;
                            myBounds.updateForPosition(p._position);
                            if (ENGINE_CFG.physicsLoggingEnabled())
                            {
                                LOG("right");
                            }
                        }
                        else if (l)
                        {
                            p._position.add(yr - ml, 0);
                            p._velocity._x = 0;
                            myBounds.updateForPosition(p._position);
                            if (ENGINE_CFG.physicsLoggingEnabled())
                            {
                                LOG("left");
                            }
                        }
                    }
                }
            }
        }
        
        for (Bounds& myBounds : _bounds)
        {
            if (!IS_BIT_SET(myBounds._flags, NFIXF_GROUND_SENSOR))
            {
                continue;
            }
            
            myBounds.updateForPosition(p._position);
            Rektangle& myBoundingBox = myBounds._boundingBox;
            for (Bounds& yourBounds : epc->_bounds)
            {
                Rektangle& yourBoundingBox = yourBounds._boundingBox;
                if (b &&
                    OverlapTester::doRektanglesOverlap(myBoundingBox, yourBoundingBox))
                {
                    ++p._numGroundContacts;
                }
            }
        }
    }
}

std::vector<Bounds>& NosControllerPhysics::bounds()
{
    return _bounds;
}

bool NosControllerPhysics::crossesBottomEdge(float yourBottom, float myTop, float tolerance)
{
    return areFloatsPracticallyEqual(myTop, yourBottom) ? false : myTop > yourBottom && myTop < yourBottom + tolerance;
}

bool NosControllerPhysics::crossesTopEdge(float yourTop, float myBottom, float tolerance)
{
    return areFloatsPracticallyEqual(myBottom, yourTop) ? false : myBottom < yourTop && myBottom > yourTop - tolerance;
}

bool NosControllerPhysics::isInside(float yourBottom, float yourTop, float myBottom, float myTop)
{
    return myBottom < yourTop || (myTop > yourBottom && myTop < yourTop);
}

void NosControllerPhysics::createFixtures()
{
    _isBodyFacingLeft = _entity->isXFlipped();
    
    float bodyWidth = _entity->width();
    float bodyHeight = _entity->height();
    
    std::vector<FixtureDef>& fixtures = _entity->entityDef()._fixtures;
    for (std::vector<FixtureDef>::iterator i = fixtures.begin(); i != fixtures.end(); ++i)
    {
        FixtureDef fd = *i;
        if (_isBodyFacingLeft)
        {
            fd._center._x = -fd._center._x;
        }
        
        float centerX = fd._center._x * bodyWidth;
        float centerY = fd._center._y * bodyHeight;
        float halfWidth = bodyWidth * fd._halfWidthFactor;
        float halfHeight = bodyHeight * fd._halfHeightFactor;
        
        _bounds.emplace_back(fd._flags, centerX, centerY, halfWidth, halfHeight);
        Bounds& b = _bounds.back();
        b.updateForPosition(_entity->position());
    }
}

void NosControllerPhysics::destroyFixtures()
{
    _bounds.clear();
}
