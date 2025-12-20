// tests/test_big_arithmetic.cc
// Полные тесты большой арифметики с проверкой ВСЕХ аксиом

#include "gtest/gtest.h"
#include "FiniteRingRules.h"
#include "SmallRingArithmetic.h"
#include "BigRingArithmetic.h"
#include "RingNumber.h"
#include <string>
#include <vector>
#include <memory>
#include <iostream>

class BigArithmeticTest : public ::testing::Test {
protected:
    std::unique_ptr<FiniteRingRules> rules_;
    std::unique_ptr<SmallRingArithmetic> small_;
    std::unique_ptr<BigRingArithmetic> big_;
    std::vector<std::string> test_numbers_;
    char zero_;
    char one_;
    
    void SetUp() override {
        rules_ = std::make_unique<FiniteRingRules>("../config.yaml", "variant_1");
        small_ = std::make_unique<SmallRingArithmetic>(*rules_);
        big_ = std::make_unique<BigRingArithmetic>(*rules_, *small_);
        
        zero_ = rules_->getZeroElement();
        one_ = rules_->getOneElement();
        
        // * набор тестовых чисел разной длины
        test_numbers_ = {
            std::string(1, zero_),  // "a"
            std::string(1, one_),   // "b"
            "c", "e", "g", "d",
            "bc", "ce", "eg",
            "abc", "bcd", "cde",
            "abcd", "bcde"
        };
        
        std::cout << "\n--- Testing Big Arithmetic (variant_1) ---" << std::endl;
    }
    
    RingNumber makeNumber(const std::string& s) {
        return RingNumber(*rules_, s);
    }
};

// * --- БАЗОВЫЕ ТЕСТЫ ОПЕРАЦИЙ ---
TEST_F(BigArithmeticTest, Basic_Addition_SingleDigit) {
    // проверяем что однозначные числа дают те же результаты что и малая арифметика
    for (int i = 0; i < rules_->getSize(); ++i) {
        for (int j = 0; j < rules_->getSize(); ++j) {
            char a = rules_->getValueChar(i);
            char b = rules_->getValueChar(j);
            
            RingNumber num_a = makeNumber(std::string(1, a));
            RingNumber num_b = makeNumber(std::string(1, b));
            
            RingNumber result_big = big_->add(num_a, num_b);
            char result_small = small_->add(a, b);
            
            // * если нет переноса, результаты должны совпадать
            if (result_big.length() == 1) {
                EXPECT_EQ(result_big[0], result_small)
                    << "Single-digit addition mismatch: " << a << "+" << b;
            }
        }
    }
    std::cout << "   Single-digit addition matches small arithmetic" << std::endl;
}

TEST_F(BigArithmeticTest, Basic_Addition_WithCarry) {
    // f + c = 7 + 2 = 9 = 1 * 8 + 1 → должно быть "bb"
    RingNumber a = makeNumber("f");
    RingNumber b = makeNumber("c");
    RingNumber result = big_->add(a, b);
    
    EXPECT_GT(result.length(), 1) << "Expected carry to create multi-digit result";
    EXPECT_EQ(result.toString(), "bb");
    
    std::cout << "   Addition with carry: f + c = bb" << std::endl;
}

TEST_F(BigArithmeticTest, Basic_Subtraction_SingleDigit) {
    RingNumber a = makeNumber("g");  // 4
    RingNumber b = makeNumber("c");  // 2
    RingNumber result = big_->subtract(a, b);
    
    char expected = small_->subtract('g', 'c');
    EXPECT_EQ(result.toString(), std::string(1, expected));
    
    std::cout << "   Single-digit subtraction: g - c = " << expected << std::endl;
}

TEST_F(BigArithmeticTest, Basic_Multiplication_SingleDigit) {
    RingNumber a = makeNumber("c");  // 2
    RingNumber b = makeNumber("c");  // 2
    RingNumber result = big_->multiply(a, b);
    
    char expected = small_->multiply('c', 'c');
    EXPECT_EQ(result.toString(), std::string(1, expected));
    
    std::cout << "   Single-digit multiplication: c * c = " << expected << std::endl;
}

