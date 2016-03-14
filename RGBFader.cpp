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

#include "RGBFader.h"

const RGB RGBFader::rainbow[] = { {255, 0,   0   },
                                  {255, 127, 0   },
                                  {255, 255, 0   },
                                  {0,   255, 0   },
                                  {0,   0,   255 },
                                  {75,  0,   130 },
                                  {139, 0,   255 } };

const uint8_t RGBFader::rainbowSize = 7;

const RGB RGBFader::rainbowAndWhite[] = { {255, 0,   0   },
                                          {255, 127, 0   },
                                          {255, 255, 0   },
                                          {0,   255, 0   },
                                          {0,   0,   255 },
                                          {75,  0,   130 },
                                          {139, 0,   255 },
                                          {255, 214, 170 } };

const uint8_t RGBFader::rainbowAndWhiteSize = 8;

const RGB RGBFader::redGreenBlue[] = { {255, 0,   0   },
                                       {0,   255, 0   },
                                       {0,   0,   255 } };

const uint8_t RGBFader::redGreenBlueSize = 3;

const uint8_t RGBFader::exponentialLookup[] PROGMEM =
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02,
  0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
  0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x05,
  0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x07, 0x07, 0x07,
  0x07, 0x07, 0x07, 0x08, 0x08, 0x08, 0x08, 0x08, 0x09, 0x09, 0x09, 0x09, 0x09, 0x0A, 0x0A, 0x0A,
  0x0A, 0x0B, 0x0B, 0x0B, 0x0B, 0x0C, 0x0C, 0x0C, 0x0D, 0x0D, 0x0D, 0x0E, 0x0E, 0x0E, 0x0E, 0x0F,
  0x0F, 0x10, 0x10, 0x10, 0x11, 0x11, 0x11, 0x12, 0x12, 0x13, 0x13, 0x14, 0x14, 0x14, 0x15, 0x15,
  0x16, 0x16, 0x17, 0x17, 0x18, 0x19, 0x19, 0x1A, 0x1A, 0x1B, 0x1B, 0x1C, 0x1D, 0x1D, 0x1E, 0x1F,
  0x1F, 0x20, 0x21, 0x22, 0x22, 0x23, 0x24, 0x25, 0x26, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C,
  0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x36, 0x37, 0x38, 0x39, 0x3B, 0x3C, 0x3D, 0x3F,
  0x40, 0x41, 0x43, 0x44, 0x46, 0x48, 0x49, 0x4B, 0x4C, 0x4E, 0x50, 0x52, 0x53, 0x55, 0x57, 0x59,
  0x5B, 0x5D, 0x5F, 0x61, 0x63, 0x66, 0x68, 0x6A, 0x6D, 0x6F, 0x72, 0x74, 0x77, 0x79, 0x7C, 0x7F,
  0x81, 0x84, 0x87, 0x8A, 0x8D, 0x90, 0x94, 0x97, 0x9A, 0x9E, 0xA1, 0xA5, 0xA8, 0xAC, 0xB0, 0xB4,
  0xB8, 0xBC, 0xC0, 0xC4, 0xC9, 0xCD, 0xD1, 0xD6, 0xDB, 0xE0, 0xE5, 0xEA, 0xEF, 0xF4, 0xF9, 0xFF };

RGBFader::RGBFader(const RGB& rgb_pins, const RGB colors[], const uint8_t colorsNum, const uint8_t color_steps, const uint8_t start_color,
                   const uint8_t brightness_speed, const uint8_t initialBrightness, const bool colorCycle, curveType curve) :
  colorCycle(colorCycle),
  freezeColor(false),
  freezeBrightness(false),
  pins(rgb_pins),
  color_steps(color_steps),
  curveFunction(curve == LINEAR ? linear : (curve == QUADRATIC ? quadratic : exponential)),
  brightness(initialBrightness),
  brightnessTarget(initialBrightness),
  cycleBrightnessTarget(initialBrightness),
  cycleBrightness(false),
  finalPauseCycles(0),
  colorOneshot(false),
  brightness_speed(brightness_speed)
{
  setColorsOffset(colors, colorsNum, 0, start_color);
  nextStep();
}

void RGBFader::setColorsOffset(const RGB colors[], const uint8_t colorsNum, uint8_t offset, uint8_t start_color) {

  for (uint8_t i = 0; i < colorsNum; i++)
    this->colors[i + offset] = colors[i];
  
  this->colorsNum = colorsNum + offset;
  prevColor = this->colors + start_color;
  nextColor = (prevColor - this->colors + 1) % colorsNum + this->colors;
  position = 0;
  color = colors[0];
  colorEnded = false;

}

void RGBFader::goOnColorIndex(uint8_t index) {
  tempColor = color;
  prevColor = &tempColor;
  nextColor = colors + index;
  colorEnded = false;
  position = 0;
  colorOneshot = true;
}

void RGBFader::nextStep() {
  bool changed = false;
  
  if (((!colorEnded || colorCycle) && !freezeColor) || colorOneshot) {   
    color.red = map(position, 0, color_steps, prevColor->red, nextColor->red);
    color.green = map(position, 0, color_steps, prevColor->green, nextColor->green);
    color.blue = map(position, 0, color_steps, prevColor->blue, nextColor->blue);
  
    if (position == color_steps - 1 && nextColor == colors + colorsNum - 1)
      colorEnded = true;
    else
      colorEnded = false;

    position = (position + 1) % color_steps;
    if (position == 0) {
      prevColor = nextColor;
      nextColor = (nextColor - colors + 1) % colorsNum + colors;
      if (colorOneshot) {
        colorOneshot = false;
        colorEnded = true;
      }
    }

    changed = true;
  }

  if (!freezeBrightness) {
    if (brightness != brightnessTarget) {
      int8_t step;
      
      if (brightness > brightnessTarget)
        step = -min(brightness - brightnessTarget, brightness_speed);
      else
        step = min(brightnessTarget - brightness, brightness_speed);
  
      brightness += step;
  
      changed = true;
    }
  
    if (cycleBrightness && brightness == brightnessTarget) {
      static uint8_t i = 0;

      if (i >= finalPauseCycles) {
        uint8_t temp = brightnessTarget;
        brightnessTarget = cycleBrightnessTarget;
        cycleBrightnessTarget = temp;
        i = 0;
      }
      else
        ++i;
    }
  }

  if (changed)
    applyChanges();
}

