#pragma once
#include <vector>
#include "LocalPlayer.cpp"
#include "Math.cpp"
#include "X11Utils.cpp"
#include "SimInput.cpp"
#include "ConfigLoader.cpp"

class NoRecoil
{
private:
    ConfigLoader *m_configLoader;
    LocalPlayer *m_localPlayer;
    X11Utils *m_x11Utils;
    SimInput *m_simInput;

    double m_previousPunchPitch = 0;
    double m_previousPunchYaw = 0;

public:
    NoRecoil(ConfigLoader *configLoader,
             LocalPlayer *localPlayer,
             X11Utils *x11Utils,
             SimInput *simInput)
    {
        m_configLoader = configLoader;
        m_localPlayer = localPlayer;
        m_x11Utils = x11Utils;
        m_simInput = simInput;
    }
    void update()
    {
        // adjust pitch
        //const double norecoilPitchStrength = m_configLoader->getNorecoilPitchStrength(); STANDARD PITCH STRENGTH FROM CONFIG
        const double punchPitch = m_localPlayer->getPunchPitch();
	const double pitchval = m_localPlayer->getPitch();
	int punchpixPitch = 0;
	int punchpixYaw = 0;
	printf("punch: %f \n", punchPitch);
	printf("%f \n", pitchval);
        if (punchPitch < 0)
        {
            //printf("punch pitch: %f \n", punchPitch);
            //const double norecoilPitchStrength = math::RandomFloat(0.80,0.99); // RANDOM PITCH STRENGTH
            const double pitch = m_localPlayer->getPitch();
            const double punchPitchDelta = (punchPitch - m_previousPunchPitch);
            //printf("punch pitch DELTA: %f \n", punchPitchDelta);
	    punchpixPitch = punchPitchDelta / (-0.020);
	    //printf("punch pitch PIXELS: %d \n", punchpixPitch);
            //m_localPlayer->setPitch(pitch - punchPitchDelta);
            m_previousPunchPitch = punchPitch;
        }

        // adjust yaw
        //const double norecoilYawStrength = m_configLoader->getNorecoilYawStrength(); STANDARD
        
        const double punchYaw = m_localPlayer->getPunchYaw();
        if (punchYaw != 0)
        {
            //const double norecoilYawStrength = math::RandomFloat(0.12,0.35); // RANDOM
            const double yaw = m_localPlayer->getYaw();
            const double punchYawDelta = (punchYaw - m_previousPunchYaw);
	    punchpixYaw = punchYawDelta / (0.04);
            //m_localPlayer->setYaw(yaw - punchYawDelta);
            m_previousPunchYaw = punchYaw;
        }
	if (punchpixPitch > 0){
		m_simInput->emit(EV_REL, REL_Y, punchpixPitch);
		m_simInput->emit(EV_REL, REL_X, 0);
		m_simInput->emit(EV_SYN, SYN_REPORT, 0);
		usleep(2000);
	}
    }
};
