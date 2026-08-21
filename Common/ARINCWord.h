#pragma once
#include <cstdint>
#include <random>

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

enum class SSM : uint8_t
{
	NormalOperation = 0b00,
	FunctionalTest = 0b01,
	NoComputedData = 0b10,
	FailureWarning = 0b11,
};


struct ARINCWord
{
	Label label;
	uint8_t sdi;
	int32_t data;
	SSM ssm = SSM::NormalOperation;
	bool parity;

};

SSM GenerateSSM();
void Delay(int milliseconds); 
uint32_t CreateARINCWord(ARINCWord& word);
ARINCWord DecodeARINCWord(uint32_t rawWord);
bool CalculateOddParity(uint32_t raw);
bool CheckParity(uint32_t raw);