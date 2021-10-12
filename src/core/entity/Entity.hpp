//
//  Entity.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 8/3/15.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/math/Vector2.hpp"
#include "core/common/Config.hpp"
#include "core/common/StringUtil.hpp"

#include "EntityLayout.hpp"

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
    BODF_FIXED_ROTATION = 1 << 2,
    BODF_PLAYER         = 1 << 3,
    BODF_DYNAMIC_SIZE   = 1 << 4
};

enum NetworkDataFieldType
{
    NDFT_BOOL = 0,
    NDFT_UINT8 = 1,
    NDFT_UINT16 = 2,
    NDFT_UINT32 = 3,
    NDFT_UINT64 = 4,
    NDFT_INT8 = 5,
    NDFT_INT16 = 6,
    NDFT_INT32 = 7,
    NDFT_INT64 = 8,
    NDFT_FLOAT = 9,
    NDFT_DOUBLE = 10,
    NDFT_SMALL_STRING = 11,
    NDFT_LARGE_STRING = 12
};

struct NetworkDataField
{
    std::string _name;
    uint8_t _type;
    std::map<std::string, bool> _valueBool;
    std::map<std::string, uint8_t> _valueUInt8;
    std::map<std::string, uint16_t> _valueUInt16;
    std::map<std::string, uint32_t> _valueUInt32;
    std::map<std::string, uint64_t> _valueUInt64;
    std::map<std::string, int8_t> _valueInt8;
    std::map<std::string, int16_t> _valueInt16;
    std::map<std::string, int32_t> _valueInt32;
    std::map<std::string, int64_t> _valueInt64;
    std::map<std::string, float> _valueFloat;
    std::map<std::string, double> _valueDouble;
    std::map<std::string, std::string> _valueString;
    
    NetworkDataField(std::string name, uint8_t type, std::string value) :
    _name(name),
    _type(type)
    {
        switch (_type)
        {
            case NDFT_BOOL:
                _valueBool.emplace(name, StringUtil::stringToBool(value));
                break;
            case NDFT_UINT8:
                _valueUInt8.emplace(name, static_cast<uint8_t>(StringUtil::stringToNumber<uint16_t>(value)));
                break;
            case NDFT_UINT16:
                _valueUInt16.emplace(name, StringUtil::stringToNumber<uint16_t>(value));
                break;
            case NDFT_UINT32:
                _valueUInt32.emplace(name, StringUtil::stringToNumber<uint32_t>(value));
                break;
            case NDFT_UINT64:
                _valueUInt64.emplace(name, StringUtil::stringToNumber<uint64_t>(value));
                break;
            case NDFT_INT8:
                _valueInt8.emplace(name, static_cast<int8_t>(StringUtil::stringToNumber<int16_t>(value)));
                break;
            case NDFT_INT16:
                _valueInt16.emplace(name, StringUtil::stringToNumber<int16_t>(value));
                break;
            case NDFT_INT32:
                _valueInt32.emplace(name, StringUtil::stringToNumber<int32_t>(value));
                break;
            case NDFT_INT64:
                _valueInt64.emplace(name, StringUtil::stringToNumber<int64_t>(value));
                break;
            case NDFT_FLOAT:
                _valueFloat.emplace(name, StringUtil::stringToNumber<float>(value));
                break;
            case NDFT_DOUBLE:
                _valueDouble.emplace(name, StringUtil::stringToNumber<double>(value));
                break;
            case NDFT_SMALL_STRING:
            case NDFT_LARGE_STRING:
                _valueString.emplace(name, value);
                break;
            default:
                break;
        }
    }
    
    friend bool operator==(const NetworkDataField& a, const NetworkDataField& b)
    {
        return
        a._name        == b._name &&
        a._type        == b._type &&
        a._valueBool   == b._valueBool &&
        a._valueUInt8  == b._valueUInt8 &&
        a._valueUInt16 == b._valueUInt16 &&
        a._valueUInt32 == b._valueUInt32 &&
        a._valueUInt64 == b._valueUInt64 &&
        a._valueInt8   == b._valueInt8 &&
        a._valueInt16  == b._valueInt16 &&
        a._valueInt32  == b._valueInt32 &&
        a._valueInt64  == b._valueInt64 &&
        a._valueFloat  == b._valueFloat &&
        a._valueDouble == b._valueDouble &&
        a._valueString == b._valueString;
    }
    
    friend bool operator!=(NetworkDataField& a, NetworkDataField& b)
    {
        return !(a == b);
    }
    
