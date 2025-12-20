// tests/test_small_arithmetic.cc

#include "gtest/gtest.h"
#include "FiniteRingRules.h"
#include "SmallRingArithmetic.h"
#include <string>
#include <vector>
#include <memory>
#include <iostream>

class SmallRingArithmeticTest : public ::testing::Test {
protected:
    std::unique_ptr<FiniteRingRules> rules_;
    std::unique_ptr<SmallRingArithmetic> small_;
    std::vector<char> symbols_;
    char zero_;
    char one_;
    int size_;

    void SetUp() override {
        rules_ = std::make_unique<FiniteRingRules>("../config.yaml", "variant_1");
        small_ = std::make_unique<SmallRingArithmetic>(*rules_);
        
        zero_ = rules_->getZeroElement();
        one_ = rules_->getOneElement();
        size_ = rules_->getSize();
        
        for (int i = 0; i < size_; ++i) {
            symbols_.push_back(rules_->getValueChar(i));
        }
        
        std::cout << "\n--- Testing variant_1, size=" << size_ 
                  << ", symbols: ";
        for (char c : symbols_) std::cout << c << " ";
        std::cout << "---" << std::endl;
    }
};

// * --- АКСИОМА 1: Коммутатиность
TEST_F(SmallRingArithmeticTest, Axiom1_Addition_Commutative) {
    int tests = 0;
    for (char a : symbols_) {
        for (char b : symbols_) {
            char ab = small_->add(a, b);
            char ba = small_->add(b, a);
            
            EXPECT_EQ(ab, ba)
                << "Addition not commutative: " << a << "+" << b 
                << "=" << ab << " but " << b << "+" << a << "=" << ba;
            tests++;
        }
    }
    std::cout << "  Addition commutative (" << tests << " checks)" << std::endl;
}

TEST_F(SmallRingArithmeticTest, Axiom1_Multiplication_Commutative) {
    int tests = 0;
    for (char a : symbols_) {
        for (char b : symbols_) {
            char ab = small_->multiply(a, b);
            char ba = small_->multiply(b, a);
            
            EXPECT_EQ(ab, ba)
                << "  Multiplication not commutative: " << a << "*" << b 
                << "=" << ab << " but " << b << "*" << a << "=" << ba;
            tests++;
        }
    }
    std::cout << "   Multiplication commutative (" << tests << " checks)" << std::endl;
}

// * --- АКСИОМА 2: нейтральный элемент
TEST_F(SmallRingArithmeticTest, Axiom2_Additive_Identity) {
    for (char x : symbols_) {
        char result = small_->add(x, zero_);
        EXPECT_EQ(result, x)
            << "Additive identity failed: " << x << "+" << zero_ 
            << "=" << result << " (expected " << x << ")";
    }
    std::cout << "    Additive identity (zero)" << std::endl;
}

TEST_F(SmallRingArithmeticTest, Axiom2_Multiplicative_Identity) {
    for (char x : symbols_) {
        char result = small_->multiply(x, one_);
        EXPECT_EQ(result, x)
            << "Multiplicative identity failed: " << x << "*" << one_ 
            << "=" << result << " (expected " << x << ")";
    }
    std::cout << "    Multiplicative identity (one)" << std::endl;
}

// * --- АКСИОМА 3: ассоциативность
TEST_F(SmallRingArithmeticTest, Axiom3_Addition_Associative) {
    int tests = 0;
    for (char a : symbols_) {
        for (char b : symbols_) {
            for (char c : symbols_) {
                char left = small_->add(small_->add(a, b), c);
                char right = small_->add(a, small_->add(b, c));
                
                EXPECT_EQ(left, right)
                    << "Addition not associative: (" << a << "+" << b << ")+" << c 
                    << "=" << left << " but " << a << "+(" << b << "+" << c << ")=" << right;
                tests++;
            }
        }
    }
    std::cout << "    Addition associative (" << tests << " checks)" << std::endl;
}

TEST_F(SmallRingArithmeticTest, Axiom3_Multiplication_Associative) {
    int tests = 0;
    for (char a : symbols_) {
        for (char b : symbols_) {
            for (char c : symbols_) {
                char left = small_->multiply(small_->multiply(a, b), c);
                char right = small_->multiply(a, small_->multiply(b, c));
                
                EXPECT_EQ(left, right)
                    << "Multiplication not associative: (" << a << "*" << b << ")*" << c 
                    << "=" << left << " but " << a << "*(" << b << "*" << c << ")=" << right;
                tests++;
            }
        }
    }
    std::cout << "   Multiplication associative (" << tests << " checks)" << std::endl;
}

// * --- АКСИОМА 4: дистрибутивность
TEST_F(SmallRingArithmeticTest, Axiom4_Distributivity) {
    int tests = 0;
    for (char a : symbols_) {
        for (char b : symbols_) {
            for (char c : symbols_) {
                // a * (b + c) = a * b + a * c
                char left = small_->multiply(a, small_->add(b, c));
                char right = small_->add(
                    small_->multiply(a, b),
                    small_->multiply(a, c)
                );
                
                EXPECT_EQ(left, right)
                    << "Distributivity failed: " << a << "*(" << b << "+" << c << ")" 
                    << "=" << left << " but " << a << "*" << b << "+" << a << "*" << c 
                    << "=" << right;
                tests++;
            }
        }
    }
    std::cout << "   Distributivity (" << tests << " checks)" << std::endl;
}

