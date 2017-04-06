#define COMET_FADE_TIME 10
#define COMET_MIN_SPEED 128
#define COMET_MAX_SPEED 255
#define COMET_MIN_VALUE 32
#define COMET_SPAWN_TIME 100

bool DrawComet(struct Comet* c) {  
  if(c->color.value <= COMET_MIN_VALUE)
    return false;

  // Determine location
  uint32_t dist = (((uint32_t)loopCount*(uint32_t)c->Speed) / 255);
  uint16_t loc = dist % (c->Max - c->Min);
  uint16_t cycles = dist / (c->Max - c->Min);
  
  // Draw Comet
  if(cycles % 2 == 0) leds[c->Min + loc] += c->color;
  else leds[c->Max - loc] += c->color;

  Fade(&c->color.value, COMET_FADE_TIME);
  return true;
}

void DrawBouncingComets(uint16_t minPixel, uint16_t maxPixel) { // 450 bytes
  static bool reduceComets = false;
  
  if(loopCount % COMET_SPAWN_TIME == 0) {
    uint8_t i = 0;
    while(i < NUM_OBJECT_POINTERS && objects[i])
      i++;

    if(i == NUM_OBJECT_POINTERS)
      reduceComets = true;
    else if(i <= NUM_OBJECT_POINTERS>>1)
      reduceComets = false;

    // Create a comet
    if(!reduceComets && !fadeOnly) {
      objects[i] = malloc(sizeof(Comet));
      RandomizeComet(objects[i], minPixel, maxPixel);
    }
  }

   // Update comets and respawn dead ones
  for(uint8_t i = 0; i < NUM_OBJECT_POINTERS; i++) {
    if(objects[i] && !DrawComet(objects[i])) {
      if(fadeOnly || reduceComets) {
        for(uint8_t j = NUM_OBJECT_POINTERS-1; j >= i; j--) {
          // Shift objects down to prevent gaps
          if(objects[j]) {
            free(objects[i]);
            objects[i] = objects[j];
            objects[j] = NULL;
            if(j <= NUM_OBJECT_POINTERS>>1)
              reduceComets = false;
            break;
          }
        }
      }
      else
        RandomizeComet(objects[i], minPixel, maxPixel);
    }
  }
}

void RandomizeComet(void* ptr, uint16_t minPixel, uint16_t maxPixel) {
  struct Comet temp = { minPixel, maxPixel, random8(COMET_MIN_SPEED, COMET_MAX_SPEED), GetRandomMotifColor() };
  memcpy(ptr, &temp, sizeof(temp));
}

