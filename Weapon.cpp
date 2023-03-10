#pragma once
#include <iostream>
#include "Utils.cpp"
#include "Offsets.cpp"
#include "Memory.cpp"

class Weapon
{
private:
    int m_entityListIndex;
    long m_basePointer = 0;
    long getUnresolvedBasePointer()
    {
        long unresolvedBasePointer = offsets::REGION + offsets::ENTITY_LIST + (m_entityListIndex << 5);
        return unresolvedBasePointer;
    }
    long getBasePointer()
    {
        if (m_basePointer == 0)
            m_basePointer = mem::ReadLong(getUnresolvedBasePointer());
        return m_basePointer;
    }

public:
    Weapon(int entityListIndex)
    {
        m_entityListIndex = entityListIndex;
    }
    void markForPointerResolution()
    {
        m_basePointer = 0;
    }
    int getAmmo()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::AMMO;
        int result = mem::ReadInt(ptrLong);
        return result;
    }
    bool isSemiAuto()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::SEMI_AUTO;
        short result = mem::ReadShort(ptrLong);
        return result > 0;
    }
    std::string getName()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::NAME;
        std::string result = mem::ReadString(ptrLong);
        return result;
    }
    std::string getInvalidReason()
    {
        if (getBasePointer() == 0)
            return "Unresolved base pointer";
        else
            return "Weapon is valid";
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
        long ptrLong = basePointer + offsets::LOCAL_ORIGIN + sizeof(float) + sizeof(float);
        float result = mem::ReadFloat(ptrLong);
        return result;
    }

    int getGlowEnable()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::GLOW_ENABLE;
        int result = mem::ReadInt(ptrLong);
        return result;
    }
    void setGlowEnable(int glowEnable)
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::GLOW_ENABLE;
        mem::WriteInt(ptrLong, glowEnable);
    }
    int getGlowThroughWall()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::GLOW_THROUGH_WALL;
        int result = mem::ReadInt(ptrLong);
        return result;
    }
    void setGlowThroughWall(int glowThroughWall)
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::GLOW_THROUGH_WALL;
        mem::WriteInt(ptrLong, glowThroughWall);
    }
    int getGlowColorRed()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::GLOW_COLOR;
        int result = mem::ReadInt(ptrLong);
        return result;
    }
    void setGlowColorRed(float color)
    {
        if (color > 100)
            color = 100;
        if (color < 0)
            color = 0;
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::GLOW_COLOR;
        mem::WriteFloat(ptrLong, color);
    }
    void setGlowColorGreen(float color)
    {
        if (color > 100)
            color = 100;
        if (color < 0)
            color = 0;
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::GLOW_COLOR + sizeof(float);
        mem::WriteFloat(ptrLong, color);
    }
    void setGlowColorBlue(float color)
    {
        if (color > 100)
            color = 100;
        if (color < 0)
            color = 0;
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::GLOW_COLOR + sizeof(float) + sizeof(float);
        ;
        mem::WriteFloat(ptrLong, color);
    }
};
