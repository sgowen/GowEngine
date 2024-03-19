//
//  Entity.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 8/3/15.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
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

enum Messages
{
    MSG_NONE = 0,
    MSG_DANGEROUS_TOUCH = 1,
    MSG_NO_TOUCH = 2,
    MSG_DAMAGE = 3
};

enum Box2DFixtureFlags
{
    BFIXF_BOX =           1 << 0,
    BFIXF_SENSOR =        1 << 1,
    BFIXF_GROUND_SENSOR = 1 << 2,
    BFIXF_CIRCLE =        1 << 3,
    BFIXF_DAMAGE_SENSOR = 1 << 4
};

enum NosFixtureFlags
{
    NFIXF_SENSOR =        1 << 0,
    NFIXF_GROUND_SENSOR = 1 << 1,
    NFIXF_IGNORE_LEFT =   1 << 2,
    NFIXF_IGNORE_RIGHT =  1 << 3,
    NFIXF_IGNORE_BOTTOM = 1 << 4,
    NFIXF_DAMAGE_TOP =    1 << 5
};

struct FixtureDef
{
    std::vector<Vector2> _vertices;
    Vector2 _center;
    float _halfWidthFactor;
    float _halfHeightFactor;
    float _restitution;
    float _density;
    float _friction;
    uint32_t _flags;
};

enum BodyFlags
{
    BODF_STATE_SENSITIVE = 1 << 0,
    BODF_DYNAMIC =         1 << 1,
    BODF_PLAYER =          1 << 2,
    BODF_FIXED_ROTATION =  1 << 3
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
    
    void setValueBool(bool value)
    {
        _valueBool[_name] = value;
    }
    
    bool valueBool()
    {
        return _valueBool[_name];
    }
    
    bool& refBool()
    {
        return _valueBool[_name];
    }
    
    void setValueUInt8(uint8_t value)
    {
        _valueUInt8[_name] = value;
    }
    
    uint8_t valueUInt8()
    {
        return _valueUInt8[_name];
    }
    
    uint8_t& refUInt8()
    {
        return _valueUInt8[_name];
    }
    
    void setValueUInt16(uint16_t value)
    {
        _valueUInt16[_name] = value;
    }
    
    uint16_t valueUInt16()
    {
        return _valueUInt16[_name];
    }
    
    uint16_t& refUInt16()
    {
        return _valueUInt16[_name];
    }
    
    void setValueUInt32(uint32_t value)
    {
        _valueUInt32[_name] = value;
    }
    
    uint32_t valueUInt32()
    {
        return _valueUInt32[_name];
    }
    
    uint32_t& refUInt32()
    {
        return _valueUInt32[_name];
    }
    
    void setValueUInt64(uint64_t value)
    {
        _valueUInt64[_name] = value;
    }
    
    uint64_t valueUInt64()
    {
        return _valueUInt64[_name];
    }
    
    uint64_t& refUInt64()
    {
        return _valueUInt64[_name];
    }
    
    void setValueInt8(int8_t value)
    {
        _valueInt8[_name] = value;
    }
    
    int8_t valueInt8()
    {
        return _valueInt8[_name];
    }
    
    int8_t& refInt8()
    {
        return _valueInt8[_name];
    }
    
    void setValueInt16(int16_t value)
    {
        _valueInt16[_name] = value;
    }
    
    int16_t valueInt16()
    {
        return _valueInt16[_name];
    }
    
    int16_t& refInt16()
    {
        return _valueInt16[_name];
    }
    
    void setValueInt32(int32_t value)
    {
        _valueInt32[_name] = value;
    }
    
    int32_t valueInt32()
    {
        return _valueInt32[_name];
    }
    
    int32_t& refInt32()
    {
        return _valueInt32[_name];
    }
    
    void setValueInt64(int64_t value)
    {
        _valueInt64[_name] = value;
    }
    
    int64_t valueInt64()
    {
        return _valueInt64[_name];
    }
    
    int64_t& refInt64()
    {
        return _valueInt64[_name];
    }
    
    void setValueFloat(float value)
    {
        _valueFloat[_name] = value;
    }
    
    float valueFloat()
    {
        return _valueFloat[_name];
    }
    
    float& refFloat()
    {
        return _valueFloat[_name];
    }
    
    void setValueDouble(double value)
    {
        _valueDouble[_name] = value;
    }
    
    double valueDouble()
    {
        return _valueDouble[_name];
    }
    
    double& refDouble()
    {
        return _valueDouble[_name];
    }
    
    void setValueString(std::string value)
    {
        _valueString[_name] = value;
    }
    
    std::string valueString()
    {
        return _valueString[_name];
    }
    
    std::string& refString()
    {
        return _valueString[_name];
    }
};

struct NetworkDataGroup
{
    uint8_t _readStateFlag;
    std::string _name;
    std::vector<NetworkDataField> _data;
    
