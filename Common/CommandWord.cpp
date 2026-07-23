#include "CommandWord.h"

uint16_t CreateCommandWord(uint8_t rtAddress, uint8_t transmit, uint8_t subAddress, uint8_t wordCount) {

    uint16_t cmd = 0;
    cmd |= (rtAddress << 11);
    cmd |= (transmit << 10);
    cmd |= (subAddress << 5);
    cmd |= wordCount;

    return cmd;
}

DecodedCommand DecodeCMD(uint16_t cmd)
{

    DecodedCommand decoded;

    decoded.cmd = cmd;
    decoded.rtAddress = (cmd >> 11) & 0x1F;
    decoded.transmit = (cmd >> 10) & 0x01;
    decoded.subAddress = (cmd >> 5) & 0x1F;
    decoded.wordCount = cmd & 0x1F;

    return decoded;
}