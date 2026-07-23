#pragma once
#include <iostream>

uint16_t CreateCommandWord(uint8_t rtAddress, uint8_t transmit, uint8_t subAddress, uint8_t wordCount);

struct DecodedCommand
{

    uint16_t cmd;
    uint8_t rtAddress;
    uint8_t transmit;
    uint8_t subAddress;
    uint8_t wordCount;

};

DecodedCommand DecodeCMD(uint16_t cmd);