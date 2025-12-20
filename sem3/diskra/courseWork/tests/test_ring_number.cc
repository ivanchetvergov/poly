// tests/test_ring_number.cc

#include "gtest/gtest.h"
#include "FiniteRingRules.h"
#include "RingNumber.h"
#include <string>
#include <memory>
#include <iostream>

class RingNumberTest : public ::testing::Test {
protected:
    std::unique_ptr<FiniteRingRules> rules_;
    char zero_;
    char one_;
    
    void SetUp() override {
        rules_ = std::make_unique<FiniteRingRules>("../config.yaml", "variant_1");
        zero_ = rules_->getZeroElement();
        one_ = rules_->getOneElement();
        
        std::cout << "\n--- Testing RingNumber (variant_1) ---" << std::endl;
    }
};

// * --- ТЕСТ 1: Создание
TEST_F(RingNumberTest, Construction_Empty) {
    RingNumber num(*rules_);
    
    EXPECT_TRUE(num.isZero());
    EXPECT_EQ(num.length(), 1);
    EXPECT_EQ(num.toString(), std::string(1, zero_));
    
    std::cout << "   Default constructor creates zero" << std::endl;
}

TEST_F(RingNumberTest, Construction_FromString) {
    RingNumber num(*rules_, "gbc");
    
    EXPECT_FALSE(num.isZero());
    EXPECT_EQ(num.length(), 3);
    EXPECT_EQ(num.toString(), "gbc");
    
    std::cout << "   Construction from string" << std::endl;
}

TEST_F(RingNumberTest, Construction_SingleChar) {
    RingNumber num(*rules_, "b");
    
    EXPECT_FALSE(num.isZero());
    EXPECT_EQ(num.length(), 1);
    EXPECT_EQ(num[0], 'b');  
    
    std::cout << "   Construction from single char" << std::endl;
}

TEST_F(RingNumberTest, Construction_InvalidChar) {
    EXPECT_THROW(RingNumber(*rules_, "xyz"), std::runtime_error);
    EXPECT_THROW(RingNumber(*rules_, "a1b"), std::runtime_error);
    
    std::cout << "   Invalid characters throw exception" << std::endl;
}

TEST_F(RingNumberTest, Construction_EmptyString) {
    EXPECT_THROW(RingNumber(*rules_, ""), std::runtime_error);
    
    std::cout << "   Empty string throws exception" << std::endl;
}

// * --- ТЕСТ 2: Нормализация
TEST_F(RingNumberTest, Normalization_LeadingZeros) {
    // "aabc" → "bc" (ведущие нули удаляются)
    RingNumber num(*rules_, "aabc");
    num.normalize();
    
    EXPECT_EQ(num.toString(), "bc");
        std::cout << "   Normalize string" << std::endl;
}