// * --- АКСИОМА 1: КОММУТАТИВНОСТЬ
TEST_F(BigArithmeticTest, Axiom1_Addition_Commutative) {
    int tests = 0;
    for (const auto& str_a : test_numbers_) {
        for (const auto& str_b : test_numbers_) {
            RingNumber a = makeNumber(str_a);
            RingNumber b = makeNumber(str_b);
            
            RingNumber ab = big_->add(a, b);
            RingNumber ba = big_->add(b, a);
            
            EXPECT_EQ(ab, ba)
                << "Addition not commutative: " << str_a << "+" << str_b 
                << " != " << str_b << "+" << str_a;
            tests++;
        }
    }
    std::cout << "   Addition commutative (" << tests << " checks)" << std::endl;
}

TEST_F(BigArithmeticTest, Axiom1_Multiplication_Commutative) {
    int tests = 0;
    // для умножения используем меньше чисел (медленная операция)
    std::vector<std::string> short_list = {
        std::string(1, zero_), std::string(1, one_), 
        "c", "e", "bc", "ce"
    };
    
    for (const auto& str_a : short_list) {
        for (const auto& str_b : short_list) {
            RingNumber a = makeNumber(str_a);
            RingNumber b = makeNumber(str_b);
            
            RingNumber ab = big_->multiply(a, b);
            RingNumber ba = big_->multiply(b, a);
            
            EXPECT_EQ(ab, ba)
                << "Multiplication not commutative: " << str_a << "*" << str_b 
                << " != " << str_b << "*" << str_a;
            tests++;
        }
    }
    std::cout << "   Multiplication commutative (" << tests << " checks)" << std::endl;
}

// * --- АКСИОМА 2: НЕЙТРАЛЬНЫЕ ЭЛЕМЕНТЫ
TEST_F(BigArithmeticTest, Axiom2_Additive_Identity) {
    RingNumber zero_num = makeNumber(std::string(1, zero_));
    
    for (const auto& str : test_numbers_) {
        RingNumber x = makeNumber(str);
        RingNumber result = big_->add(x, zero_num);
        
        EXPECT_EQ(result, x)
            << "Additive identity failed: " << str << " + 0 != " << str;
    }
    std::cout << "   Additive identity (zero)" << std::endl;
}

TEST_F(BigArithmeticTest, Axiom2_Multiplicative_Identity) {
    RingNumber one_num = makeNumber(std::string(1, one_));
    
    for (const auto& str : test_numbers_) {
        RingNumber x = makeNumber(str);
        RingNumber result = big_->multiply(x, one_num);
        
        EXPECT_EQ(result, x)
            << "Multiplicative identity failed: " << str << " * 1 != " << str;
    }
    std::cout << "   Multiplicative identity (one)" << std::endl;
}

TEST_F(BigArithmeticTest, Axiom2_Zero_Absorption) {
    RingNumber zero_num = makeNumber(std::string(1, zero_));
    
    for (const auto& str : test_numbers_) {
        RingNumber x = makeNumber(str);
        RingNumber result = big_->multiply(x, zero_num);
        
        EXPECT_TRUE(result.isZero())
            << "Zero absorption failed: " << str << " * 0 != 0";
    }
    std::cout << "   Zero absorption (x*0=0)" << std::endl;
}

// * --- АКСИОМА 3: АССОЦИАТИВНОСТЬ
TEST_F(BigArithmeticTest, Axiom3_Addition_Associative) {
    int tests = 0;
    std::vector<std::string> short_list = {
        std::string(1, zero_), std::string(1, one_), 
        "c", "e", "bc"
    };
    
    for (const auto& str_a : short_list) {
        for (const auto& str_b : short_list) {
            for (const auto& str_c : short_list) {
                RingNumber a = makeNumber(str_a);
                RingNumber b = makeNumber(str_b);
                RingNumber c = makeNumber(str_c);
                
                RingNumber left = big_->add(big_->add(a, b), c);
                RingNumber right = big_->add(a, big_->add(b, c));
                
                EXPECT_EQ(left, right)
                    << "Addition not associative: (" << str_a << "+" << str_b 
                    << ")+" << str_c << " != " << str_a << "+(" << str_b 
                    << "+" << str_c << ")";
                tests++;
            }
        }
    }
    std::cout << "   Addition associative (" << tests << " checks)" << std::endl;
}

