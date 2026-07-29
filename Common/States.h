#pragma once
#include <cstdint>

struct GPSState
{
	uint32_t latitude = 39000000;
	uint32_t longitude = 32000000;
	uint32_t altitude = 15000;
	uint32_t groundSpeed = 280;

};

struct ADCState
{
	uint32_t airspeed = 300;
	uint32_t baroAltitude = 15100;
};

struct IRSState
{
	uint32_t heading = 0;
	int32_t roll = 2;
	int32_t pitch = 3;
};

struct RadioState
{
	uint32_t radioAltitude = 14900;
};