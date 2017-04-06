void DrawSolidSimplePattern() {
  for(CRGB & pixel : leds(0, NUM_LEDS/2 - 1)) { pixel = cPrimary; }
  for(CRGB & pixel : leds(NUM_LEDS/2, NUM_LEDS*3/4 - 1)) { pixel = cNeutral; }
  for(CRGB & pixel : leds(NUM_LEDS*3/4, NUM_LEDS - 1)) { pixel = cContrast; }
  for(CRGB & pixel : leds(0, 3)) { pixel = cAccent; }
  for(CRGB & pixel : leds(NUM_LEDS-4, NUM_LEDS-1)) { pixel = cAccent; }
  for(CRGB & pixel : leds(NUM_LEDS/2 - 2, NUM_LEDS/2 + 1)) { pixel = cAccent; } // Highlights at the transitions
  for(CRGB & pixel : leds(NUM_LEDS*3/4 - 2, NUM_LEDS*3/4 + 1)) { pixel = cAccent; }
}

void DrawBrokenSimplePattern() {
  for(CRGB & pixel : leds(0, NUM_LEDS/4 - 1)) { pixel = cContrast; }
  for(CRGB & pixel : leds(NUM_LEDS/4, NUM_LEDS*3/8-1)) { pixel = CRGB::Black; }
  for(CRGB & pixel : leds(NUM_LEDS*3/8, NUM_LEDS*5/8-1)) { pixel = cPrimary; }
  for(CRGB & pixel : leds(NUM_LEDS*5/8, NUM_LEDS*3/4-1)) { pixel = CRGB::Black; }
  for(CRGB & pixel : leds(NUM_LEDS*3/4, NUM_LEDS-1)) { pixel = cContrast; }
}

#define DIAMOND_HEIGHT 9
void DrawDiamondPattern() {
  uint8_t offset = loopCount % (DIAMOND_HEIGHT-1); // Height of 9 = 8 cycles per pattern
  if(offset > DIAMOND_HEIGHT / 2)
    offset = DIAMOND_HEIGHT - 1 - offset;

  uint8_t numOuter = offset;
  uint8_t numInner = DIAMOND_HEIGHT - 2 - 2*numOuter;
  uint8_t cycles = NUM_LEDS / (DIAMOND_HEIGHT-1);

  for(uint8_t i = 0; i < cycles*(DIAMOND_HEIGHT-1);) {
    if(offset == 0) {
      leds[i++] = CHSV(0, 0, maxBrightness);
      leds(i, i+numInner-1) = cPrimary;
      i+=numInner;
    }
    else {
      leds(i, i+numOuter-1) = cNeutral;
      i+=numOuter;

      leds[i++] = CHSV(0, 0, maxBrightness);

      if(offset != DIAMOND_HEIGHT/2) {
        leds(i, i+numInner-1) = cPrimary;
        i+=numInner;
  
        leds[i++] = CHSV(0, 0, maxBrightness);
      }

      if(offset > 1) {
        leds(i, i+numOuter-2) = cNeutral;
        i+=(numOuter-1);
      }
    }
  }
}

#define SQUARE_EDGE_LENGTH 20
void DrawSquarePattern() {
  if(loopCount % SQUARE_EDGE_LENGTH < 3)
    leds(0, NUM_LEDS-1) = CRGB::Black;
  else if(loopCount % SQUARE_EDGE_LENGTH < 6)
    leds(0, NUM_LEDS-1) = cAccent;
  else if(loopCount % SQUARE_EDGE_LENGTH < 9)
    leds(0, NUM_LEDS-1) = CRGB::Black;
  else {
    uint8_t cycle = (loopCount/SQUARE_EDGE_LENGTH) % 4;
    CHSV mainColor = cycle == 0 ? cPrimary : cycle == 2 ? cContrast : cNeutral;
    leds(0, NUM_LEDS-1) = mainColor;
  }
}

