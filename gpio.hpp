#pragma once

#include <wiringPiCpp>

/**
 * @brief This class makes it possible to use the GPIO pins of the Raspberry Pi.
 */
class GPIO {
  public:
    /**
     * @brief Decides if a pin is set to input or output mode.
     */
    enum class PinMode {
      Input = static_cast<int>(wiringPi::PinMode::Input),   /**< Pin mode is set to input mode */
      Output = static_cast<int>(wiringPi::PinMode::Output)  /**< Pin mode is set to output mode */
    };

    /**
     * @brief Decides if a pin is set to logical high (1) or logical low (0).
     */
    enum class Digital {
      High = static_cast<int>(wiringPi::Digital::High), /**< Pin is set to logical high (1) */
      Low = static_cast<int>(wiringPi::Digital::Low)    /**< Pin is set to logical low (0) */
    };

    /**
     * @brief Stores the SPI pins of the Raspberry Pi.
     */
    enum Spi {
      Clock = 18,   /**< SPI clock pin */
      Miso = 23,    /**< SPI MISO pin */
      Mosi = 24,    /**< SPI MOSI pin */
      Cs = 25       /**< SPI CS pin */
    };

    /**
     * @brief Stores the commands of the Adafruit PWM/servo driver specified in the datasheet on page 9:
     * http://www.adafruit.com/datasheets/PCA9685.pdf
     */
    enum Adafruit {
      Mode1 = 0x00,         /**< Mode register 1 */
      Prescale = 0xFE,      /**< Prescaler for output frequency */
      Led0OnLow = 0x06,     /**< LED0 output and brightness control byte 0 */
      Led0OnHigh = 0x07,    /**< LED0 output and brightness control byte 1 */
      Led0OffLow = 0x08,    /**< LED0 output and brightness control byte 2 */
      Led0OffHigh = 0x09    /**< LED0 output and brightness control byte 3 */
    };

    explicit GPIO(const unsigned int& i2cDeviceId);

    GPIO(const GPIO&) = delete;
    GPIO& operator=(const GPIO&) = delete;

    void setPWMFreq(const double& frequency) const;

    void setPinMode(const unsigned int& pin, const PinMode& mode) const;
    void writePin(const unsigned int& pin, const Digital& value) const;
    Digital readPin(const unsigned int& pin) const;

    void writeI2C(const unsigned int& reg, const unsigned int& value) const;
    unsigned int readI2C(const unsigned int& reg) const;



  protected:
    /**
     * @brief Instance of the C++ wiringPi wrapper to control the GPIO pins
     */
    const wiringPi::WiringPi wiringPi_;

    /**
     * @brief I2C instance of the C++ wiringPi wrapper to support I2C with wiringPi
     */
    const wiringPi::I2C i2c_;
};