// * --- АКСИОМА 5: ОБРАТНЫЕ ЭЛЕМЕНТЫ
TEST_F(SmallRingArithmeticTest, Axiom5_Additive_Inverse) {
    for (char x : symbols_) {
        char neg_x = small_->findAdditiveInverse(x);
        char sum = small_->add(x, neg_x);
        
        EXPECT_EQ(sum, zero_)
            << "Additive inverse failed: " << x << "+(-" << x << ")=" 
            << sum << " (expected " << zero_ << "), where -" << x << "=" << neg_x;
    }
    std::cout << "    Additive inverses exist for all elements" << std::endl;
}

TEST_F(SmallRingArithmeticTest, Axiom5_Multiplicative_Inverse_Exists) {
    int invertible = 0;
    int non_invertible = 0;
    
    for (char x : symbols_) {
        if (x == zero_) {
            // ноль не имеет мультипликативного обратного
            EXPECT_THROW(small_->findMultiplicativeInverse(x), std::runtime_error);
            non_invertible++;
            continue;
        }
        
        try {
            char inv = small_->findMultiplicativeInverse(x);
            char product = small_->multiply(x, inv);
            
            EXPECT_EQ(product, one_)
                << "Multiplicative inverse failed: " << x << "*" << inv 
                << "=" << product << " (expected " << one_ << ")";
            invertible++;
        } catch (const std::runtime_error&) {
            non_invertible++;
        }
    }
    
    std::cout << "    Multiplicative inverses: " << invertible << " invertible, " 
              << non_invertible << " non-invertible" << std::endl;
}

// * --- АКСИОМА 6: ЗАМКНУТОСТЬ
TEST_F(SmallRingArithmeticTest, Axiom6_Closure_Addition) {
    for (char a : symbols_) {
        for (char b : symbols_) {
            char result = small_->add(a, b);
            
            // результат должен быть валидным символом кольца
            EXPECT_TRUE(rules_->isValidChar(result))
                << "Addition not closed: " << a << "+" << b << "=" << result;
        }
    }
    std::cout << "    Addition is closed" << std::endl;
}

TEST_F(SmallRingArithmeticTest, Axiom6_Closure_Multiplication) {
    for (char a : symbols_) {
        for (char b : symbols_) {
            char result = small_->multiply(a, b);
            
            EXPECT_TRUE(rules_->isValidChar(result))
                << "Multiplication not closed: " << a << "*" << b << "=" << result;
        }
    }
    std::cout << "    Multiplication is closed" << std::endl;
}

TEST_F(SmallRingArithmeticTest, Axiom6_Closure_Subtraction) {
    for (char a : symbols_) {
        for (char b : symbols_) {
            char result = small_->subtract(a, b);
            
            EXPECT_TRUE(rules_->isValidChar(result))
                << "Subtraction not closed: " << a << "-" << b << "=" << result;
        }
    }
    std::cout << "    Subtraction is closed" << std::endl;
}

// * --- ДОПОЛНИТЕЛЬНЫЕ ТЕСТЫ
TEST_F(SmallRingArithmeticTest, Subtraction_Correctness) {
    for (char a : symbols_) {
        for (char b : symbols_) {
            // * a - b + b = a
            char diff = small_->subtract(a, b);
            char check = small_->add(diff, b);
            
            EXPECT_EQ(check, a)
                << "Subtraction incorrect: (" << a << "-" << b << ")+" << b 
                << "=" << check << " (expected " << a << ")";
        }
    }
    std::cout << "    Subtraction: (a-b)+b=a" << std::endl;
}

TEST_F(SmallRingArithmeticTest, Division_Correctness) {
    for (char a : symbols_) {
        for (char b : symbols_) {
            if (b == zero_) {
                EXPECT_THROW(small_->divide(a, b), std::runtime_error);
                continue;
            }
            
            try {
                // * (a / b) * b = a
                char quotient = small_->divide(a, b);
                char check = small_->multiply(quotient, b);
                
                EXPECT_EQ(check, a)
                    << "Division incorrect: (" << a << "/" << b << ")*" << b 
                    << "=" << check << " (expected " << a << ")";
            } catch (const std::runtime_error&) {
                // b не имеет обратного - это нормально
            }
        }
    }
    std::cout << "    Division: (a/b)*b=a (where defined)" << std::endl;
}

TEST_F(SmallRingArithmeticTest, PlusOne_Rule) {
    // проверяем что X + 1 дает следующий элемент
    for (int i = 0; i < size_; ++i) {
        char current = rules_->getValueChar(i);
        char next_expected = rules_->getValueChar((i + 1) % size_);
        char next_computed = small_->add(current, one_);
        
        EXPECT_EQ(next_computed, next_expected)
            << "Plus-one rule failed: " << current << "+1=" << next_computed 
            << " (expected " << next_expected << ")";
    }
    std::cout << "    Plus-one rule: X+1 gives next element" << std::endl;
}

// int main(int argc, char** argv) {
//     ::testing::InitGoogleTest(&argc, argv);
    
//     int result = RUN_ALL_TESTS();

//     return result;
// }
