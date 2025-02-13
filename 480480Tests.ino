/*******************************************************************************

From scratch build! 



*/
// * Start of Arduino_GFX setting

#include <Arduino_GFX_Library.h>
#define GFX_BL 38
Arduino_ESP32RGBPanel *bus = new Arduino_ESP32RGBPanel(
  39 /* CS */, 48 /* SCK */, 47 /* SDA */,
  18 /* DE */, 17 /* VSYNC */, 16 /* HSYNC */, 21 /* PCLK */,
  11 /* R0 */, 12 /* R1 */, 13 /* R2 */, 14 /* R3 */, 0 /* R4 */,
  8 /* G0 */, 20 /* G1 */, 3 /* G2 */, 46 /* G3 */, 9 /* G4 */, 10 /* G5 */,
  4 /* B0 */, 5 /* B1 */, 6 /* B2 */, 7 /* B3 */, 15 /* B4 */
);
Arduino_ST7701_RGBPanel *gfx =  new Arduino_ST7701_RGBPanel(
    bus, GFX_NOT_DEFINED /* RST */, 0 /* rotation */,
    false /* IPS */, 480 /* width */, 480 /* height */,
    st7701_type1_init_operations, sizeof(st7701_type1_init_operations),     true /* BGR */,
    10 /* hsync_front_porch */, 8 /* hsync_pulse_width */, 50 /* hsync_back_porch */,
    10 /* vsync_front_porch */, 8 /* vsync_pulse_width */, 20 /* vsync_back_porch */);



/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

void setup()
{
  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  // while(!Serial);
  Serial.println("PROGMEM Image Viewer");


#ifdef GFX_BL
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);
#endif
  // Init Display
  gfx->begin();
  gfx->fillScreen(BLACK);

#ifdef GFX_BL
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);
#endif
  gfx->setTextColor(WHITE);
  gfx->setCursor(180, 50);
  gfx->println(F("START "));
  delay(500); // .5 seconds
}

void Writeat(int h,int v, const char* text){
  gfx->setCursor(h, v);
  gfx->println(text);
}

void loop()
{
gfx->setTextSize(4);


   gfx->fillScreen(WHITE);gfx->setTextColor(RED);Writeat(180,50,"WHITE");delay(1000);
   gfx->fillScreen(RED);gfx->setTextColor(WHITE);Writeat(180,50,"RED");delay(1000);
  gfx->fillScreen(GREEN);gfx->setTextColor(RED);Writeat(180,50,"GREEN");delay(1000);
  gfx->fillScreen(BLUE);gfx->setTextColor(BLACK);Writeat(180,50,"BLUE");delay(1000);
  gfx->fillScreen(BLACK);gfx->setTextColor(WHITE);Writeat(180,50,"BLACK");delay(1000);

  delay(1000); // 1 second
}
