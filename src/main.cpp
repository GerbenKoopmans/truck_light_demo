#include <Arduino.h>
#include "FastLED.h"

#define DATA_PIN 3
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS 30
CRGB leds[NUM_LEDS];

#define BRIGHTNESS 96
#define FRAMES_PER_SECOND 120

#define LDR_PIN A0
#define SLIDER_PIN A1

uint8_t function;
enum function_types_t
{
    MORNING,
    DAY,
    EVENING,
    NIGHT
};

void setup()
{
    Serial.begin(9600);
    // put your setup code here, to run once:
    // tell FastLED about the LED strip configuration
    FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    // FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

    // set master brightness control
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear();
    FastLED.show();
    function = DAY;
}

void slider_modes(function_types_t mode)
{
    switch (mode)
    {
    case MORNING:
        fill_solid(leds, NUM_LEDS, CRGB(255, 200, 120)); // Warm orange tones to mimic sunrise
        break;

    case DAY:
        fill_solid(leds, NUM_LEDS, CRGB(255, 255, 240)); // Bright white light for alertness
        break;

    case EVENING:
        fill_solid(leds, NUM_LEDS, CRGB(255, 140, 70)); // Soft amber tones to mimic sunset
        break;

    case NIGHT:
        fill_solid(leds, NUM_LEDS, CRGB(100, 20, 0)); // Dim red tones to promote sleep
        break;
    }
}
void loop()
{
    // Read the analog value (0-1023)
    int sensorValue = analogRead(LDR_PIN);
    // Serial.println(sensorValue);
    int sliderValue = analogRead(SLIDER_PIN);
    Serial.println(sliderValue);

    // Map slider value to circadian modes
    if (sliderValue < 256)
        function = MORNING; // Early morning
    else if (sliderValue < 512)
        function = DAY; // Daylight
    else if (sliderValue < 768)
        function = EVENING; // Evening
    else
        function = NIGHT; // Night

    // Apply circadian lighting based on the mode
    slider_modes(function);

    // Serial.println(sliderValue);

    // Map the analog value to a brightness range (0-255)
    int brightness = map(sensorValue, 200, 800, 80, 0);

    // Set all LEDs to white
    // fill_solid(leds, NUM_LEDS, CRGB::White);

    // Adjust the brightness of the strip
    FastLED.setBrightness(brightness);

    // Show the updated LED colors and brightness
    FastLED.show();

    // Add a small delay for stability
    delay(10);
}