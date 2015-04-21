#include "LED_Bar_GPIO.hpp"
#include <iostream>


/*

Basic Control translated from Basic Control

*/
int main(){

    std::shared_ptr<GPIO> gpio(new GPIO(0x40));
    //gpio*, clock, data, greentoRed
    LED_Bar_GPIO ledBar (gpio, 8, 10, false);




    std::cout << "begin:" << std::endl;
    ledBar.begin();

    for (int i = 0; i <= 20; ++i) {
        std::cout << "setLevel: "<< i*0.5 << std::endl;
    ledBar.setLevel(i*0.5);

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    // Turn on all LEDs
    std::cout << "All LEDs on" << std::endl;
    ledBar.setBits(0x3ff);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // Turn off all LEDs
    std::cout << "All LEDs off" << std::endl;
    ledBar.setBits(0x0);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // Turn on LED 1
    std::cout << "LED 1 on" << std::endl;
    // 0b000000000000001 can also be written as 0x1:
    ledBar.setBits(0b000000000000001);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // Turn on LEDs 1 and 3
    std::cout << "LED 1 + 3 on" << std::endl;
    // 0b000000000000101 can also be written as 0x5:
    ledBar.setBits(0b000000000000101);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // Turn on LEDs 1, 3, 5, 7, 9
    std::cout << "LED 1, 3, 5, 7 + 9 on" << std::endl;
    ledBar.setBits(0x155);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // Turn on LEDs 2, 4, 6, 8, 10
    std::cout << "LED 2, 4, 6, 8 + 10 on" << std::endl;
    ledBar.setBits(0x2AA);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // Turn on LEDs 1, 2, 3, 4, 5
    std::cout << "LED 1, 2, 3, 4 + 5 on" << std::endl;
    // 0b000000000011111 == 0x1F
    ledBar.setBits(0b000000000011111);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // Turn on LEDs 6, 7, 8, 9, 10
    std::cout << "LED 6, 7, 8, 9 + 10 on" << std::endl;
    // 0b000001111100000 == 0x3E0
    ledBar.setBits(0b000001111100000);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::cout << "Finished LED Test" << std::endl;
    return EXIT_SUCCESS;
}
