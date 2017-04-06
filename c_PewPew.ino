/*#define NUM_PEW_PEWS 5
PewPew pews[NUM_PEW_PEWS];

#define PEW_PEW_WIDTH 3
void DrawPewPews() {
  uint8_t minEmptySlot = 0xFF;
  for(uint8_t i = 0; i < NUM_PEW_PEWS; i++) {
    if(pews[i]) {
      if(pews[i].Loc == NUM_LEDS) {
        // Delete this pew
        free(pews[i]);
        pews[i] = NULL;
        if(minEmptySlot == 0xFF)
          minEmptySlot = i;
      }
      else {
        // Draw this pew
        uint16_t minLoc = pews[i].Loc - PEW_PEW_WIDTH + 1;
        while(minLoc > pews[i].Loc)
          minLoc++;
      
        if(pews[i].loc >= PEW_PEW_WIDTH)
          leds[pews[i].loc - PEW_PEW_WIDTH] = CHSV(0,0,0);
        leds(minLoc, pews[i].loc) = pews[i].color);
        
        pews[i].loc++;
        if(pews[i].loc < minLoc)
          minLoc = pews[i].loc;
      }
    }
    else if(minEmptySlot == 0xFF) {
      minEmptySlot = i;
    }
  }

  if(minEmptySlot != 0xFF && minLoc > 10 && random8(20) == 0) {
    pews[minEmptySlot] = malloc(sizeof(PewPew));
    struct PewPew temp = { 0, GetRandomMotifColor() };
    memcpy(pews[minEmptySlot], &temp, sizeof(temp));
  }
}
*/

#define PEW_PEW_WIDTH 3
void DrawPewPews() { // 370 bytes
  for(uint16_t i = 0; i < NUM_LEDS-1; i++)
    leds[i] = leds[i+1];
  leds[NUM_LEDS-1] = CRGB::Black;

  if(leds[NUM_LEDS-1-PEW_PEW_WIDTH] == CRGB(0,0,0) && leds[NUM_LEDS-2] != CRGB(0,0,0))
    leds[NUM_LEDS-1] = leds[NUM_LEDS-2];
  else if(leds[NUM_LEDS-5] == CRGB(0,0,0) && leds[NUM_LEDS-4] == CRGB(0,0,0) && leds[NUM_LEDS-3] == CRGB(0,0,0) && leds[NUM_LEDS-2] == CRGB(0,0,0) && leds[NUM_LEDS-1] == CRGB(0,0,0)) {
    if(random8(40) == 0)
      leds[NUM_LEDS-1] = GetRandomMotifColor();
  }
}

