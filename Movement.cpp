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
    bool startSg = false;
    float startjumpTime;
    bool gliding = false;
	
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
	// auto tap trafe
	if (!m_localPlayer->isGrounded() && !m_localPlayer->isSkydiving())
	{
		if (jumpstart == false)
		{
			jumpstart = true;
			strafeTick = 0;
		}
		else if (m_localPlayer->isDucking() || (strafeTick > 20 && strafeTick < 170 && m_localPlayer->getForwardDown() == 33))
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
		
	// auto superglide
	auto worldTime = m_localPlayer->getTime();
	auto hangTime =  worldTime - m_localPlayer->getTraversalStart();
	float traversalProgress = m_localPlayer->getTraversalProgress();
	if (traversalProgress > 0.87f && !startSg && hangTime > 0.05f && hangTime < 1.2f) {
	//start SG
		startjumpTime = worldTime;
		startSg = true;
	}
	
	else if (startSg && !gliding) {
	//press button
		m_localPlayer->setJumpState(7);
		if ((worldTime - startjumpTime) > 0.007) {
			m_localPlayer->setDuckState(6);
			gliding = true;
		}
		printf("how many times? \n");
	}
	if (( startSg && gliding){
	//need to release button
		m_localPlayer->setJumpState(4);;
		m_localPlayer->setDuckState(4);
		gliding = false;
		startSg = false;
	}
    }
};
