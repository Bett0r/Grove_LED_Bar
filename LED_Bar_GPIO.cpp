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

#include "LED_Bar_GPIO.hpp"
#include <thread>
#include <chrono>

LED_Bar_GPIO::LED_Bar_GPIO(const std::shared_ptr<GPIO> gpio, unsigned char pinClock, unsigned char pinData, bool greenToRed)
: gpio_(gpio)
{
  __pinClock = pinClock;
  __pinData = pinData;
  __greenToRed = greenToRed;  // ascending or decending

  for (auto i = 0; i < 10; i++)
    __state[i] = 0x00;  // persist state so individual leds can be toggled

  gpio_->setPinMode(__pinClock,GPIO::PinMode::Output);
  gpio_->setPinMode(__pinData, GPIO::PinMode::Output);
}


// Send the latch command
void LED_Bar_GPIO::latchData()
{
    gpio_->writePin(__pinData, GPIO::Digital::Low);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

  for (unsigned char i = 0; i < 4; i++)
  {
	  gpio_->writePin(__pinData, GPIO::Digital::High);
	  gpio_->writePin(__pinData, GPIO::Digital::Low);

  }
}


// Send 16 bits of data
void LED_Bar_GPIO::sendData(unsigned int data)
{
  for (unsigned char i = 0; i < 16; i++)
  {
    GPIO::Digital state = (data & 0x8000) ? GPIO::Digital::High : GPIO::Digital::Low;
    gpio_->writePin(__pinData, state);

    state = (gpio_->readPin(__pinClock)==GPIO::Digital::High) ? GPIO::Digital::Low : GPIO::Digital::High;
    gpio_->writePin(__pinData, state);

    data <<= 1;
  }
}


// Change the orientation
// Green to red, or red to green
void LED_Bar_GPIO::setGreenToRed(bool greenToRed)
{
  __greenToRed = greenToRed;

  setBits(__state);
}


// Set level (0-10)
// Level 0 means all leds off
// Level 10 means all leds on
// Level 4.5 means 4 LEDs on and the 5th LED's half on
void LED_Bar_GPIO::setLevel(float level)
{
  level = std::max(0, std::min(10, level));l
  level *= 8; // there are 8 (noticable) levels of brightness on each segment
  
  // Place number of 'level' of 1-bits on __state
  for (auto i = 0; i < 10; i++) {
    __state[i] = (level > 8) ? ~0 :
                 (level > 0) ? ~(~0 << byte(level)) : 0;
               
    level -= 8;
  };

  setBits(__state);
}


// Set a single led
// led (1-10)
// brightness (0-1)
void LED_Bar_GPIO::setLed(unsigned char led, float brightness)
{
  led = max(1, min(10, led));
  brightness = max(0, min(brightness, 1));

  // Zero based index 0-9 for bitwise operations
  led--;

  // 8 (noticable) levels of brightness
  // 00000000 darkest
  // 00000011 brighter
  // ........
  // 11111111 brightest
  __state[led] = ~(~0 << (unsigned char) (brightness*8));

  setBits(__state);
}


// Toggle a single led
// led (1-10)
void LED_Bar_GPIO::toggleLed(unsigned char led)
{
  led = max(1, min(10, led));

  // Zero based index 0-9 for bitwise operations
  led--;

  __state[led] = __state[led] ? 0 : ~0;

  setBits(__state);
}


// each element in the state will hold the brightness level
// 00000000 darkest
// 00000011 brighter
// ........
// 11111111 brightest
void LED_Bar_GPIO::setBits(unsigned char bits[])
{

  __state = bits & 0x3FF;

  sendData(GLB_CMDMODE);

  for (unsigned char i = 0; i < 10; i++)
  {
    if (__greenToRed)
    {
	  // Go backward on __state
      sendData(bits[10-i-1]);
    }
    else
    {
	  // Go forward on __state
      sendData(bits[i]);
    }
  }

  // Two extra empty bits for padding the command to the correct length
  sendData(0x00);
  sendData(0x00);

  latchData();
}


// Return the current state
unsigned char const *LED_Bar_GPIO::getBits()
{
  return __state;
}
