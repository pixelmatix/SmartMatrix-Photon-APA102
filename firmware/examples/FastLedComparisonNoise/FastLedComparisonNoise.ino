#define SMARTMATRIX_ENABLED   0

#include "application.h"

#if (SMARTMATRIX_ENABLED == 1)
  #include "SmartMatrix3/SmartMatrix3.h"
#endif

#include "FastLEDSmartMatrix/FastLEDSmartMatrix.h"
FASTLED_USING_NAMESPACE;

#if (SMARTMATRIX_ENABLED == 1)
  #define COLOR_DEPTH 24                  // known working: 24, 48 - If the sketch uses type `rgb24` directly, COLOR_DEPTH must be 24
  const uint8_t kMatrixWidth = 16;        // known working: 16, 32, 48, 64
  const uint8_t kMatrixHeight = 16;       // known working: 32, 64, 96, 128
  const uint8_t kRefreshDepth = 36;       // known working: 24, 36, 48
  const uint8_t kDmaBufferRows = 4;       // known working: 2-4, use 2 to save memory, more to keep from dropping frames and automatically lowering refresh rate
  const uint8_t kPanelType = 0;   // use SMARTMATRIX_HUB75_16ROW_MOD8SCAN for common 16x32 panels
  const uint8_t kMatrixOptions = (SMARTMATRIX_OPTIONS_NONE);      // see http://docs.pixelmatix.com/SmartMatrix for options
  const uint8_t kBackgroundLayerOptions = (SM_BACKGROUND_OPTIONS_NONE);
  const uint8_t kScrollingLayerOptions = (SM_SCROLLING_OPTIONS_NONE);
  
  SMARTMATRIX_ALLOCATE_BUFFERS(matrix, kMatrixWidth, kMatrixHeight, kRefreshDepth, kDmaBufferRows, kPanelType, kMatrixOptions);
  SMARTMATRIX_ALLOCATE_BACKGROUND_LAYER(backgroundLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kBackgroundLayerOptions);
  SMARTMATRIX_ALLOCATE_SCROLLING_LAYER(scrollingLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kScrollingLayerOptions);
  
  rgb24 *buffer;
  
  const uint16_t NUM_LEDS = kMatrixWidth * kMatrixHeight;
#else
  #define DATA_PIN D2
  #define CLOCK_PIN D4
  
  #define COLOR_ORDER BGR
  #define CHIPSET     APA102
  
  // Params for width and height
  const uint8_t kMatrixWidth = 16;
  const uint8_t kMatrixHeight = 16;
  
  const uint16_t NUM_LEDS = kMatrixWidth * kMatrixHeight;
  
  CRGB leds_plus_safety_pixel[ NUM_LEDS + 1];
  CRGB* leds( leds_plus_safety_pixel + 1);
#endif

const uint8_t scale = 256 / kMatrixWidth;

#if (SMARTMATRIX_ENABLED == 1)
  uint16_t XY(uint8_t x, uint8_t y) {
    return kMatrixWidth * y + x;
  }
#else
  uint16_t XY( uint8_t x, uint8_t y)
  {
    uint16_t i;
    
    if( y & 0x01) {
      // Odd rows run backwards
      uint8_t reverseX = (kMatrixWidth - 1) - x;
      i = (y * kMatrixWidth) + reverseX;
    } else {
      // Even rows run forwards
      i = (y * kMatrixWidth) + x;
    }
    
    return i;
  }
#endif

#define BRIGHTNESS 10

void setup() {
#if (SMARTMATRIX_ENABLED == 1)
  matrix.addLayer(&backgroundLayer);
  matrix.addLayer(&scrollingLayer);
  matrix.begin();
  matrix.setBrightness(BRIGHTNESS);
  backgroundLayer.enableColorCorrection(false);
#else
  FastLED.addLeds<CHIPSET, DATA_PIN, CLOCK_PIN, COLOR_ORDER, DATA_RATE_MHZ(1)>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );

  // FastLED.setDither(BINARY_DITHER);
  FastLED.setDither(DISABLE_DITHER);
