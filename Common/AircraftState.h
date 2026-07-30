#pragma once
#include <cstdint>
#include "ARINCWord.h"
struct AircraftState
{
    // GPS
    uint32_t latitude = 0;
    SSM  latitudeSSM = SSM::NormalOperation;

    uint32_t longitude = 0;
    SSM longitudeSSM = SSM::NormalOperation;

    uint32_t gpsAltitude = 0;
    SSM gpsAltitudeSSM = SSM::NormalOperation;

    uint32_t groundSpeed = 0;
    SSM groundSpeedSSM = SSM::NormalOperation;

    // Air Data Computer
    uint32_t airspeed = 0;
    SSM airspeedSSM = SSM::NormalOperation;

    uint32_t baroAltitude = 0;
    SSM baroAltitudeSSM = SSM::NormalOperation;

    // IRS
    uint32_t heading = 0;
    SSM headingSSM = SSM::NormalOperation;

    int32_t roll = 0;
    SSM rollSSM = SSM::NormalOperation;

    int32_t pitch = 0;
    SSM pitchSSM = SSM::NormalOperation;

    // Radio Altimeter
    uint32_t radioAltitude = 0;
    SSM radioAltitudeSSM = SSM::NormalOperation;
};