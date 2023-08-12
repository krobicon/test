#pragma once
#include "LocalPlayer.cpp"
#include "Level.cpp"
#include "X11Utils.cpp"
#include "SimInput.cpp"
#include "ConfigLoader.cpp"

class Farming
{
private:
    Level *m_level;
    LocalPlayer *m_localPlayer;
    X11Utils *m_x11Utils;
    SimInput *m_simInput;

    bool findMatch = false;
	
public:
    Farming(Level *level,
             LocalPlayer *localPlayer,
             X11Utils *x11Utils,
	    SimInput *simInput)
    {
        m_level = level;
        m_localPlayer = localPlayer;
        m_x11Utils = x11Utils;
	m_simInput = simInput;
    }
    void update(int counter)
    {
	if (!m_level->isPlayable() && counter % 100 == 0) {
		    if (findMatch) {
			    printf("finding \n");
		    }
		    else {
				int k = 0;
			    	int j = 0;
				while (k < 125) {
					m_simInput->emit(EV_REL, REL_Y, 16);
					m_simInput->emit(EV_REL, REL_X, -16);
					m_simInput->emit(EV_SYN, SYN_REPORT, 0);
					k++;
					usleep(5000);
				}
			 	while (j < 11) {
					m_simInput->emit(EV_REL, REL_Y, -8);
					m_simInput->emit(EV_REL, REL_X, 8);
					m_simInput->emit(EV_SYN, SYN_REPORT, 0);
					j++;
					usleep(5000);
				}
			    	usleep(100000);
			    	m_simInput->click();
			    	findMatch = true;	
		    }
	    }
	else if (!m_localPlayer->isDead() && !m_localPlayer->isKnocked()){
		findMatch = false;
		if (counter % 2500 == 0 && m_localPlayer->isGrounded() && rand() % 2 == 0)
			m_simInput->click();
	}
	if (m_level->isPlayable() && m_localPlayer->isDead() && counter % 500 == 0) {
				int k = 0;
			    	int j = 0;
			    	int p = 0;
				m_simInput->emit(EV_KEY, KEY_SPACE, 1);
				m_simInput->emit(EV_SYN, SYN_REPORT, 0);
				m_simInput->emit(EV_KEY, KEY_SPACE, 0);
				m_simInput->emit(EV_SYN, SYN_REPORT, 0);
				usleep(10000);
			    	while (k < 400) {
					m_simInput->emit(EV_REL, REL_Y, 5);
					m_simInput->emit(EV_REL, REL_X, -5);
					m_simInput->emit(EV_SYN, SYN_REPORT, 0);
					k++;
					usleep(1500);
				}
			    	while (j < 120) {
					m_simInput->emit(EV_REL, REL_Y, -3);
					m_simInput->emit(EV_REL, REL_X, 7);
					m_simInput->emit(EV_SYN, SYN_REPORT, 0);
					j++;
					usleep(5000);
				}
			    	m_simInput->click();
			    	usleep(1500000);
			    	while (p < 130) {
					m_simInput->emit(EV_REL, REL_Y, 2);
					m_simInput->emit(EV_REL, REL_X, 1);
					m_simInput->emit(EV_SYN, SYN_REPORT, 0);
					p++;
					usleep(75000);
				}
				usleep(700000);
			    	m_simInput->click();
			    	usleep(700000);
			    	m_simInput->click();
			    	usleep(1000000);
			    	m_simInput->click();
				usleep(1000000);
			    	m_simInput->click();
		    }
    }
};
