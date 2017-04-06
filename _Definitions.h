// ------------
// ---CONFIG---
// ------------

// ---Compiler config---
//#define DEBUG
//#define USING_SERIAL
#define USING_SD
#define USING_ACCEL
//#define WRITE_ACCEL_TO_SD
#define USING_IMAGES
//#define DEBUG_BUTTONS_TO_SERIAL
//#define DETECT_ACTION

// ---Hardware config---
#define NUM_LEDS 142
#define NUM_BTNS 3
#define PIN_BTN0 A0//9
#define PIN_BTN1 A1//10
#define PIN_BTN2 A2//11
#define PIN_DATA 12
#define ANALOG_BTN_THRESHOLD 700

// ---Software config---
#define NUM_OBJECT_POINTERS 20
#define INIT_MODE mChaser
#define INIT_MODE_SWITCHING true
#define INIT_MODE_SWITCH_TIME 60
#define INIT_MOTIF mStrobe
#define INIT_MOTIF_SWITCHING true
#define INIT_MOTIF_SWITCH_TIME 45
#define INIT_MOTIF_WALKING true
#define INIT_MOTIF_WALK_RATE 2
#define INIT_MOTIF_WALK_STYLE mwsNone
#define INIT_SCHEME sNone
#define MAX_MOTIF_WALK_RATE 20
#define TIME_ACTION_SWITCH 400
#ifdef USING_IMAGES
#define LOOPS_PER_IMAGE 2000
#endif

// ---Update Rates---
#define UPDATE_RATE_IO 2
#define UPDATE_RATE_ACCEL 10
#define UPDATE_RATE_MOTIF_WALK 5
#define UPDATE_RATE_MODE_SWITCHING 5
#define UPDATE_RATE_MOTIF_SWITCHING 5

// ---Graphics---
#define MIN_BRIGHTNESS 35
#define INIT_MAX_BRIGHTNESS 200

// ---IO---
#define TIME_LONG_PRESS_INC 334
#define TIME_DEBOUNCE 20
#define TIME_LONG_PRESS_INIT 1000
#define TIME_DOUBLE_CLICK 500


// -----------
// ---Enums---
// -----------

//Modes
#define NUM_MODES 16
enum Modes {
  mBouncingComets = 0,
  mFireStaff = 1,
  mRepeatingShimmer = 2,
  mGradientTwoColor = 3,
  mGradientRainbow = 4,
  mWalkingColors = 5,
  mColorExplosion = 6,
  //mPewPews = 7,
  //mBreathingSaturation = 8,
  //mBreathingValue = 9,
  mTwinkle = 7,
  mChaser = 8,
  mChangingChaser = 9,
  //mSimpleSolid = 13,
  mSimpleBroken = 10,
  //mOrb = 15,
  //mDiamondPattern = 16,
  mSquarePattern = 11,
  mWindmillsFull = 12,
  mWindmillsHalf = 13,
  mSpeckles = 14,
  mSimpleImages = 15
};

// Motifs and Schemes







// Structs
struct Comet {
  uint16_t Min;
  uint16_t Max;
  uint8_t Speed;
  CHSV color;
};
struct Chaser {
  uint16_t Min;
  uint16_t Max;
  uint16_t Loc;
  uint8_t Width;
  bool Increasing;
  CRGB color;
};


// Help for the compiler
#ifdef USING_SD
void InitSD();
#endif
void PowerUpAnimation();
void ClearGlobalObjects();
void UpdateChaser(Chaser* c);
void DrawChaser(Chaser* c);
void DrawSolidSimplePattern();
void DrawBrokenSimplePattern();
void DrawBouncingComets(uint16_t minPixel, uint16_t maxPixel);
void Orb(uint16_t centerPoint, uint16_t minRadius, uint16_t maxRadius, CHSV* posColor, CHSV* negColor);
void Shimmer(CHSV* color, uint16_t minPixel, uint16_t maxPixel, uint8_t changeAmount, bool dimOnly);
void ScrollingGradient_Rainbow(uint16_t minIndex, uint16_t maxIndex);
void ScrollingGradient_TwoColor(uint16_t minIndex, uint16_t maxIndex, uint8_t transitionLength, CHSV* cPri, CHSV* cCon);
void ScrollingDim(uint16_t minIndex, uint16_t maxIndex, uint8_t fullDarkLEDs, uint8_t fullBrightLEDs, uint16_t iter);
void ScrollingGlimmerDots(uint16_t minIndex, uint16_t maxIndex, uint8_t segmentLength, uint8_t brighteningCycles);
void Twinkle(uint16_t minIndex, uint16_t maxIndex, uint8_t minColor, uint8_t numColors);
void ColorExplosion(uint16_t minIndex, uint16_t maxIndex, uint8_t propChance, uint8_t minColor, uint8_t numColors);
void DrawDiamondPattern();
void DrawSquarePattern();
void Fire2012();
#ifdef USING_IMAGES
  void NextImage();
  void StreamImage();
#endif
