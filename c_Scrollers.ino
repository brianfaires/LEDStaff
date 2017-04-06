void ScrollingGradient_Rainbow(uint16_t minIndex, uint16_t maxIndex) { // 50 bytes
  for(uint16_t i = minIndex; i <= maxIndex; i++)
    leds[i] = CHSV((i+loopCount) % 256, 255, maxBrightness);
}

void ScrollingGradient_TwoColor(uint16_t minIndex, uint16_t maxIndex, uint8_t transitionLength, CHSV* cPri, CHSV* cCon) { // 400 bytes
  const unsigned char cyclePeriodPattern = 2*transitionLength;
  uint8_t stepSizeHue = (cCon->hue - cPri->hue) / transitionLength;
  uint8_t stepSizeSat = (cCon->saturation - cPri->saturation) / transitionLength;

  unsigned char idx;
  unsigned int j = 0;
  unsigned int extendedLEDCount = (((maxIndex - minIndex)/cyclePeriodPattern)+1)*cyclePeriodPattern;
  while (j < extendedLEDCount) {
    for (int i = 0; i < transitionLength; i++) {
      idx = minIndex + (j + loopCount) % extendedLEDCount;
      if (j++ >= extendedLEDCount){ break; }
      if (idx > maxIndex){ continue; }

      CHSV temp = CHSV(cPri->hue + stepSizeHue*i, cPri->saturation + stepSizeSat*i, maxBrightness);
      leds[idx] = temp;
    }
    
    for (int i = 0; i < transitionLength; i++) {
      idx = minIndex + (j + loopCount) % extendedLEDCount;
      if (j++ >= extendedLEDCount){ break; }
      if (idx > maxIndex){ continue; }
      
      leds[idx] = CHSV(cCon->hue - stepSizeHue*i, cCon->saturation - stepSizeSat*i, maxBrightness);
    }
  }
}

void ScrollingDim(uint16_t minIndex, uint16_t maxIndex, uint8_t fullDarkLEDs, uint8_t fullBrightLEDs, uint16_t iter) { // 430 bytes
  uint8_t cyclePeriod = 14 + fullDarkLEDs + fullBrightLEDs;
  uint16_t extendedNumLED = (((maxIndex - minIndex)/cyclePeriod)+1)*cyclePeriod;

  uint16_t i = 0;
  while(i < extendedNumLED) {
    uint16_t idx;
    // progressively dim the LEDs
    for(uint8_t j = 0; j < 7; j++) {
      idx = minIndex + (i + iter) % extendedNumLED;
      if(i++ >= extendedNumLED) { return; }
      if(idx > maxIndex) { continue; }
      
      leds[idx].r >>= j;
      leds[idx].g >>= j;
      leds[idx].b >>= j;
    }
    
    // turn off these LEDs
    for (uint8_t j = 0; j < fullDarkLEDs; j++) {
      idx = minIndex + (i + iter) % extendedNumLED;
      if(i++ >= extendedNumLED) { return; }
      if(idx > maxIndex) { continue; }
      
      leds[idx] = CRGB(0,0,0);
    }
    
    // progressively bring these LEDs back
    for (uint8_t j = 0; j < 7; j++) {
      idx = minIndex + (i + iter) % extendedNumLED;
      if(i++ >= extendedNumLED) { return; }
      if(idx > maxIndex) { continue; }

      leds[idx].r >>= (7 - j);
      leds[idx].g >>= (7 - j);
      leds[idx].b >>= (7 - j); 
    }
    
    // skip over these LEDs to leave them at full brightness
    i += fullBrightLEDs;
  }
}

void ScrollingGlimmerDots(uint16_t minIndex, uint16_t maxIndex, uint8_t segmentLength, uint8_t brighteningCycles) { // 220 bytes
  uint16_t extendedLEDCount = (((maxIndex-minIndex)/(4*segmentLength))+1)*(4*segmentLength);

  for (uint8_t i = 0; i < extendedLEDCount; i++) {
    uint8_t brightness = loopCount%brighteningCycles + 1;
    uint16_t cycle = loopCount / brighteningCycles;

    // Move once per full brightening cycle
    uint16_t idx = minIndex + (i + cycle)%extendedLEDCount;
    
    if (idx <= maxIndex && i % segmentLength == 0) {
      // if this is an LED that we are coloring, set the color based on idx and the brightness based on where we are in the brightening cycle
      CHSV col;
      switch ((i/segmentLength)%4) {
        case 0:
          col = cPrimary;
          break;
         case 1:
          col = cNeutral;
          break;
        case 2:
          col = cContrast;
          break;
        case 3:
          col = cAccent;
          break;
      }

      leds[idx] = CHSV(col.hue, col.saturation, maxBrightness * brightness / brighteningCycles);
    }
  }
}

