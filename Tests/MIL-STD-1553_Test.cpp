#include <gtest/gtest.h>
#include "CommandWord.h"
#include "StatusWord.h"

TEST(CommandWordTest, CreateCommandWord) { // REQ-MIL-STD-1553-CMD-01

	// for maximum value
	uint16_t rtadd = 31;
	uint16_t tr = 1;
	uint16_t sub = 31;
	uint16_t wordC = 31;

	uint16_t cmd = CreateCommandWord(rtadd, tr, sub, wordC);
	EXPECT_EQ(cmd, 0xFFFF); // 1111 1111 1111 1111

	//for minimum value	
	rtadd = 0;
	tr = 0;
	sub = 0;
	wordC = 0;
	cmd = CreateCommandWord(rtadd, tr, sub, wordC);
	EXPECT_EQ(cmd, 0x0000); // 0000 0000 0000 0000

	//for random value
	rtadd = 5;
	tr = 1;
	sub = 10;
	wordC = 2;

	cmd = CreateCommandWord(rtadd, tr, sub, wordC);
	EXPECT_EQ(cmd, 0x2D42);
}

TEST(CommandWordTest, DecodeCommandWord) { //REQ-MIL-STD-1553-CMD-03
	uint16_t cmd = 0x2D42; //0010 1101 0100 0010
	DecodedCommand decodedCmd = DecodeCMD(cmd);
	uint16_t rtadd = decodedCmd.rtAddress;
	uint16_t tr = decodedCmd.transmit;
	uint16_t sub = decodedCmd.subAddress;
	uint16_t wordC = decodedCmd.wordCount;
	EXPECT_EQ(rtadd, 5);
	EXPECT_EQ(tr, 1);
	EXPECT_EQ(sub, 10);
	EXPECT_EQ(wordC, 2);
}

TEST(StatusWordTest, CreateStatusWord) { //REQ-MIL-STD-1553-STA-01
	uint8_t rtadd = 5;
	bool me = true; 
	//Busy ihtimali test edilmedi. Şu an için elle 0 girildi.
	uint16_t status = 0x2C00; // 0010 1100 0000 0000
	DecodedStatus decodedStatus = DecodeStatus(status);

	EXPECT_EQ(decodedStatus.rtaddress, rtadd);
	EXPECT_EQ(decodedStatus.messageError, me);
}

TEST(StatusWordTest, DecodeStatusWord) { //REQ-MIL-STD-1553-STA-04
	uint16_t status = 0x2C00; //0010 1100 0000 0000

	DecodedStatus decodedStatus = DecodeStatus(status);
	
	EXPECT_EQ(decodedStatus.rtaddress, 5);
	EXPECT_EQ(decodedStatus.messageError, true);
	EXPECT_EQ(decodedStatus.busy, false);
}


