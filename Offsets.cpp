// https://pastebin.com/raw/mkJxaGuW (good on march 9 2023)
// https://pastebin.com/raw/bPAkJfRz (may 26)
// https://pastebin.com/raw/Rydupvup (june 1)

#pragma once
namespace offsets
{
    // buttons
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    const long IN_ATTACK = 0x07404610;  // [Buttons] -> in_attack
    const long IN_FORWARD = 0x074045a0; // [Buttons] -> in_forward
    const long IN_JUMP = 0x074046a0;    // [Buttons] -> in_jump
    const long IN_WALK = 0x0bd29870;    // [Buttons] -> in_walk
    const long IN_RELOAD = 0x074045d0;  // [Buttons] -> in_reload
    // core
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    const long REGION = 0x140000000;              // [Mine]          -> Region
    const long LEVEL = 0x165ce80;                 // [Miscellaneous] -> LevelName
    const long LOCAL_PLAYER = (0x0213a640 + 0x8); // [Miscellaneous] -> AVC_GameMovement+0x8
    const long ENTITY_LIST = 0x1d89048;           // [Miscellaneous] -> cl_entitylist
    const long GAMEMODE = 0x02173850;             // [ConVars]       -> mp_gamemode+0x58
    // entity
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    const long LOCAL_ORIGIN = 0x0158;               // [DataMap.CBaseViewModel]     -> m_localOrigin
    const long CAMERA_POS = 0x1f58;                 // [Miscellaneous]              -> CPlayer!camera_origin
    // player
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    const long GLOW_ENABLE = (0x03c0 + 0x8);        // [RecvTable.DT_HighlightSettings] -> m_highlightServerContextID + 0x8
    const long GLOW_THROUGH_WALL = (0x03c0 + 0x10); // [RecvTable.DT_HighlightSettings] -> m_highlightServerContextID + 0x10
    const long GLOW_COLOR = 0x1d0;                  // [RecvTable.DT_HighlightSettings] -> m_highlightParams + 0x18
    const long TEAM_NUMBER = 0x044c;                // [RecvTable.DT_BaseEntity]        -> m_iTeamNum
    const long NAME = 0x0589;                       // [RecvTable.DT_BaseEntity]        -> m_iName
    const long LIFE_STATE = 0x0798;                 // [RecvTable.DT_Player]            -> m_lifeState
    const long VEC_PUNCH_WEAPON_ANGLE = 0x24c0;     // [DataMap.C_Player]               -> m_currentFrameLocalPlayer.m_vecPunchWeapon_Angle
    const long VIEW_ANGLE = (0x25bc - 0x14);        // [DataMap.C_Player]               -> m_ammoPoolCapacity - 0x14
    const long BLEEDOUT_STATE = 0x2750;             // [RecvTable.DT_Player]            -> m_bleedoutState
    const long FLAGS = 0x0098;                      // [RecvTable.DT_Player]            -> m_fFlags
    const long ZOOMING = 0x1c61;                    // [RecvTable.DT_Player]            -> m_bZooming
    const long DUCK_STATE = 0x2a4c;                 // [RecvTable.DT_Player]            -> m_duckState
    const long LAST_VISIBLE_TIME = 0x1A80;          // [Miscellaneous]                  -> CPlayer!lastVisibleTime
    const long START_CROSSHAIR_TIME = 0x1A84;       // [Miscellaneous]                  -> CPlayer!lastCrosshairTargetTime - 0x4
    const long LAST_CROSSHAIR_TIME = 0x1A88;        // [Miscellaneous]                  -> CPlayer!lastCrosshairTargetTime
    const long CURRENT_SHIELDS = 0x0170;            // [RecvTable.DT_BaseEntity]        -> m_shieldHealth
    const long SKYDIVE_STATE = 0x46a4;              // [RecvTable.DT_Player]            -> m_skydiveState
    const long BONES = (0x0e98 + 0x48);             // [RecvTable.DT_BaseAnimating]     -> m_nForceBone + 0x48
    const long SIGNIFIER_NAME = 0x0580;             // [RecvTable.DT_BaseEntity]        -> m_iSignifierName
    const long SPEC_YAW =  (0x22c4 - 0x8);          // [DataMap.C_Player]               -> m_currentFramePlayer.m_ammoPoolCount - 0x8
    // weapon stuff
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    const long WEAPON = 0x1a24;                     // [RecvTable.DT_BaseCombatCharacter]     -> m_latestPrimaryWeapons
    const long READY_TIME = 0x1648;                 // [RecvTable.DT_WeaponX_LocalWeaponData] -> m_nextReadyTime
    const long AMMO = 0x1670;                       // [RecvTable.DT_WeaponX_LocalWeaponData] -> m_ammoInClip
    const long SEMI_AUTO = 0x1c14;                  // [Miscellaneous]                        -> CWeaponX!m_isSemiAuto
    const long ZOOM_FOV = (0x16c0 + 0x00bc);          // [DataMap.WeaponPlayerData]             -> m_playerData + m_targetZoomFOV
}
