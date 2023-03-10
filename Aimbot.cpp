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
    void update()
    {
	Weapon *localWeapon = new Weapon(m_localPlayer->getWeaponHandle()); //TESTING THIS SHIT
        // validations
        if (m_localPlayer->isWalking())
        {
            m_lockedOnPlayer = nullptr;
            return;
        }
	if (!m_localPlayer->isInAttack() && !m_localPlayer->isZooming())
    	{
	    m_lockedOnPlayer = nullptr;
	    return;
    	}
	    
        // get desired angle to an enemy
        double desiredViewAngleYaw = 0;
        double desiredViewAnglePitch = 0;
	double distanceToTarget;
        if (m_level->isTrainingArea())
        {
            printf("X:%.6f \t Y: %.6f \t Z:%.6f \n", m_localPlayer->getLocationX(), m_localPlayer->getLocationY(), m_localPlayer->getLocationZ());
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
            if (m_lockedOnPlayer == nullptr || !m_lockedOnPlayer->isVisible())
                m_lockedOnPlayer = findClosestEnemy();
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
        }

        // Setup Pitch
        const double pitch = m_localPlayer->getPitch();
        const double pitchAngleDelta = calculatePitchAngleDelta(pitch, desiredViewAnglePitch);
        const double pitchAngleDeltaAbs = abs(pitchAngleDelta);
        if (pitchAngleDeltaAbs > m_configLoader->getAimbotActivationFOV() / 2)
            return;
        double newPitch = normalizePitch(pitch + (pitchAngleDelta / m_configLoader->getAimbotSmoothing()));
        m_localPlayer->setPitch(newPitch);

        // Setup Yaw
        const double yaw = m_localPlayer->getYaw();
        const double angleDelta = calculateAngleDelta(yaw, desiredViewAngleYaw);
        const double angleDeltaAbs = abs(angleDelta);
        if (angleDeltaAbs > m_configLoader->getAimbotActivationFOV())
            return;
        double newYaw = normalizeYaw(yaw + (angleDelta / m_configLoader->getAimbotSmoothing()));
        m_localPlayer->setYaw(newYaw);
	if (m_lockedOnPlayer != nullptr)
	{
		printf("X:%.6f \t Y: %.6f \t Z:%.6f \n", m_localPlayer->getLocationX(), m_localPlayer->getLocationY(), m_localPlayer->getLocationZ())
	}
	//TESTING TRIGGERBOT
	if (m_lockedOnPlayer != nullptr && localWeapon->getAmmo() > 0 && localWeapon->getReadyTime() == 0 && localWeapon->isSemiAuto())
	{
		if (distanceToTarget < 10 & m_lockedOnPlayer->isCrosshair())
		{
			m_x11Utils->mouseClick(1);
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
	}
    }
    double flipYawIfNeeded(double angle)
    {
        double myAngle = angle;
        if (myAngle > 180)
            myAngle = (360 - myAngle) * -1;
        else if (myAngle < -180)
            myAngle = (360 + myAngle);
        return myAngle;
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
		double localPlayerLocationZ;
        double enemyPlayerLocationZ;
        if (m_localPlayer->isDucking())
        {
            double localPlayerLocationZ = m_localPlayer->getLocationZ() - 27;
        }
        else
        {
            double localPlayerLocationZ = m_localPlayer->getLocationZ();
        }
        if (m_targetPlayer->isDucking())
        {
            double enemyPlayerLocationZ = m_targetPlayer->getLocationZ() - 18;
        }
        else
        {
            double enemyPlayerLocationZ = m_targetPlayer->getLocationZ();
        }
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
            if (!player->isValid())
                continue;
            if (player->isKnocked())
                continue;
            if (player->getTeamNumber() == m_localPlayer->getTeamNumber())
                continue;
            if (!player->isVisible())
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
