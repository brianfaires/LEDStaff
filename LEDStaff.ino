#include <FastLED.h>
#include <SPI.h>
#include <SparkFunLIS3DH.h>
#include <Wire.h>
#include <SdFat.h>
#include "_Definitions.h"

// -------------
// ---Globals---
// -------------

// ---Library classes
CRGBArray<NUM_LEDS> leds;
#ifdef USING_SD
SdFat SD;
#endif
#ifdef USING_ACCEL
LIS3DH myIMU; //Default constructor is I2C, addr 0x19.
float accelX, accelY, accelZ;
uint16_t curAccel = 0;
#endif


// ---Reusable objects---
bool objectsAreValues = false;
void *objects[NUM_OBJECT_POINTERS];


// ---Mode
uint8_t curMode = INIT_MODE;
bool enableModeSwitching = INIT_MODE_SWITCHING;
uint8_t modeSwitchTime = INIT_MODE_SWITCH_TIME;
uint32_t lastModeSwitchTime = 0;



// --- Looping
uint16_t loopCount = 0;
uint8_t delayRate = 0;


// ---Graphics
#define maxBrightness INIT_MAX_BRIGHTNESS
uint8_t fadeRate = 0;
bool fadeOnly = false;



void setup() {
#ifdef DEBUG
  InitDiagnostics();
#ifdef USING_SERIAL
  Serial.println("InitDiagnostics() complete.");
#endif
  return;
#endif

  InitIO();
#ifdef USING_SD
  InitSD();
#endif

#ifdef USING_ACCEL
  myIMU.begin();
#endif

  FastLED.show();
  delay(500);

#ifdef USING_SERIAL
  InitSerial();
#endif
  Serial.begin(57600);
  ClearGlobalObjects();

  InitMotifValues();
  FastLED.setBrightness( maxBrightness );
  //TO DO: FastLED.setColorCorrection()
  PowerUpAnimation();

#ifdef USING_SERIAL
  Serial.println("Init complete");
#endif
}

void loop() {
#ifdef DEBUG
  LoopDiagnostics();
  return;
#endif

  // Poll IO
#ifdef USING_ACCEL
  if (loopCount % UPDATE_RATE_ACCEL == 0) UpdateAccel();
#endif


  // Check for mode switch
  if (enableModeSwitching && loopCount % UPDATE_RATE_MODE_SWITCHING == 0) {
    uint32_t curTime = millis();
    if (curTime >= lastModeSwitchTime + modeSwitchTime) {
      lastModeSwitchTime = curTime;
      NextMode();
    }
  }
  else { // Same mode as last cycle
    // Global Fade
    if (fadeRate) {
      if (fadeRate == 255)
        leds(0, NUM_LEDS - 1) = CRGB::Black;
      else
        for (CRGB & pixel : leds) {
          pixel.fadeToBlackBy(fadeRate);
        }
    }

    // Motif changes
    if (enableMotifSwitching && loopCount % UPDATE_RATE_MOTIF_SWITCHING == 0) {
      uint32_t curTime = millis();
      if (curTime >= lastMotifSwitchTime + motifSwitchTime) {
        lastMotifSwitchTime = curTime;
        NextMotif();
      }
    }
    else if (enableMotifWalking && loopCount % UPDATE_RATE_MOTIF_WALK == 0) {
      WalkMotif();
    }
  }

  // Draw current mode
  if (loopCount == 0) InitNewMode();
  DrawCurMode();
  FastLED.show();

  // Update Loop Variables
  ++loopCount;
  delay(delayRate);
}

