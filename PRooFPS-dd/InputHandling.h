#pragma once

/*
    ###################################################################################
    InputHandling.h
    Input handling for PRooFPS-dd
    Made by PR00F88, West Whiskhyll Entertainment
    2023
    ###################################################################################
*/

#include <map>
#include "CConsole.h"

#include "PGE.h"

#include "Durations.h"
#include "GameMode.h"
#include "Maps.h"
#include "Player.h"
#include "PRooFPS-dd-packet.h"
#include "Sounds.h"

namespace proofps_dd
{

    class InputHandling
    {
    public:

        enum PlayerAppActionRequest
        {
            None,
            Exit
        };

        static constexpr unsigned int m_nKeyPressOnceWpnHandlingMinumumWaitMilliseconds = 500;
        static constexpr unsigned int m_nKeyPressOnceJumpMinumumWaitMilliseconds = 100;
        static constexpr unsigned int m_nKeyPressSomersaultMaximumWaitMilliseconds = 300;
        static constexpr unsigned int m_nWeaponActionMinimumWaitMillisecondsAfterSwitch = 1000;
        static constexpr unsigned int m_nPlayerAngleYSendIntervalMilliseconds = 100;
        static constexpr unsigned int m_nWeaponAngleZBigChangeSendIntervalMilliseconds = 100;
        static constexpr unsigned int m_nWeaponAngleZSmallChangeSendIntervalMilliseconds = 200;
        static constexpr float m_fWeaponAngleZBigChangeThreshold = 30.f;

        static const char* getLoggerModuleName();

        // ---------------------------------------------------------------------------

        CConsole& getConsole() const;

        InputHandling(
            PGE& pge,
            proofps_dd::Durations& durations,
            std::map<pge_network::PgeNetworkConnectionHandle, proofps_dd::Player>& mapPlayers,
            proofps_dd::Maps& maps,
            proofps_dd::Sounds& sounds);

        InputHandling(const InputHandling&) = delete;
        InputHandling& operator=(const InputHandling&) = delete;
        InputHandling(InputHandling&&) = delete;
        InputHandling&& operator=(InputHandling&&) = delete;

    protected:

        PlayerAppActionRequest handleInputWhenConnectedAndSendUserCmdMove(
            proofps_dd::GameMode& gameMode,
            bool& won,
            proofps_dd::Player& player,
            PureObject3D& objXHair,
            const unsigned int nTickrate,
            const unsigned int nClUpdateRate,
            const unsigned int nPhysicsRateMin);

        PlayerAppActionRequest handleInputWhenDisconnected();

        bool handleUserCmdMoveFromClient(
            pge_network::PgeNetworkConnectionHandle connHandleServerSide,
            const proofps_dd::MsgUserCmdFromClient& msg);

    private:

        static const char* proofps_dd::InputHandling::getMsgAppIdName(const proofps_dd::PRooFPSappMsgId& id);
        static const size_t getLongestMsgAppIdNameLength();

        // ---------------------------------------------------------------------------

        PGE& m_pge;
        proofps_dd::Durations& m_durations;
        std::map<pge_network::PgeNetworkConnectionHandle, proofps_dd::Player>& m_mapPlayers;
        proofps_dd::Maps& m_maps;
        proofps_dd::Sounds& m_sounds;

        proofps_dd::Strafe m_prevStrafe;
        proofps_dd::Strafe m_strafe;
        bool m_bPrevAttack;
        bool m_bAttack;
        bool m_bPrevCrouch;
        bool m_bCrouch;
        TPureFloat m_fLastPlayerAngleYSent;
        TPureFloat m_fLastWeaponAngleZSent;

        
        PlayerAppActionRequest keyboardWhenConnected(
            proofps_dd::GameMode& gameMode,
            bool& won,
            pge_network::PgePacket& pkt, proofps_dd::Player& player,
            const unsigned int nTickrate,
            const unsigned int nClUpdateRate,
            const unsigned int nPhysicsRateMin);

        PlayerAppActionRequest keyboardWhenDisconnected();

        bool mouse(
            proofps_dd::GameMode& gameMode,
            bool& won,
            pge_network::PgePacket& pkt,
            proofps_dd::Player& player,
            PureObject3D& objXHair);

        void updatePlayerAsPerInputAndSendUserCmdMove(
            bool& won,
            pge_network::PgePacket& pkt,
            proofps_dd::Player& player,
            PureObject3D& objXHair);

        void mouseWheel(
            const short int& nMouseWheelChange,
            pge_network::PgePacket& pkt,
            proofps_dd::Player& player);

        void RegTestDumpToFile(
            proofps_dd::GameMode& gameMode,
            proofps_dd::Player& player,
            const unsigned int nTickrate,
            const unsigned int nClUpdateRate,
            const unsigned int nPhysicsRateMin);  // TODO: could be const if m_mapPlayers wouldnt be used with [] operator ...

    }; // class InputHandling

} // namespace proofps_dd