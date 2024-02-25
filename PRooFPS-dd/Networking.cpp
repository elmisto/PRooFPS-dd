/*
    ###################################################################################
    Networking.cpp
    Network handling for PRooFPS-dd
    Made by PR00F88, West Whiskhyll Entertainment
    2023
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "stdafx.h"  // PCH

#include <cassert>
#include <chrono>

#include "Networking.h"
#include "PRooFPS-dd-packet.h"


// ############################### PUBLIC ################################


proofps_dd::Networking::Networking(
    PGE& pge,
    proofps_dd::Durations& durations) : 
    m_nServerSideConnectionHandle(pge_network::ServerConnHandle),
    m_pge(pge),
    m_durations(durations)
{
    // note that the following should not be touched here as they are not fully constructed when we are here:
    // pge, durations
    // But they can be used in other functions.

    // Since this class is used to build up the PRooFPSddPGE class which is derived from PGE class, PGE is not yet initialized
    // when this ctor is invoked. PRooFPSddPGE initializes PGE later. Furthermore, even the pimpl object inside PGE might not
    // be existing at this point, only isGameRunning() is safe to call. The following assertion is reminding me of that:
    assert(!pge.isGameRunning());
}

bool proofps_dd::Networking::reinitialize()
{
    return m_pge.getNetwork().reinitialize();
}

bool proofps_dd::Networking::isServer() const
{
    return m_pge.getNetwork().isServer();
}

CConsole& proofps_dd::Networking::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
}

const char* proofps_dd::Networking::getLoggerModuleName()
{
    return "Networking";
}


// ############################## PROTECTED ##############################


bool proofps_dd::Networking::isMyConnection(const pge_network::PgeNetworkConnectionHandle& connHandleServerSide) const
{
    // TODO: it would be much better if this function was part of PGE and not application.
    // However, before any refactor could be done, PgeGnsClient::m_hConnectionServerSide should be properly filled, and then
    // getters could be added to classes to retrieve this info. Then we can have a function in PGE which can tell if this
    // is our connection or not.
    return m_nServerSideConnectionHandle == connHandleServerSide;
}

void proofps_dd::Networking::allowListAppMessages()
{
    m_pge.getNetwork().getClient().getAllowListedAppMessages().clear();
    m_pge.getNetwork().getServer().getAllowListedAppMessages().clear();
    
    // following messages are received/sent by both clients and server over network:
    m_pge.getNetwork().getClient().getAllowListedAppMessages().insert(static_cast<pge_network::MsgApp::TMsgId>(proofps_dd::MsgUserNameChange::id));
    m_pge.getNetwork().getServer().getAllowListedAppMessages().insert(static_cast<pge_network::MsgApp::TMsgId>(proofps_dd::MsgUserNameChange::id));

    if (m_pge.getNetwork().isServer())
    {
        m_pge.getNetwork().getServer().getAllowListedAppMessages().insert(static_cast<pge_network::MsgApp::TMsgId>(proofps_dd::MsgUserCmdFromClient::id));
    }
    else
    {
        // MsgMapChangeFromServer is also processed by server, but it injects this pkt into its own queue when needed.
        // MsgMapChangeFromServer MUST NOT be received by server over network!
        // MsgMapChangeFromServer is received only by clients over network!
        m_pge.getNetwork().getClient().getAllowListedAppMessages().insert(static_cast<pge_network::MsgApp::TMsgId>(proofps_dd::MsgMapChangeFromServer::id));

        // MsgUserSetupFromServer is also processed by server, but it injects this pkt into its own queue when needed.
        // MsgUserSetupFromServer MUST NOT be received by server over network!
        // MsgUserSetupFromServer is received only by clients over network!
        m_pge.getNetwork().getClient().getAllowListedAppMessages().insert(static_cast<pge_network::MsgApp::TMsgId>(proofps_dd::MsgUserSetupFromServer::id));

        // MsgUserUpdateFromServer is also processed by server, but it injects this pkt into its own queue when needed.
        // MsgUserUpdateFromServer MUST NOT be received by server over network!
        // MsgUserUpdateFromServer is received only by clients over network!
        m_pge.getNetwork().getClient().getAllowListedAppMessages().insert(static_cast<pge_network::MsgApp::TMsgId>(proofps_dd::MsgUserUpdateFromServer::id));

        // following messages are received only by clients over network:
        m_pge.getNetwork().getClient().getAllowListedAppMessages().insert(static_cast<pge_network::MsgApp::TMsgId>(proofps_dd::MsgBulletUpdateFromServer::id));
        m_pge.getNetwork().getClient().getAllowListedAppMessages().insert(static_cast<pge_network::MsgApp::TMsgId>(proofps_dd::MsgMapItemUpdateFromServer::id));
        m_pge.getNetwork().getClient().getAllowListedAppMessages().insert(static_cast<pge_network::MsgApp::TMsgId>(proofps_dd::MsgWpnUpdateFromServer::id));
        m_pge.getNetwork().getClient().getAllowListedAppMessages().insert(static_cast<pge_network::MsgApp::TMsgId>(proofps_dd::MsgCurrentWpnUpdateFromServer::id));
    }
}


// ############################### PRIVATE ###############################
