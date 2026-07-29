#include "ARINCWord.h"
#include <chrono>
#include <thread>

bool CalculateOddParity(uint32_t raw) {
	uint8_t parityCount = 0;
	for (int i = 0; i < 31; ++i) {
		if ((raw >> i) & 1) {
			++parityCount;
		}
	}
	return (parityCount % 2 == 0);
}

bool CheckParity(uint32_t raw) {
	bool calculatedParity = CalculateOddParity(raw & 0x7FFFFFFF);
	bool receivedParity = (raw >> 31) & 1;
	return calculatedParity == receivedParity;
}

uint32_t CreateARINCWord(const ARINCWord& word) {
	int32_t raw = 0;

	raw |= static_cast<uint32_t>(word.label);

	raw |= (static_cast<uint32_t>(word.sdi) & 0x03) << 8;

	raw |= (word.data & 0x7FFFF) << 10;

	raw |= (static_cast<uint32_t>(word.ssm) & 0x03) << 29;


	
	bool parity = CalculateOddParity(raw);
	raw |= static_cast<uint32_t>(parity) << 31; 
	return raw;
}

ARINCWord DecodeARINCWord(uint32_t rawWord) {
	ARINCWord word;
	word.label = static_cast<Label>(rawWord & 0xFF);
	word.sdi = static_cast<uint8_t>((rawWord >> 8) & 0x03);
	int32_t data = (rawWord >> 10) & 0x7FFFF;

	if (data & (1 << 18))   // 19 bitin en üst biti 1 ise
	{
		data |= ~0x7FFFF;   // üst bitleri 1 yap
	}

	word.data = data;
	word.ssm = static_cast<uint8_t>((rawWord >> 29) & 0x03);
	word.parity = (rawWord >> 31) & 0x01;
	return word;
}

void Delay(int milliseconds) {
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}


