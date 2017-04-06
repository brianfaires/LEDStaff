void NextMode() {
  loopCount = 0;


  curMode = (curMode+1) % NUM_MODES;
}

void InitMode(uint8_t newFadeRate, uint8_t newDelayRate) {
  fadeRate = newFadeRate;
  fadeOnly = false;
  delayRate = newDelayRate;
  loopCount = 0;
  for(CRGB & pixel : leds) { pixel = CRGB::Black; }
  ClearGlobalObjects();
}

void InitNewMode() { // 650 bytes
  switch(curMode) {
      case mChaser:
        InitMode(255, 10);
        objectsAreValues = false;
        objects[0] = malloc(sizeof(Chaser));
        {
          struct Chaser temp = { 0, NUM_LEDS-1, 0, 5, true, cPrimary };
          memcpy(objects[0], &temp, sizeof(temp));
        }
        if(loopCount == 500) NextMode();
        break;
      case mChangingChaser:
        objectsAreValues = false;
        if(!objects[0] || ((Chaser*)objects[0])->Loc < NUM_LEDS/3) {
          InitMode(255, 10);
          objects[0] = malloc(sizeof(Chaser));
          {
            struct Chaser temp = { 0, NUM_LEDS-1, NUM_LEDS/3, 5, true, cPrimary };
            memcpy(objects[0], &temp, sizeof(temp));
          }
        }
        if(loopCount == 500) NextMode();
        break;

      case mSimpleBroken:
        InitMode(0, 5);
        break;
      case mBouncingComets:
        objectsAreValues = false;
        InitMode(64, 10);
        break;

      case mFireStaff:
        InitMode(0, 20);
        break;
      case mRepeatingShimmer:
        objectsAreValues = true;
        InitMode(0, 10);
        break;
      case mTwinkle:
        InitMode(0, 10);
        break;
      case mGradientTwoColor:
        InitMode(0, 5);
        break;
      case mGradientRainbow:
        InitMode(0, 3);
        break;
      case mWalkingColors:
        InitMode(32, 3);
        break;
      case mColorExplosion:
        InitMode(0, 5);
        break;

      case mSquarePattern:
        InitMode(0, 4);
        break;

      case mWindmillsFull:
      case mWindmillsHalf:
      case mSpeckles:
        InitMode(0, 0);
        break;
      #ifdef USING_IMAGES
      case mSimpleImages:
        InitMode(0, 0);
        break;
      #endif
    }
}

void DrawCurMode() { //1070 bytes
    switch(curMode) {
    
    
    case mChangingChaser:
      UpdateChaser(objects[0]);
    case mChaser:
      ((Chaser*)objects[0])->color = cPrimary;
      DrawChaser(objects[0]);
      break;
      
      
    case mSimpleBroken:
      DrawBrokenSimplePattern();
      break;
      
    case mBouncingComets:
      DrawBouncingComets(0, NUM_LEDS-1);
      break;

      
    case mRepeatingShimmer:
      if(loopCount % 420 == 0) {
        loopCount = 0;
        uint8_t r = random8(2);
        objects[0] = r==0 ? &cPrimary : r==1 ? &cContrast : &cNeutral;
        r = (r+1+random8(1)) % 3;
        objects[1] = r==0 ? &cPrimary : r==1 ? &cContrast : &cNeutral;
      }
      Shimmer(objects[0], 0, NUM_LEDS-1, 2, loopCount % 420 >= 300);
      Shimmer(objects[1], 0, NUM_LEDS/2 - 1, 2, loopCount % 420 >= 300);
      break;
       
    case mGradientTwoColor:
      ScrollingGradient_TwoColor(0, NUM_LEDS-1, 40, &cPrimary, &cContrast);
      ScrollingDim(0, NUM_LEDS-1, 5, 2, loopCount>>1); // TO DO: Background color
      break;
        
    case mWalkingColors:
      ScrollingGlimmerDots(0, NUM_LEDS-1, 5, 10);
      break;

    case mTwinkle:
      DrawTwinkle();
      break;
      
    case mColorExplosion:
      DrawColorExplosion();
      break;


    case mSquarePattern:
      DrawSquarePattern();
      break;


    case mWindmillsFull:
      FullWindmills();
      break;
    case mWindmillsHalf:
      HalfWindmills();
      break;
    case mSpeckles:
      Speckles();
      break;

 // NOT PLUGGED INTO MOTIFS
    #ifdef USING_IMAGES
    case mSimpleImages:
      if(loopCount >= LOOPS_PER_IMAGE) {
        NextImage();
        loopCount = 0;
      }
      StreamImage();
      break;
    #endif  
    case mFireStaff:
      random16_add_entropy(random());
      Fire2012();
      //Fire(NUM_LEDS/2-1, 1, loopCount);
      break;
      
    case mGradientRainbow:
      ScrollingGradient_Rainbow(0, NUM_LEDS-1);
      ScrollingDim(0, NUM_LEDS-1, 5, 2, loopCount>>1);
     break;
      
    /*case sTwoBeamStream: // TO DO: Fix this
      //Orb(LED_COUNT/2, 5, 20, 700, 4, cNeutral, (rgb_color){ cNeutral.red>>2, cNeutral.green>>1, cNeutral.blue>>1 }, loopCount/3);
      
      { // To keep variables in scope
        rgb_color origA = colors[55];
        rgb_color origB = colors[87];
        colors[55] = MergeColors(MergeColors(cPrimary, origA), origA);
        colors[87] = MergeColors(MergeColors(cContrast, origB), origB);
      }

      if(loopCount % 7 == 0)
      {
        StepLEDs(0, 55, false);
        StepLEDs(87, LED_COUNT - 1, true);
        FadeAll(7);
      }
      
      RandomWalk(&schemeStepSize, 1, 8, 1, 3);
      
      delayRate = 0;
      break;*/
  }
}