#endif
}

uint16_t noisex;
uint16_t noisey;
uint16_t noisez;

int noisespeedx;
int noisespeedy;
int noisespeedz;

uint16_t noisescale;

CRGBPalette16 palette;

void drawNoise()
{
  for (uint8_t x = 0; x < kMatrixWidth; x++) {
    for (uint8_t y = 0; y < kMatrixHeight; y++) {
      uint16_t xoffset = noisescale * x;
      uint16_t yoffset = noisescale * y;

      uint8_t index = inoise8(x + xoffset + noisex, y + yoffset + noisey, noisez);

      // The range of the inoise8 function is roughly 16-238.
      // These two operations expand those values out to roughly 0..255
      // You can comment them out if you want the raw noise data.
      index = qsub8(index, 16);
      index = qadd8(index, scale8(index, 39));

#if (SMARTMATRIX_ENABLED == 1)
      buffer[XY(x, y)] = ColorFromPalette(palette, index);
#else
      leds[XY(x, y)] = ColorFromPalette(palette, index);
#endif
    }
  }

  noisex += noisespeedx;
  noisey += noisespeedy;
  noisez += noisespeedz;
}

void fireNoise() {
  noisespeedx = 0;
  noisespeedy = 64;
  noisespeedz = 0;
  noisescale = 64;
  palette = HeatColors_p;
  drawNoise();
}

void fireNoise2() {
  noisespeedx = 0;
  noisespeedy = 192;
  noisespeedz = 4;
  noisescale = 128;
  palette = HeatColors_p;
  drawNoise();
}

void lavaNoise() {
  noisespeedx = 0;
  noisespeedy = -3;
  noisespeedz = 2;
  noisescale = 8;
  palette = LavaColors_p;
  drawNoise();
}

void rainbowNoise() {
  noisespeedx = 8;
  noisespeedy = 0;
  noisespeedz = 0;
  noisescale = 16;
  palette = RainbowColors_p;
  drawNoise();
}

void rainbowStripeNoise() {
  noisespeedx = 2;
  noisespeedy = 0;
  noisespeedz = 0;
  noisescale = 4;
  palette = RainbowStripeColors_p;
  drawNoise();
}

void partyNoise() {
  noisespeedx = 0;
  noisespeedy = 8;
  noisespeedz = 1;
  noisescale = 16;
  palette = PartyColors_p;
  drawNoise();
}

void forestNoise() {
  noisespeedx = 2;
  noisespeedy = 0;
  noisespeedz = 0;
  noisescale = 4;
  palette = ForestColors_p;
  drawNoise();
}

void cloudNoise() {
  noisespeedx = 0;
  noisespeedy = 4;
  noisespeedz = 0;
  noisescale = 16;
  palette = CloudColors_p;
  drawNoise();
}

void oceanNoise() {
  noisespeedx = 0;
  noisespeedy = 0;
  noisespeedz = 16;
  noisescale = 16;
  palette = OceanColors_p;
  drawNoise();
}

typedef void (*Pattern)();
typedef Pattern PatternList[];

const PatternList patterns = {
  fireNoise,
  fireNoise2,
  lavaNoise,
  rainbowNoise,
  rainbowStripeNoise,
  partyNoise,
  forestNoise,
  cloudNoise,
  oceanNoise,
};

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
const uint8_t patternCount = ARRAY_SIZE(patterns);

uint8_t patternIndex = 0;

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  patternIndex = (patternIndex + 1) % patternCount;
}

void loop() {
   EVERY_N_MILLISECONDS(1000/30) {

#if (SMARTMATRIX_ENABLED == 1)
    buffer = backgroundLayer.backBuffer();
#endif      

  patterns[patternIndex]();
    
#if (SMARTMATRIX_ENABLED == 1)
    backgroundLayer.swapBuffers(true);
#endif
  }

#if (SMARTMATRIX_ENABLED == 0)
  FastLED.show();
#endif

  EVERY_N_SECONDS( 5 ) {
    nextPattern();  // change patterns periodically
  }
}