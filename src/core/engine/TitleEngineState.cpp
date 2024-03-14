//
//  TitleEngineState.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 9/29/21.
//  Copyright © 2020 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

void TitleEngineState::onEnter(Engine* e)
{
    // Empty
}

void TitleEngineState::onAssetsLoaded(Engine *e)
{
    // Empty
}

void TitleEngineState::onExit(Engine* e)
{
    setState(TESS_DEFAULT);
    _inputProcessor.clearTextInput();
    _userEnteredIPAddress.clear();
}

void TitleEngineState::onUpdate(Engine* e)
{
#if IS_DESKTOP
    if (STEAM_GAME_SERVICES)
    {
        STEAM_GAME_SERVICES->update();
        
        if (STEAM_GAME_SERVICES->getStatus() == STEAM_INIT_SUCCESS)
        {
            if (STEAM_GAME_SERVICES->isRequestingToJoinServer())
            {
                CSteamID serverToJoinSteamID = STEAM_GAME_SERVICES->getServerToJoinSteamID();
                
                Config args;
                args.putBool(ARG_IS_HOST, false);
                args.putUInt64(ARG_STEAM_ADDRESS, serverToJoinSteamID.ConvertToUint64());
                e->pushState(&ENGINE_STATE_GAME, args);
            }
        }
        else
        {
            e->popState();
        }
    }
#endif
    
    switch (_state)
    {
        case TESS_DEFAULT:
        {
            uint8_t inputState = _inputProcessor.update();
            switch (inputState)
            {
                case IPS_EXIT:
                    e->popState();
                    break;
                case IPS_ACTION:
                    setState(_inputProcessor.getAction());
                    break;
                default:
#if IS_MOBILE
                    for (CursorEvent* ce : INPUT_MGR.getCursorEvents())
                    {
                        if (ce->isUp())
                        {
                            Config args;
                            args.putBool(ARG_IS_HOST, true);
                            args.putString(ARG_USERNAME, "Mobile User");
                            e->pushState(&ENGINE_STATE_GAME, args);
                            break;
                        }
                    }
#endif
                    break;
            }
        }
            break;
        case TESS_INPUT_CREATE:
        {
#if IS_DESKTOP
            e->pushState(&ENGINE_STATE_IMGUI_DEMO);
#endif
            return;
        }
            break;
        case TESS_INPUT_HOST_NAME:
        {
#if IS_DESKTOP
            if (ENGINE_CFG.useSteamNetworking())
            {
                // We don't need to input a host name on Steam
                Config args;
                args.putBool(ARG_IS_HOST, true);
                e->pushState(&ENGINE_STATE_GAME, args);
                return;
            }
#endif
            
            uint8_t inputState = _inputProcessor.updateReadText();
            switch (inputState)
            {
                case IPS_EXIT:
                    setState(TESS_DEFAULT);
                    break;
                case IPS_TEXT_INPUT_READY:
                {
                    Config args;
                    args.putBool(ARG_IS_HOST, true);
                    if (!_inputProcessor.getTextInput().empty())
                    {
                        args.putString(ARG_USERNAME, _inputProcessor.getTextInput());
                        e->pushState(&ENGINE_STATE_GAME, args);
                    }
                    break;
                }
            }
        }
            break;
        case TESS_INPUT_IP:
        {
            uint8_t inputState = _inputProcessor.updateReadText();
            switch (inputState)
            {
                case IPS_EXIT:
                    setState(TESS_DEFAULT);
                    break;
                case IPS_TEXT_INPUT_READY:
                    _userEnteredIPAddress = _inputProcessor.getTextInput();
                    _inputProcessor.clearTextInput();
                    setState(TESS_INPUT_JOIN_NAME);
                    break;
            }
        }
            break;
        case TESS_INPUT_JOIN_NAME:
        {
            uint8_t inputState = _inputProcessor.updateReadText();
            switch (inputState)
            {
                case IPS_EXIT:
                    setState(TESS_INPUT_IP);
                    _inputProcessor.setTextInput(_userEnteredIPAddress);
                    break;
                case IPS_TEXT_INPUT_READY:
                {
                    Config args;
                    args.putBool(ARG_IS_HOST, false);
                    args.putString(ARG_IP_ADDRESS, _userEnteredIPAddress);
                    if (!_inputProcessor.getTextInput().empty())
                    {
                        args.putString(ARG_USERNAME, _inputProcessor.getTextInput());
                        e->pushState(&ENGINE_STATE_GAME, args);
                    }
                    break;
                }
            }
        }
            break;
        case TESS_INPUT_LAN_SERVER:
        case TESS_INPUT_INTERNET_SERVER:
        {
#if IS_DESKTOP
            uint8_t inputState = _inputProcessor.updateReadText(1);
            switch (inputState)
            {
                case IPS_EXIT:
                    setState(TESS_DEFAULT);
                    break;
                case IPS_TEXT_INPUT_READY:
                {
                    std::string textInput = _inputProcessor.getTextInput();
                    int32_t serverIndex = StringUtil::stringToNumber<int32_t>(textInput);
                    if (serverIndex >= 0 && serverIndex <= 3)
                    {
                        if (STEAM_GAME_SERVICES && !STEAM_GAME_SERVICES->isRequestingServers())
                        {
                            std::vector<SteamServerInfo> gameServers = STEAM_GAME_SERVICES->getGameServers();
                            if (gameServers.size() > serverIndex)
                            {
                                STEAM_GAME_SERVICES->initiateServerConnection(gameServers[serverIndex].getSteamID());
                            }
                        }
                    }
                }
                    break;
            }
#endif
        }
            break;
        default:
            break;
    }
}

