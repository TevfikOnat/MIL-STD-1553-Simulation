#include "CommandWord.h"
using namespace std;

uint16_t CreateCommandWord(uint8_t rtAddress, uint8_t transmit, uint8_t subAddress, uint8_t wordCount) {

    uint16_t cmd = 0;
    cmd |= (rtAddress << 11);
    cmd |= (transmit << 10);
    cmd |= (subAddress << 5);
    cmd |= wordCount;

    return cmd;
}

uint16_t EncodeCommandWord() {
    int rtadd;
    int tr;
    int sub;
    int wordC;

    cout << "RT Address, T/R, Subaddress, Word Count: " << endl;
    cin >> rtadd >> tr >> sub >> wordC;

    return CreateCommandWord(static_cast<uint8_t>(rtadd),
        static_cast<uint8_t>(tr),
        static_cast<uint8_t>(sub),
        static_cast<uint8_t>(wordC));

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