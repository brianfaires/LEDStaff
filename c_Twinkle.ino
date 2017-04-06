void DrawTwinkle() {
  if (loopCount >= 1300) {
    fadeOnly = false;
    loopCount = 0;
  }
  if (loopCount < 400) {
    // green, cyan, blue, white, red, yellow, magenta
    if(curMotif == mStrobe || curMotif == mPastels || curMotif == mLove)
      Twinkle(0, NUM_LEDS-1, 3, 1);  // white
    else if(curMotif == mBright || curMotif == mFire || curMotif == mEarth)
      Twinkle(0, NUM_LEDS-1, 4, 1); // red
    else if(curMotif == mWater || curMotif == mLife)
      Twinkle(0, NUM_LEDS-1, 2, 1); // cyan
  }
  else if (loopCount < 800) {
    if(curMotif == mStrobe || curMotif == mPastels || curMotif == mLove)
      Twinkle(0, NUM_LEDS-1, 3, 2);  // white and red
    else if(curMotif == mBright || curMotif == mEarth || curMotif == mFire)
      Twinkle(0, NUM_LEDS-1, 4, 2); // red, yellow
    else if(curMotif == mWater)
      Twinkle(0, NUM_LEDS-1, 2, 2); // cyan and blue
    else if(curMotif == mLife)
      Twinkle(0, NUM_LEDS-1, 0, 2); // green and cyan
  }
  else {
    fadeOnly = loopCount >= 1200;
    if(curMotif == mStrobe || curMotif == mPastels || curMotif == mLove || curMotif == mFire)
      Twinkle(0, NUM_LEDS-1, 3, 3); // white, red, yellow
    else if(curMotif == mBright || curMotif == mEarth)
      Twinkle(0, NUM_LEDS-1, 3, 3); // red, yellow, magenta
    else if(curMotif == mWater || curMotif == mLife)
      Twinkle(0, NUM_LEDS-1, 0, 3); // green, cyan, blue
  }
}

void Twinkle(uint16_t minIndex, uint16_t maxIndex, uint8_t minColor, uint8_t numColors) { // 220 bytes
  for (uint16_t i = minIndex; i <= maxIndex; i++) {
    TwinkleColorAdjust(&leds[i].r);
    TwinkleColorAdjust(&leds[i].g);
    TwinkleColorAdjust(&leds[i].b);
  }
  
  if (!fadeOnly) {
    // Randomly pick 1/32 LEDs to light up
    uint16_t numNewLights = 1 + (maxIndex - minIndex) >> 5;
    for (uint8_t i = 0; i < numNewLights; i++) {
      uint16_t j = minIndex + random8(maxIndex - minIndex);
      if (leds[j].r == 0 && leds[j].g == 0 && leds[j].b == 0)
        RandomColorInit(&leds[j], minColor, numColors);
    }
  }
}

void TwinkleColorAdjust(uint8_t *color) {
  if (*color >= maxBrightness) {
    *color = 254; // if max brightness, set to an even value to start fade
    while(*color >= maxBrightness)
      *color -= 2;
  }
  else if (*color % 2)
    *color = *color * 2 + 1; // ~Double brightness
  else if (*color > 0) {
    Fade(color, 4);
    if (*color % 2)
      (*color)--;  // if faded color is odd, subtract one to keep it even
  }
}
