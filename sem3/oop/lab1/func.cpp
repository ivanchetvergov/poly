// func.cpp
#include "func.hpp"
#include "iostream"

void f1() {
	for (double x = 0.; x <= 4.; x += 0.1){
		double y;
		if (x <= 1 && x >= 0) y = 0;
		else if (x <= 2) y = 2 * x - 2; 
		else if (x <= 3) y = -2 * x + 6;
		else y = 0; 
		std::cout << "x = " << x << ", y = " << y << std::endl;
	}
}

void f2() {
    for (double x = 0.; x <= 4.; x += 0.1) {
        double y = (x < 1.) ? 0. : ((x <= 2.) ? 2. * x - 2. : ((x <= 3.) ? -2. * x + 6. : 0.));
        std::cout << "x = " << x << ", y = " << y << std::endl;
    }
}

void f3() {
    std::cout << "\nвведите символы (a, b, c, q или Esc):\n";
    int count = 0;
    for (;;) { // inf loop
        char c;
        std::cin >> c; //getch
        count++;

        switch (c) {
            case 'a':
                std::cout << "Ok\n";
                break;
            case 'b':
                std::cout << "Bell (alert - звуковой сигнал)\n";
                std::cout << "\a";
                break;
            case 'c':
                std::cout << "кол-во введенных символов: " << count << std::endl;
                break;
            case 27: // ASCII Esc
                std::cout << "to quit use 'q'\n";
                break;
            case 'q':
                std::cout << "Bye\n";
                return; // exit
            default:
                std::cout << "unkwnown command\n";
                break;
        }
    }
}

void f4() {
     short flags = 0xaabb; // 10101010 10111011
    std::cout << "was: " << std::hex << flags << std::endl;
    
    // достаем байты путем сдвига
    unsigned short high_byte = (flags >> 8). & (0x00ff);  
    unsigned short low_byte = (flags << 8); 

    unsigned short swapped_flags = high_byte | low_byte; // "складываем" результат

    std::cout << "now: " << std::hex << swapped_flags << std::endl;
}

void f5() {
    unsigned char byte = 0x26; //  0010 0110
    std::cout << "was: " << std::hex << (int)byte << std::endl;

    
    byte |= 0b00001000;
    std::cout << "третий бит: " << std::hex << (int)byte << std::endl; //  0x2e (0010 1110)

     bbyte ^= 0b00000011; // XOR 
     bbyte ^= 0b00000011; // XOR 

     std::cout << "инверт 2 мл бита: " << std::hex << (int)byte << std::endl; // 0x2d (0010 1101)
    
    byte &= 0b11110000; // "умножаем хвост"
    std::cout << "занулили 4 мл бита: " << std::hex << (int)byte << std::endl; // Результат: 0x20 (0010 0000)
}