    bool& valueBool()
    {
        return _valueBool[_name];
    }
    
    uint8_t& valueUInt8()
    {
        return _valueUInt8[_name];
    }
    
    uint16_t& valueUInt16()
    {
        return _valueUInt16[_name];
    }
    
    uint32_t& valueUInt32()
    {
        return _valueUInt32[_name];
    }
    
    uint64_t& valueUInt64()
    {
        return _valueUInt64[_name];
    }
    
    int8_t& valueInt8()
    {
        return _valueInt8[_name];
    }
    
    int16_t& valueInt16()
    {
        return _valueInt16[_name];
    }
    
    int32_t& valueInt32()
    {
        return _valueInt32[_name];
    }
    
    int64_t& valueInt64()
    {
        return _valueInt64[_name];
    }
    
    float& valueFloat()
    {
        return _valueFloat[_name];
    }
    
    double& valueDouble()
    {
        return _valueDouble[_name];
    }
    
    std::string& valueString()
    {
        return _valueString[_name];
    }
};

struct NetworkDataGroup
{
    uint8_t _readStateFlag;
    std::string _name;
    std::vector<NetworkDataField> _data;
    std::vector<NetworkDataField> _dataCache;
    
    NetworkDataGroup(uint8_t readStateFlag, std::string name, std::vector<NetworkDataField> data) :
    _readStateFlag(readStateFlag),
    _name(name),
    _data(data),
    _dataCache(_data)
    {
        // Empty
    }
};

struct NetworkData
{
    std::vector<NetworkDataGroup> _data;
    
    NetworkData(std::vector<NetworkDataGroup> data) :
    _data(data)
    {
        // Empty
    }
};

enum ReadStateFlag
{
    RSTF_POSE =              1 << 0,
    RSTF_STATE =             1 << 1,
    RSTF_EXTRA_DATA_BEGIN =  1 << 2
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
    std::map<uint8_t, std::map<uint8_t, std::string> > _textureMappings;
    std::map<uint8_t, std::string> _soundMappings;
    std::vector<FixtureDef> _fixtures;
    uint8_t _bodyFlags;
    uint8_t _width;
    uint8_t _height;
    Config _metadata;
    NetworkData _networkData;
    
    EntityDef(uint32_t key,
              std::string name,
              std::string keyName,
              std::string controller,
              std::string networkController,
              std::string physicsController,
              std::string renderController,
              std::map<uint8_t, std::map<uint8_t, std::string> > textureMappings,
              std::map<uint8_t, std::string> soundMappings,
              std::vector<FixtureDef> fixtures,
              uint8_t bodyFlags,
              uint8_t width,
              uint8_t height,
              Config metadata,
              NetworkData networkData) :
    _key(key),
    _name(name),
    _keyName(keyName),
    _controller(controller),
    _networkController(networkController),
    _physicsController(physicsController),
    _renderController(renderController),
    _textureMappings(textureMappings),
    _soundMappings(soundMappings),
    _fixtures(fixtures),
    _bodyFlags(bodyFlags),
    _width(width),
    _height(height),
    _metadata(metadata),
    _networkData(networkData)
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
    void message(uint16_t message);
    EntityDef& entityDef();
    Config& metadata();
    NetworkData& data();
    NetworkDataField& dataField(std::string name);
    uint16_t stateTime();
    Vector2& position();
    Vector2& velocity();
    float width();
    float height();
    void resetWidth();
    void resetHeight();
    float angle();
    const uint32_t getID();
    bool isGrounded();
    bool isXFlipped();
    bool isBody();
    bool isLayer();
    bool isStatic();
    bool isDynamic();
    bool isPlayer();
    bool isDynamicSize();
    bool isFixedRotation();
    bool isServer();
    
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
    
    struct Pose
    {
        Vector2 _position;
        Vector2 _velocity;
        float _width;
        float _height;
        float _angle;
        uint8_t _numGroundContacts;
        bool _isXFlipped;
        
        Pose(float x, float y, float width, float height) :
        _position(x, y),
        _velocity(VECTOR2_ZERO),
        _width(width),
        _height(height),
        _angle(0),
        _numGroundContacts(0),
        _isXFlipped(false)
        {
            // Empty
        }
        
        friend bool operator==(Pose& a, Pose& b)
        {
            return
            a._position          == b._position &&
            a._velocity          == b._velocity &&
            a._width             == b._width &&
            a._height            == b._height &&
            a._angle             == b._angle &&
            a._numGroundContacts == b._numGroundContacts &&
            a._isXFlipped        == b._isXFlipped;
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
