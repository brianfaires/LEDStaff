void DrawFadingBand(uint16_t centerIndex, uint8_t radius, CHSV* bandColor) {
  leds(centerIndex-radius, centerIndex + radius) = *bandColor;
  
  for(uint8_t i = 1; i <= radius; i++) {
    leds[centerIndex - i].fadeToBlackBy(255 * ((float)i/(radius)));
    leds[centerIndex + i].fadeToBlackBy(255 * ((float)i/(radius)));
  }
}

void PowerUpAnimation() {
  uint8_t count = 0;
  while(count++ < 100 + NUM_LEDS + 3) {
    Wipe(NUM_LEDS-1, 0, 4, count, &CHSV(HUE_RED, 255, maxBrightness), &CHSV(HUE_RED, 255, maxBrightness > count+20 ? count+20 : maxBrightness), true);
    if(count >= 50)
      Wipe(NUM_LEDS-1, 0, 4,count-50, &CHSV(HUE_BLUE, 255, maxBrightness), &CHSV(HUE_PURPLE, 220, maxBrightness > count ? count : maxBrightness), true);
    if(count >= 100)
      Wipe(NUM_LEDS-1, 0, 4, count-100, &CHSV(HUE_GREEN, 255, maxBrightness), &CHSV(0, 0, maxBrightness > count-70 ? count-70 : maxBrightness), true);
    delay(7);
    FastLED.show();
  }
}

bool Wipe(int16_t minIndex, int16_t maxIndex, int8_t headSize, int16_t iter, CHSV* headColor, CHSV* tailColor, bool stopAtEnd) {
  if(maxIndex > minIndex) {
    if(iter > maxIndex - minIndex + (stopAtEnd ? 0 : headSize))
      return true;
    int16_t head = minIndex + iter;
    if(head <= maxIndex)
        leds[head] += *headColor;

    if(iter >= headSize && (head - headSize >= minIndex))
        leds[head - headSize] = *tailColor;
  }
  else {
    if(iter > minIndex - maxIndex + (stopAtEnd ? 0 : headSize))
      return true;
    int16_t head = minIndex - iter;
    if(head >= maxIndex)
        leds[head] += *headColor;

    if(iter >= headSize && (head + headSize >= maxIndex))
        leds[head + headSize] = *tailColor;
  }

  return false;
}