    NetworkDataGroup(uint8_t readStateFlag, std::string name, std::vector<NetworkDataField> data) :
    _readStateFlag(readStateFlag),
    _name(name),
    _data(data)
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
    RSTF_PLAYER_INFO =       1 << 2,
    RSTF_EXTRA_DATA_BEGIN =  1 << 3
};

struct SoundMapping
{
    uint16_t _stateTime;
    std::string _soundID;
    
    SoundMapping(uint16_t stateTime,
                 std::string soundID) :
    _stateTime(stateTime),
    _soundID(soundID)
    {
        // Empty
    }
};

struct EntityDef
{
    uint32_t _key;
    std::string _name;
    std::string _keyName;
    std::string _controller;
    std::string _controllerScript;
    std::string _controllerAI;
    std::string _controllerAIScript;
    std::string _controllerInput;
    std::string _inputMapping;
    std::string _controllerNetwork;
    std::string _controllerRender;
    std::vector<std::string> _inputStateFlags;
    std::vector<std::string> _states;
    std::vector<std::string> _stateFlags;
    std::map<uint8_t, std::string> _textureMappings;
    // Need to also include totalTime for each state
    // to correlate state animations to sounds
    // running should play left, right, left, right sounds
    // even after the stateTime > animationTime
    // because we can just soundStateTime = stateTime % animationTime
    std::map<uint8_t, std::vector<SoundMapping> > _soundMappings;
    std::vector<FixtureDef> _fixtures;
    uint8_t _bodyFlags;
    uint8_t _width;
    uint8_t _height;
    float _scale;
    Config _data;
    NetworkData _networkData;
    
    EntityDef(uint32_t key,
              std::string name,
              std::string keyName,
              std::string controller,
              std::string controllerScript,
              std::string controllerAI,
              std::string controllerAIScript,
              std::string controllerInput,
              std::string inputMapping,
              std::string controllerNetwork,
              std::string controllerRender,
              std::vector<std::string> inputStateFlags,
              std::vector<std::string> states,
              std::vector<std::string> stateFlags,
              std::map<uint8_t, std::string> textureMappings,
              std::map<uint8_t, std::vector<SoundMapping>> soundMappings,
              std::vector<FixtureDef> fixtures,
              uint8_t bodyFlags,
              uint8_t width,
              uint8_t height,
              float scale,
              Config data,
              NetworkData networkData) :
    _key(key),
    _name(name),
    _keyName(keyName),
    _controller(controller),
    _controllerScript(controllerScript),
    _controllerAI(controllerAI),
    _controllerAIScript(controllerAIScript),
    _controllerInput(controllerInput),
    _inputMapping(inputMapping),
    _controllerNetwork(controllerNetwork),
    _controllerRender(controllerRender),
    _inputStateFlags(inputStateFlags),
    _states(states),
    _stateFlags(stateFlags),
    _textureMappings(textureMappings),
    _soundMappings(soundMappings),
    _fixtures(fixtures),
    _bodyFlags(bodyFlags),
    _width(width),
    _height(height),
    _scale(scale),
    _data(data),
    _networkData(networkData)
    {
        // Empty
    }
};

struct EntityInstanceDef;
class EntityController;
class EntityControllerAI;
class EntityControllerInput;
class EntityControllerNetwork;
class EntityControllerPhysics;
class EntityControllerRender;
class World;
struct CursorEvent;
struct GamepadEvent;
struct KeyboardEvent;

class Entity
{
    friend class EntityControllerInput;
    friend class EntityControllerNetwork;
    
public:
    static Entity* createEntity(const EntityInstanceDef& eid);

    static Entity* createEntity(const EntityDef& ed, const EntityInstanceDef& eid);
    
    Entity(const EntityDef& ed, const EntityInstanceDef& eid);
    ~Entity();
    
    void beginFrame();
    void runAI();
    void processEvent(uint16_t& inputState, CursorEvent* e);
    void processEvent(uint16_t& inputState, GamepadEvent* e);
    void processEvent(uint16_t& inputState, KeyboardEvent* e);
    void processInput(uint16_t inputState);
    void update(uint32_t numMovesProcessed);
    void message(uint16_t message, Entity* fromEntity);
    std::string message(uint16_t message);
    uint16_t message(std::string message);
    EntityDef& entityDef();
    uint16_t inputStateFlag(std::string inputStateFlag);
    uint8_t state(std::string key);
    std::string state(uint8_t state);
    uint8_t stateFlag(std::string key);
    Config& data();
    NetworkData& networkData();
    NetworkDataField& networkDataField(std::string name);
    uint16_t stateTime();
    Vector2& position();
    Vector2& velocity();
    bool isGrounded();
    bool isXFlipped();
    float width();
    float height();
    float angle();
    const uint32_t getID();
    bool isLayer();
    bool isStateSensitive();
    bool isStatic();
    bool isDynamic();
    bool isPlayer();
    bool isServer();
    
