//
//  SteamP2PAuth.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/20/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#if IS_DESKTOP

SteamP2PAuth::SteamP2PAuth(NetworkHelper* networkHelper) :
_networkHelper(networkHelper),
_networkTransport(new SteamP2PNetworkTransport())
{
    // no players yet
	for (uint8 i = 0; i < MAX_NUM_PLAYERS; ++i)
	{
		_rgpP2PAuthPlayer[i] = nullptr;
	}

	// no queued messages
	for (uint8 i = 0; i < MAX_NUM_PLAYERS; ++i)
	{
		_rgpQueuedMessage[i] = nullptr;
	}
}

SteamP2PAuth::~SteamP2PAuth()
{
    delete _networkTransport;
}

void SteamP2PAuth::playerDisconnect(uint8 iSlot)
{
	if (_rgpP2PAuthPlayer[iSlot])
	{
		_rgpP2PAuthPlayer[iSlot]->endGame();
		delete _rgpP2PAuthPlayer[iSlot];
		_rgpP2PAuthPlayer[iSlot] = nullptr;
	}
}

void SteamP2PAuth::endGame()
{
	for (uint8 i = 0; i < MAX_NUM_PLAYERS; ++i)
	{
		if (_rgpP2PAuthPlayer[i])
		{
			_rgpP2PAuthPlayer[i]->endGame();

			delete _rgpP2PAuthPlayer[i];
			_rgpP2PAuthPlayer[i] = nullptr;
		}
	}
}

void SteamP2PAuth::internalinitPlayer(uint8 iSlot, CSteamID steamID, bool bStartAuthProcess)
{
	LOG("P2P:: startAuthPlayer slot=%d account=%d \n", iSlot, steamID.GetAccountID());
	_rgpP2PAuthPlayer[iSlot] = new SteamP2PAuthPlayer(_networkHelper, _networkTransport);
	_rgpP2PAuthPlayer[iSlot]->initPlayer(steamID);
	if (bStartAuthProcess)
    {
        _rgpP2PAuthPlayer[iSlot]->startAuthPlayer();
    }

	// check our queued messages list to see if this guys ticket message arrived before we noticed him
	for (uint8 i = 0; i < MAX_NUM_PLAYERS; ++i)
	{
		if (_rgpQueuedMessage[i] != nullptr)
		{
			if (_rgpP2PAuthPlayer[i]->handleMessage(_rgpQueuedMessage[i]))
			{
				LOG("P2P:: Consumed queued message");
                delete _rgpQueuedMessage[i];
				_rgpQueuedMessage[i] = nullptr;
			}
		}
	}
}

void SteamP2PAuth::registerPlayer(uint8 iSlot, CSteamID steamID)
{
	if (iSlot < MAX_NUM_PLAYERS)
    {
        internalinitPlayer(iSlot, steamID, false);
    }
}

void SteamP2PAuth::startAuthPlayer(uint8 iSlot, CSteamID steamID)
{
	if (iSlot < MAX_NUM_PLAYERS)
    {
        internalinitPlayer(iSlot, steamID, true);
    }
}

bool SteamP2PAuth::handleMessage(uint8 packetType, InputMemoryBitStream& inInputStream)
{
	switch (packetType)
	{
		// message from another player providing his ticket
        case k_EMsgP2PSendingTicket:
        {
            uint64 steamID;
            inInputStream.read(steamID);

            uint32 uTokenLen;
            inInputStream.read(uTokenLen);

            char token[1024];
            inInputStream.readBytes(&token, uTokenLen);

            MsgP2PSendingTicket *msg = new MsgP2PSendingTicket();
            msg->setSteamID(steamID);
            msg->setToken(token, uTokenLen);

            for (uint8 i = 0; i < MAX_NUM_PLAYERS; ++i)
            {
                if (_rgpP2PAuthPlayer[i])
                {
                    if (_rgpP2PAuthPlayer[i]->handleMessage(msg))
                    {
                        return true; // message handled
                    }
                }
            }

            // if we dont have the player in our list yet, lets queue the message and assume he will show up soon
            LOG("P2P:: handleMessage queueing message");
            for (uint8 i = 0; i < MAX_NUM_PLAYERS; ++i)
            {
                if (_rgpQueuedMessage[i] == nullptr)
                {
                    _rgpQueuedMessage[i] = msg;
                    return true; // message handled
                }
            }

            delete msg;

            break;
        }
        default:
            // this is not a P2P auth message
            break;
	}

	// message didn't get handled here
	return false;
}

