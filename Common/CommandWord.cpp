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

    while (true){

        cout << "RT Address, T/R, Subaddress, Word Count: ";

        cin >> rtadd >> tr >> sub >> wordC;

        if (cin.fail()){
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Please enter only numbers(in intervals)." << endl;
            continue;
        }

        if (rtadd < 1 || rtadd > 31){
            cout << "RT Address must be between 0 and 31." << endl;
            continue;
        }

        if (tr != 0 && tr != 1){
            cout << "T/R must be 0 or 1." << endl;
            continue;
        }

        if (sub < 0 || sub > 31){
            cout << "Subaddress must be between 0 and 31." << endl;
            continue;
        }

        if (wordC < 1 || wordC > 32){
            cout << "Word Count must be between 1 and 32." << endl;
            continue;
        }

        break;
    }

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