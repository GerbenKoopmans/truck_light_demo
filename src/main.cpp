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
unsigned long debounceDelay = 50;   // the debounce time; increase if the output flickers
int lastState = LOW;
int buttonState = LOW;

unsigned long wakeUpTimer = 0;
unsigned long wakeUpDelay = 100;
int wakeUpBrightness = 10;

enum function_types_t
{
    WAKE_UP,
    DRIVING,
    BREAK,
    EVENING,
    SLEEP
};
uint8_t function = WAKE_UP;

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

int readLDR()
{
    // Read the analog value (0-1023)
    int sensorValue = analogRead(LDR_PIN);
    // Serial.print("LDR value: ");
    // Serial.println(sensorValue);

    // Map the analog value to a brightness range (0-255)
    int brightness = map(sensorValue, 200, 800, 80, 0);
    return brightness;
}

int readSlider()
{
    int sliderValue = analogRead(SLIDER_PIN);
    // Serial.print("Slider value: ");
    // Serial.println(sliderValue);

    // Map slider value to circadian modes
    if (sliderValue < 200)
        return WAKE_UP;
    else if (sliderValue < 750)
        return DRIVING;
    else if (sliderValue < 950)
        return EVENING;

    return SLEEP;
}

void switchFunction()
{
    // If slider value changed
    if (function != BREAK)
        function = readSlider();

    switch (function)
    {
    case (WAKE_UP):
        // Set all LEDs to white
        fill_solid(leds, NUM_LEDS, CRGB::White);

        if (wakeUpDelay < millis() - wakeUpTimer)
        {
            // Reset timer
            wakeUpTimer = millis();

            if (wakeUpBrightness < 250)
            {
                wakeUpBrightness += 1;
                // Adjust the brightness of the strip
                FastLED.setBrightness(wakeUpBrightness);
            }
        }

        break;

    case (DRIVING):
        // Set all LEDs to white
        fill_solid(leds, NUM_LEDS, CRGB::Yellow);
        FastLED.setBrightness(readLDR());

        if (readButton(BUTTON_PIN1))
        {
            function = BREAK;
            Serial.println("Break started");
        }
        else if (readButton(BUTTON_PIN2))
        {
        }
        break;

    case (BREAK):
        fill_solid(leds, NUM_LEDS, CRGB::Orange);
        if (readButton(BUTTON_PIN1))
        {
            function = DRIVING;
            Serial.println("Break ended");
        }
        break;

    case (EVENING):
        fill_solid(leds, NUM_LEDS, CRGB::Red); // Dim red tones to promote sleep
        break;

    case (SLEEP):
        fill_solid(leds, NUM_LEDS, CRGB::Black); // Turn LEDs off
        break;
    }
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
    function = WAKE_UP;
}

void loop()
{
    switchFunction();
    // Show the updated LED colors and brightness
    FastLED.show();
    // Add a small delay for stability
    delay(10);
}