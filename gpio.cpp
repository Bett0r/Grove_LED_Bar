#include <stewartPlatform/gpio.hpp>

#include <thread>
#include <chrono>
#include <cmath>

/**
 * @brief Set up the I2C connection with wiringPi.
 * @param i2cDeviceId   ID of the connected I2C device
 */
GPIO::GPIO(const unsigned int& i2cDeviceId)
  : wiringPi_(wiringPi::PinNumbering::Gpio),
    i2c_(i2cDeviceId) {
  writeI2C(Adafruit::Mode1, 0x00);
  setPWMFreq(1000.0);

  //initialize GPIO pins. debugging distinction in GPIO::pinMode
  setPinMode(Spi::Mosi, PinMode::Output);
  setPinMode(Spi::Miso, PinMode::Input);
  setPinMode(Spi::Clock, PinMode::Output);
  setPinMode(Spi::Cs, PinMode::Output);
}


/**
 * @brief Set the PWM frequency of the I2C device.
 * @param frequency Desired frequency
 */
void GPIO::setPWMFreq(const double& frequency) const {
  double prescaleval = 25000000.0;
  prescaleval /= 4096.0;
  prescaleval /= frequency;
  prescaleval -= 1.0;

  unsigned int prescale = std::floor(prescaleval + 0.5);

  unsigned int oldmode = readI2C(Adafruit::Mode1);
  writeI2C(Adafruit::Mode1, (oldmode & 0x7F) | 0x10);
  writeI2C(Adafruit::Prescale, prescale);
  writeI2C(Adafruit::Mode1, oldmode);
  std::this_thread::sleep_for(std::chrono::milliseconds(5));
  writeI2C(Adafruit::Mode1, oldmode | 0xa1);    // This sets the MODE1 register to turn on auto increment.
}


/**
 * @brief Sets the mode of the specified pin.
 * @param pin   Desired pin to change the mode from
 * @param mode  Desired pin mode to set the pin to
 */
void GPIO::setPinMode(const unsigned int& pin, const PinMode& mode) const {
  wiringPi_.pinMode(pin, static_cast<wiringPi::PinMode>(mode));
}


/**
 * @brief Write value to the specified pin (pin mode has to be PinMode::Input).
 * @param pin   Pin to write the value to
 * @param value Value to write to pin
 */
void GPIO::writePin(const unsigned int& pin, const Digital& value) const {
  wiringPi_.digitalWrite(pin, static_cast<wiringPi::Digital>(value));
  std::this_thread::sleep_for(std::chrono::microseconds(100));
}


/**
 * @brief Read value from the specified pin (pin mode has to be PinMode::Output).
 * @param pin   Desired pin to read the vslue from
 * @return      Returns the input value of the specified pin
 */
GPIO::Digital GPIO::readPin(const unsigned int& pin) const {
  return static_cast<GPIO::Digital>(wiringPi_.digitalRead(pin));
}


/**
 * @brief Write I2C value to the specified pin (pin mode has to be PinMode::Input).
 * @param reg   Pin to write the value to
 * @param value Value to write to the pin
 */
void GPIO::writeI2C(const unsigned int& reg, const unsigned int& value) const {
  i2c_.writeReg8(reg, value);
  std::this_thread::sleep_for(std::chrono::microseconds(100));
}


/**
 * @brief   Reads the I2C value of the specified pin (pin mode has o be PinMode::Input).
 * @param reg   Pin to read from
 * @return  Incoming value of the specified pin
 */
unsigned int GPIO::readI2C(const unsigned int& reg) const {
  return i2c_.readReg8(reg);
}
