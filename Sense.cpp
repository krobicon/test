#pragma once
#include <vector>
#include "LocalPlayer.cpp"
#include "Player.cpp"
#include "Math.cpp"
#include "Level.cpp"
#include "X11Utils.cpp"
#include "ConfigLoader.cpp"

class Sense
{
private:
    ConfigLoader *m_configLoader;
    Level *m_level;
    LocalPlayer *m_localPlayer;
    std::vector<Player *> *m_players;
    X11Utils *m_x11Utils;

public:
    Sense(ConfigLoader *configLoader,
          Level *level,
          LocalPlayer *localPlayer,
          std::vector<Player *> *players,
          X11Utils *x11Utils)
    {
        m_configLoader = configLoader;
        m_level = level;
        m_localPlayer = localPlayer;
        m_players = players;
        m_x11Utils = x11Utils;
    }
    void update(int counter)
    {
        for (int i = 0; i < m_players->size(); i++)
        {
            Player *player = m_players->at(i);
            if (!player->isValid() || player->isDead())
                continue;
            if (m_level->isSpecialMode() && player->getTeamSpecial() == m_localPlayer->getTeamSpecial() && !m_configLoader->isGunrunOn())
                continue;
            if (player->getTeamNumber() == m_localPlayer->getTeamNumber())
               continue;
            
            double distanceToTarget = math::calculateDistanceInMeters(m_localPlayer->getLocationX(),
                                                          m_localPlayer->getLocationY(),
                                                          m_localPlayer->getLocationZ(),
                                                          player->getLocationX(),
                                                          player->getLocationY(),
                                                          player->getLocationZ());
            
            if (distanceToTarget < 300)
            {
                if (player->isVisible(true))
                {
                    player->setGlowEnable(5);
                    player->setGlowThroughWall(1);
                    //player->setGlowColorRed(0);
                    //player->setGlowColorGreen(3);
                    //player->setGlowColorBlue(0);
                }
                else if (m_configLoader->isSenseOn())
                {
                    /*const int enemyShields = player->getShieldsValue();
                    int r, g, b;
                    if (enemyShields >= 120)
                    {
                        r = 3;
                        g = 0;
                        b = 0;
                    }
                    else if (enemyShields >= 100)
                    {
                        r = 1.5;
                        g = 0;
                        b = 1.5;
                    }
                    else if (enemyShields >= 75)
                    {
                        r = 0;
                        g = 1;
                        b = 2;
                    }
                    else
                    {
                        r = 1;
                        g = 1;
                        b = 1;
                    }*/
                    player->setGlowEnable(7);
                    player->setGlowThroughWall(2);
                    //player->setGlowEnable(2);
                    //player->setGlowThroughWall(5);
                    //player->setGlowColorRed(r);
                    //player->setGlowColorGreen(g);
                    //player->setGlowColorBlue(b);
                }
                else if (player->getGlowEnable() == 5) {
                    player->setGlowEnable(2);
                    player->setGlowThroughWall(5);
                }
            }
            else if (player->getGlowEnable() == 5 || player->getGlowEnable() == 7)
            {
                player->setGlowEnable(2);
                player->setGlowThroughWall(5);
            }
        }
    }

};
