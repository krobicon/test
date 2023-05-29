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
#include "Trigger.cpp"
#include "Movement.cpp"
#include "Weapon.cpp"
#include "X11Utils.cpp"
#include "SimInput.cpp"
#include "ConfigLoader.cpp"
#include <fcntl.h>
#include <linux/uinput.h>

int main(int argc, char *argv[])
{
    ConfigLoader *configLoader = new ConfigLoader();
    /*if (getuid())
    {
        printf("MUST RUN AS ROOT!\n");
        return -1;
    }*/
    if (mem::GetPID() == 0) {
        printf("NOPE!\n");
        return -1;
    }
    
    bool findMatch = false;
	
    Level *level = new Level();
    LocalPlayer *localPlayer = new LocalPlayer();
    X11Utils *x11Utils = new X11Utils();
    SimInput *simInput = new SimInput();
    simInput->setup();
    std::vector<Player *> *players = new std::vector<Player *>;
    for (int i = 0; i < 60; i++) {
        players->push_back(new Player(i));
    }
    Sense *sense = new Sense(configLoader, level, localPlayer, players, x11Utils);
    NoRecoil *noRecoil = new NoRecoil(configLoader, localPlayer, x11Utils, simInput);
    Aimbot *aimbot = new Aimbot(configLoader, level, localPlayer, players, x11Utils);
    Movement *movement = new Movement(level, localPlayer, x11Utils);
    Trigger *trigger = new Trigger(configLoader, level, localPlayer, players, x11Utils, simInput);

    // Main loop
    printf("ksenz\n");
    int counter = 1;
    while (1)
    {
        try
        {
            if (counter % 300 == 0)
                configLoader->reloadFile(); // will attempt to reload config if there have been any updates to it

            // resolve pointers
            localPlayer->markForPointerResolution();
            for (int i = 0; i < players->size(); i++) {
                Player *player = players->at(i);
                player->markForPointerResolution();
            }
	    
	    if (!level->isPlayable() && counter % 1900 == 0) {
		    if (findMatch) {
			    printf("finding \n");
		    }
		    else {
				int k = 0;
			    	int j = 0;
				while (k < 300) {
					simInput->emit(EV_REL, REL_Y, 8);
					simInput->emit(EV_REL, REL_X, -8);
					simInput->emit(EV_SYN, SYN_REPORT, 0);
					k++;
					usleep(10000);
				}
			 	while (j < 22) {
					simInput->emit(EV_REL, REL_Y, -4);
					simInput->emit(EV_REL, REL_X, 4);
					simInput->emit(EV_SYN, SYN_REPORT, 0);
					j++;
					usleep(10000);
				}
			    	simInput->click();
			    	findMatch = true;	
		    }
	    }
		
	    if (level->isPlayable())
	    {
		    if (localPlayer->isDead() && counter % 1000 == 0) {
			        int k = 0;
			    	int j = 0;
			    	int p = 0;
				simInput->emit(EV_KEY, KEY_SPACE, 1);
				simInput->emit(EV_SYN, SYN_REPORT, 0);
				simInput->emit(EV_KEY, KEY_SPACE, 0);
				simInput->emit(EV_SYN, SYN_REPORT, 0);
				usleep(10000);
			    	while (k < 300) {
					simInput->emit(EV_REL, REL_Y, 7);
					simInput->emit(EV_REL, REL_X, -7);
					simInput->emit(EV_SYN, SYN_REPORT, 0);
					k++;
					usleep(10000);
				}
			    	while (j < 120) {
					simInput->emit(EV_REL, REL_Y, -3);
					simInput->emit(EV_REL, REL_X, 7);
					simInput->emit(EV_SYN, SYN_REPORT, 0);
					j++;
					usleep(10000);
				}
			    	simInput->click();
			    	usleep(10000);
			    	while (p < 40) {
					simInput->emit(EV_REL, REL_Y, 10);
					simInput->emit(EV_REL, REL_X, 2);
					simInput->emit(EV_SYN, SYN_REPORT, 0);
					p++;
					usleep(10000);
				}
			    	simInput->click();
			    	usleep(100000);
			    	simInput->click();
			    	usleep(100000);
			    	simInput->click();
		    }
            // run features
		if (configLoader->isSenseOn())
		    sense->update();
		    
		if (!localPlayer->isDead() && !localPlayer->isKnocked())
		{
			findMatch = false;
			if (counter % 2500 == 0 && localPlayer->isGrounded())
				simInput->click();
			movement->update();
			Weapon *localWeapon = new Weapon(localPlayer->getWeaponHandle());
			bool semiauto = localWeapon->isSemiAuto();
			
			if (semiauto) {
			   trigger->update(counter, localWeapon);
			}
			else if (configLoader->isAimbotOn()){
			    aimbot->update(counter);
			}
			//else if (rand() % 3 == 0 && localPlayer->isInAttack()){
			else if (rand() % 7 == 0 && localPlayer->isInAttack()){
			    noRecoil->update();
			}
			else if (!localPlayer->isInAttack()){
			    noRecoil->reset();
			}
			/*else if (counter % 500 == 0){
				printf("Pitch: [%f]. \n", localPlayer->getPitch());
				printf("Zoom FOV: [%f]. \n", localWeapon->getZoomFOV());
				int k = 0;
				while (k < 100){
					simInput->emit(EV_REL, REL_Y, 1);
					simInput->emit(EV_REL, REL_X, 0);
					simInput->emit(EV_SYN, SYN_REPORT, 0);
					usleep(2000);
					k++;
				}
			}*/

			if (localWeapon->getAmmo() == 1 && !semiauto){
				localPlayer->setAttackState(4);
				localPlayer->setReloadState(5);
			}
			else if (localPlayer->getReloadState() == 5){
				localPlayer->setReloadState(4);
			}
		}
	    }

            // all ran fine
            if (counter % 3000 == 0)
            {
                printf("UPDATE[%d] OK. \n", counter);
		counter = 0;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        catch (...)
        {
            printf("SLEEPING[%d]\n", counter);
            std::this_thread::sleep_for(std::chrono::seconds(9));
        }
        counter++;
    }
}
