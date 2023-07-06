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
#include "Farming.cpp"
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
    bool aimSwitch = false;
	
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
    Aimbot *aimbot = new Aimbot(configLoader, level, localPlayer, players, x11Utils, simInput);
    Movement *movement = new Movement(level, localPlayer, x11Utils, simInput);
    Trigger *trigger = new Trigger(configLoader, level, localPlayer, players, x11Utils, simInput);
    Farming *farming = new Farming(level, localPlayer, x11Utils, simInput);

    // Main loop
    printf("reign\n");
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
	    
	    if (configLoader->isFarmingOn()) {
		    farming->update(counter);
	    }
	    
	    if (level->isPlayable()) {
            	// run features
		sense->update(counter);
		    
		if (!localPlayer->isDead() && !localPlayer->isKnocked()) {
			movement->update(counter);
			Weapon *localWeapon = new Weapon(localPlayer->getWeaponHandle());
			bool triggergun = localWeapon->isSemiAuto() && !localWeapon->isBurst();
			//if (counter % 100 == 0) {
				//printf("is climbing?: %d\n", localPlayer->isClimbing());
				//if (localPlayer->isClimbing())
					//printf("climb time? %f\n", localPlayer->getTime() - localPlayer->getWallrunStart());
				//printf("jump down?: %d\n", localPlayer->getJumpDown());
				//printf("jump state?: %d\n", localPlayer->getJumpState());
			//}
			if (configLoader->isAimbotOn()){
				if (counter % 30 == 0) {
					if (rand() % 5 == 0) {
						aimSwitch = !aimSwitch;
					}
					if (!aimSwitch) {
						if (rand() % 2 == 0) {
							aimSwitch = true;
						}
					}
				}
			    if (aimSwitch && rand() % 3 == 0)
			    	aimbot->update(counter, triggergun);
			}
			if (triggergun) {
			   trigger->update(counter, localWeapon);
			}
			else if (rand() % 3 == 0 && localPlayer->isInAttack()) {
			    noRecoil->update();
			}
			else if (!localPlayer->isInAttack()) {
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
			if (localWeapon->getAmmo() == 1 && !triggergun) {
				localPlayer->setAttackState(4);
				localPlayer->setReloadState(5);
			}
			else if (localPlayer->getReloadState() == 5) {
				localPlayer->setReloadState(4);
			}
		}
	    }

            // all ran fine
            if (counter % 3001 == 0) {
                printf("UPDATE[%d] OK. \n", counter);
		counter = 0;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        catch (...) {
            printf("SLEEPING[%d]\n", counter);
            std::this_thread::sleep_for(std::chrono::seconds(10));
        }
        counter++;
    }
}
