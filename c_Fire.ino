// TO DO: Fix and test this; Plug into motif and maxBrightness
void Fire(uint16_t centerIndex, uint8_t directions) {
  uint8_t coreRadius = Fire_DrawCore(centerIndex, directions);  
  uint8_t radiationRadius;
  if(loopCount > 1000) {
    if(directions > 0) { radiationRadius = Fire_DrawRadiation(true, centerIndex + coreRadius + 1); }
    if(directions < 2) { radiationRadius = Fire_DrawRadiation(false, centerIndex - coreRadius - 1); }
  }
  
  uint8_t convPosRadius=0, convNegRadius=0, minIdx=0, maxIdx=0;
  if(loopCount > 2000) {
    if(directions > 0) { convPosRadius = Fire_DrawConvection(true, centerIndex + coreRadius + radiationRadius); }
    if(directions < 2) { convNegRadius = Fire_DrawConvection(false, centerIndex - coreRadius - radiationRadius); }
  }
  
  if(loopCount > 3000)
  {
    if(directions > 0) { Fire_DrawFlares(true, centerIndex + 30); }
    if(directions < 2) { Fire_DrawFlares(false, centerIndex - 30); }    
  }

  if(directions > 0) { Fire_Fade(true, centerIndex + coreRadius + radiationRadius + convPosRadius + 1, minIdx); }
  if(directions < 2) { Fire_Fade(false, centerIndex - coreRadius - radiationRadius - convNegRadius - 1, maxIdx); }
}

uint8_t Fire_DrawCore(uint16_t centerIndex, uint8_t directions) {
  static uint8_t coreRadius = 0;

  // Center Point
  if(loopCount == 0)
    leds[centerIndex] = CRGB(255, 160, 90);
  else {
    uint8_t walkFactor = 1;
    if(random8(100) == 0)
      walkFactor = 100;
    RandWalk(&leds[centerIndex].r, 230, 255, walkFactor*2, 3);
    RandWalk(&leds[centerIndex].g, 130, 180, walkFactor*4, 3);
    RandWalk(&leds[centerIndex].b, 50, 110, walkFactor*5, 2);
  }

  // Walk core radius
  RandWalk(&coreRadius, 0, 5, 1, 3);
  if(coreRadius == 5 && random8(4) != 0)
    coreRadius--;

  static uint8_t corePosGreenPenalty = 0;
  static uint8_t coreNegGreenPenalty = 0;

  if(loopCount % 10 == 0)
    corePosGreenPenalty = random8(9);
  else if(loopCount % 10 == 5)
    coreNegGreenPenalty = random8(9);
  
  for(int8_t i = coreRadius - 1; i >= 0; i--) {
    if(directions > 0) {
      uint8_t penalty = i == 0 ? corePosGreenPenalty : 0;
      int idx = centerIndex + 1 + i;
      if(idx < NUM_LEDS)
        leds[idx] = CRGB(leds[idx-1].r * 5/6 + 1, leds[idx-1].g * 2/3 - 3 - penalty, leds[idx-1].b >> (i+2));
    }
    
    if(directions < 2)
    {
      uint8_t penalty = i == 0 ? coreNegGreenPenalty : 0;
      uint16_t idx = centerIndex - 1 - i;
      if(idx >= 0)
        leds[idx] = CRGB(leds[idx+1].r * 5/6 + 1, leds[idx+1].g * 2/3 - 3 - penalty, leds[idx+1].b >> (i+2));
    }
  }

  return coreRadius;
}

uint8_t Fire_DrawRadiation(bool inPosDirection, int minIndex) {
  // Radiation zone: randomly walk toward orange/red and dim each successive LED
  const uint8_t maxRadius = 14;
  const uint8_t minRadius = 6;
  static uint8_t radiationRadius = 10;
  static bool radiationIncreasing = true;

  if(loopCount % 3 == 0) {
    radiationRadius += radiationIncreasing ? 1 : -1;
    if(radiationRadius >= maxRadius) { radiationIncreasing = false; }
    else if(radiationRadius <= minRadius) { radiationIncreasing = true; }
  }

  for(int8_t i = radiationRadius; i >= 0; i--) {
    uint16_t idx;
    
    if(inPosDirection) {
      idx = minIndex + i;
      if(idx < NUM_LEDS) {
        uint8_t dec = random8(i+12/(i+1) - 1);
        leds[idx].r = leds[idx-1].r;
        leds[idx].g = dec >= leds[idx-1].g ? 0 : leds[idx-1].g-dec;
      }
    }
    else {
      idx = minIndex - i;
      if(idx >= 0) {
        uint8_t dec = random8(i+12/(i+1) - 1);
        leds[idx].r = leds[idx+1].r;
        leds[idx].g = dec >= leds[idx+1].g ? 0 : leds[idx+1].g-dec;
      }
    }

    leds[idx].b = 0;

    if(2*leds[idx].g < leds[idx].r) {
      leds[idx].g += 1;
      leds[idx].r -= 2;
    }
    else if(leds[idx].g > leds[idx].r * 4/5) {
      leds[idx].g -= 2;
      leds[idx].r += 2;
    }
  }

  return radiationRadius;
}

