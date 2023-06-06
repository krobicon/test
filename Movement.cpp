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
    float startjumpTime;
    bool gliding = false;
    bool longclimb = false;
    float previousTraversal;
    int superglideTimer;
    bool superglideStart = false;
    float onWallOffTmp;
    float onWallTmp;
    int wallJumpNow;
    bool startSg = false;
	
public:
    Movement(Level *level,
             LocalPlayer *localPlayer,
             X11Utils *x11Utils)
    {
        m_level = level;
        m_localPlayer = localPlayer;
        m_x11Utils = x11Utils;
    }
    void update(int counter)
    {
	//don't tap strafe if making a long climb to mantle
	if (m_localPlayer->isClimbing()) {
		auto climbTime = m_localPlayer->getTime() - m_localPlayer->getWallrunStart();
		if (climbTime > 0.8) {
			longclimb = true;
			return;	
		}
	}
	if (longclimb) {
		if (m_localPlayer->getTime() > m_localPlayer->getWallrunClear() + 0.1)
			longclimb = false;
	}
	// auto tap trafe
	if (!m_localPlayer->isGrounded() && !m_localPlayer->isSkydiving() && !longclimb)
	{
		if (jumpstart == false) {
			jumpstart = true;
			strafeTick = 0;
		}
		else if (m_localPlayer->isDucking() || (strafeTick > 20 && strafeTick < 130 && m_localPlayer->getForwardDown() == 33)) {
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
	else if (jumpstart == true && m_localPlayer->isGrounded()) {
		jumpstart = false;
		if (m_localPlayer->getForwardDown() == 0) {
			m_localPlayer->setForwardState(0);
		}
		else if (m_localPlayer->getForwardDown() == 33) {
			m_localPlayer->setForwardState(1);
		}
	}
	/////////////// bunny hop
	else if (m_localPlayer->getJumpDown() == 65) {
		if (counter % 2 == 0)
			m_localPlayer->setJumpState(5);
		else
			m_localPlayer->setJumpState(4);
	}
	
	// auto superglide
	auto worldTime = m_localPlayer->getTime();
	auto hangTime =  worldTime - m_localPlayer->getTraversalStart();
	float traversalProgress = m_localPlayer->getTraversalProgress();
	if (counter % 10 == 0) {
		printf("hangtime: %f \n", hangTime);
	}

	if (traversalProgress > 0.85f && !startSg && hangTime > 0.05f && hangTime < 0.8f && m_localPlayer->getForwardDown()) {
	//start SG
		startjumpTime = worldTime;
		startSg = true;
	}
	
	if (startSg && !gliding) {
	//press button
		m_localPlayer->setJumpState(5);
		if ((worldTime - startjumpTime) >= 0.01) {
			m_localPlayer->setDuckState(5);
			gliding = true;
		}
		printf("how many times? \n");
	}
	else if ((worldTime - startjumpTime) >= 0.5f && startSg){
	//need to release button
		m_localPlayer->setJumpState(4);
		if (m_localPlayer->getDuckDown() != 79) {
			m_localPlayer->setDuckState(4);
		}
		gliding = false;
		startSg = false;
	}
	    
	/*
	float traversalProgress = m_localPlayer->getTraversalProgress();
	if (traversalProgress > 0.85f && traversalProgress < 1.0f) {
		superglideStart = true;
		printf("SUPER~~\n");
	}
	if (superglideStart) {
		superglideTimer++;
		if (superglideTimer == 5)
			m_localPlayer->setJumpState(5);
		else if (superglideTimer == 6)
			m_localPlayer->setDuckState(5);
		else if (superglideTimer == 14) {
			m_localPlayer->setJumpState(4);
			m_localPlayer->setDuckState(4);
			previousTraversal = traversalProgress;
		}
		else if (superglideTimer > 10 && traversalProgress != previousTraversal) {
			superglideStart = false;
			superglideTimer = 0;
		}
	}*/
	//////////////////////////// TEST AUTO WALL JUMP /////////////
	/*float onWall = m_localPlayer->getWallrunStart();
	if (onWall != onWallTmp) {
		if (m_localPlayer->getForwardDown() == 0) {
			wallJumpNow = 1;
			m_localPlayer->setJumpState(5);
		}
	}
	onWallTmp = onWall;

	float onWallOff = m_localPlayer->getWallrunClear();
	if (wallJumpNow == 1) {
		if (onWallOff != onWallOffTmp) {
			wallJumpNow = 0;
			m_localPlayer->setJumpState(4);
		}
	}
	onWallOffTmp = onWallOff;
	/*
	float onEdge = mem.Read<float>(lPlayer + OFFSET_TRAVERSAL_PROGRESS);
	if (traversalProgress != previousTraversal){
		if (mem.Read<int>(g_Base + OFFSET_MOVE_FORWARD) == 0) {
			wallJumpNow = 2;
			mem.Write<int>(g_Base + OFFSET_FORCE_JUMP, 5);
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
	}
	onEdgeTmp = onEdge;

	if (wallJumpNow == 2) {
		if ((mem.Read<uint32_t>(lPlayer + OFFSET_FLAGS) & 0x1) == 1){
			wallJumpNow = 0;
			mem.Write<int>(g_Base + OFFSET_FORCE_JUMP, 4);
		}
	}*/
    }
};