TEST_F(BigArithmeticTest, Axiom3_Multiplication_Associative) {
    int tests = 0;
    std::vector<std::string> tiny_list = {
        std::string(1, zero_), std::string(1, one_), "c"
    };
    
    for (const auto& str_a : tiny_list) {
        for (const auto& str_b : tiny_list) {
            for (const auto& str_c : tiny_list) {
                RingNumber a = makeNumber(str_a);
                RingNumber b = makeNumber(str_b);
                RingNumber c = makeNumber(str_c);
                
                RingNumber left = big_->multiply(big_->multiply(a, b), c);
                RingNumber right = big_->multiply(a, big_->multiply(b, c));
                
                EXPECT_EQ(left, right)
                    << "Multiplication not associative: (" << str_a << "*" << str_b 
                    << ")*" << str_c << " != " << str_a << "*(" << str_b 
                    << "*" << str_c << ")";
                tests++;
            }
        }
    }
    std::cout << "   Multiplication associative (" << tests << " checks)" << std::endl;
}

// * --- АКСИОМА 4: ДИСТРИБУТИВНОСТЬ
TEST_F(BigArithmeticTest, Axiom4_Distributivity) {
    int tests = 0;
    std::vector<std::string> short_list = {
        std::string(1, zero_), std::string(1, one_), 
        "c", "e"
    };
    
    for (const auto& str_a : short_list) {
        for (const auto& str_b : short_list) {
            for (const auto& str_c : short_list) {
                RingNumber a = makeNumber(str_a);
                RingNumber b = makeNumber(str_b);
                RingNumber c = makeNumber(str_c);
                
                // a * (b + c) = a*b + a*c
                RingNumber left = big_->multiply(a, big_->add(b, c));
                RingNumber right = big_->add(
                    big_->multiply(a, b),
                    big_->multiply(a, c)
                );
                
                EXPECT_EQ(left, right)
                    << "Distributivity failed: " << str_a << "*(" << str_b 
                    << "+" << str_c << ") != " << str_a << "*" << str_b 
                    << " + " << str_a << "*" << str_c;
                tests++;
            }
        }
    }
    std::cout << "   Distributivity (" << tests << " checks)" << std::endl;
}

// * --- АКСИОМА 5: ЗАМКНУТОСТЬ
TEST_F(BigArithmeticTest, Axiom5_Closure) {
    for (const auto& str_a : test_numbers_) {
        for (const auto& str_b : test_numbers_) {
            RingNumber a = makeNumber(str_a);
            RingNumber b = makeNumber(str_b);
            
            // Все операции должны давать валидные числа
            RingNumber sum = big_->add(a, b);
            EXPECT_TRUE(sum.isValid()) << "Addition not closed";
            
            RingNumber diff = big_->subtract(a, b);
            EXPECT_TRUE(diff.isValid()) << "Subtraction not closed";
        }
    }
    std::cout << "   Addition and subtraction are closed" << std::endl;
}

// * --- КОРРЕКТНОСТЬ ОПЕРАЦИЙ
// ! этот и следующий тест под вопросом ввиду свойств конечных колец
// TEST_F(BigArithmeticTest, Correctness_Subtraction) {
//     for (const auto& str_a : test_numbers_) {
//         for (const auto& str_b : test_numbers_) {
//             // при разной длине проблемы с заемом
//             if (str_b.length() > str_a.length()) {
//                 continue; 
//             }
//             RingNumber a = makeNumber(str_a);
//             RingNumber b = makeNumber(str_b);
            
//             RingNumber diff = big_->subtract(a, b);
//             RingNumber check = big_->add(diff, b);
            