uint8_t Fire_DrawConvection(bool inPosDirection, int minIndex) {
  // Convection zone: move toward orange
  if(inPosDirection) {
    uint8_t posConvectionRadius = 14 * leds[minIndex].r / (leds[minIndex].r + leds[minIndex].g);
    static uint8_t prevPosConvRadius = 1;
    if(posConvectionRadius > prevPosConvRadius + 1)
      posConvectionRadius = prevPosConvRadius + 1;
    prevPosConvRadius = posConvectionRadius;
    
    for(int8_t i = posConvectionRadius; i >= 0; i--) {
      uint16_t idx = minIndex + i;
      if(i < NUM_LEDS) {
        if(2*(uint16_t)leds[idx-1].r >= 3*(uint16_t)leds[idx-1].g) {
          leds[idx].r = leds[idx-1].r - 2;
          leds[idx].g = leds[idx-1].g + 3;
          leds[idx].b = 0;
        }
        else {
          leds[idx].r = leds[idx-1].r + 2;
          leds[idx].g = leds[idx-1].g - 1;
          leds[idx].b = 0;
        }
      }
    }

    return posConvectionRadius;
  }
  else {
    uint8_t negConvectionRadius = 14 * leds[minIndex].r / (leds[minIndex].r + leds[minIndex].g);
    static uint8_t prevNegConvRadius = 1;
    if(negConvectionRadius > prevNegConvRadius + 1)
      negConvectionRadius = prevNegConvRadius + 1;
    prevNegConvRadius = negConvectionRadius;
    
    for(int8_t i = negConvectionRadius; i >= 0; i--) {
      int idx = minIndex - i;
      if(i >= 0) {
        if(2*(uint16_t)leds[idx+1].r >= 3*(uint16_t)leds[idx+1].g)
        {
          leds[idx].r = leds[idx+1].r - 1;
          leds[idx].g = leds[idx+1].g + 1;
          leds[idx].b = 0;
        }
        else {
          leds[idx].r = leds[idx+1].r;
          leds[idx].g = leds[idx+1].g - 1;
          leds[idx].b = 0;
        }
      }
    }

    return negConvectionRadius;
  }
}

uint8_t Fire_DrawFlares(bool inPosDirection, int minIndex) {
  const uint8_t flareWidth = 2;
  const uint8_t maxFlareDistance = 25;
  if(inPosDirection) {
    int16_t minIdx = -1;
    for(int16_t i = maxFlareDistance; i >= 0; i--) {
      if(minIndex + i < NUM_LEDS) {
        if(leds[minIndex + i].r == 0 && leds[minIndex + i].g == 0 && leds[minIndex + i].b == 0)
          minIdx = minIndex + i-2;
        if(minIndex + i >= minIdx)
          leds[minIndex + i] = CRGB(leds[minIndex + i - 1].r, leds[minIndex + i - 1].g, leds[minIndex + i - 1].b);
      }
    }
    if(minIdx == minIndex - flareWidth)
      minIdx = -1;

    return minIdx;
  }
  else {
    uint16_t maxIdx = NUM_LEDS-1;
    for(int16_t i = maxFlareDistance; i >= 0; i--) {
      if(minIndex - i >= 0) {
        if(leds[minIndex - i].r == 0 && leds[minIndex - i].g == 0 && leds[minIndex - i].b == 0)
          maxIdx = minIndex - i + flareWidth;
        if(minIndex - i <= maxIdx)
          leds[minIndex - i] = CRGB(leds[minIndex - i + 1].r, leds[minIndex - i + 1].g, leds[minIndex - i + 1].b);
      }
    }
    if(maxIdx == minIndex + flareWidth)
      maxIdx = -1;
  
    return maxIdx;
  }
}

void Fire_Fade(bool inPosDirection, uint16_t minIndex, uint16_t maxIndex) {
  if(inPosDirection) {
    for(; minIndex < NUM_LEDS; minIndex++) {
      if(minIndex == maxIndex || minIndex == maxIndex-1)
        leds[minIndex].fadeToBlackBy(60+random8(64));
      else
        leds[minIndex].fadeToBlackBy(128 + random8(128));
    }
  }
  else {
    for(; minIndex >= 0; minIndex--) {
      if(minIndex == maxIndex || minIndex == maxIndex+1)
        leds[minIndex].fadeToBlackBy(60+random8(64));
      else
        leds[minIndex].fadeToBlackBy(128+random8(128));
    }
  }
}
