#pragma once
#include <cstdint>

struct AircraftState
{
    // GPS
    uint32_t latitude = 0;
    uint32_t longitude = 0;
    uint32_t gpsAltitude = 0;
	uint32_t groundSpeed = 0;

    // Air Data Computer
    uint32_t airspeed = 0;
    uint32_t baroAltitude = 0;

    // IRS
    uint32_t heading = 0;
    uint32_t roll = 0;
    uint32_t pitch = 0;

    // Radio Altimeter
    uint32_t radioAltitude = 0;
};