// Questions: why does for(leds & pixel) sometimes take more space than leds(0, NUM_LEDS-1) ??

// Enhancements
//    Add second LED buffer, called background; use to overlay 2 graphics that use buffer for Mode data
//    Use global fade only in between Modes?
//    Accelerometer/SD card writing; use to study diff moves

// Hardware
//    Max brightness Adjust
//    Walk Mode on/off, plus rate controls
//    Mic
  //    bpm detection

// Contact Stuff
//  Elastic/sin wave chaser

// 2d Patterns
//  Move a white pixel up/down. At peak, draw X pixels white up or down
//  C shapes
//  Solid pattern, reverses color pattern on regular basis
//  contiguous chunks of LEDs that sum to a given saturation then randomly walk it (moving white back and forth between the chunks)
//  Flickering bars, one at each end
//  Outer and Inner segments of LEDs, on/off at diff times.  Maybe flicker each 3-4 times before switching to the other
//  Windmills
//  Spirals (via scrolling glimmer dots?)
//  Yin/Yang (with border)
//  Lines that create a curved-looking surface
//  Avocado petals
//  Spider web (spider comes out!!?)
//  Pixels: flash on briefly, 80% empty, diff colors in diff segments
//  X's and O's

// Animations
//   Flower blooming
//   Stick figure doing something

// Random unrelated notes
//  Painting that constantly changes; user input to draw diff colors

// ---DEBUG mode---
// Check that SD card and accelerometer are working, outputing data to Serial. Then enter the motif editor.
#ifdef DEBUG
void InitDiagnostics() {
  InitIO();
  #ifdef USING_SERIAL
    InitSerial();
  #endif

  // Init Motif
  InitMotifValues();
  ApplyScheme();
  InitMotifWalkTargets();

  leds(0, NUM_LEDS-1) = CRGB::Black;
  FastLED.show();

  #ifdef USING_ACCEL
    #ifdef USING_SERIAL
      Serial.print("Initializing Accelerometer...");
    #endif
    if(myIMU.begin() == IMU_SUCCESS)
      #ifdef USING_SERIAL
        Serial.println("Accelerometer initialization done.")
      #endif
      ;
    else
      #ifdef USING_SERIAL
        Serial.println("Accelerometer initialization failed!")
      #endif
      ;
  #endif

  #ifdef USING_SD
    #ifdef USING_SERIAL
      Serial.print("Initializing SD card...");
    #endif
    if (SD.begin())
      #ifdef USING_SERIAL
        Serial.println("SD initialization done.")
      #endif
      ;
    else
      #ifdef USING_SERIAL
        Serial.println("SD initialization failed!")
      #endif
      ;

    #ifdef USING_SERIAL
      Serial.println("Reading SD card...");
      PrintDirectory();
      Serial.println("done!");
    #endif
  #endif
}
#endif

#ifdef USING_SD
#ifdef USING_SERIAL
void PrintDirectory() {
  SdFile file;
  while (file.openNext(SD.vwd(), O_READ)) {
    file.printName(&Serial);
    Serial.write('\t');
    file.printFileSize(&Serial);
    Serial.write('\t');
    file.printModifyDateTime(&Serial);
    Serial.println();
    file.close();
  }
}
#endif
#endif

