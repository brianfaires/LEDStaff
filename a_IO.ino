bool isHolding[NUM_BTNS];
uint32_t depressionTime[NUM_BTNS];
uint32_t lastPressTime[NUM_BTNS];

#ifdef USING_SD
File rootSD;
File curFile;
#endif

void InitIO() {
  FastLED.addLeds<NEOPIXEL, PIN_DATA>(leds, NUM_LEDS);
  //pinMode(PIN_BTN0, INPUT_PULLUP);
  //pinMode(PIN_BTN1, INPUT_PULLUP);
  //pinMode(PIN_BTN2, INPUT_PULLUP);
}

// Button 0: Mode.  Press once to change mode,  double press to toggle switching, hold to adjust mode switch time
// Button 1: Motif. Press once to change motif, double press to toggle switching, hold to adjust motif switch time
// Button 2: Other. Press once to change scheme, double press to toggle color walking, hold to adjust walk rate, double-press hold to adjust walk style
bool UpdateIO() {
  bool retVal = true;
  for(uint8_t i = 0; i < NUM_BTNS; i++) {
    //if(digitalRead(i == 0 ? PIN_BTN0 : i == 1 ? PIN_BTN1 : PIN_BTN2) == LOW) {
    if(analogRead(i == 0 ? PIN_BTN0 : i == 1 ? PIN_BTN1 : PIN_BTN2) > ANALOG_BTN_THRESHOLD) {
      if(isHolding[i]) {
        if(depressionTime[i] - lastPressTime[i] > TIME_DOUBLE_CLICK) {
          LongPress(i, 1);
        }
        else {
          LongPress(i, 2);
        }
      }
      else if(depressionTime[i] == 0) {
        depressionTime[i] = millis();
        delay(TIME_DEBOUNCE);
      }
      else if(millis() - depressionTime[i] > TIME_LONG_PRESS_INIT) {
        isHolding[i] = true;
      }
    }
    else if(lastPressTime[i] && (millis() - lastPressTime[i] > TIME_DOUBLE_CLICK)) {
      SingleClick(i);
      lastPressTime[i] = 0;
    }
    else if(depressionTime[i] != 0) {
      if(isHolding[i]) {
        isHolding[i] = false;
        depressionTime[i] = 0;
        lastPressTime[i] = 0;
      }
      else if(depressionTime[i] - lastPressTime[i] > TIME_DOUBLE_CLICK) {
        lastPressTime[i] = millis();
      }
      else {
        DoubleClick(i);
        lastPressTime[i] = 0;
      }
  
      depressionTime[i] = 0;
      delay(TIME_DEBOUNCE);
    }

    retVal &= isHolding[i];
  }

  return retVal;
}

void SingleClick(uint8_t btnNum) {
  if(btnNum == 0)
    NextMode();
  else if(btnNum == 1)
    NextMotif();
  else
    NextScheme();
}

void DoubleClick(uint8_t btnNum) {
  if(btnNum == 0)
    enableModeSwitching = !enableModeSwitching;
  else if(btnNum == 1)
    enableMotifSwitching = !enableMotifSwitching;
  else
    enableMotifWalking = !enableMotifWalking;
}

void LongPress(uint8_t btnNum, uint8_t action) {
  return;
  if(btnNum == 2) {
    if(action == 1) {
      // Standard press and hold
      if(++motifWalkRate > MAX_MOTIF_WALK_RATE)
        motifWalkRate = 0;
    }
    else {
      // Double-click and hold
      motifWalkStyle = (motifWalkStyle+1) % NUM_MOTIF_WALK_STYLES;
    }
  }
  else { // Combined for buttons 1 and 2 since the timers have the same possible discrete values
    uint8_t* targetValue = btnNum == 0 ? &modeSwitchTime : &motifSwitchTime;
    if(action == 1) {
      // Standard press and hold
      *targetValue =  *targetValue == 2 ? 5 :
                      *targetValue == 5 ? 10 :
                      *targetValue == 10 ? 20 :
                      *targetValue == 20 ? 30 :
                      *targetValue == 30 ? 45 :
                      *targetValue == 45 ? 60 :
                      *targetValue == 60 ? 90 :
                      *targetValue == 90 ? 120 :
                      *targetValue == 120 ? 180 : 2;
    }
    else {
      // Double-click and hold
      *targetValue =  *targetValue == 5 ? 2 :
                      *targetValue == 10 ? 5 :
                      *targetValue == 20 ? 10 :
                      *targetValue == 30 ? 20 :
                      *targetValue == 45 ? 30 :
                      *targetValue == 60 ? 45 :
                      *targetValue == 90 ? 60 :
                      *targetValue == 120 ? 90 :
                      *targetValue == 180 ? 120 : 180;      
    }
  }
}

void DrawOptionsMode() {
  return;
  leds(0, NUM_LEDS-1) = CRGB::Black;

  // Mode switch time: 2s, 5s, 10s, 20s, 30s, 45s, 60s, 90s, 120s, 180s
  uint16_t numToDraw = modeSwitchTime;
  CRGB col = CRGB::Red;

  if(numToDraw == 90) col = CRGB::Violet;
  else if(numToDraw == 120) col = CRGB::Blue;
  else if(numToDraw == 180) col = CRGB::Green;
  
  if(numToDraw > 60) numToDraw = 60;
  leds(0, numToDraw-1) = col;  


  // Motif switch time: 2s, 5s, 10s, 20s, 30s, 45s, 60s, 90s, 120s, 180s
  numToDraw = motifSwitchTime;
  col = CRGB::Red;

  if(numToDraw == 90) col = CRGB::Violet;
  else if(numToDraw == 120) col = CRGB::Blue;
  else if(numToDraw == 180) col = CRGB::Green;
  
  if(numToDraw > 60) numToDraw = 60;
  leds(NUM_LEDS/2 - 1 - numToDraw, NUM_LEDS/2 - 1) = col;


  // Motif walk rate: Any int from 1-20
  numToDraw = motifWalkRate;
  col = motifWalkStyle == mwsNone ? CRGB::Black : motifWalkStyle == mwsRandom ? CRGB::White : CRGB::Yellow;
  if(numToDraw > 0) leds((NUM_LEDS - numToDraw)/2, (NUM_LEDS + numToDraw)/2 - 1) = col;

  FastLED.show();
  delay(TIME_LONG_PRESS_INC);
}

#ifdef USING_ACCEL
void UpdateAccel() {
  accelX = myIMU.readFloatAccelX();
  accelY = myIMU.readFloatAccelY();
  accelZ = myIMU.readFloatAccelZ();

  #ifdef USING_SERIAL
    Serial.print("X = ");
    Serial.println(accelX);
    Serial.print("Y = ");
    Serial.println(accelY);
    Serial.print("Z = ");
    Serial.println(accelZ);
  #endif

  #ifdef WRITE_ACCEL_TO_SD
    WriteToSD(accelX, accelY, accelZ);
  #endif
}

#ifdef WRITE_ACCEL_TO_SD
void WriteToSD(float x, float y, float z) {
  if(curFile) {
    char fileName[14];
    curFile.getName(fileName, 14);
    if(fileName != "AccelData.txt") curFile.close();
  }
  else {
    curFile = SD.open("AccelData.txt", FILE_WRITE);
  }

  // if the file opened okay, write to it:
  if (curFile) {
    curFile.print("X = ");
    curFile.println(accelX);
    curFile.print("Y = ");
    curFile.println(accelY);
    curFile.print("Z = ");
    curFile.println(accelZ);
    curFile.close();
  }
}
#endif
#endif