void TitleEngineState::onRender(Renderer& r)
{
    bool steam = false;
#if IS_DESKTOP
    if (ENGINE_CFG.useSteamNetworking())
    {
        steam = true;
    }
#endif
    
    r.bindFramebuffer("main");
    
#if IS_DESKTOP
    r.setTextVisible("create", _state == TESS_DEFAULT);
#else
    r.setTextVisible("create", false);
#endif
    
    r.setTextVisible("hostServer", _state == TESS_DEFAULT);
    r.setTextVisible("joinServer", _state == TESS_DEFAULT && !steam);
    r.setTextVisible("joinLANServer", _state == TESS_DEFAULT && steam);
    r.setTextVisible("joinInternetServer", _state == TESS_DEFAULT && steam);
    r.setTextVisible("enterIP", _state == TESS_INPUT_IP);
    r.setTextVisible("enterName", !steam && (_state == TESS_INPUT_HOST_NAME || _state == TESS_INPUT_JOIN_NAME));
    r.setTextVisible("input", _state == TESS_INPUT_HOST_NAME || _state == TESS_INPUT_IP || _state == TESS_INPUT_JOIN_NAME || _state == TESS_INPUT_LAN_SERVER || _state == TESS_INPUT_INTERNET_SERVER);
    
    r.setText("input", _inputProcessor.getTextInput());
  
#if IS_DESKTOP
    if (steam)
    {
        r.setTextVisible("server0", _state == TESS_INPUT_LAN_SERVER || _state == TESS_INPUT_INTERNET_SERVER);
        r.setTextVisible("server1", _state == TESS_INPUT_LAN_SERVER || _state == TESS_INPUT_INTERNET_SERVER);
        r.setTextVisible("server2", _state == TESS_INPUT_LAN_SERVER || _state == TESS_INPUT_INTERNET_SERVER);
        r.setTextVisible("server3", _state == TESS_INPUT_LAN_SERVER || _state == TESS_INPUT_INTERNET_SERVER);
        
        std::vector<SteamServerInfo> gameServers;
        if (STEAM_GAME_SERVICES && !STEAM_GAME_SERVICES->isRequestingServers())
        {
            gameServers = STEAM_GAME_SERVICES->getGameServers();
            
            if (gameServers.size() >= 1)
            {
                r.setText("server0", STRING_FORMAT("[%i] %s", 0, gameServers[0].getName()).c_str());
            }
            
            if (gameServers.size() >= 2)
            {
                r.setText("server1", STRING_FORMAT("[%i] %s", 1, gameServers[1].getName()).c_str());
            }
        }
        
        if (gameServers.empty())
        {
            r.setText("server0", "");
            r.setText("server1", "");
            r.setText("server2", "");
            r.setText("server3", "");
        }
    }
#endif
    
    r.renderImageViews();
    r.renderTextViews();

    r.renderFramebufferToScreen("main");
}

void TitleEngineState::setState(uint8_t state)
{
    _state = state;
    
#if IS_DESKTOP
    if (ENGINE_CFG.useSteamNetworking())
    {
        if (_state == TESS_INPUT_LAN_SERVER && STEAM_GAME_SERVICES)
        {
            STEAM_GAME_SERVICES->refreshLANServers();
        }
        else if (_state == TESS_INPUT_INTERNET_SERVER && STEAM_GAME_SERVICES)
        {
            STEAM_GAME_SERVICES->refreshInternetServers();
        }
    }
#endif
}

TitleEngineState::TitleEngineState() : EngineState("json/title/Config.json"),
_state(TESS_DEFAULT),
_inputProcessor(),
_userEnteredIPAddress("")
{
    _inputProcessor.registerActionForKey(GOW_KEY_C, TESS_INPUT_CREATE);
    _inputProcessor.registerActionForKey(GOW_KEY_H, TESS_INPUT_HOST_NAME);
    _inputProcessor.registerActionForKey(GOW_KEY_J, TESS_INPUT_IP);
    _inputProcessor.registerActionForKey(GOW_KEY_L, TESS_INPUT_LAN_SERVER);
    _inputProcessor.registerActionForKey(GOW_KEY_I, TESS_INPUT_INTERNET_SERVER);
    
    _inputProcessor.registerActionForButton(GPEB_BUTTON_A, TESS_INPUT_HOST_NAME);
}
