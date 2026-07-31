#pragma once
#include <iostream>
#include <random>


uint16_t CreateStatusWord(uint8_t rtaddress, bool me);

struct DecodedStatus {

	uint8_t rtaddress;
	bool messageError;	
	bool busy;
};

DecodedStatus DecodeStatus(uint16_t stat);