#ifdef DEBUG
void LoopDiagnostics() {
  #ifdef DEBUG_BUTTONS_TO_SERIAL
  // Test button pressing only
  int temp = analogRead(PIN_BTN0);
  if(temp > ANALOG_BTN_THRESHOLD) {
    Serial.print("A0 = ");
    Serial.println(temp);
  }
  delay(5);
  temp = analogRead(PIN_BTN1);
  if(temp > ANALOG_BTN_THRESHOLD) {
    Serial.print("A1 = ");
    Serial.println(temp);
  }
  delay(5);
  temp = analogRead(PIN_BTN2);
  if(temp > ANALOG_BTN_THRESHOLD) {
    Serial.print("A2 = ");
    Serial.println(temp);
  }
  delay(5);

  return;
  #else
    
  static bool btnPressed = true;
 
  if(btnPressed)
    LoopMotifDesigner();
  else {
    #ifdef USING_ACCEL
      // Output Accelerometer and Analog Readings until any button is pressed
      float aX = myIMU.readFloatAccelX();
      float aY = myIMU.readFloatAccelY();
      float aZ = myIMU.readFloatAccelZ();

      #ifdef USING_SERIAL
        Serial.print("\nAccelerometer:\n");
        Serial.print(" X = ");
        Serial.println(aX, 4);
        Serial.print(" Y = ");
        Serial.println(aY, 4);
        Serial.print(" Z = ");
        Serial.println(aZ, 4);
  
        Serial.print("A0 = ");
        Serial.println(analogRead(PIN_BTN0));
        Serial.print("A1 = ");
        Serial.println(analogRead(PIN_BTN1));
        Serial.print("A2 = ");
        Serial.println(analogRead(PIN_BTN2));
      #endif
      
      #ifdef WRITE_ACCEL_TO_SD
        WriteToSD(aX, aY, aZ);
      #endif
      
      uint32_t exitTime = millis() + 1000;
      while(millis() < exitTime)
          //if(digitalRead(PIN_BTN0) == LOW || digitalRead(PIN_BTN1) == LOW || digitalRead(PIN_BTN2) == LOW)
          if(analogRead(PIN_BTN0) > ANALOG_BTN_THRESHOLD || analogRead(PIN_BTN1) > ANALOG_BTN_THRESHOLD || analogRead(PIN_BTN2) > ANALOG_BTN_THRESHOLD) {
            delay(TIME_DEBOUNCE);
            if(analogRead(PIN_BTN0) > ANALOG_BTN_THRESHOLD || analogRead(PIN_BTN1) > ANALOG_BTN_THRESHOLD || analogRead(PIN_BTN2) > ANALOG_BTN_THRESHOLD)
              btnPressed = true;
          }
    #else
      LoopMotifDesigner();
    #endif
  }
  #endif
}