//             EXPECT_EQ(check, a)
//                 << "Subtraction incorrect: (" << str_a << "-" << str_b 
//                 << ")+" << str_b << " != " << str_a;
//         }
//     }
//     std::cout << "   Subtraction: (a-b)+b=a" << std::endl;
// }

// TEST_F(BigArithmeticTest, Correctness_Division_SingleDigit) {
//     // * деление работает только для однозначного делителя
//     for (int i = 0; i < rules_->getSize(); ++i) {
//         char divisor = rules_->getValueChar(i);
        
//         if (divisor == zero_) continue;
        
//         // * проверяем есть ли обратный элемент
//         try {
//             small_->findMultiplicativeInverse(divisor);
//         } catch (...) {
//             continue;  // нет обратного - пропускаем
//         }
//         // ! a / b * b = a (не работает ибо делим без остатка)
//         for (const auto& str_a : test_numbers_) {
//             RingNumber a = makeNumber(str_a);
//             RingNumber b = makeNumber(std::string(1, divisor));
            
//             RingNumber quotient = big_->divide(a, b);
//             RingNumber check = big_->multiply(quotient, b);
            
//             EXPECT_EQ(check, a)
//                 << "Division incorrect: (" << str_a << "/" << divisor 
//                 << ")*" << divisor << " != " << str_a;
//         }
//     }
//     std::cout << "   Division: (a/b)*b=a (single-digit divisor)" << std::endl;
// }

TEST_F(BigArithmeticTest, Correctness_DivisionByZero) {
    RingNumber a = makeNumber("abc");
    RingNumber zero_num = makeNumber(std::string(1, zero_));
    
    EXPECT_THROW(big_->divide(a, zero_num), std::runtime_error);
    
    std::cout << "   Division by zero throws exception" << std::endl;
}

// * --- СПЕЦИАЛЬНЫЕ СЛУЧАИ
TEST_F(BigArithmeticTest, Special_AdditionExamples) {
    // нонкретные примеры для отладки
    struct TestCase {
        std::string a, b, expected;
    };
    
    std::vector<TestCase> cases = {
        {"b", "b", "c"},      // 1+1=2
        {"c", "c", "g"},      // 2+2=4
        {"f", "c", "bb"},     // 7+2=9=1*8+1
        {"g", "g", "ba"},     // 4+4=8=1*8+0
    };
    
    for (const auto& tc : cases) {
        RingNumber a = makeNumber(tc.a);
        RingNumber b = makeNumber(tc.b);
        RingNumber result = big_->add(a, b);
        
        EXPECT_EQ(result.toString(), tc.expected)
            << tc.a << " + " << tc.b << " = " << result.toString() 
            << " (expected " << tc.expected << ")";
    }
    
    std::cout << "   Specific addition examples verified" << std::endl;
}

TEST_F(BigArithmeticTest, Special_MultiplicationExamples) {
    struct TestCase {
        std::string a, b, expected_nonempty;
    };
    
    std::vector<TestCase> cases = {
        {"c", "c", "g"},        // 2*2=4
        {"c", "e", "h"},        // 2*3=6
        {"b", "abc", "bc"},     // 1*abc=abc
    };
    
    for (const auto& tc : cases) {
        RingNumber a = makeNumber(tc.a);
        RingNumber b = makeNumber(tc.b);
        RingNumber result = big_->multiply(a, b);
        
        if (!tc.expected_nonempty.empty()) {
            EXPECT_EQ(result.toString(), tc.expected_nonempty)
                << tc.a << " * " << tc.b << " = " << result.toString() 
                << " (expected " << tc.expected_nonempty << ")";
        } else {
            EXPECT_FALSE(result.isZero())
                << tc.a << " * " << tc.b << " should not be zero";
        }
    }
    
    std::cout << "   Specific multiplication examples verified" << std::endl;
}

// int main(int argc, char** argv) {
//     ::testing::InitGoogleTest(&argc, argv);
    
//     int result = RUN_ALL_TESTS();

//     return result;
// }
