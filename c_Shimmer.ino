void Shimmer(CHSV* color, uint16_t minPixel, uint16_t maxPixel, uint8_t changeAmount, bool dimOnly) { // 500 bytes
  static uint16_t shimmerLastSeedChange = 0;
  static uint16_t seed = random16(0xFFFF);
  if (loopCount % 6 == 0 && shimmerLastSeedChange != loopCount) {
    shimmerLastSeedChange = loopCount;
    seed = random16(0xFFFF);
  }
  random16_set_seed(seed);

    
  for (uint16_t i = minPixel; i <= maxPixel; i += 2) {
    CRGB changeAmount = leds[i].nscale8(random8(8));
    if(random8(2))
      leds[i] += changeAmount;
    else
      leds[i] -= changeAmount;
  }


  // every odd LED gets set to a quarter the brighness of the preceding even LED
  for(uint16_t i = minPixel+1; i <= maxPixel; i += 2)
      leds[i] = CRGB(leds[i-1].r >> 2, leds[i-1].g >> 2, leds[i-1].b >> 2);
}

