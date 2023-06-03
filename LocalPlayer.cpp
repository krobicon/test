#pragma once
#include <iostream>
#include "Utils.cpp"
#include "Memory.cpp"
#include "Offsets.cpp"

class LocalPlayer
{
private:
    long m_basePointer = 0;
    long m_bonePointer = 0;
    long getUnresolvedBasePointer()
    {
        long unresolvedBasePointer = offsets::REGION + offsets::LOCAL_PLAYER;
        return unresolvedBasePointer;
    }
    long getBasePointer()
    {
        if (m_basePointer == 0)
            m_basePointer = mem::ReadLong(getUnresolvedBasePointer());
        return m_basePointer;
    }

public:
    void markForPointerResolution()
    {
        m_basePointer = 0;
    }
    int getWeaponHandle()
    {
	long basePointer = getBasePointer();
	long ptrLong = basePointer + offsets::WEAPON;
	ulong wepHandle = mem::ReadLong(ptrLong);
	return wepHandle & 0xFFFF;
    }
    long getWeaponEntity()
    {
	int wepHandle = getWeaponHandle();
	long unresolvedBasePointer = offsets::REGION + offsets::ENTITY_LIST + (wepHandle << 5);
	return mem::ReadLong(unresolvedBasePointer);
    }
    int getAmmo()
    {
	long basePointer = getWeaponEntity();
	long ptrLong = basePointer + offsets::AMMO;
	int result = mem::ReadInt(ptrLong);
	return result;
    }
    float getTime() {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::TIME;
        float result = mem::ReadFloat(ptrLong);
        return result;
    }
    float getTraversalStart() {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::TRAVERSAL_START;
        float result = mem::ReadFloat(ptrLong);
        return result;
    }
    float getTraversalProgress() {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::TRAVERSAL_PROGRESS;
        float result = mem::ReadFloat(ptrLong);
        return result;
    }
    float getLocationX()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::LOCAL_ORIGIN;
        float result = mem::ReadFloat(ptrLong);
        return result;
    }
    float getLocationY()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::LOCAL_ORIGIN + sizeof(float);
        float result = mem::ReadFloat(ptrLong);
        return result;
    }
    float getLocationZ()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::LOCAL_ORIGIN + (sizeof(float) * 2);
        float result = mem::ReadFloat(ptrLong);
        return result;
    }
    float getCameraZ()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::CAMERA_POS + (sizeof(float) * 2);
	float result = mem::ReadFloat(ptrLong);
        return result;
    }
    int getTeamNumber()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::TEAM_NUMBER;
        int result = mem::ReadInt(ptrLong);
        return result;
    }
    int getTeamSpecial()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::TEAM_NUMBER;
        int result = mem::ReadInt(ptrLong);
	if (result % 2){
		return 1;
	}
	else {
		return 2;
	}
    }
    bool isInAttack()
    {
        long basePointer = getBasePointer();
        long ptrLong = offsets::REGION + offsets::IN_ATTACK;
        int result = mem::ReadInt(ptrLong);
        return result > 0;
    }
    int getAttackState()
    {
        long basePointer = getBasePointer();
        long ptrLong = offsets::REGION + offsets::IN_ATTACK + 0x8;
        int result = mem::ReadInt(ptrLong);
        return result;
    }
    void setAttackState(int state)
    {
        long basePointer = getBasePointer();
        long ptrLong = offsets::REGION + offsets::IN_ATTACK + 0x8;
        mem::WriteInt(ptrLong, state);
    }
    bool isWalking()
    {
        long basePointer = getBasePointer();
        long ptrLong = offsets::REGION + offsets::IN_WALK;
        int result = mem::ReadInt(ptrLong);
        return result > 0;
    }
    std::string getName()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::NAME;
        std::string result = mem::ReadString(ptrLong);
        return result;
    }
    bool isKnocked()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::BLEEDOUT_STATE;
        short result = mem::ReadShort(ptrLong);
        return result > 0;
    }
    bool isDead()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::LIFE_STATE;
        short result = mem::ReadShort(ptrLong);
        return result > 0;
    }
    bool isSkydiving()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::SKYDIVE_STATE;
        short result = mem::ReadShort(ptrLong);
        return result > 0;
    }
    bool isDucking()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::DUCK_STATE;
        short result = mem::ReadShort(ptrLong);
        return result > 0;
    }
    int getForwardDown()
    {
        long basePointer = getBasePointer();
        long ptrLong = offsets::REGION + offsets::IN_FORWARD;
        int result = mem::ReadInt(ptrLong);
        return result;
    }
    int getForwardState()
    {
        long basePointer = getBasePointer();
        long ptrLong = offsets::REGION + offsets::IN_FORWARD + 0x8;
        int result = mem::ReadInt(ptrLong);
        return result;
    }
    void setForwardState(int state)
    {
        long basePointer = getBasePointer();
        long ptrLong = offsets::REGION + offsets::IN_FORWARD + 0x8;
        mem::WriteInt(ptrLong, state);
    }
    int getJumpDown()
    {
        long basePointer = getBasePointer();
        long ptrLong = offsets::REGION + offsets::IN_JUMP;
        int result = mem::ReadInt(ptrLong);
        return result;
    }
    int getJumpState()
	{
	long basePointer = getBasePointer();
	long ptrLong = offsets::REGION + offsets::IN_JUMP + 0x8;
	int result = mem::ReadInt(ptrLong);
        return result;
    }
    void setJumpState(int state)
    {
        long basePointer = getBasePointer();
        long ptrLong = offsets::REGION + offsets::IN_JUMP + 0x8;
        mem::WriteInt(ptrLong, state);
    }
    int getDuckDown()
    {
        long basePointer = getBasePointer();
        long ptrLong = offsets::REGION + offsets::IN_DUCK;
        int result = mem::ReadInt(ptrLong);
        return result;
    }
    int getDuckState()
	{
	long basePointer = getBasePointer();
	long ptrLong = offsets::REGION + offsets::IN_DUCK + 0x8;
	int result = mem::ReadInt(ptrLong);
        return result;
    }
    void setDuckState(int state)
    {
        long basePointer = getBasePointer();
        long ptrLong = offsets::REGION + offsets::IN_DUCK + 0x8;
        mem::WriteInt(ptrLong, state);
    }
    int getReloadDown()
    {
        long basePointer = getBasePointer();
        long ptrLong = offsets::REGION + offsets::IN_RELOAD;
        int result = mem::ReadInt(ptrLong);
        return result;
    }
    int getReloadState()
    {
        long basePointer = getBasePointer();
        long ptrLong = offsets::REGION + offsets::IN_RELOAD + 0x8;
        int result = mem::ReadInt(ptrLong);
        return result;
    }
    void setReloadState(int state)
    {
        long basePointer = getBasePointer();
        long ptrLong = offsets::REGION + offsets::IN_RELOAD + 0x8;
        mem::WriteInt(ptrLong, state);
    }
    float getPunchPitch()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::VEC_PUNCH_WEAPON_ANGLE;
        float result = mem::ReadFloat(ptrLong);
        return result;
    }
    float getPunchYaw()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::VEC_PUNCH_WEAPON_ANGLE + sizeof(float);
        float result = mem::ReadFloat(ptrLong);
        return result;
    }
    float getPitch()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::VIEW_ANGLE;
        float result = mem::ReadFloat(ptrLong);
        return result;
    }
    /*void setPitch(float angle)
    {
        if (angle > 90 || angle < -90)
            return;
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::VIEW_ANGLE;
        mem::WriteFloat(ptrLong, angle);
    }*/
    float getYaw()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::VIEW_ANGLE + sizeof(float);
        float result = mem::ReadFloat(ptrLong);
        return result;
    }
    /*void setYaw(float angle)
    {
        if (angle > 180 || angle < -180)
            return;
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::VIEW_ANGLE + sizeof(float);
        mem::WriteFloat(ptrLong, angle);
    }*/
    bool isZooming()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::ZOOMING;
        short result = mem::ReadShort(ptrLong);
        return result > 0;
    }
    bool isGrounded()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::FLAGS;
        uint32_t result = mem::ReadUint32(ptrLong);
        return (result & 0x1) != 0;
    }
    float getSpecYaw()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::SPEC_YAW;
        float result = mem::ReadFloat(ptrLong);
        return result;
    }
};
