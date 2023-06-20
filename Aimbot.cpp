#pragma once
#include <vector>
#include "LocalPlayer.cpp"
#include "Player.cpp"
#include "Math.cpp"
#include "Level.cpp"
#include "math.h"
#include "Weapon.cpp"
#include "X11Utils.cpp"
#include "ConfigLoader.cpp"

class Aimbot
{
private:
    ConfigLoader *m_configLoader;
    Level *m_level;
    LocalPlayer *m_localPlayer;
    std::vector<Player *> *m_players;
    X11Utils *m_x11Utils;
    SimInput *m_simInput;

    Player *m_lockedOnPlayer = nullptr;
    bool triggerCache = false;
    int pitchpixels = 0;
    int yawpixels = 0;

public:
    Aimbot(ConfigLoader *configLoader,
           Level *level,
           LocalPlayer *localPlayer,
           std::vector<Player *> *players,
           X11Utils *x11Utils,
	   SimInput *simInput)
    {
        m_configLoader = configLoader;
        m_level = level;
        m_localPlayer = localPlayer;
        m_players = players;
        m_x11Utils = x11Utils;
	m_simInput = simInput;
    }
    void update(int counter)
    {
	int smooth = m_configLoader->getAimbotSmoothing() + rand() % 40;
	float fov = m_configLoader->getAimbotActivationFOV();
        // validations
        if (m_localPlayer->isWalking()) {
            m_lockedOnPlayer = nullptr;
            return;
        }
	if (!m_localPlayer->isInAttack() && !m_localPlayer->isZooming()) {
	    m_lockedOnPlayer = nullptr;
	    return;
    	}
	    
        // get desired angle to an enemy
        double desiredViewAngleYaw = 0;
        double desiredViewAnglePitch = 0;
	double distanceToTarget;
	    
	    if (m_lockedOnPlayer == nullptr || !m_lockedOnPlayer->isVisible(false)) {
		m_lockedOnPlayer = findClosestEnemy();
	    }
	    if (m_lockedOnPlayer == nullptr)
		return;
	    distanceToTarget = math::calculateDistanceInMeters(m_localPlayer->getLocationX(),
							       m_localPlayer->getLocationY(),
							       m_localPlayer->getLocationZ(),
							       m_lockedOnPlayer->getLocationX(),
							       m_lockedOnPlayer->getLocationY(),
							       m_lockedOnPlayer->getLocationZ());
	    if (distanceToTarget > m_configLoader->getAimbotMaxRange())
		return;
	    desiredViewAngleYaw = calculateDesiredYaw(m_localPlayer->getLocationX(),
						      m_localPlayer->getLocationY(),
						      m_lockedOnPlayer->getLocationX(),
						      m_lockedOnPlayer->getLocationY());
	    desiredViewAnglePitch = calculateDesiredPitch(m_localPlayer, m_lockedOnPlayer);

        // Setup Angles
        const double pitch = m_localPlayer->getPitch();
        const double pitchAngleDelta = calculatePitchAngleDelta(pitch, desiredViewAnglePitch);
        const double pitchAngleDeltaAbs = abs(pitchAngleDelta);
	    
	const double yaw = m_localPlayer->getYaw();
        const double yawAngleDelta = calculateAngleDelta(yaw, desiredViewAngleYaw);
        const double yawAngleDeltaAbs = abs(yawAngleDelta);


	
	if (distanceToTarget > 14)
	{
		fov = fov * 12 / distanceToTarget;
	}

	if (counter % 10 == 0) {
		printf("target fov: %f \n", fov);
	}
	    
        if (yawAngleDeltaAbs > fov || pitchAngleDeltaAbs > fov / 2)
	{
	    m_lockedOnPlayer = nullptr;
	    return;
	}
	    
	/*printf("distance: %f \n", distanceToTarget);
	printf("fov: %f \n", fov);
	printf("pitch delta: %f \n", pitchAngleDeltaAbs);
	printf("yaw delta: %f \n", yawAngleDeltaAbs);
	printf("----------------------------------------------------------------------------------- \n");*/
	
	//printf("AMMO: [%d] \n", localWeapon->getAmmo());
	//printf("READY TIME: [%f] \n", localWeapon->getReadyTime());
	//printf("SEMI?: [%d] \n", localWeapon->isSemiAuto());
	//printf("TRIGGER: [%d] \n", trigger);
	//printf("SMOOTH: [%d] \n", smooth);
	//printf("FOV: [%d] \n", fov);
	int pixelsPitch = pitchAngleDelta / (0.03);
	int pixelsYaw = yawAngleDelta / (-0.03);
	//pitchpixels += pixPitch;
	bool move = false;
	 
	if (pixelsPitch > -3 && pixelsPitch < 3){
		m_simInput->emit(EV_REL, REL_Y, pixelsPitch);
		move = true;
	}
	else if (pixelsPitch >= 3) {
		m_simInput->emit(EV_REL, REL_Y, 3);
		move = true;
	}
	else if (pixelsPitch <= -3) {
		m_simInput->emit(EV_REL, REL_Y, -3);
		move = true;
	}
	if (abs(pixelsYaw) < 3) {
		m_simInput->emit(EV_REL, REL_X, pixelsYaw);
		yawpixels = 0;
		move = true;
	}
	else if (pixelsYaw >= 3) {
		m_simInput->emit(EV_REL, REL_X, 3);
		move = true;
	}
	else if (pixelsYaw <= -3) {
		m_simInput->emit(EV_REL, REL_X, -3);
		move = true;
	}
	if (move) {
		m_simInput->emit(EV_SYN, SYN_REPORT, 0);
		usleep(2000);
	}
	  
        double newPitch = normalizePitch(pitch + (pitchAngleDelta / smooth));
	double newYaw = normalizeYaw(yaw + (yawAngleDelta / smooth));
        //m_localPlayer->setPitch(newPitch);
        //m_localPlayer->setYaw(newYaw);
    }
    
