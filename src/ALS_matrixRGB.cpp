#include "ALS_matrixRGB.h"
ALS_matrixRGB * ALS_matrixRGB_ptr;
ALS_matrixRGB * ALS_matrixRGBPtr_get(){
    return ALS_matrixRGB_ptr;
}
ALS_matrixRGB::ALS_matrixRGB(uint16_t mW, uint16_t mH, uint16_t nL, uint16_t tw){
    MATRIX_WIDTH    = mW;
    MATRIX_HEIGHT   = mH;
    THIS_WIDTH      = tw;
    NUM_LEDS        = nL;
    ALS_matrixRGB_ptr = this;
}
// ###############################################################################
// FROM : https://github.com/Intrinsically-Sublime/esp8266-fastled-webserver
int ALS_matrixRGB::_XY(int x, int y) {
    if(y > MATRIX_HEIGHT) { y = MATRIX_HEIGHT; }
    if(y < 0) { y = 0; }
    if(x > MATRIX_WIDTH) { x = MATRIX_WIDTH;}
    if(x < 0) { x = 0; }
    if(x % 2 == 1) {  
        return (x * (MATRIX_WIDTH) + (MATRIX_HEIGHT - y -1));
    } else {
    // use that line only, if you have all rows beginning at the same side
        return (x * (MATRIX_WIDTH) + y);  
    }
}
uint16_t ALS_matrixRGB::XY( uint8_t x, uint8_t y) // maps the matrix to the strip
{
    uint16_t i;
    i = (y * MATRIX_WIDTH) + (MATRIX_WIDTH - x);
    i = (NUM_LEDS - 1) - i;
    if (i > NUM_LEDS) i = NUM_LEDS;
    return i;
}
uint16_t ALS_matrixRGB::XY_R( uint8_t x, uint8_t y) // maps the matrix to the strip
{
  uint16_t i;
  i = (y * MATRIX_WIDTH) + (MATRIX_WIDTH - x);
  if (i > NUM_LEDS) i = NUM_LEDS;
  return i;
}

int ALS_matrixRGB::wrapX(int x) {   // Used by XY function and tempMatrix
    #ifdef CYLINDRICAL_MATRIX
        if (x >= MATRIX_WIDTH) {
            return x - MATRIX_WIDTH;
        } else if (x < 0) {
            return x + MATRIX_WIDTH;
        } else {
            return x;
        }
    #else
        if (x < 0 ) return 0;
        if (x >= MATRIX_WIDTH) return MATRIX_WIDTH-1;
        return x;
    #endif
}

int ALS_matrixRGB::_XY_(int x, int y) { // x = Width, y = Height
    int XorY; 


    // map anything outside of the matrix to the extra hidden pixel
    if (y >= MATRIX_HEIGHT || x >= MATRIX_WIDTH || x < 0 || y < 0) { return NUM_LEDS; }

    #ifdef MIRROR_WIDTH
        x = (MATRIX_WIDTH - 1) - x;
    #endif

    #ifndef MIRROR_HEIGHT   // Default is to mirror the height to be compatible with irregular matrix array that is read from the top left corner
        y = (MATRIX_HEIGHT - 1) - y;
    #endif

    if (matrix_horizontal) {
        uint8_t xx = x;
        x = y;
        y = xx;
        XorY = MATRIX_WIDTH;
    }
    else {
        XorY = MATRIX_HEIGHT;
    }

    uint16_t pixNum;

    #ifdef SERPENTINE
        if(x%2 == 0) {
            pixNum = (x * XorY) + y;
        } else {
            pixNum = (x * XorY) + ((XorY - 1) - y);
        }
    #elif defined IRREGULAR_MATRIX
        if (NUM_LEDS < 256) {
            pixNum = pgm_read_byte(XYTable + ((y * MATRIX_WIDTH) + x));
        else
            pixNum = pgm_read_word(XYTable + ((y * MATRIX_WIDTH) + x));
        }
    #else
        pixNum = (x * XorY) + y;
    #endif

    return pixNum;
}
int ALS_matrixRGB::getXyFromLedNum(int ledNum) {
  return _XY_(ledNum/MATRIX_HEIGHT,ledNum%MATRIX_HEIGHT);
}
// ###############################################################################
// FROM : Torch: https://github.com/evilgeniuslabs/torch
void ALS_matrixRGB::drawCircle(int16_t x0, int16_t y0, uint16_t radius, const CRGB& color)
{
    int a = radius, b = 0;
    int radiusError = 1 - a;

    if (radius == 0) {
        leds[_XY_(x0, y0)] = color;
        return;
    }

    while (a >= b) {
        leds[_XY_(a + x0, b + y0)] = color;
        leds[_XY_(b + x0, a + y0)] = color;
        leds[_XY_(-a + x0, b + y0)] = color;
        leds[_XY_(-b + x0, a + y0)] = color;
        leds[_XY_(-a + x0, -b + y0)] = color;
        leds[_XY_(-b + x0, -a + y0)] = color;
        leds[_XY_(a + x0, -b + y0)] = color;
        leds[_XY_(b + x0, -a + y0)] = color;

        b++;
        if (radiusError < 0)
            radiusError += 2 * b + 1;
        else {
            a--;
            radiusError += 2 * (b - a + 1);
        }
    }
}
void ALS_matrixRGB::drawCircle(int16_t x0, int16_t y0, uint16_t radius, const CHSV& color)
{
    int a = radius, b = 0;
    int radiusError = 1 - a;

    if (radius == 0) {
        leds[_XY_(x0, y0)] = color;
        return;
    }

    while (a >= b) {
        leds[_XY_(a + x0, b + y0)] = color;
        leds[_XY_(b + x0, a + y0)] = color;
        leds[_XY_(-a + x0, b + y0)] = color;
        leds[_XY_(-b + x0, a + y0)] = color;
        leds[_XY_(-a + x0, -b + y0)] = color;
        leds[_XY_(-b + x0, -a + y0)] = color;
        leds[_XY_(a + x0, -b + y0)] = color;
        leds[_XY_(b + x0, -a + y0)] = color;

        b++;
        if (radiusError < 0)
            radiusError += 2 * b + 1;
        else {
            a--;
            radiusError += 2 * (b - a + 1);
        }
    }
}
void ALS_matrixRGB::drawFastVLine(uint16_t x, uint16_t y0, uint16_t y1, const CRGB& color) {
  uint16_t i;

   for (i = y0; i < (y1 + y0); i++) {
      leds[_XY_(x, i)] = color;
   }
}
void ALS_matrixRGB::drawFastHLine(uint16_t x, uint16_t y0, uint16_t y1, const CRGB& color) {
  uint16_t i;

   for (i = y0; i < (y1 + y0); i++) {
      leds[_XY_(i, x)] = color;
   }
}




