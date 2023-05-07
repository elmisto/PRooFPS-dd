#pragma once

/*
    ###################################################################################
    InputHandling.h
    Input handling for PRooFPS-dd
    Made by PR00F88, West Whiskhyll Entertainment
    2023
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../../../CConsole/CConsole/src/CConsole.h"

#include "../../../PGE/PGE/PGEInputHandler.h"
#include "../../../PGE/PGE/Network/PgeNetwork.h"

#include "Durations.h"
#include "GameMode.h"
#include "Maps.h"
#include "Player.h"
#include "PRooFPS-dd-packet.h"

namespace proofps_dd
{

    class InputHandling
    {
    public:

        static const unsigned int m_nWeaponActionMinimumWaitMillisecondsAfterSwitch = 1000;

        static const char* getLoggerModuleName();

        // ---------------------------------------------------------------------------

        CConsole& getConsole() const;

        InputHandling(
            PGEcfgProfiles& cfg,
            PGEInputKeyboard& keybd,
            PGEInputMouse& mouse,
            pge_network::PgeNetwork& network,
            PR00FsUltimateRenderingEngine& gfx,
            proofps_dd::Durations& m_durations,
            proofps_dd::Maps& maps);

    protected:

        InputHandling(const InputHandling&) :
            m_cfgProfiles(m_cfgProfiles),
            m_keyboard(m_keyboard),
            m_mouse(m_mouse),
            m_network(m_network),
            m_gfx(m_gfx),
            m_durations(m_durations),
            m_maps(m_maps)
        {}

        InputHandling& operator=(const InputHandling&)
        {
            return *this;
        }

        void keyboard(
            proofps_dd::GameMode& gameMode,
            int fps,
            bool& won,
            pge_network::PgePacket& pkt, Player& player);

        bool mouse(
            proofps_dd::GameMode& gameMode,
            int fps,
            bool& won,
            pge_network::PgePacket& pkt,
            proofps_dd::Player& player,
            PureObject3D& objXHair);

    private:

        // ---------------------------------------------------------------------------

        PGEcfgProfiles& m_cfgProfiles;
        PGEInputKeyboard& m_keyboard;
        PGEInputMouse& m_mouse;
        pge_network::PgeNetwork& m_network;
        PR00FsUltimateRenderingEngine& m_gfx;
        proofps_dd::Durations& m_durations;
        proofps_dd::Maps& m_maps;

        bool m_bShowGuiDemo;

        void mouseWheel(
            const short int& nMouseWheelChange,
            pge_network::PgePacket& pkt,
            proofps_dd::Player& player);

        void RegTestDumpToFile(
            proofps_dd::GameMode& gameMode,
            proofps_dd::Player& player);  // TODO: could be const if m_mapPlayers wouldnt be used with [] operator ...

    }; // class InputHandling

} // namespace proofps_dd