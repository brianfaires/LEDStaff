void RandWalk(unsigned char *val, unsigned char minVal, unsigned char maxVal, unsigned char changeAmount, unsigned char directions) {
  unsigned char walk = random8(directions);
  if (walk == 0) { // Decrease
    if (*val >= minVal + changeAmount)
      *val -= changeAmount;
    else
      *val = minVal;
  }
  else if (walk == 1) { // Increase
    if (*val <= maxVal - changeAmount)
      *val += changeAmount;
    else
      *val = maxVal;
  }
}

void Fade(uint8_t *val, uint8_t fadeTime) {
  if (*val != 0)
  {
    uint8_t subAmt = *val >> fadeTime;  // val * 2^-fadeTime
    if (subAmt < 1)
      subAmt = 1;  // make sure we always decrease by at least 1
    *val -= subAmt;  // decrease value of byte pointed to by val
  }
}

void ClearGlobalObjects() {
  for(uint8_t i = 0; i < NUM_OBJECT_POINTERS; i++) {
    if(objects[i]) {
      if(!objectsAreValues)
        free(objects[i]);
      objects[i] = NULL;
    }
  }
}



#ifdef USING_SERIAL
void InitSerial() {
  Serial.begin(57600);
  delay(1000);
  while (!Serial);
}
#endif

// ---UNUSED CODE---
CRGB Merge(CRGB* a, CRGB* b) {
  return CRGB((a->r + b->r) / 2, (a->g + b->g) / 2, (a->b + b->b) / 2);
}
/*
void UpdateMaxBrightness(uint8_t newMaxBrightness) {
  maxBrightness = newMaxBrightness;
  
  uint8_t maxValue = cPrimary.value;
  if(cContrast.value > maxValue)
    maxValue = cContrast.value;
  if(cNeutral.value > maxValue)
    maxValue = cNeutral.value;
  if(cAccent.value > maxValue)
    maxValue = cAccent.value;

  if(maxValue > maxBrightness) {
    float scaleFactor = maxBrightness / (float)maxValue;
    cPrimary.value = round(cPrimary.value * scaleFactor);
    cContrast.value = round(cContrast.value * scaleFactor);
    cNeutral.value = round(cNeutral.value * scaleFactor);
    cAccent.value = round(cAccent.value * scaleFactor);
  }
}
*/

