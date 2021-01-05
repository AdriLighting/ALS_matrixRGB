#ifndef ALS_MATRIXRGB_H
	#define ALS_MATRIXRGB_H 
	#include <Arduino.h>
	#include <FastLED.h>
	#define MATRIX_TYPE      1              // Matrix type (by control): 0 - zigzag, 1 - parallel
	#define THIS_X x
	#define THIS_Y y	
	class ALS_matrixRGB
	{
		uint16_t MATRIX_HEIGHT;
		uint16_t MATRIX_WIDTH;
		uint16_t NUM_LEDS;
		uint16_t THIS_WIDTH;
		boolean matrix_horizontal = true;
		CRGB * leds;
	public:
		ALS_matrixRGB(uint16_t mW, uint16_t mH, uint16_t nL, uint16_t tw);
		~ALS_matrixRGB(){};
		int _XY(int x, int y);
		uint16_t XY(uint8_t x, uint8_t y);
		uint16_t XY_R(uint8_t x, uint8_t y);
		int wrapX(int x);
		int _XY_(int x, int y);
		int getXyFromLedNum(int ledNum);
void drawCircle(int16_t x0, int16_t y0, uint16_t radius, const CRGB& color);
void drawCircle(int16_t x0, int16_t y0, uint16_t radius, const CHSV& color);
void drawFastVLine(uint16_t x, uint16_t y0, uint16_t y1, const CRGB& color);
void drawFastHLine(uint16_t x, uint16_t y0, uint16_t y1, const CRGB& color);

uint16_t ChangeParameterValue(uint16_t value, int8_t delta, uint16_t limit /*= 255*/, boolean saturation /*= true*/);
uint16_t getPixelNumber(uint8_t x, uint8_t y);
void drawPixelXY(uint8_t x, uint8_t y, CRGB color);
uint32_t getPixelColorXY(uint8_t x, uint8_t y);
uint32_t getPixelColor(uint16_t index);
void fadePixel(uint8_t x, uint8_t y, uint8_t step);
void fadeMatrix(uint8_t step);
void shiftMatrixDown();
	};
	ALS_matrixRGB * ALS_matrixRGBPtr_get();
#endif