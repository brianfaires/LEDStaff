void Orb(uint16_t centerPoint, uint16_t minRadius, uint16_t maxRadius, CHSV* posColor, CHSV* negColor) { // 500 bytes
  uint16_t curRadius;
  CHSV* colorToUse;
  
  int16_t radiusOffset = round((maxRadius-minRadius) * (sin8(loopCount % 255) - 128.0) / 128.0);
  if(radiusOffset < 0) {
    colorToUse = negColor;
    curRadius = minRadius - radiusOffset;
  }
  else {
    colorToUse = posColor;
    curRadius = minRadius + radiusOffset;
  }
  
  // Draw
  leds[centerPoint] = *colorToUse;

  for(uint16_t i = 1; i <= curRadius; i++) {
    if(centerPoint >= i)
      leds[centerPoint - i] = *colorToUse;
    if(centerPoint + i < NUM_LEDS)
      leds[centerPoint + i] = *colorToUse;
  }
}