    double normalizeYaw(double angle)
    {
        double myAngle = angle;
        if (myAngle > 180)
            myAngle -= 360;
        else if (myAngle < -180)
            myAngle += 360;
        return myAngle;
    }
    
    double normalizePitch(double angle)
    {
        double myAngle = angle;
        if (myAngle > 89)
            myAngle -= 180;
        else if (myAngle < -89)
            myAngle += 180;
        return myAngle;
    }
/*
    double pitchDeadzone(double desiredAngle, float xfov)
    {
	double MinX = normalizePitch(desiredAngle - xfov);
	double MaxX = normalizePitch(desiredAngle + xfov);
	double xa = normalizePitch(MinX - pitch);
	double xb = normalizePitch(MaxX - pitch);
	double x = xa * xb < 0 && abs(xa - xb) < 89 ? desiredAngle : abs(xa) < abs(xb) ? MinX : MaxX;
	return x;
    }

    double yawDeadzone(double desiredAngle, float yfov)
    {
	double MinY = normalizeYaw(desiredAngle - yfov);
	double MaxY = normalizeYaw(desiredAngle + yfov);
	double ya = normalizeYaw(MinX - yaw);
	double yb = normalizeYaw(MaxX - yaw);
	double y = ya * yb < 0 && abs(ya - yb) < 180 ? desiredAngle : abs(ya) < abs(yb) ? MinY : MaxY;
	return y;
    }
    
    double GetSmoothPitch(double currentPitch, double deadzonePitch)
    {
        double dx = normalizePitch(currentPitch - deadzonePitch);
        double sx = normalizePitch(currentPitch - dx / m_configLoader->getAimbotSmoothing());
        return sx;
    }
	
    double Vector GetSmoothYaw(double currentYaw, double deadzoneYaw)
    {
	double dy = normalizeYaw(currentYaw - deadzoneYaw);
        double sy = normalizeYaw(currentYaw - dy / m_configLoader->getAimbotSmoothing());
        return sy;
    }*/
    
