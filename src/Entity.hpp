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
    std::string _networkController;
    std::string _physicsController;
    std::string _renderController;
    std::map<uint8_t, std::string> _textureMappings;
    std::map<uint8_t, uint16_t> _soundMappings;
    std::map<uint8_t, std::vector<uint16_t> > _soundRandomMappings;
    std::vector<FixtureDef> _fixtures;
    int _bodyFlags;
    int _width;
    int _height;
    
    EntityDef(uint32_t key,
              std::string name,
              std::string keyName,
              std::string controller,
              std::string networkController,
              std::string physicsController,
              std::string renderController,
              std::map<uint8_t, std::string> textureMappings,
              std::map<uint8_t, uint16_t> soundMappings,
              std::map<uint8_t, std::vector<uint16_t> > soundRandomMappings,
              std::vector<FixtureDef> fixtures,
              int bodyFlags,
              int width,
              int height) :
    _key(key),
    _name(name),
    _keyName(keyName),
    _controller(controller),
    _networkController(networkController),
    _physicsController(physicsController),
    _renderController(renderController),
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
class EntityNetworkController;
class EntityPhysicsController;
class EntityRenderController;

class Entity
{
    friend class EntityController;
    friend class EntityNetworkController;
    friend class EntityPhysicsController;
    friend class EntityRenderController;
    
public:
    Entity(EntityDef ed, EntityInstanceDef eid);
    ~Entity();
    
    void update();
    void message(uint16_t message, void* data = NULL);
    uint16_t stateTime();
    Vector2& position();
    Vector2& velocity();
    float width();
    float height();
    float angle();
    const uint32_t getID();
    bool isGrounded();
    bool isFacingLeft();
    bool isServer();
    
    EntityDef& entityDef();
    
    template<typename T = EntityController>
    T* controller()
    {
        return static_cast<T*>(_controller);
    }
    
    template<typename T = EntityNetworkController>
    T* networkController()
    {
        return static_cast<T*>(_networkController);
    }

    template<typename T = EntityPhysicsController>
    T* physicsController()
    {
        return static_cast<T*>(_physicsController);
    }
    
    template<typename T = EntityRenderController>
    T* renderController()
    {
        return static_cast<T*>(_renderController);
    }
    
    enum ReadStateFlag
    {
        RSTF_POSE =  1 << 0,
        RSTF_STATE = 1 << 1
    };
    
    struct Pose
    {
        Vector2 _position;
        Vector2 _velocity;
        float _angle;
        uint8_t _numGroundContacts;
        bool _isFacingLeft;
        
        Pose(float x, float y) :
        _position(x, y),
        _velocity(VECTOR2_ZERO),
        _angle(0),
        _numGroundContacts(0),
        _isFacingLeft(false)
        {
            // Empty
        }
        
        friend bool operator==(Pose& a, Pose& b)
        {
            return
            a._position          == b._position &&
            a._velocity          == b._velocity &&
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
        uint8_t _state;
        uint8_t _stateFlags;
        uint16_t _stateTime;
        
        State()
        {
            _state = 0;
            _stateFlags = 0;
            _stateTime = 0;
        }
        
        friend bool operator==(State& a, State& b)
        {
            return
            a._state             == b._state &&
            a._stateFlags        == b._stateFlags &&
            a._stateTime         == b._stateTime;
        }
        
        friend bool operator!=(State& a, State& b)
        {
            return !(a == b);
        }
    };
    State& state();
    State& stateCache();
    
    void requestDeletion();
    bool isRequestingDeletion();
    
private:
    EntityDef _entityDef;
    EntityInstanceDef _entityInstanceDef;
    EntityController* _controller;
    EntityNetworkController* _networkController;
    EntityPhysicsController* _physicsController;
    EntityRenderController* _renderController;
    Pose _pose;
    Pose _poseCache;
    State _state;
    State _stateCache;
    bool _isRequestingDeletion;
};
