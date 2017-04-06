void DrawColorExplosion() { // 600 bytes
  if(loopCount >= 600)
    loopCount = 0;
  fadeOnly = loopCount > 500  ||  loopCount < 200 && loopCount % 3 != 0;

  if(curMotif == mStrobe || curMotif == mLove )
    ColorExplosion(0, NUM_LEDS-1, 6, 3, 2); // white, red
  else if(curMotif == mPastels || curMotif == mFire)
    ColorExplosion(0, NUM_LEDS-1, 6, 3, 3); // white, red, yellow
  else if(curMotif == mBright || curMotif == mEarth)
    ColorExplosion(0, NUM_LEDS-1, 6, 4, 2); // red, yellow
  else if(curMotif == mWater)
    ColorExplosion(0, NUM_LEDS-1, 6, 1, 3); // cyan, blue, white
  else //if(curMotif == mLife)
    ColorExplosion(0, NUM_LEDS-1, 6, 0, 3); // green, cyan, blue
}

void ColorExplosion(uint16_t minIndex, uint16_t maxIndex, uint8_t propChance, uint8_t minColor, uint8_t numColors) { // 550 bytes
  // Adjust existing colors
  ExplosionColorAdjust(&leds[minIndex], propChance, NULL, &leds[minIndex+1]);
  for (uint16_t i = minIndex; i < maxIndex; i++)
    ExplosionColorAdjust(&leds[i], propChance, &leds[i-1], &leds[i+1]);
  ExplosionColorAdjust(&leds[maxIndex], propChance, &leds[maxIndex-1], NULL);

  if (!fadeOnly)
    RandomColorInit(&leds[random8(minIndex, maxIndex)], minColor, numColors);
}

void RandomColorInit(CRGB* led, uint8_t minColor, uint8_t numColors) { // 350 bytes
  switch(random8(minColor, minColor + numColors)) {
    case 0:
      if (led->g == 0)
        led->g = 1;
      break;
    
    case 1:
      if ((led->g == 0) && (led->b == 0)) {
        led->g = 1;
        led->b = 1;
      }
      break;
    case 2:
      if (led->b == 0)
        led->b = 1;
      break;
    case 3:
      if (*led == CRGB(0,0,0))
        *led = CRGB(1,1,1);
      break;
    case 4:
      if (led->r == 0)
        led->r = 1;
      break;
    case 5:
      if ((led->r == 0) && (led->g == 0)) {
        led->r = 1;
        led->g = 1;
      }
      break;
    case 6:
      if ((led->r == 0) && (led->b == 0)) {
        led->r = 1;
        led->b = 1;
      }
      break;
  }
}

void ExplosionColorAdjust(CRGB* color, uint8_t propChance, CRGB* leftColor, CRGB* rightColor) { // 400 bytes
  // Propagate to neighbors
  uint8_t maxB = color->r;
  if(color->g > maxB) maxB = color->g;
  if(color->b > maxB) maxB = color->b;

  // TO DO: blending colors
  if (maxB == 31 && random8(propChance) != 0) {
    CRGB temp = CRGB(color->r == 31, color->g == 31, color->b == 31);
    if (leftColor && *leftColor == CRGB(0,0,0))
      *leftColor = temp;
    if (rightColor && *rightColor == CRGB(0,0,0))
      *rightColor = temp;
  }

  TwinkleColorAdjust(&color->r);
  TwinkleColorAdjust(&color->b);
  TwinkleColorAdjust(&color->g);
}