SteamP2PAuthPlayer::SteamP2PAuthPlayer(NetworkHelper *networkHelper,
                                       SteamP2PNetworkTransport *pNetworkTransport) :
_networkHelper(networkHelper),
_callbackBeginAuthResponse(this, &SteamP2PAuthPlayer::OnBeginAuthResponse)
{
    _networkTransport = pNetworkTransport;
    _bSentTicket = false;
    _bSubmittedHisTicket = false;
    _bHaveAnswer = false;
    _connectTime = _networkHelper->timeTracker().realTime();
    _cubTicketIGaveThisUser = 0;
    _cubTicketHeGaveMe = 0;
}

SteamP2PAuthPlayer::~SteamP2PAuthPlayer()
{
    endGame();

    _networkTransport->closeConnection(SteamUser()->GetSteamID());
    _networkTransport = nullptr;
}

void SteamP2PAuthPlayer::OnBeginAuthResponse(ValidateAuthTicketResponse_t *pCallback)
{
    if (_steamID == pCallback->m_SteamID)
    {
        LOG("P2P:: Received steam response for account=%d\n", _steamID.GetAccountID());
        _answerTime = _networkHelper->timeTracker().realTime();
        _bHaveAnswer = true;
        _eAuthSessionResponse = pCallback->m_eAuthSessionResponse;
    }
}

void SteamP2PAuthPlayer::initPlayer(CSteamID steamID)
{
    _steamID = steamID;
    _bSentTicket = false;
    _bSubmittedHisTicket = false;
    _bHaveAnswer = false;
    _connectTime = _networkHelper->timeTracker().realTime();
    _cubTicketIGaveThisUser = 0;
    _cubTicketHeGaveMe = 0;
}

void SteamP2PAuthPlayer::startAuthPlayer()
{
    // send him the ticket if we havent yet
    if (_cubTicketIGaveThisUser == 0)
    {
        _hAuthTicketIGaveThisUser = SteamUser()->GetAuthSessionTicket(_rgubTicketIGaveThisUser, sizeof(_rgubTicketIGaveThisUser), &_cubTicketIGaveThisUser);
    }

    // sample has no retry here
    _networkTransport->sendTicket(_networkHelper, SteamUser()->GetSteamID(), _steamID, _cubTicketIGaveThisUser, _rgubTicketIGaveThisUser);

    _bSentTicket = true;

    // start a timer on this, if we dont get a ticket back within reasonable time, mark him timed out
    _ticketTime = _networkHelper->timeTracker().realTime();
}

bool SteamP2PAuthPlayer::isAuthOk()
{
    if (_steamID.IsValid())
    {
        // Timeout if we fail to establish communication with this player
        if (!_bSentTicket && !_bSubmittedHisTicket)
        {
            if (_networkHelper->timeTracker().realTime() - _connectTime > 30)
            {
//                LOG("P2P:: Nothing received for account=%d\n", _steamID.GetAccountID());
//                return false;
            }
        }

        // first ticket check: if i submitted his ticket - was it good?
        if (_bSubmittedHisTicket && _eBeginAuthSessionResult != k_EBeginAuthSessionResultOK)
        {
//            LOG("P2P:: Ticket from account=%d was bad\n", _steamID.GetAccountID());
//            return false;
        }

        // second ticket check: if the steam backend replied, was that good?
        if (_bHaveAnswer && _eAuthSessionResponse != k_EAuthSessionResponseOK)
        {
//            LOG("P2P:: Steam response for account=%d was bad\n", _steamID.GetAccountID());
//            return false;
        }

        // last: if i sent him a ticket and he has not reciprocated, time out after 30 sec
        if (_bSentTicket && !_bSubmittedHisTicket)
        {
            if (_networkHelper->timeTracker().realTime() - _ticketTime > 30)
            {
//                LOG("P2P:: No ticket received for account=%d\n", _steamID.GetAccountID());
//                return false;
            }
        }
    }
    return true;
}

