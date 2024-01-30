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
//    if (!ENGINE_CFG.musicDisabled())
//    {
//        AUDIO_ENGINE.playSound("music_title", 0, 1.0f, true);
//    }
  
#if IS_DESKTOP
    if (ENGINE_CFG.useSteamNetworking())
    {
        SteamGameServices::create(ENGINE_CFG.steamGameDir().c_str());
    }
#endif
}

void TitleEngineState::onExit(Engine* e)
{
    _state = TESS_DEFAULT;
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
                args.putUInt64(ARG_STEAM_ADDRESS, serverToJoinSteamID.ConvertToUint64());
                e->pushState(&ENGINE_STATE_GAME_CLNT, args);
            }
        }
        else
        {
            if (STEAM_GAME_SERVICES)
            {
                SteamGameServices::destroy();
            }
            e->setRequestedHostAction(ERHA_EXIT);
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
                    e->setRequestedHostAction(ERHA_EXIT);
                    break;
                case IPS_ACTION:
                    _state = _inputProcessor.getAction();
                    break;
                default:
#if IS_MOBILE
                    for (CursorEvent* ce : INPUT_MGR.getCursorEvents())
                    {
                        if (ce->isUp())
                        {
                            Config args;
                            args.putString(ARG_USERNAME, "Mobile User");
                            e->pushState(&ENGINE_STATE_GAME_HOST, args);
                            break;
                        }
                    }
#endif
                    break;
            }
        }
            break;
        case TESS_INPUT_HOST_NAME:
        {
            uint8_t inputState = _inputProcessor.updateReadText();
            switch (inputState)
            {
                case IPS_EXIT:
                    _state = TESS_DEFAULT;
                    break;
                case IPS_TEXT_INPUT_READY:
                {
                    Config args;
                    args.putString(ARG_USERNAME, _inputProcessor.getTextInput());
                    e->pushState(&ENGINE_STATE_GAME_HOST, args);
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
                    _state = TESS_DEFAULT;
                    break;
                case IPS_TEXT_INPUT_READY:
                    _userEnteredIPAddress = _inputProcessor.getTextInput();
                    _inputProcessor.clearTextInput();
                    _state = TESS_INPUT_JOIN_NAME;
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
                    _state = TESS_INPUT_IP;
                    _inputProcessor.setTextInput(_userEnteredIPAddress);
                    break;
                case IPS_TEXT_INPUT_READY:
                {
                    Config args;
                    args.putString(ARG_IP_ADDRESS, _userEnteredIPAddress);
                    args.putString(ARG_USERNAME, _inputProcessor.getTextInput());
                    e->pushState(&ENGINE_STATE_GAME_CLNT, args);
                    break;
                }
            }
        }
            break;
        case TESS_START_OFFLINE:
        {
            Config args;
            args.putBool(ARG_OFFLINE_MODE, true);
            e->pushState(&ENGINE_STATE_GAME_CLNT, args);
        }
            break;
        default:
            break;
    }
}

void TitleEngineState::onRender(Renderer& r, double extrapolation)
{
    r.bindFramebuffer();
    
    r.setTextVisible("hostServer", _state == TESS_DEFAULT);
    r.setTextVisible("joinServer", _state == TESS_DEFAULT);
    r.setTextVisible("enterIP", _state == TESS_INPUT_IP);
    r.setTextVisible("enterName", _state == TESS_INPUT_HOST_NAME || _state == TESS_INPUT_JOIN_NAME);
    r.setTextVisible("input", _state == TESS_INPUT_HOST_NAME || _state == TESS_INPUT_IP || _state == TESS_INPUT_JOIN_NAME);
    r.setText("input", _inputProcessor.getTextInput());
    
    r.renderImageViews();
    r.renderTextViews();

    r.renderToScreen();
}

TitleEngineState::TitleEngineState() : EngineState("data/json/TitleEngineState/Config.json"),
_state(TESS_DEFAULT),
_inputProcessor(),
_userEnteredIPAddress("")
{
    _inputProcessor.registerActionForKey(GOW_KEY_H, TESS_INPUT_HOST_NAME);
    _inputProcessor.registerActionForKey(GOW_KEY_J, TESS_INPUT_IP);
    _inputProcessor.registerActionForKey(GOW_KEY_O, TESS_START_OFFLINE);
    
    _inputProcessor.registerActionForButton(GPEB_BUTTON_A, TESS_INPUT_HOST_NAME);
    _inputProcessor.registerActionForButton(GPEB_BUTTON_X, TESS_START_OFFLINE);
}
