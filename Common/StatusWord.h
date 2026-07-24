#pragma once
#include <iostream>

uint16_t CreateStatusWord(uint8_t rtaddress, bool me, bool busy);

struct DecodedStatus {

	uint8_t rtaddress;
	bool messageError;	
	bool busy;
};

DecodedStatus DecodeStatus(uint16_t stat);