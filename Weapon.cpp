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
    float getZoomFOV()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::ZOOM_FOV;
        float result = mem::ReadFloat(ptrLong);
        return result;
    }
    float getReadyTime()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::READY_TIME;
        float result = mem::ReadFloat(ptrLong);
        return result;
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
};
