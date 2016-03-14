/*
 * RGB Fader
 * Copyright (C) 2016 Nicola Corna <nicola@corna.info>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <Arduino.h>

#define MAX_COLORS 16

class RGB {
public:
  uint8_t red;
  uint8_t green;
  uint8_t blue;

  RGB(uint8_t red, uint8_t green, uint8_t blue) : red(red), green(green), blue(blue) {}
  RGB(const RGB& copy) : red(copy.red), green(copy.green), blue(copy.blue) {}
  RGB() : RGB(0, 0, 0) {}
};

class RGBFader {
public:
  enum curveType {
    LINEAR,
    QUADRATIC,
    EXPONENTIAL
  };

  static const RGB rainbow[];
  static const uint8_t rainbowSize;
  static const RGB rainbowAndWhite[];
  static const uint8_t rainbowAndWhiteSize;
  static const RGB redGreenBlue[];
  static const uint8_t redGreenBlueSize;
  
  static const uint8_t exponentialLookup[256];
  
  RGBFader(const RGB& rgb_pins, const RGB colors[], const uint8_t colorsNum, const uint8_t color_steps = 64, const uint8_t start_color = 0,
           const uint8_t brightness_speed = 4, const uint8_t initialBrightness = 255, const bool colorCycle = false, curveType curve = EXPONENTIAL);
  void nextStep();
  void goOnColorIndex(uint8_t index);
  
  static inline void linear(int pin, uint8_t value) {
    analogWrite(pin, value);
  }

  static inline void quadratic(int pin, uint8_t value) {
    analogWrite(pin, map((uint16_t)(value)*value, 0, 65025, 0, 255));
  }

  static inline void exponential(int pin, uint8_t value) {
    analogWrite(pin, pgm_read_byte_near(exponentialLookup + value));
  }

/*
  static inline void exponential(int pin, uint8_t value) {
    analogWrite(pin, round(pow(2, value / 31.875)) - 1);
  }
*/

  inline void setColors(const RGB colors[], const uint8_t colorsNum) {
    setColorsOffset(colors, colorsNum, 0, 0);
  }

  inline void setColors(const RGB& colors) {
    setColorsOffset(&colors, 1, 0, 0);
  }
  
  inline void setNextColors(const RGB colors[], const uint8_t colorsNum) {
    this->colors[0] = color;
    setColorsOffset(colors, colorsNum, 1, 0);
  }

  inline void setNextColors(const RGB& colors) {
    this->colors[0] = color;
    setColorsOffset(&colors, 1, 1, 0);
  }

  inline RGB getColor() {
    return color;
  }

  inline void setBrightness(uint8_t brightnessTarget) {
    this->brightnessTarget = brightnessTarget;
    cycleBrightness = false;
  }

  inline void setBrightnessCycle(uint8_t brightnessTarget1, uint8_t brightnessTarget2, uint8_t finalPauseCycles = 0) {
    brightnessTarget = brightnessTarget1;
    cycleBrightnessTarget = brightnessTarget2;
    cycleBrightness = true;
    this->finalPauseCycles = finalPauseCycles;
  }

  inline uint8_t getBrightness() {
    return brightness;
  }

  inline uint8_t getBrightnessTarget() {
    return brightnessTarget;
  }

  inline void setBrightnessSpeed(uint8_t brightness_speed) {
    this->brightness_speed = brightness_speed;
  }

  bool colorCycle;
  bool freezeColor;
  bool freezeBrightness;

protected:
  const RGB pins;
  const uint8_t color_steps;
  void (*const curveFunction)(int, uint8_t);
  
  const RGB *prevColor;
  const RGB *nextColor;

  RGB colors[MAX_COLORS];
  uint8_t colorsNum;
  RGB color;
  RGB tempColor;
  uint8_t brightness;
  uint8_t brightnessTarget;
  uint8_t cycleBrightnessTarget;
  bool cycleBrightness;
  uint8_t finalPauseCycles;
  uint8_t position;
  bool colorEnded;
  bool colorOneshot;
  uint8_t brightness_speed;

  void setColorsOffset(const RGB colors[], const uint8_t colorsNum, uint8_t offset, uint8_t start_color);

  inline void applyChanges() {
    curveFunction(pins.red, map(color.red, 0, 255, 0, brightness));
    curveFunction(pins.green, map(color.green, 0, 255, 0, brightness));
    curveFunction(pins.blue, map(color.blue, 0, 255, 0, brightness));
  }
};
