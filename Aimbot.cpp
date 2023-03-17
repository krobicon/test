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

    Player *m_lockedOnPlayer = nullptr;
    bool triggerCache = false;

public:
    Aimbot(ConfigLoader *configLoader,
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
    void update(int counter, Weapon *m_localWeapon)
    {
	//Weapon *localWeapon = new Weapon(m_localPlayer->getWeaponHandle()); //TESTING THIS SHIT
	bool trigger = false;
	int smooth = m_configLoader->getAimbotSmoothing() + rand() % 11;
	int fov = m_configLoader->getAimbotActivationFOV();
        // validations
        if (m_localPlayer->isWalking())
        {
            m_lockedOnPlayer = nullptr;
            return;
        }
	if (m_localWeapon->getAmmo() > 0 && m_localWeapon->isSemiAuto())
	{
	    trigger = true;
	}
	else if (!m_localPlayer->isInAttack() && !m_localPlayer->isZooming())
    	{
	    m_lockedOnPlayer = nullptr;
	    return;
    	}
	    
        // get desired angle to an enemy
        double desiredViewAngleYaw = 0;
        double desiredViewAnglePitch = 0;
	double distanceToTarget;
        if (m_level->isTrainingArea() &&  1 > 20)
        {
            //printf("X:%.6f \t Y: %.6f \t Z:%.6f \n", m_localPlayer->getLocationX(), m_localPlayer->getLocationY(), m_localPlayer->getLocationZ());
            const float dummyX = 31408.732422;
            const float dummyY = -6711.955566;
            const float dummyZ = -29234.839844;
            distanceToTarget = math::calculateDistanceInMeters(m_localPlayer->getLocationX(), m_localPlayer->getLocationY(), m_localPlayer->getLocationZ(), dummyX, dummyY, dummyZ);
            if (distanceToTarget > m_configLoader->getAimbotMaxRange())
                return;
            desiredViewAngleYaw = calculateDesiredYaw(m_localPlayer->getLocationX(), m_localPlayer->getLocationY(), dummyX, dummyY);
            //desiredViewAnglePitch = calculateDesiredPitch(m_localPlayer->getLocationX(), m_localPlayer->getLocationY(), m_localPlayer->getLocationZ(), dummyX, dummyY, dummyZ);
        }
        else
        {
	    printf("debugging \n");
            if (m_lockedOnPlayer == nullptr || !m_lockedOnPlayer->isVisible(false))
                m_lockedOnPlayer = findClosestEnemy();
            if (m_lockedOnPlayer == nullptr)
                return;
	    float bonez = m_lockedOnPlayer->getBoneZ(8);
            float realz = m_lockedOnPlayer->getLocationZ();
	    printf("bone z: [%f] \n", bonez);
   	    printf("real z: [%f] \n", realz);
            distanceToTarget = math::calculateDistanceInMeters(m_localPlayer->getLocationX(),
                                                               m_localPlayer->getLocationY(),
                                                               m_localPlayer->getLocationZ(),
                                                               m_lockedOnPlayer->getLocationX(),
                                                               m_lockedOnPlayer->getLocationY(),
                                                               m_lockedOnPlayer->getLocationZ());
	    //printf("DISTANCE CHECK 1: [%f] \n", distanceToTarget);
            if (distanceToTarget > m_configLoader->getAimbotMaxRange())
                return;
            desiredViewAngleYaw = calculateDesiredYaw(m_localPlayer->getLocationX(),
                                                      m_localPlayer->getLocationY(),
                                                      m_lockedOnPlayer->getLocationX(),
                                                      m_lockedOnPlayer->getLocationY());
            desiredViewAnglePitch = calculateDesiredPitch(m_localPlayer, m_lockedOnPlayer);
        }

        // Setup Angles
        const double pitch = m_localPlayer->getPitch();
        const double pitchAngleDelta = calculatePitchAngleDelta(pitch, desiredViewAnglePitch);
        const double pitchAngleDeltaAbs = abs(pitchAngleDelta);
	    
	const double yaw = m_localPlayer->getYaw();
        const double yawAngleDelta = calculateAngleDelta(yaw, desiredViewAngleYaw);
        const double yawAngleDeltaAbs = abs(yawAngleDelta);
	
	if (trigger == true && distanceToTarget < 13 && m_lockedOnPlayer != nullptr)
	//if (trigger == true && distanceToTarget < 13)
	{
		//bool triggerSent = false;
		smooth = smooth/2;
		fov = fov*2;
		/*if (counter % 10 == 0 && m_lockedOnPlayer->isCrosshair())
		//if (localWeapon->getReadyTime() == 0 && (yawAngleDeltaAbs < 3 && pitchAngleDeltaAbs < 4))
		{
			m_localPlayer->setAttackState(5);
			triggerSent = true;
			m_x11Utils->mouseClick(1);
			printf("SENT \n");
		}
		/*if (triggerCache == true && triggerSent == false)
		{
			m_localPlayer->setAttackState(4);
		}
		triggerCache = triggerSent;*/
	}
	    
        if (yawAngleDeltaAbs > fov || pitchAngleDeltaAbs > fov / 2)
	{
	    m_lockedOnPlayer = nullptr;
	    return;
	}
	
	//printf("AMMO: [%d] \n", localWeapon->getAmmo());
	//printf("READY TIME: [%f] \n", localWeapon->getReadyTime());
	//printf("SEMI?: [%d] \n", localWeapon->isSemiAuto());
	//printf("TRIGGER: [%d] \n", trigger);
	//printf("SMOOTH: [%d] \n", smooth);
	//printf("FOV: [%d] \n", fov);
	    
	// Write angles
        //double newPitch = normalizePitch(pitch + (pitchAngleDelta / m_configLoader->getAimbotSmoothing()));
	//double newYaw = normalizeYaw(yaw + (yawAngleDelta / m_configLoader->getAimbotSmoothing()));
        double newPitch = normalizePitch(pitch + (pitchAngleDelta / smooth));
	double newYaw = normalizeYaw(yaw + (yawAngleDelta / smooth));
        m_localPlayer->setPitch(newPitch);
        m_localPlayer->setYaw(newYaw);
	printf("SET PITCH: [%f] \n", newPitch); 
	printf("SET YAW: [%f] \n", newYaw); 
	
	
	//if (trigger == true && localWeapon->getReadyTime() == 0 && m_lockedOnPlayer != nullptr && distanceToTarget < 13)
	if (trigger == true && m_lockedOnPlayer != nullptr && distanceToTarget < 13)
	{
		printf("AIM COUNTER: [%d] \n", counter);
		//if (m_lockedOnPlayer->isCrosshair())
		if ((yawAngleDeltaAbs < 2 && pitchAngleDeltaAbs < 4))
		{
			m_localPlayer->setAttackState(5);
			m_x11Utils->mouseClick(1);
			printf("TRIGGER SENT \n");
		}
	}
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
	double localPlayerLocationZ = m_localPlayer->getLocationZ() + m_localPlayer->getBoneZ(8);
        double enemyPlayerLocationZ = m_targetPlayer->getLocationZ() + m_targetPlayer->getBoneZ(8);
        /*if (m_localPlayer->isDucking())
        {
            localPlayerLocationZ -= 10;
        }
        if (m_targetPlayer->isDucking())
        {
            enemyPlayerLocationZ -= 18;
        }*/
	printf ("my location z: %f /n", localPlayerLocationZ);
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
            if (!player->isValid() || player->isKnocked())
	    {
                continue;
	    }
            if (m_level->isSpecialMode() && player->getTeamSpecial() == m_localPlayer->getTeamSpecial())
                continue;
            //if (player->getTeamNumber() == m_localPlayer->getTeamNumber())
                //continue;
            if (!player->isVisible(false))
                continue;
            double desiredViewAngleYaw = calculateDesiredYaw(m_localPlayer->getLocationX(),
                                                             m_localPlayer->getLocationY(),
                                                             player->getLocationX(),
                                                             player->getLocationY());
            double yawangleDelta = calculateAngleDelta(m_localPlayer->getYaw(), desiredViewAngleYaw);
	    double desiredViewAnglePitch = calculateDesiredPitch(m_localPlayer, player);
	    double pitchangleDelta = calculatePitchAngleDelta(m_localPlayer->getPitch(), desiredViewAnglePitch);
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
