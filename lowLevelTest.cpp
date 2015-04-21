#include <wiringPiCpp>
#include <cstdlib>
#include <array>
#include <chrono>
#include <thread>

wiringPi::WiringPi gpio(wiringPi::PinNumbering::Gpio);

void setLevel(double level) {
  sendData({false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false}); // sendData(GLB_CMDMODE);

  for (unsigned int n = 0; n < std::floor(level); ++n) {
    sendData({false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, true}); // data
  }

  if (std::floor(level) != level) {
    unsigned int discreteLevel = 15 - std::floor(level * 8);
    std::array<bool, 16> data = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};

    for (unsigned int n= 15; n > discreteLevel; --n) {
      data.at(k) = true;
    }

    sendData(data);
  }

  for (unsigned int n = std::ceil(level); n < 10; ++n) {
    sendData({false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, true}); // data
    level -= 1;
  }

  sendData({false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false}); // unused LEDs
  sendData({false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false}); // unused LEDs

  sendLatchData();
}

void sendData(std::array<bool, 16> data) {
  for (std::size_t n = 0; n < data.size(); ++n) {
    gpio.digitalWrite(10, data.at(i) ? wiringPi::Digital::High : wiringPi::Digital::Low);
    gpio.digitalWrite(8, gpio.digitalRead(8) == wiringPi::Digital::High ? wiringPi::Digital::Low : wiringPi::Digital::High);
  }
}

void sendLatchData() {
  gpio.digitalWrite(10, wiringPi::Digital::Low);
  std::this_thread::sleep_for(std::chrono::microseconds(10));

  for (unsigned char n = 0; n < 4; ++n) {
    gpio.digitalWrite(10, wiringPi::Digital::High);
    gpio.digitalWrite(10, wiringPi::Digital::Low);
  }
}

int main() {

  for(int n = 0; n < 11; ++n) {
    setLevel(n);
    std::this_thread::sleep_for(std::chrono::seconds(2));
  }

  for(int n = 10; n >= 0; --n) {
      setLevel(n);
      std::this_thread::sleep_for(std::chrono::seconds(2));
  }

  return EXIT_SUCCESS;
}