void LoopMotifDesigner() {
  // Set 4 colors and write their values to serial port
  // Pressing/holding button 0 moves the hue, button 1 moves the saturation. Move value up and down on its own.
  // Button 2 reverses direction of hue/saturation movement. Double click switches to next color. Hold to go to next motif.

  leds(0, NUM_LEDS-1) = CRGB::Black;
  
  uint8_t curColor = 0;
  uint8_t curValue = 128;
  bool increaseValue = true;
  bool increaseOther = true;
  bool lastPressWas2 = false;
  loopCount = 0;
  
  while(true) {
    // Walk value
    if(increaseValue) {
      if(++curValue >= maxBrightness - 1) increaseValue = false;
    }
    else {
      if(--curValue <= MIN_BRIGHTNESS + 1) increaseValue = true;
    }

    if(curColor == 0) cPrimary.value = curValue;
    else if(curColor == 1) cContrast.value = curValue;
    else if(curColor == 2) cNeutral.value = curValue;
    else cAccent.value = curValue;

    // Adjust hue
    //if(digitalRead(PIN_BTN0) == LOW && loopCount % 4 == 0) {
    if(analogRead(PIN_BTN0) > ANALOG_BTN_THRESHOLD && loopCount % 4 == 0) {
      delay(TIME_DEBOUNCE);
      if(analogRead(PIN_BTN0) > ANALOG_BTN_THRESHOLD) {
        lastPressWas2 = false;
        //delay(20);
        if(increaseOther) {
          if(curColor == 0) cPrimary.hue++;
          else if(curColor == 1) cContrast.hue++;
          else if(curColor == 2) cNeutral.hue++;
          else cAccent.hue++;
        }
        else {
          if(curColor == 0) cPrimary.hue--;
          else if(curColor == 1) cContrast.hue--;
          else if(curColor == 2) cNeutral.hue--;
          else cAccent.hue--;
        }
      }
    }

    
    // Adjust saturation
    //if(digitalRead(PIN_BTN1) == LOW && loopCount % 2 == 0) {
    if(analogRead(PIN_BTN1) > ANALOG_BTN_THRESHOLD && loopCount % 2 == 0) {
      delay(TIME_DEBOUNCE);
      if(analogRead(PIN_BTN1) > ANALOG_BTN_THRESHOLD) {
        lastPressWas2 = false;
        //delay(20);
        if(increaseOther) {
          if(curColor == 0) cPrimary.saturation++;
          else if(curColor == 1) cContrast.saturation++;
          else if(curColor == 2) cNeutral.saturation++;
          else cAccent.saturation++;
        }
        else {
          if(curColor == 0) cPrimary.saturation--;
          else if(curColor == 1) cContrast.saturation--;
          else if(curColor == 2) cNeutral.saturation--;
          else cAccent.saturation--;
        }
      }
    }


    // Handle button2 logic
    //if(digitalRead(PIN_BTN2) == LOW) {
    if(analogRead(PIN_BTN2) > ANALOG_BTN_THRESHOLD) {
      delay(TIME_DEBOUNCE);
      if(analogRead(PIN_BTN2) > ANALOG_BTN_THRESHOLD) {
        uint32_t depTime = millis();
        //while(digitalRead(PIN_BTN2) == LOW)
        while(analogRead(PIN_BTN2) > ANALOG_BTN_THRESHOLD)
          if(millis() - depTime > 1000) break;
  
        if(millis() - depTime > 1000) {
          #ifdef USING_SERIAL
            WriteCurMotif();
          #endif
          NextMotif();
          lastPressWas2 = false;
          curColor = 0;
          loopCount = 0;
          DrawMotifDesigner();
        }
        else if(lastPressWas2) {
          curColor = (curColor+1) % 4;
          increaseOther = !increaseOther;
          lastPressWas2 = false;
        }
        else {
          increaseOther = !increaseOther;
          lastPressWas2 = true;
        }
      }
    }

    loopCount++;
    DrawMotifDesigner();
    delay(10);
  }

  NextMotif();
}

void DrawMotifDesigner() {
  leds(0, 4) = cAccent;
  leds(5, 25) = cContrast;
  leds(26, 29) = cAccent;
  leds(30, 50) = cNeutral;
  leds(51, 54) = cAccent;
  leds(55, 86) = cPrimary;
  leds(87, 90) = cAccent;
  leds(91, 111) = cNeutral;
  leds(112, 115) = cAccent;
  leds(116, 136) = cContrast;
  leds(137, 141) = cAccent;

  FastLED.show();
}

#ifdef USING_SERIAL
void WriteCurMotif() {
  Serial.print("curMotif = ");
  Serial.println(curMotif);
  
  Serial.print("cPrimary = (");
  Serial.print(cPrimary.hue);
  Serial.print(", ");
  Serial.print(cPrimary.saturation);
  Serial.print(", ");
  Serial.print(cPrimary.value);
  Serial.println(")");

  Serial.print("cContrast = (");
  Serial.print(cContrast.hue);
  Serial.print(", ");
  Serial.print(cContrast.saturation);
  Serial.print(", ");
  Serial.print(cContrast.value);
  Serial.println(")");

  Serial.print("cNeutral = (");
  Serial.print(cNeutral.hue);
  Serial.print(", ");
  Serial.print(cNeutral.saturation);
  Serial.print(", ");
  Serial.print(cNeutral.value);
  Serial.println(")");

  Serial.print("cAccent = (");
  Serial.print(cAccent.hue);
  Serial.print(", ");
  Serial.print(cAccent.value);
  Serial.print(", ");
  Serial.print(cAccent.saturation);
  Serial.println(")");
}
#endif
#endif
