#pragma once
#include <vector>
#include "LocalPlayer.cpp"
#include "Math.cpp"
#include "Level.cpp"
#include "X11Utils.cpp"
#include "ConfigLoader.cpp"

class NoRecoil
{
private:
    ConfigLoader *m_configLoader;
    LocalPlayer *m_localPlayer;
    X11Utils *m_x11Utils;

    double m_previousPunchPitch = 0;
    double m_previousPunchYaw = 0;

public:
    NoRecoil(ConfigLoader *configLoader,
             LocalPlayer *localPlayer,
             X11Utils *x11Utils)
    {
        m_configLoader = configLoader;
        m_localPlayer = localPlayer;
        m_x11Utils = x11Utils;
    }
    void update()
    {
        // adjust pitch
        //const double norecoilPitchStrength = m_configLoader->getNorecoilPitchStrength(); STANDARD PITCH STRENGTH FROM CONFIG
        const double punchPitch = m_localPlayer->getPunchPitch();
        if (punchPitch != 0)
        {
            const double norecoilPitchStrength = math::RandomFloat(0.38,0.60); // RANDOM PITCH STRENGTH
            const double pitch = m_localPlayer->getPitch();
            const double punchPitchDelta = (punchPitch - m_previousPunchPitch) * norecoilPitchStrength;
            m_localPlayer->setPitch(pitch - punchPitchDelta);
            m_previousPunchPitch = punchPitch;
        }

        // adjust yaw
        //const double norecoilYawStrength = m_configLoader->getNorecoilYawStrength(); STANDARD
        
        const double punchYaw = m_localPlayer->getPunchYaw();
        if (punchYaw != 0)
        {
            const double norecoilYawStrength = math::RandomFloat(0.40,0.60); // RANDOM
            const double yaw = m_localPlayer->getYaw();
            const double punchYawDelta = (punchYaw - m_previousPunchYaw) * norecoilYawStrength;
            m_localPlayer->setYaw(yaw - punchYawDelta);
            m_previousPunchYaw = punchYaw;
        }
    }
};
