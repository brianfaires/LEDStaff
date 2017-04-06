#define WINDMILL_WIDTH 5
CRGB curWindmillColor1, curWindmillColor2;
void FullWindmills() {
  if(loopCount % (2*WINDMILL_WIDTH) == 0) {
    curWindmillColor1 = GetRandomMotifColor();
  }
  if(loopCount % (2*WINDMILL_WIDTH) < 5)
    leds(0, NUM_LEDS-1) = curWindmillColor1;
  else
    leds(0, NUM_LEDS-1) = CRGB::Black;
}

void HalfWindmills() {
  if(loopCount % 10 == 0) {
    curWindmillColor1 = GetRandomMotifColor();
  }
  else if(loopCount % (2*WINDMILL_WIDTH) == WINDMILL_WIDTH) {
    curWindmillColor2 = GetRandomMotifColor();
  }
  
  if(loopCount % (2*WINDMILL_WIDTH) < 5) {
    leds(0, NUM_LEDS/2-1) = curWindmillColor1;
    leds(NUM_LEDS/2, NUM_LEDS-1) = CRGB::Black;
  }
  else {
    leds(0, NUM_LEDS/2-1) = CRGB::Black;
    leds(NUM_LEDS/2, NUM_LEDS-1) = curWindmillColor2;
  }
}

#define SPECKLE_DISTANCE 5

void Speckles() {
  leds(0, NUM_LEDS-1) = CRGB::Black;
  for(uint8_t i = random8(SPECKLE_DISTANCE); i < NUM_LEDS-1; i += 2 + random8(SPECKLE_DISTANCE)) {
    leds[i] = i < NUM_LEDS/4 ? cContrast : i < NUM_LEDS/2 ? cNeutral : i < NUM_LEDS*4/5 ? cPrimary : cAccent;
  }
}

