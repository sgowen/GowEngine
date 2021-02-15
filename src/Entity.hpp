//
//  Entity.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 8/3/15.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "EntityLayoutManager.hpp"
#include "Vector2.hpp"

#include <stdint.h>
#include <string>
#include <map>
#include <vector>

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
    uint32_t _key;
    std::string _name;
    std::string _keyName;
    std::string _controller;
    std::string _physicsController;
    std::string _networkController;
    std::map<int, std::string> _textureMappings;
    std::map<int, int> _soundMappings;
    std::map<int, std::vector<int> > _soundRandomMappings;
    std::vector<FixtureDef> _fixtures;
    int _bodyFlags;
    int _width;
    int _height;
    
    EntityDef(uint32_t key,
              std::string name,
              std::string keyName,
              std::string controller,
              std::string physicsController,
              std::string networkController,
              std::map<int, std::string> textureMappings,
              std::map<int, int> soundMappings,
              std::map<int, std::vector<int> > soundRandomMappings,
              std::vector<FixtureDef> fixtures,
              int bodyFlags,
              int width,
              int height) :
    _key(key),
    _name(name),
    _keyName(keyName),
    _controller(controller),
    _physicsController(physicsController),
    _networkController(networkController),
    _textureMappings(textureMappings),
    _soundMappings(soundMappings),
    _soundRandomMappings(soundRandomMappings),
    _fixtures(fixtures),
    _bodyFlags(bodyFlags),
    _width(width),
    _height(height)
    {
        // Empty
    }
};

class EntityController;
class EntityPhysicsController;
class EntityNetworkController;

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
    Pose& poseCache();
    
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
    State& stateCache();
    
    Entity(EntityDef ed, EntityInstanceDef eid, bool isServer);
    ~Entity();
    
    void update();
    void message(uint16_t message, void* data = NULL);
    EntityDef& entityDef();
    EntityController* controller();
    EntityPhysicsController* physicsController();
    EntityNetworkController* networkController();
    uint16_t stateTime();
    void setPosition(Vector2 position);
    void setPosition(float x, float y);
    const Vector2& getPosition();
    void setVelocity(Vector2 velocity);
    void setVelocity(float x, float y);
    const Vector2& getVelocity();
    float width();
    float height();
    void setAngle(float angle);
    float getAngle();
    const uint32_t getID();
    bool isGrounded();
    void requestDeletion();
    bool isRequestingDeletion();
    bool isFacingLeft();
    std::string getTextureMapping();
    std::string getTextureMapping(uint8_t state);
    int getSoundMapping(int state);
    
private:
    EntityDef _entityDef;
    EntityInstanceDef _entityInstanceDef;
    EntityController* _controller;
    EntityPhysicsController* _physicsController;
    EntityNetworkController* _networkController;
    Pose _pose;
    Pose _poseCache;
    State _state;
    State _stateCache;
    bool _isRequestingDeletion;
};
