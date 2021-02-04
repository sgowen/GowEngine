//
//  Entity.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 8/3/15.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <box2d/b2_math.h>

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
    std::vector<b2Vec2> _vertices;
    b2Vec2 _center;
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
    std::map<int, std::vector<int> > _soundCollectionMappings;
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
    void setPosition(b2Vec2 position);
    const b2Vec2& getPosition();
    void setVelocity(b2Vec2 velocity);
    const b2Vec2& getVelocity();
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
        b2Vec2 _velocity;
        b2Vec2 _position;
        float _angle;
        uint8_t _numGroundContacts;
        bool _isFacingLeft;
        
        Pose(float x, float y)
        {
            _velocity = b2Vec2_zero;
            _position = b2Vec2(x, y);
            _angle = 0;
            _numGroundContacts = 0;
            _isFacingLeft = false;
        }
        
        friend bool operator==(Pose& lhs, Pose& rhs)
        {
            return
            lhs._velocity          == rhs._velocity &&
            lhs._position          == rhs._position &&
            lhs._angle             == rhs._angle &&
            lhs._numGroundContacts == rhs._numGroundContacts &&
            lhs._isFacingLeft      == rhs._isFacingLeft;
        }
        
        friend bool operator!=(Pose& lhs, Pose& rhs)
        {
            return !(lhs == rhs);
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
        
        friend bool operator==(State& lhs, State& rhs)
        {
            return
            lhs._stateTime         == rhs._stateTime &&
            lhs._state             == rhs._state &&
            lhs._stateFlags        == rhs._stateFlags;
        }
        
        friend bool operator!=(State& lhs, State& rhs)
        {
            return !(lhs == rhs);
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
    float _deadZoneY;
    bool _isRequestingDeletion;
    bool _isBodyFacingLeft;
    
    void createFixtures();
    void destroyFixtures();
};