    double calculatePitchAngleDelta(double oldAngle, double newAngle)
    {
        double wayA = newAngle - oldAngle;
        return wayA;
    }
    double calculateAngleDelta(double oldAngle, double newAngle)
    {
        double wayA = newAngle - oldAngle;
        double wayB = 360 - abs(wayA);
        if (wayA > 0 && wayB > 0)
            wayB *= -1;
        if (abs(wayA) < abs(wayB))
            return wayA;
        return wayB;
    }
    double calculateDesiredYaw(
        double localPlayerLocationX,
        double localPlayerLocationY,
        double enemyPlayerLocationX,
        double enemyPlayerLocationY)
    {
        const double locationDeltaX = enemyPlayerLocationX - localPlayerLocationX;
        const double locationDeltaY = enemyPlayerLocationY - localPlayerLocationY;
        const double yawInRadians = atan2(locationDeltaY, locationDeltaX);
        const double yawInDegrees = yawInRadians * (180 / M_PI);
        return yawInDegrees;
    }
    double calculateDesiredPitch( LocalPlayer* m_localPlayer, Player* m_targetPlayer )
    {
	//double localPlayerLocationZ = m_localPlayer->getLocationZ() + 56;
	double localPlayerLocationZ = m_localPlayer->getCameraZ();
        double enemyPlayerLocationZ = m_targetPlayer->getLocationZ() + m_targetPlayer->getBoneZ(3);
        /*if (m_localPlayer->isDucking())
        {
            localPlayerLocationZ -= 16;
        }
        if (m_targetPlayer->isDucking())
        {
            enemyPlayerLocationZ -= 18;
        }*/
        const double locationDeltaZ = enemyPlayerLocationZ - localPlayerLocationZ;
        const double distanceBetweenPlayers = math::calculateDistance2D(m_targetPlayer->getLocationX(), m_targetPlayer->getLocationY(), m_localPlayer->getLocationX(), m_localPlayer->getLocationY());
        const double pitchInRadians = atan2(-locationDeltaZ, distanceBetweenPlayers);
        const double pitchInDegrees = pitchInRadians * (180 / M_PI);
        return pitchInDegrees;
    }
    Player *findClosestEnemy()
    {
        Player *closestPlayerSoFar = nullptr;
        double closestPlayerAngleSoFar;
        for (int i = 0; i < m_players->size(); i++)
        {
            Player *player = m_players->at(i);
	    /*if (counter % 1000 == 0)
	    {
		    printf("counter: %d \n", counter);
		    if (player->isDead())
		    {
			    printf("This player (%d) is dead! \n", i);
			    printf("The yaw: %f \n", player->getSpecYaw());
			    printf("----------------------------------- \n");
			    continue;
		    }
	    }*/
	    if (!player->isValid())
            	continue;
            if (player->isKnocked() || !player->isVisible(false))
                continue;
            //if (m_level->isSpecialMode() && player->getTeamSpecial() == m_localPlayer->getTeamSpecial())
                //continue;
            //if (player->getTeamNumber() == m_localPlayer->getTeamNumber())
                //continue;
	    double dist = math::calculateDistanceInMeters(m_localPlayer->getLocationX(),
				       m_localPlayer->getLocationY(),
				       m_localPlayer->getLocationZ(),
				       player->getLocationX(),
				       player->getLocationY(),
				       player->getLocationZ());
	    if (dist > 40) {
		    continue;
	    }
	    float fovcheck = 10;
            double desiredViewAngleYaw = calculateDesiredYaw(m_localPlayer->getLocationX(),
                                                             m_localPlayer->getLocationY(),
                                                             player->getLocationX(),
                                                             player->getLocationY());
	    double desiredViewAnglePitch = calculateDesiredPitch(m_localPlayer, player);
	    double yawangleDelta = calculateAngleDelta(m_localPlayer->getYaw(), desiredViewAngleYaw);
	    double pitchangleDelta = calculatePitchAngleDelta(m_localPlayer->getPitch(), desiredViewAnglePitch);

	    if ( abs(yawangleDelta) > fovcheck || abs(pitchangleDelta) > (fovcheck / 3)) {
	    	continue;
	    }
		
            if (closestPlayerSoFar == nullptr)
            {
                closestPlayerSoFar = player;
                closestPlayerAngleSoFar = abs(yawangleDelta)+abs(pitchangleDelta);
            }
            else
            {
                if ( (abs(yawangleDelta)+abs(pitchangleDelta)) < closestPlayerAngleSoFar)
                {
                    closestPlayerSoFar = player;
                    closestPlayerAngleSoFar = abs(yawangleDelta)+abs(pitchangleDelta);
                }
            }
        }
        return closestPlayerSoFar;
    }
};
