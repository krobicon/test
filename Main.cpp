#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <chrono>
#include <thread>
#include "Utils.cpp"
#include "Offsets.cpp"
#include "Level.cpp"
#include "LocalPlayer.cpp"
#include "Player.cpp"
#include "Sense.cpp"
#include "NoRecoil.cpp"
#include "Aimbot.cpp"
#include "X11Utils.cpp"
#include "ConfigLoader.cpp"

int main(int argc, char *argv[])
{
    ConfigLoader *configLoader = new ConfigLoader();
    if (getuid())
    {
        printf("MUST RUN AS ROOT!\n");
        return -1;
    }
    if (mem::GetPID() == 0)
    {
        printf("GAME NOT FOUND. EXITING!\n");
        return -1;
    }
    Level *level = new Level();
    LocalPlayer *localPlayer = new LocalPlayer();
    X11Utils *x11Utils = new X11Utils();
    std::vector<Player *> *players = new std::vector<Player *>;
    for (int i = 0; i < 60; i++)
    {
        players->push_back(new Player(i));
    }
    Sense *sense = new Sense(configLoader, level, localPlayer, players, x11Utils);
    NoRecoil *noRecoil = new NoRecoil(configLoader, level, localPlayer, players, x11Utils);
    Aimbot *aimbot = new Aimbot(configLoader, level, localPlayer, players, x11Utils);

    // Main loop
    printf("MYAPEX STARTING MAIN LOOP\n");
    int counter = 0;
    bool jumpstart = false;
    int strafeTick;
    while (1)
    {
        try
        {
            if (counter % 200 == 0)
                configLoader->reloadFile(); // will attempt to reload config if there have been any updates to it

            // resolve pointers
            localPlayer->markForPointerResolution();
            for (int i = 0; i < players->size(); i++)
            {
                Player *player = players->at(i);
                player->markForPointerResolution();
            }

            // run features
            if (configLoader->isAimbotOn())
                aimbot->update();

            if (configLoader->isNorecoilOn())
                noRecoil->update();

            if (configLoader->isSenseOn())
                sense->update();

            // all ran fine
            if (counter % 5000 == 0)
            {
                printf("UPDATE[%d] OK. \n", counter);
            }
            if (!localPlayer->isGrounded())
            {
                if (jumpstart == false)
                {
                    jumpstart = true;
                    strafeTick = 0;
                }
                if (strafeTick > 50)
                {
                    if (localPlayer->getForwardState() == 0)
                    {
                        localPlayer->setForwardState(5);
                        printf("Forward State:[%d] \n", localPlayer->getForwardState());
                    }
                    else
                    {
                        localPlayer->setForwardState(4);
                        printf("Forward State:[%d] \n", localPlayer->getForwardState());
                    }
                }
                strafeTick++;
            }
            else
            {
                jumpstart = false;
            }
                 /*if (localPlayer->getForwardDown1())
                {
                    printf("Forward Down1:[%d] \n", localPlayer->getForwardDown1());
                }*/
            /*else
            {
                if (localPlayer->getForwardState() == 4 && localPlayer->getForwardDown1() != 0)
                {
                    localPlayer->setForwardState(5);
                }
                else if(localPlayer->getForwardState() == 5 && localPlayer->getForwardDown1() == 0)
                {
                    localPlayer->setForwardState(4);
                }
            }*/
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        catch (...)
        {
            printf("UPDATE[%d] ERROR (LOADING SCREEN?). SLEEPING FOR 10 SECONDS\n", counter);
            std::this_thread::sleep_for(std::chrono::seconds(10));
        }
        counter++;
        if (counter > 1000)
            counter = 0;
    }
}
