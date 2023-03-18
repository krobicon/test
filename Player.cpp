#pragma once
#include <iostream>
#include "Utils.cpp"
#include "Offsets.cpp"
#include "Memory.cpp"

class Player
{
private:
    int m_entityListIndex;
    float m_lastVisibleTime;
    float m_lastCrosshairTime;
    long m_basePointer = 0;
    long m_bonePointer = 0;
    long getUnresolvedBasePointer()
    {
        long unresolvedBasePointer = offsets::REGION + offsets::ENTITY_LIST + ((m_entityListIndex + 1) << 5);
        return unresolvedBasePointer;
    }
    long getBasePointer()
    {
        if (m_basePointer == 0)
        {
            m_basePointer = mem::ReadLong(getUnresolvedBasePointer());
        }
        return m_basePointer;
    }
    long getBonePointer()
    {
        if (m_bonePointer == 0)
        {
            long basePointer = getBasePointer();
            m_bonePointer = mem::ReadLong(basePointer + offsets::BONES);
        }
        return m_bonePointer;
    }

public:
    Player(int entityListIndex)
    {
        m_entityListIndex = entityListIndex;
    }
    void markForPointerResolution()
    {
        m_basePointer = 0;
    }
    bool isDead()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::LIFE_STATE;
        short result = mem::ReadShort(ptrLong);
        return result > 0;
    }
    bool isKnocked()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::BLEEDOUT_STATE;
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
    std::string getName()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::NAME;
        std::string result = mem::ReadString(ptrLong);
        return result;
    }
    std::string getSignifierName()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::SIGNIFIER_NAME;
        std::string result = mem::ReadString(ptrLong);
        return result;
    }
   bool isDummy()
    {
        if (getSignifierName().compare("npc_dummie") == 0)
            return true;
        return false;
    }
    bool isValid()
    {
        return getBasePointer() > 0 && !isDead();
    }
    std::string getInvalidReason()
    {
        if (getBasePointer() == 0)
            return "Unresolved base pointer";
        else if (isDead())
            return "Player is dead";
        else if (getName().empty())
            return "Name is empty";
        else
            return "Player is valid";
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
    float getBoneZ(int id)
    {
        long bonePointer = getBonePointer();
        uint32_t boneLoc = (id * 0x30);
        float result = mem::ReadFloat(bonePointer + boneLoc + 0xCC + 0x4 + 0xC + 0x4 + 0xC);
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
    int getShieldsValue()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::CURRENT_SHIELDS;
        int result = mem::ReadInt(ptrLong);        
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
    float getLastVisibleTime()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::LAST_VISIBLE_TIME;
        float result = mem::ReadFloat(ptrLong);
        return result;
    }
    bool isVisible(bool update)
    {
        const float lastVisibleTime = getLastVisibleTime();
        const bool isVisible = lastVisibleTime > m_lastVisibleTime;
        /*if (update == true)
        {
            m_lastVisibleTime = lastVisibleTime;
        }*/
        m_lastVisibleTime = lastVisibleTime;
        return isVisible;
    }
    float getLastCrosshairTime()
    {
        long basePointer = getBasePointer();
        long ptrLong = basePointer + offsets::LAST_CROSSHAIR_TIME;
        float result = mem::ReadFloat(ptrLong);
        return result;
    }
    bool isCrosshair()
    {
        const float lastCrosshairTime = getLastCrosshairTime();
        /*if (m_lastCrosshairTime == -1.f)
        {
            m_lastCrosshairTime = lastCrosshairTime;
            printf("CHECK 1 LAST CROSSHAIR TIME: [%f] \n", m_lastCrosshairTime );
            return false;
        }*/
        bool isCrosshair = lastCrosshairTime > m_lastCrosshairTime;
        m_lastCrosshairTime = lastCrosshairTime;
        //printf("CHECK 2 LAST CROSSHAIR TIME: [%f] \n", m_lastCrosshairTime );
        return isCrosshair;
    }
    /*bool isCrosshair()
    {
        const float lastCrosshairTime = getLastCrosshairTime();
        const bool isCrosshair = lastCrosshairTime > m_lastCrosshairTime;
        m_lastCrosshairTime = lastCrosshairTime;
        return isCrosshair;
    }*/
    void print()
    {
        std::cout << "Player[" + std::to_string(m_entityListIndex) + "]:\n";
        std::cout << "\tUnresolvedBasePointer:\t\t\t" + mem::convertPointerToHexString(getUnresolvedBasePointer()) + "\n";
        std::cout << "\tBasePointer:\t\t\t\t" + mem::convertPointerToHexString(getBasePointer()) + "\n";
        std::cout << "\tIsValid:\t\t\t\t" + std::to_string(isValid()) + "\n";
        std::cout << "\tInvalidReason:\t\t\t\t" + getInvalidReason() + "\n";
        if (!isValid())
        {
            std::cout << "\tLocationOriginX:\t\t\t" + utils::convertNumberToString(getLocationX()) + "\n";
            std::cout << "\tLocationOriginY:\t\t\t" + utils::convertNumberToString(getLocationY()) + "\n";
            std::cout << "\tLocationOriginZ:\t\t\t" + utils::convertNumberToString(getLocationZ()) + "\n";
            std::cout << "\tTeamNumber:\t\t\t\t" + utils::convertNumberToString(getTeamNumber()) + "\n";
            std::cout << "\tGlowEnable:\t\t\t\t" + utils::convertNumberToString(getGlowEnable()) + "\n";
            std::cout << "\tGlowThroughWall:\t\t\t" + utils::convertNumberToString(getGlowThroughWall()) + "\n";
        }
    }
};
