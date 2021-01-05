
#include <ALS_matrixRGB.h>

#include <FastLED.h>
FASTLED_USING_NAMESPACE

#define DATA_PIN    D4
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    224

CRGB 			leds[NUM_LEDS];
ALS_matrixRGB	* _myMatrix = nullptr;

void setup()
{
	FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);

	_myMatrix = new ALS_matrixRGB(16, 14, NUM_LEDS, 16);	

	_myMatrix->leds = leds;

    FastLED.clear();
    FastLED.show();	
}

void loop()
{

    for (int16_t i = 0; i < 16; i++) {
        FastLED.clear();
        _myMatrix->drawPixelXY(i, 0, CRGB(CRGB::Red));
        _myMatrix->drawPixelXY(0, i, CRGB(CRGB::Green));
        FastLED.show();
        delay(250);
    }

    FastLED.clear();
    FastLED.show();	
}
