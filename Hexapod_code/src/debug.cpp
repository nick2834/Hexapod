#include <Arduino.h>
#include "header.h"

// ================================================================
// ===                          debug                           ===
// ================================================================
#ifdef DEBUG_SERIAL

void Debug_Serial_out()
{
    Serial.print("loopTime: ");
    Serial.print(loopTime);

    if (loopTime < 10)
    {
        Serial.print("   ");
    }
    else if (loopTime < 100)
    {
        Serial.print("  ");
    }

    Serial.print(" || ");

    Serial.println();
}

void Debug_WaitForSerial()
{
    // wait for USB connection
    while (!Serial)
    {
        delay(100);
    }

    Serial.println("press any button to start");

    // wait for serial input
    while (!Serial.available())
    {
        delay(100);
    }
    // clear serial data

    Serial.println();
    Serial.println("================================================================================");
    Serial.println();
}

void Debug_delay()
{
    for (int i = 0; i < 10; i++)
    {
        Serial.print(".");
        delay(500);
    }
}
#endif

#ifdef DEBUG_ARDUINO

void Debug_TransferData(const float &value)
{
    uint32_t floatBits; // Variable to store the bits of the float

    memcpy(&floatBits, &value, sizeof(float));

    digitalWrite(DEBUG_START, HIGH);

    for (int i = 0; i < 32; i++)
    {
        // Extract the i-th bit from the floatBits variable
        bool bitValue = (floatBits >> i) & 1;

        // Send the bit over the data line
        digitalWrite(DEBUG_DATA, bitValue);

        // make receiver read bit
        digitalWrite(DEBUG_CLOCK, LOW);
        delay(1);
        digitalWrite(DEBUG_CLOCK, HIGH);
        delay(1);
        digitalWrite(DEBUG_CLOCK, LOW);
    }

    digitalWrite(DEBUG_START, LOW);

    Debug_LineReturn();
}

void Debug_LineReturn()
{
    delay(1);
    digitalWrite(DEBUG_LINE_RETURN, LOW);
    delay(1);
    digitalWrite(DEBUG_LINE_RETURN, HIGH);
    delay(1);
    digitalWrite(DEBUG_LINE_RETURN, LOW);
}

#endif

#ifdef DEBUG_LED

// using the middle eye ring to represent a 8bit error code
void Debug_Led_8bit(const int8_t &error_code)
{
    for (int bitPosition = 0; bitPosition < 8; ++bitPosition)
    {
        // Use bitwise AND to check the value of the bit at the given position
        if ((error_code & (1 << bitPosition)))
        {
            // Bit is 1
            Led_update(bitPosition + 1, CRGB::Blue);
        }
        else
        {
            // Bit is 0
            Led_update(bitPosition + 1, CRGB::Black);
        }
    }

    FastLED.show();
}

// using the outer eye ring to represent a 16bit error code (good for numbers that might be really large)
void Debug_Led_16bit(int16_t error_code)
{
    if (error_code < 0)
    {
        Led_update(0, CRGB::Blue);

        error_code *= -1;
    }
    else
    {
        Led_update(0, CRGB::Black);
    }

    for (int bitPosition = 0; bitPosition < 16; ++bitPosition)
    {
        // Use bitwise AND to check the value of the bit at the given position
        if ((error_code & (1 << bitPosition)))
        {
            // Bit is 1
            Led_update(bitPosition + 9, CRGB::Blue);
        }
        else
        {
            // Bit is 0
            Led_update(bitPosition + 9, CRGB::Black);
        }
    }

    FastLED.show();
}
#endif