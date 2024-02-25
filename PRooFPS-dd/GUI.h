#pragma once

/*
    ###################################################################################
    GUI.h
    GUI for PRooFPS-dd
    Made by PR00F88, West Whiskhyll Entertainment
    2024
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include <functional>
#include <string>

#include "../../Console/CConsole/src/CConsole.h"

#include "PGE.h"

#include "Config.h"
#include "Maps.h"
#include "Networking.h"

namespace proofps_dd
{

    class GUI
    {
    public:

        enum class MenuState
        {
            None,       /* Menu is not displayed (gaming) */
            Main,
            CreateGame,
            JoinGame,
            Settings,
            Exiting     /* User requested closing the app */
        };

        static constexpr char* CVAR_GUI_MAINMENU = "gui_mainmenu";

        static GUI& getGuiInstance(
            PGE& pge,
            proofps_dd::Config& config,
            proofps_dd::Maps& maps,
            proofps_dd::Networking& networking);   /**< Gets the singleton instance. */

        static const char* getLoggerModuleName();
        static CConsole& getConsole();

        // ---------------------------------------------------------------------------

        void initialize();
        void shutdown();

        const MenuState& getMenuState() const;
        void resetMenuState(bool bExitingFromGameSession);

        void showLoadingScreen(
            int nProgress,
            const std::string& sMapFilename);
        void hideLoadingScreen();
        bool showBgWithLogo();
        bool hideBgWithLogo();

        void textForNextFrame(const std::string& s, int x, int y) const;
        void textPermanent(const std::string& s, int x, int y) const;

    protected:

    private:

        static PGE* m_pPge;
        static Config* m_pConfig;
        static Maps* m_pMaps;
        static Networking* m_pNetworking;
        static MenuState m_currentMenu;

        static float getCenterPosXForText(const std::string& text);
        static void addHintToItemByCVar(std::string& sHint, const PGEcfgVariable& cvar);
        static float calcContentStartY(const float& fContentHeight, const float& fRemainingSpaceY);
        static void drawMainMenu(const float& fRemainingSpaceY);
        static float drawPlayerNameInputBox();
        static void drawCreateGameMenu(const float& fRemainingSpaceY);
        static void drawJoinGameMenu(const float& fRemainingSpaceY);
        static void drawSettingsMenu(const float& fRemainingSpaceY);
        static void drawMainMenuCb(); // this needs to be static, causing a lot of other members also need to be static

        // ---------------------------------------------------------------------------

        static PureObject3D* m_pObjLoadingScreenBg;
        static PureObject3D* m_pObjLoadingScreenLogoImg;

        GUI();
        ~GUI();

        GUI(const GUI&) = delete;
        GUI& operator=(const GUI&) = delete;
        GUI(GUI&&) = delete;
        GUI&& operator=(GUI&&) = delete;

    }; // class GUI

} // namespace proofps_dd
