#include <gtest/gtest.h>
#include "CommandWord.h"

TEST(CommandWordTest, EncodeTest) {
	uint16_t rtadd = 5;
	uint16_t tr = 1;
	uint16_t sub = 10;
	uint16_t wordC = 2;
	
	uint16_t cmd = CreateCommandWord(rtadd, tr, sub, wordC);

	EXPECT_EQ(cmd, 0x2D42); 
}

TEST(CommandWordTest, DecodeTest) {
	uint16_t cmd = 0x2D42;
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

TEST(CommandWordTest, EncodeRTAddress)
{
	uint16_t cmd = CreateCommandWord(5, 1, 10, 2);

	EXPECT_EQ((cmd >> 11) & 0x1F, 5);
}