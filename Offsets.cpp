// https://pastebin.com/raw/mkJxaGuW (good on march 9 2023)
// https://pastebin.com/raw/bPAkJfRz (may 26)
// https://pastebin.com/raw/Rydupvup (june 1)

#pragma once
namespace offsets
{
    // buttons
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    const long IN_ATTACK = 0x07472e98;  // [Buttons] -> in_attack
    const long IN_FORWARD = 0x074730d8; // [Buttons] -> in_forward
    const long IN_BACKWARD = 0x07473100;
    const long IN_JUMP = 0x07472fa8;    // [Buttons] -> in_jump
    const long IN_WALK = 0x07473068;    // [Buttons] -> in_walk
    const long IN_RELOAD = 0x07472f18;  // [Buttons] -> in_reload
    const long IN_DUCK = 0x07473098;    // [Buttons] -> in_duck
    // core
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    const long REGION = 0x140000000;              // [Mine]          -> Region
    const long LEVEL = 0x16eed90;                 // [Miscellaneous] -> LevelName
    const long LOCAL_PLAYER = (0x2224528); // [Miscellaneous] -> AVC_GameMovement+0x8 STILL NEED UPDATE
    const long ENTITY_LIST = 0x1e743a8;           // [Miscellaneous] -> cl_entitylist
    const long GAMEMODE = (0x0225d3a0 + 0x58);             // [ConVars]       -> mp_gamemode+0x58 (not sur eif you ened to add 0x58)
    // local player
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    const long LOCAL_ORIGIN = 0x0188;               // [DataMap.CBaseViewModel]     -> m_localOrigin
    const long CAMERA_POS = 0x1f80;                 // [Miscellaneous]              -> CPlayer!camera_origin
    const long TIME = 0x2138;                       // [DataMap.C_Player]           -> m_currentFramePlayer.timeBase
    const long TRAVERSAL_START = 0x2ba0;            // [DataMap.C_Player]           -> m_traversalStartTime
    const long TRAVERSAL_PROGRESS = 0x2b9c;         // [DataMap.C_Player]           -> m_traversalProgress
    const long WALLRUN_START = 0x35fc;              // [DataMap.C_Player]           -> m_wallRunStartTime
    const long WALLRUN_CLEAR = 0x3600;              // [DataMap.C_Player]           -> m_wallRunClearTime
    const long FPS_MAX = 0x01626b60;                // [ConVars]                    -> fps_max
    // player entity
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    const long GLOW_ENABLE = (0x03f0 + 0x8);        // [RecvTable.DT_HighlightSettings] -> m_highlightServerContextID + 0x8
    const long GLOW_THROUGH_WALL = (0x03f0 + 0x10); // [RecvTable.DT_HighlightSettings] -> m_highlightServerContextID + 0x10
    const long GLOW_COLOR = 0x01e8;                  // [RecvTable.DT_HighlightSettings] -> m_highlightParams + 0x18
    const long TEAM_NUMBER = 0x0480;                // [RecvTable.DT_BaseEntity]        -> m_iTeamNum
    const long NAME = 0x05c1;                       // [RecvTable.DT_BaseEntity]        -> m_iName
    const long LIFE_STATE = 0x07d0;                 // [RecvTable.DT_Player]            -> m_lifeState
    const long VEC_PUNCH_WEAPON_ANGLE = 0x24e8;     // [DataMap.C_Player]               -> m_currentFrameLocalPlayer.m_vecPunchWeapon_Angle
    const long VIEW_ANGLE = (0x25e4 - 0x14);        // [DataMap.C_Player]               -> m_ammoPoolCapacity - 0x14
    const long BLEEDOUT_STATE = 0x2790;             // [RecvTable.DT_Player]            -> m_bleedoutState
    const long FLAGS = 0x00c8;                      // [RecvTable.DT_Player]            -> m_fFlags
    const long ZOOMING = 0x1c81;                    // [RecvTable.DT_Player]            -> m_bZooming
    const long DUCK_STATE = 0x2a8c;                 // [RecvTable.DT_Player]            -> m_duckState
    const long LAST_VISIBLE_TIME = 0x1AA0;          // [Miscellaneous]                  -> CPlayer!lastVisibleTime
    const long START_CROSSHAIR_TIME = 0x1AA4;       // [Miscellaneous]                  -> CPlayer!lastCrosshairTargetTime - 0x4
    const long LAST_CROSSHAIR_TIME = 0x1AA8;        // [Miscellaneous]                  -> CPlayer!lastCrosshairTargetTime
    const long CURRENT_SHIELDS = 0x01a0;            // [RecvTable.DT_BaseEntity]        -> m_shieldHealth
    const long SKYDIVE_STATE = 0x46f0;              // [RecvTable.DT_Player]            -> m_skydiveState
    const long BONES = (0x0ec8 + 0x48);             // [RecvTable.DT_BaseAnimating]     -> m_nForceBone + 0x48
    const long SIGNIFIER_NAME = 0x05b8;             // [RecvTable.DT_BaseEntity]        -> m_iSignifierName
    const long SPEC_YAW =  (0x22ec - 0x8);          // [DataMap.C_Player]               -> m_currentFramePlayer.m_ammoPoolCount - 0x8
    // weapon stuff
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    const long WEAPON = 0x1a44;                     // [RecvTable.DT_BaseCombatCharacter]     -> m_latestPrimaryWeapons
    const long READY_TIME = 0x1668;                 // [RecvTable.DT_WeaponX_LocalWeaponData] -> m_nextReadyTime
    const long AMMO = 0x1690;                       // [RecvTable.DT_WeaponX_LocalWeaponData] -> m_ammoInClip
    const long AMMO_STOCKPILE = 0x1694;             // [RecvTable.DT_WeaponX_LocalWeaponData] -> m_ammoInStockpile
    const long SEMI_AUTO = 0x1c2c;                  // [Miscellaneous]                        -> CWeaponX!m_isSemiAuto STILL NEED UPDATE
    const long BURST = 0x17f8;                      // [DataMap.CWeaponX]                     -> m_burstFireCount
    const long ZOOM_FOV = (0x16e0 + 0x00bc);        // [DataMap.WeaponPlayerData]             -> m_playerData + m_targetZoomFOV
}
