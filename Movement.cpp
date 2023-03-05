#pragma once
#include "LocalPlayer.cpp"
#include "Level.cpp"
#include "X11Utils.cpp"

class Movement
{
private:
    Level *m_level;
    LocalPlayer *m_localPlayer;
    X11Utils *m_x11Utils;

    bool jumpstart = false;
    int strafeTick;
	
public:
    Movement(Level *level,
             LocalPlayer *localPlayer,
             X11Utils *x11Utils)
    {
        m_level = level;
        m_localPlayer = localPlayer;
        m_x11Utils = x11Utils;
    }
	void update()
    {
        // validations
        if (!m_level->isPlayable())
            return;
        if (m_localPlayer->isDead())
            return;
        if (m_localPlayer->isKnocked())
            return;
		
		// auto tap trafe
		if (!m_localPlayer->isGrounded())
		{
			if (jumpstart == false)
			{
				jumpstart = true;
				strafeTick = 0;
			}
			else if (m_localPlayer->isDucking() || (strafeTick > 30 && m_localPlayer->getForwardDown() == 33))
			{
				if (m_localPlayer->getForwardState() == 0)
				{
					m_localPlayer->setForwardState(5);
					//printf("Forward State set:[%d] \n", m_localPlayer->getForwardState());
				}
				else
				{
					m_localPlayer->setForwardState(4);
				}
			}
			strafeTick++;
		}
		else if (jumpstart == true && m_localPlayer->isGrounded())
		{
			jumpstart = false;
			if (m_localPlayer->getForwardDown() == 0)
			{
				m_localPlayer->setForwardState(0);
			}
			else if (m_localPlayer->getForwardDown() == 33)
			{
				m_localPlayer->setForwardState(1);
			}
		}
    }
};