    template<typename T = EntityController>
    T* controller()
    {
        assert(_controller != nullptr);
        return static_cast<T*>(_controller);
    }
    
    template<typename T = EntityControllerAI>
    T* controllerAI()
    {
        assert(_controllerAI != nullptr);
        return static_cast<T*>(_controllerAI);
    }
    
    template<typename T = EntityControllerInput>
    T* controllerInput()
    {
        assert(_controllerInput != nullptr);
        return static_cast<T*>(_controllerInput);
    }
    
    template<typename T = EntityControllerNetwork>
    T* controllerNetwork()
    {
        assert(_controllerNetwork != nullptr);
        return static_cast<T*>(_controllerNetwork);
    }

    void setControllerPhysics(EntityControllerPhysics* controllerPhysics)
    {
        _controllerPhysics = controllerPhysics;
    }
    void destroyControllerPhysics()
    {
        if (_controllerPhysics != nullptr)
        {
            delete _controllerPhysics;
            _controllerPhysics = nullptr;
        }
    }
    
    template<typename T = EntityControllerPhysics>
    T* controllerPhysics()
    {
        assert(_controllerPhysics != nullptr);
        return static_cast<T*>(_controllerPhysics);
    }
    
    template<typename T = EntityControllerRender>
    T* controllerRender()
    {
        assert(_controllerRender != nullptr);
        return static_cast<T*>(_controllerRender);
    }
    
    struct Pose
    {
        Vector2 _position;
        Vector2 _velocity;
        uint8_t _numGroundContacts;
        bool _isXFlipped;
        bool _isZeroGravity;
        bool _isSlowed;
        
        Pose(float x, float y) :
        _position(x, y),
        _velocity(VECTOR2_ZERO),
        _numGroundContacts(0),
        _isXFlipped(false),
        _isZeroGravity(false),
        _isSlowed(false)
        {
            // Empty
        }
        
        friend bool operator==(Pose& a, Pose& b)
        {
            return
            a._position          == b._position &&
            a._velocity          == b._velocity &&
            a._numGroundContacts == b._numGroundContacts &&
            a._isXFlipped        == b._isXFlipped &&
            a._isZeroGravity     == b._isZeroGravity &&
            a._isSlowed          == b._isSlowed;
        }
        
        friend bool operator!=(Pose& a, Pose& b)
        {
            return !(a == b);
        }
    };
    Pose& pose();
    
    enum StateFlags
    {
        STTF_EXILED = 0x80
    };
    
    struct State
    {
        uint8_t _state;
        uint8_t _stateFlags;
        uint16_t _stateTime;
        uint16_t _lastProcessedInputState;
        
        State()
        {
            _state = 0;
            _stateFlags = 0;
            _stateTime = 0;
            _lastProcessedInputState = 0;
        }
        
        friend bool operator==(State& a, State& b)
        {
            return
            a._state                   == b._state &&
            a._stateFlags              == b._stateFlags &&
            a._stateTime               == b._stateTime &&
            a._lastProcessedInputState == b._lastProcessedInputState;
        }
        
        friend bool operator!=(State& a, State& b)
        {
            return !(a == b);
        }
    };
    State& state();
    
    struct PlayerInfo
    {
        uint64_t _playerAddressHash;
        std::string _playerName;
        std::string _playerAddress;
        uint8_t _playerID;
        
        PlayerInfo()
        {
            _playerAddressHash = 0;
            _playerName = "Robot";
            _playerAddress = "";
            _playerID = 0;
        }
        
        friend bool operator==(PlayerInfo& lhs, PlayerInfo& rhs)
        {
            return
            lhs._playerAddressHash == rhs._playerAddressHash &&
            lhs._playerName        == rhs._playerName &&
            lhs._playerAddress     == rhs._playerAddress &&
            lhs._playerID          == rhs._playerID;
        }
        
        friend bool operator!=(PlayerInfo& lhs, PlayerInfo& rhs)
        {
            return !(lhs == rhs);
        }
    };
    PlayerInfo& playerInfo();
    
    void exile();
    bool isExiled();
    bool isRequestingDeletion();
    uint16_t lastProcessedInputState();
    
    void setWorld(World* w);
    World* world();
    
private:
    EntityDef _entityDef;
    EntityInstanceDef _entityInstanceDef;
    EntityController* _controller;
    EntityControllerAI* _controllerAI;
    EntityControllerInput* _controllerInput;
    EntityControllerNetwork* _controllerNetwork;
    EntityControllerPhysics* _controllerPhysics;
    EntityControllerRender* _controllerRender;
    Pose _pose;
    State _state;
    PlayerInfo _playerInfo;
    uint16_t _exileStateTime;
    float _width;
    float _height;
    float _angle;
    World* _world;
};
