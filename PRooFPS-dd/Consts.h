#pragma once

/*
    ###################################################################################
    Consts.h
    Common Constants not fitting into a single place
    Made by PR00F88, West Whiskhyll Entertainment
    2022 - 2023
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

namespace proofps_dd
{

    static const std::string GAME_NAME = "PRooFPS-dd";
    static const std::string GAME_VERSION = "0.1.3.0 Private Beta";

    static const float GAME_BLOCK_SIZE_X = 1.0f;
    static const float GAME_BLOCK_SIZE_Y = 1.0f;
    static const float GAME_BLOCK_SIZE_Z = 1.0f;

    static const float GAME_GRAVITY_MIN = -0.3f;
    static const float GAME_GRAVITY_MAX = 0.4f;

    static const float GAME_PLAYER_W = 0.95f;
    static const float GAME_PLAYER_H = 1.88f;
    static const unsigned GAME_PLAYER_RESPAWN_SECONDS = 3;

    static constexpr char* GAME_WPN_DEFAULT = "pistol.txt";

    static constexpr char* GAME_AUDIO_DIR = "gamedata/audio/";
    static constexpr char* GAME_MAPS_DIR = "gamedata/maps/";
    static constexpr char* GAME_TEXTURES_DIR = "gamedata/textures/";
    static constexpr char* GAME_WEAPONS_DIR = "gamedata/weapons/";

    static constexpr char* GAME_REG_TEST_DUMP_FILE_SERVER = "RegTestDumpServer.txt";
    static constexpr char* GAME_REG_TEST_DUMP_FILE_CLIENT = "RegTestDumpClient.txt";

} // namespace proofps_dd
