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
#include "Movement.cpp"
#include "Weapon.cpp"
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
    Movement *movement = new Movement(level, localPlayer, x11Utils);

    // Main loop
    printf("Ksenz STARTING MAIN LOOP\n");
    int counter = 0;
    while (1)
    {
        try
        {
            if (counter % 300 == 0)
                configLoader->reloadFile(); // will attempt to reload config if there have been any updates to it

            // resolve pointers
            localPlayer->markForPointerResolution();
            for (int i = 0; i < players->size(); i++)
            {
                Player *player = players->at(i);
                player->markForPointerResolution();
            }
	    
	    if (level->isPlayable())
	    {
            // run features
		if (configLoader->isSenseOn())
		    sense->update();
		    
		if (!localPlayer->isDead() && !localPlayer->isKnocked())
		{
			//Weapon *localWeapon = new Weapon(localPlayer->getWeaponHandle());
			if (configLoader->isAimbotOn())
			    aimbot->update(counter);

			if (configLoader->isNorecoilOn())
			    noRecoil->update();
			
			movement->update();
			/*if (localWeapon->getAmmo() == 1 && !localWeapon->isSemiAuto())
			{
				localPlayer->setAttackState(4);
				localPlayer->setReloadState(5);
			}
			else if (localPlayer->getReloadState() == 5)
			{
				localPlayer->setReloadState(4);
			}
			/*else if (localWeapon->getAmmo() == 3 && localWeapon->isSemiAuto())
			{
				x11Utils->mouseClick(1);
			}
			printf("READY TIME: [%f] \n", localWeapon->getReadyTime());*/
		}
	    }

            // all ran fine
            if (counter % 5000 == 0)
            {
                printf("UPDATE[%d] OK. \n", counter);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        catch (...)
        {
            printf("UPDATE[%d] ERROR (LOADING SCREEN?). SLEEPING FOR 10 SECONDS\n", counter);
            std::this_thread::sleep_for(std::chrono::seconds(10));
        }
        counter++;
        if (counter > 10000)
            counter = 0;
    }
}
