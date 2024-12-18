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

#define BUTTON_PIN1 4
#define BUTTON_PIN2 2

// Debounce timer
unsigned long lastDebounceTime = 0; // the last time the output pin was toggled
int debounceDelay = 50;             // the debounce time; increase if the output flickers
int lastState = LOW;
int buttonState = LOW;

uint8_t function;
enum function_types_t
{
    WAKE_UP,
    DRIVING,
    BREAK
};

boolean readButton(int button)
{
    int currentState = digitalRead(button);

    // If the switch/button changed, due to noise or pressing:
    if (currentState != lastState)
    {
        // reset the debouncing timer
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay)
    {
        // whatever the reading is at, it's been there for longer than the debounce
        // delay, so take it as the actual current state:

        // if the button state has changed:
        if (currentState != buttonState)
        {
            buttonState = currentState;
            if (buttonState == LOW)
            {
                lastState = currentState;
                Serial.println("Button pressed");
                return true;
            }
        }
    }
    lastState = currentState;
    return false;
}

void readLDR()
{
    // Read the analog value (0-1023)
    int sensorValue = analogRead(LDR_PIN);
    // Serial.println(sensorValue);
    int sliderValue = analogRead(SLIDER_PIN);
    // Serial.println(sliderValue);
    Serial.println(readButton(BUTTON_PIN2));

    // Map the analog value to a brightness range (0-255)
    int brightness = map(sensorValue, 200, 800, 80, 0);

    // Set all LEDs to white
    fill_solid(leds, NUM_LEDS, CRGB::White);

    // Adjust the brightness of the strip
    FastLED.setBrightness(brightness);

    // Show the updated LED colors and brightness
    FastLED.show();
}

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
    switch (function)
    {
    case (WAKE_UP):
        break;
    case (DRIVING):
        if (readButton(BUTTON_PIN1))
        {
            function = BREAK;
        }
        break;
    case (BREAK):
        break;
    }
}