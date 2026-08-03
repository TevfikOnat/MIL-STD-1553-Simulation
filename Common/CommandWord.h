#pragma once
#include <iostream>

uint16_t CreateCommandWord(uint8_t rtAddress, uint8_t transmit, uint8_t subAddress, uint8_t wordCount);
uint16_t EncodeCommandWord();

enum class Sync : uint8_t
{
    COMMAND = 0x01,
    STATUS = 0X02,
    DATA = 0X03
};

struct Packet1553 {
    Sync sync;
    uint16_t Word; 
};

struct DecodedCommand
{

    uint16_t cmd;
    uint8_t rtAddress;
    uint8_t transmit;
    uint8_t subAddress;
    uint8_t wordCount;

};

DecodedCommand DecodeCMD(uint16_t cmd);