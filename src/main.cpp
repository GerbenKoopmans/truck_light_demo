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
enum status_types_t
{
    IDLE,
    REALTIME,
    PAST_IDLE,
    PAST_ACTIVE,
    FUTURE_IDLE,
    FUTURE_ACTIVE,
    EXERCISE_1,
    EXERCISE_2
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
}

void loop()
{
    // Read the analog value (0-1023)
    int sensorValue = analogRead(LDR_PIN);
    // Serial.println(sensorValue);
    int sliderValue = analogRead(SLIDER_PIN);
    Serial.println(sliderValue);

    // Map the analog value to a brightness range (0-255)
    int brightness = map(sensorValue, 200, 800, 80, 0);

    // Set all LEDs to white
    fill_solid(leds, NUM_LEDS, CRGB::White);

    // Adjust the brightness of the strip
    FastLED.setBrightness(brightness);

    // Show the updated LED colors and brightness
    FastLED.show();

    // Add a small delay for stability
    delay(10);
}