// Change the parameter's value
// Input parameters:
//    value - current value: unsigned integer inside [0:255]
//    delta - signed delta to change (negative value for decreasing, positive for increasing)
//    limit - upper limit (255 if not set)
//    saturation - boundary cases behavior:
//      true:  value will remain in set boundaries
//      false: value will cross around boundaries from minimum (maximum) to maximum (minimum)
// Output value:
//    returned value will be inside [0:limit]
uint16_t ALS_matrixRGB::ChangeParameterValue(uint16_t value, int8_t delta, uint16_t limit /*= 255*/, boolean saturation /*= true*/) {
  int16_t new_value = (int16_t)(value + delta);

  // Low limit is always 0
  if (new_value < 0) {
    if (saturation)
      new_value = 0;
    else
      new_value = (int16_t)limit;
  } 

  // Upper limit may be less than 255, see input parameter
  if (new_value > (int16_t)limit) {
    if (saturation)
      new_value = limit;
    else
      new_value = 0;
  }

  return new_value;
}

// Get the pixel number in the tape by coordinates
uint16_t ALS_matrixRGB::getPixelNumber(uint8_t x, uint8_t y) {
  if ((THIS_Y % 2 == 0) || MATRIX_TYPE)            // even line
    return (THIS_WIDTH * THIS_Y + THIS_X);
  else                                             // odd line
    return (THIS_WIDTH * THIS_Y + THIS_WIDTH - THIS_X - 1);
}

// Set color of the pixel 'in matrix' by X,Y coordinates
void ALS_matrixRGB::drawPixelXY(uint8_t x, uint8_t y, CRGB color) {
#ifdef USE_PARAMS_CHECK
  if (x <= MATRIX_WIDTH - 1 || y <= MATRIX_HEIGHT - 1)
    leds[getPixelNumber(x, y)] = color;
#else
    leds[getPixelNumber(x, y)] = color;
#endif
}

// Get color of the pixel 'in strip' by X,Y coordinates
uint32_t ALS_matrixRGB::getPixelColorXY(uint8_t x, uint8_t y) {
  register uint16_t index = getPixelNumber(x, y);
#ifdef USE_PARAMS_CHECK
  if (x <= MATRIX_WIDTH - 1 || y <= MATRIX_HEIGHT - 1)
    return (uint32_t)leds[getPixelNumber(x, y)];
  return 0;
#else
  // return (uint32_t)leds[getPixelNumber(x, y)];
  return ( ((uint32_t)leds[index].r << 16) | ((uint16_t)leds[index].g << 8 ) | (uint16_t)leds[index].b );
#endif
}

// Get color of the pixel 'in strip' by index
uint32_t ALS_matrixRGB::getPixelColor(uint16_t index) {
#ifdef USE_PARAMS_CHECK
  if (index <= NUM_LEDS - 1)
    return (uint32_t)leds[index];
  return 0;
#else
  // return (uint32_t)leds[index];
  return ( ((uint32_t)leds[index].r << 16) | ((uint16_t)leds[index].g << 8 ) | (uint16_t)leds[index].b );
#endif
}

// Attenuation the pixel: check the current pixel's brightness and reduce it
void ALS_matrixRGB::fadePixel(uint8_t x, uint8_t y, uint8_t step) {
  uint16_t pixelNum = getPixelNumber(x, y);
  if (getPixelColor(pixelNum) == 0) 
    return;

  if (leds[pixelNum].r >= 16 ||
      leds[pixelNum].g >= 16 ||
      leds[pixelNum].b >= 16 ) 
    leds[pixelNum].fadeToBlackBy(step);
  else 
    leds[pixelNum] = 0;
}

// Attenuation all pixels in the matrix
void ALS_matrixRGB::fadeMatrix(uint8_t step) {
  for (uint8_t i = 0; i < MATRIX_WIDTH; i++)
    for (uint8_t j = 0; j < MATRIX_HEIGHT; j++)
      fadePixel(i, j, step);
}

// Shift matrix 'down' (-1 by Y)
void ALS_matrixRGB::shiftMatrixDown() {
  // for (uint8_t x = 0; x <= WIDTH - 1; x++)
  //   for (uint8_t y = 0; y <= HEIGHT - 2; y++)
  //     drawPixelXY(x, y, getPixelColorXY(x, y + 1));

  for (uint8_t x = 0; x <= MATRIX_WIDTH - 1; x++)
    for (uint8_t y = 0; y <= MATRIX_HEIGHT - 2; y++)
      leds[getPixelNumber(x, y)]=leds[getPixelNumber(x, y+1)];      
}
