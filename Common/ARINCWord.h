#pragma once

#include <cstdint>

enum class Label : uint8_t
{
	GPSAltitude = 203,
	groundSpeed = 14,
	Latitude = 205,
	Longitude = 206,

	BaroAltitude = 200,
	Airspeed = 13,

	Heading = 80,
	Roll = 21,
	Pitch = 22,

	RadioAltitude =	201
};


struct ARINCWord
{
	Label label;
	uint8_t sdi;
	int32_t data;
	uint8_t ssm;
	bool parity;

};

void Delay(int milliseconds); 
uint32_t CreateARINCWord(const ARINCWord& word);
ARINCWord DecodeARINCWord(uint32_t rawWord);
bool CalculateOddParity(uint32_t raw);