#include <gtest/gtest.h>
#include "ARINCWord.h"
 
TEST(ARINCWordTest, CalculateOddParity) { // REQ-ARC-01

	uint32_t raw = 0b000000000000000000000000000001; // odd set bits
	EXPECT_FALSE(CalculateOddParity(raw));

	raw = 0b0000000000000000000000000000011; // even set bits
	EXPECT_TRUE(CalculateOddParity(raw));
}

TEST(ARINCWordTest, CheckParity) { // REQ-ARC-05
	uint32_t raw = 0x80000001; // odd set bits with parity bit set
	EXPECT_FALSE(CheckParity(raw));
	raw = 0x00000001; // odd set bits with parity bit not set
	EXPECT_TRUE(CheckParity(raw));
}

TEST(ARINCWordTest, CreateARINCWord) { // REQ-ARC-09

	ARINCWord word;

	word.label = static_cast<Label>(0x01);
	word.sdi = 0x01;
	word.data = 0x000015;

	uint32_t raw = CreateARINCWord(word);

	EXPECT_EQ(raw & 0xFF, 0x01); // label
	EXPECT_EQ((raw >> 8) & 0x03, 0x01); // sdi
	EXPECT_EQ((raw >> 10) & 0x7FFFF, 0x00015); // data
	EXPECT_EQ((raw >> 29) & 0x03, static_cast<uint32_t>(word.ssm)); // ssm

}

TEST(ARINCWordTest, DecodeARINCWord) { // REQ-ARC-12
	uint32_t raw = 0x60005501; // label=1, sdi=1, data=21, ssm=3, parity=1
	ARINCWord word = DecodeARINCWord(raw);
	EXPECT_EQ(word.label, static_cast<Label>(0x01));
	EXPECT_EQ(word.sdi, 0x01);
	EXPECT_EQ(word.data, 0x00015);
	EXPECT_EQ(word.ssm, SSM::FailureWarning);
	EXPECT_EQ(word.parity, false);
}

TEST(ARINCWordTest, GenerateSSM) { // REQ-ARC-15
	
	SSM ssm = GenerateSSM();

	if (ssm == SSM::NormalOperation) {
		EXPECT_TRUE(GenerateSSM() == SSM::NormalOperation);
		EXPECT_FALSE(GenerateSSM() == SSM::FunctionalTest);
		EXPECT_FALSE(GenerateSSM() == SSM::NoComputedData);
	}
	else if (ssm == SSM::NoComputedData) {
		EXPECT_TRUE(GenerateSSM() == SSM::NoComputedData);
		EXPECT_FALSE(GenerateSSM() == SSM::NormalOperation);
		EXPECT_FALSE(GenerateSSM() == SSM::FunctionalTest);
	}
	else if (ssm == SSM::FailureWarning) {
		EXPECT_TRUE(GenerateSSM() == SSM::FailureWarning);
		EXPECT_FALSE(GenerateSSM() == SSM::NormalOperation);
		EXPECT_FALSE(GenerateSSM() == SSM::FunctionalTest);
	}
}