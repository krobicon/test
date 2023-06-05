#pragma once
#include <iostream>
#include "Memory.cpp"
#include "Offsets.cpp"

class Level
{
public:
    long getBasePointer()
    {
        long basePointer = offsets::REGION + offsets::LEVEL;
        return basePointer;
    }
    long getFpsPointer() {
        long basePointer = offsets::REGION + offsets::FPS_MAX;
        return basePointer;
    }
    int getFpsMax() {
        long fpsPointer = mem::ReadLong(getFpsPointer()+0x6c);
        int result = mem::ReadInt(fpsPointer);
        return result;
    }
    std::string getName()
    {
        long basePointer = getBasePointer();
        std::string result = mem::ReadString(basePointer);
        return result;
    }
    long getGamemodePointer()
    {
        long gamemodePointer = offsets::REGION + offsets::GAMEMODE;
        return gamemodePointer;
    }
    std::string getGamemode()
    {
        long gamemodePointer = mem::ReadLong(getGamemodePointer()+0x58);
        std::string result = mem::ReadString(gamemodePointer);
        return result;
    }
    bool isSpecialMode()
    {
        if (getGamemode().compare("control") == 0 || getGamemode().compare("freedm") == 0)
            return true;
        return false;
    }
    bool isPlayable()
    {
        if (getName().empty())
            return false;
        if (getName().compare("mp_lobby") == 0)
            return false;
        return true;
    }
    bool isTrainingArea()
    {
        if (getName().compare("mp_rr_canyonlands_staging") == 0)
            return true;
        return false;
    }
    void print()
    {
        std::string str;
        str += "Level:\n";
        str += "\tBasePointer:\t\t\t\t" + mem::convertPointerToHexString(getBasePointer()) + "\n";
        str += "\tName:\t\t\t\t\t" + getName() + "\n";
        std::cout << str;
    }
};
