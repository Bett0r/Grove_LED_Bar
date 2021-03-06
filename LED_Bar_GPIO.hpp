/*
  LED bar library V2.0
  2010 Copyright (c) Seeed Technology Inc.  All right reserved.

  Original Author: LG

  Modify: Loovee, 2014-2-26
  User can choose which Io to be used.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
/*
  Modify: Long, 2015-01-07
  User can change the brightness level for each LED segment
  Rename constant to avoid name conflict
*/
#ifndef Grove_LED_Bar_H
#define Grove_LED_Bar_H

// Avoid name conflict
#define GLB_CMDMODE 0x00  // Work on 8-bit mode
#define GLB_ON      0xff  // 8-bit 1 data
#define GLB_OFF     0x00  // 8-bit 0 data

#include <memory>
#include "gpio.hpp"
#include <thread>
#include <chrono>

class LED_Bar_GPIO
{

private:

  unsigned int __pinClock;  // Clock pin
  unsigned int __pinData;   // Data pin
  bool __greenToRed;        // Orientation (0 = red to green, 1 = green to red)
  unsigned char __state[10];// Led state, brightness for each LED

  void sendData(unsigned int data);  // Send a word to led bar
  void latchData(void);              // Load data into the latch register
  
protected:
  /**
   * @brief Instance of the GPIO object to read the sensor values from.
   */

  const std::shared_ptr<GPIO> gpio_;

public:

  explicit LED_Bar_GPIO(const std::shared_ptr<GPIO> gpio, unsigned char pinClock, unsigned char pinData, bool greenToRed);  // Initialize
  void begin(){gpio_->setPinMode(__pinClock, GPIO::PinMode::Output); gpio_->setPinMode(__pinData, GPIO::PinMode::Output);}
  void setGreenToRed(bool greenToRed);             // (Re)set orientation
  void setLevel(float level);                      // Set level, range from 0 to 10
  void setLed(unsigned char led, float brightness);// Set brightness for a single led, range from 0 to 1
  void toggleLed(unsigned char led);               // Toggle a single led
  void setBits(unsigned char bits[]);              // Toggle leds to match given bits
  void setBits(int bits);                          // Toggle leds to match given bits
  unsigned char const *getBits();                        // Get the current state
};

#endif
