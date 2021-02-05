//
//  Entity.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 8/3/15.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <box2d/b2_math.h>

#include "Vector2.hpp"

#include <stdint.h>
#include <string>
#include <map>
#include <vector>

class EntityController;
class EntityNetworkController;
class b2World;
class b2Body;
class b2Fixture;
class b2Contact;

enum FixtureFlags
{
    FIXF_BOX =            1 << 0,
    FIXF_SENSOR =         1 << 1,
    FIXF_GROUND_CONTACT = 1 << 2,
    FIXF_CIRCLE =         1 << 3
};

struct FixtureDef
{
    std::vector<Vector2> _vertices;
    Vector2 _center;
    float _restitution;
    float _density;
    float _friction;
    int _flags;
};

enum BodyFlags
{
    BODF_STATIC =         1 << 0,
    BODF_DYNAMIC =        1 << 1,
    BODF_FIXED_ROTATION = 1 << 2
};

struct EntityDef
{
    std::string _name;
    std::string _keyName;
    uint32_t _entityID;
    uint32_t _key;
    std::string _controller;
    std::string _networkController;
    std::map<int, std::string> _textureMappings;
    std::map<int, int> _soundMappings;
    std::map<int, std::vector<int> > _soundRandomMappings;
    std::vector<FixtureDef> _fixtures;
    int _bodyFlags;
    int _x;
    int _y;
    int _width;
    int _height;
    bool _server;
};

class Entity
{
    friend class EntityController;
    friend class EntityNetworkController;
    friend class EntityPhysicsController;
    
public:
    enum ReadStateFlag
    {
        RSTF_POSE =  1 << 0,
        RSTF_STATE = 1 << 1
    };
    
    Entity(EntityDef ed);
    ~Entity();
    
    void update();
    bool shouldCollide(Entity* e, b2Fixture* fixtureA, b2Fixture* fixtureB);
    void handleBeginContact(Entity* e, b2Fixture* fixtureA, b2Fixture* fixtureB);
    void handleEndContact(Entity* e, b2Fixture* fixtureA, b2Fixture* fixtureB);
    void initPhysics(b2World& world);
    void deinitPhysics();
    void updatePoseFromBody();
    void updateBodyFromPose();
    EntityDef& getEntityDef();
    EntityController* getController();
    EntityNetworkController* getNetworkController();
    uint16_t getStateTime();
    b2Body* getBody();
    void setPosition(Vector2 position);
    void setPosition(float x, float y);
    const Vector2& getPosition();
    void setVelocity(Vector2 velocity);
    void setVelocity(float x, float y);
    const Vector2& getVelocity();
    float getWidth();
    float getHeight();
    void setAngle(float angle);
    float getAngle();
    const uint32_t getID();
    bool isGrounded();
    void requestDeletion();
    bool isRequestingDeletion();
    bool isServer();
    bool isFacingLeft();
    std::string getTextureMapping();
    std::string getTextureMapping(uint8_t state);
    int getSoundMapping(int state);
    bool isFixedRotation() const;
    
    struct Pose
    {
        Vector2 _velocity;
        Vector2 _position;
        float _angle;
        uint8_t _numGroundContacts;
        bool _isFacingLeft;
        
        Pose(float x, float y) :
        _velocity(VECTOR2_ZERO),
        _position(x, y),
        _angle(0),
        _numGroundContacts(0),
        _isFacingLeft(false)
        {
            // Empty
        }
        
        friend bool operator==(Pose& a, Pose& b)
        {
            return
            a._velocity          == b._velocity &&
            a._position          == b._position &&
            a._angle             == b._angle &&
            a._numGroundContacts == b._numGroundContacts &&
            a._isFacingLeft      == b._isFacingLeft;
        }
        
        friend bool operator!=(Pose& a, Pose& b)
        {
            return !(a == b);
        }
    };
    Pose& pose();
    Pose& poseNetworkCache();
    
    struct State
    {
        uint16_t _stateTime;
        uint8_t _state;
        uint8_t _stateFlags;
        
        State()
        {
            _stateTime = 0;
            _state = 0;
            _stateFlags = 0;
        }
        
        friend bool operator==(State& a, State& b)
        {
            return
            a._stateTime         == b._stateTime &&
            a._state             == b._state &&
            a._stateFlags        == b._stateFlags;
        }
        
        friend bool operator!=(State& a, State& b)
        {
            return !(a == b);
        }
    };
    State& state();
    State& stateNetworkCache();
    
private:
    EntityDef _entityDef;
    EntityController* _controller;
    EntityNetworkController* _networkController;
    b2Body* _body;
    std::vector<b2Fixture*> _fixtures;
    b2Fixture* _groundSensorFixture;
    Pose _pose;
    Pose _poseNetworkCache;
    State _state;
    State _stateNetworkCache;
    const uint32_t _ID;
    bool _isRequestingDeletion;
    bool _isBodyFacingLeft;
    
    void createFixtures();
    void destroyFixtures();
};