void SteamP2PAuthPlayer::endGame()
{
    if (_bSentTicket)
    {
        SteamUser()->CancelAuthTicket(_hAuthTicketIGaveThisUser);
        _bSentTicket = false;
    }

    if (_bSubmittedHisTicket)
    {
        SteamUser()->EndAuthSession(_steamID);
        _bSubmittedHisTicket = false;
    }

    if (_steamID.IsValid())
    {
        _networkTransport->closeConnection(_steamID);
    }
}

bool SteamP2PAuthPlayer::handleMessage(MsgP2PSendingTicket* msg)
{
    // message from another player providing his ticket
    // is this message for me?
    if (msg->getSteamID() != _steamID.ConvertToUint64())
    {
        return false;
    }

    _cubTicketHeGaveMe = msg->getTokenLen();
    memcpy(_rgubTicketHeGaveMe, msg->getTokenPtr(), _cubTicketHeGaveMe);
    _eBeginAuthSessionResult = SteamUser()->BeginAuthSession(_rgubTicketHeGaveMe, _cubTicketHeGaveMe, _steamID);
    _bSubmittedHisTicket = true;
    LOG("P2P:: ReceivedTicket from account=%d \n", _steamID.GetAccountID());
    if (!_bSentTicket)
    {
        startAuthPlayer();
    }
    return true;
}

SteamP2PNetworkTransport::SteamP2PNetworkTransport() :
_outgoingPacketAddress(new SteamAddress()),
_CallbackP2PSessionRequest(this, &SteamP2PNetworkTransport::onP2PSessionRequest),
_CallbackP2PSessionConnectFail(this, &SteamP2PNetworkTransport::onP2PSessionConnectFail)
{
    // Empty
}

SteamP2PNetworkTransport::~SteamP2PNetworkTransport()
{
    delete _outgoingPacketAddress;
}

void SteamP2PNetworkTransport::sendTicket(NetworkHelper* networkHelper, CSteamID steamIDFrom, CSteamID steamIDTo, uint32 cubTicket, uint8 *pubTicket)
{
    MsgP2PSendingTicket msg;
    msg.setToken((char *)pubTicket, cubTicket);
    msg.setSteamID(steamIDFrom.ConvertToUint64());

    OutputMemoryBitStream packet(NW_MAX_PACKET_SIZE);
    packet.write(static_cast<uint8_t>(msg.getMessageType()));
    packet.write(msg.getSteamID());
    packet.write(msg.getTokenLen());
    packet.writeBytes(msg.getTokenPtr(), msg.getTokenLen());

    _outgoingPacketAddress->setSteamID(steamIDTo);
    _outgoingPacketAddress->setReliable(true);

    networkHelper->sendPacket(packet, _outgoingPacketAddress);
}

void SteamP2PNetworkTransport::closeConnection(CSteamID steamID)
{
    SteamNetworking()->CloseP2PSessionWithUser(steamID);
}

void SteamP2PNetworkTransport::onP2PSessionRequest(P2PSessionRequest_t *pP2PSessionRequest)
{
    // always accept packets
    // the packet itself will come through when you call SteamNetworking()->ReadP2PPacket()
    SteamNetworking()->AcceptP2PSessionWithUser(pP2PSessionRequest->m_steamIDRemote);
}

void SteamP2PNetworkTransport::onP2PSessionConnectFail(P2PSessionConnectFail_t *pP2PSessionConnectFail)
{
    // we've sent a packet to the user, but it never got through
    // we can just use the normal timeout
}

MsgP2PSendingTicket::MsgP2PSendingTicket() : _messageType(k_EMsgP2PSendingTicket)
{
    // Empty
}

uint8 MsgP2PSendingTicket::getMessageType()
{
    return _messageType;
}

void MsgP2PSendingTicket::setToken(const char *pchToken, uint32 unLen)
{
    _uTokenLen = unLen;

    memcpy(_rgchToken, pchToken, GOW_MIN(unLen, sizeof(_rgchToken)));
}

uint32 MsgP2PSendingTicket::getTokenLen()
{
    return _uTokenLen;
}

const char* MsgP2PSendingTicket::getTokenPtr()
{
    return _rgchToken;
}

void MsgP2PSendingTicket::setSteamID(uint64 ulSteamID)
{
    _ulSteamID = ulSteamID;
}

uint64 MsgP2PSendingTicket::getSteamID()
{
    return _ulSteamID;
}

#endif /* IS_DESKTOP */
