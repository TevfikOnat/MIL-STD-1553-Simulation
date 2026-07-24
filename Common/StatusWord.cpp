#include "StatusWord.h"
using namespace std;

uint16_t CreateStatusWord(uint8_t rtaddress, bool me, bool busy) {

	uint16_t status=0;
	status |= (rtaddress << 11);
	status |= (me << 10);
	status |= (busy << 9);

	return status;
}

DecodedStatus DecodeStatus(uint16_t stat) {

	DecodedStatus status;
	status.rtaddress = (stat >> 11) & (0x1F);
	status.messageError = (stat >> 10) & (0x01);
	status.busy = (stat >> 9) & (0x01);

	return status;
}