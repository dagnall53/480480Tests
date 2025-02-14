/*******************************************************************************

From scratch build! 



*/
// * Start of Arduino_GFX setting

#include <Arduino_GFX_Library.h>
//select pin definitions for installed GFX version (test with 1.5.3 and 1.3.1)
#include "GUITIONESP32-S3-4848S040_GFX_131.h"

/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/
 #include <TAMC_GT911.h>
#define TOUCH_ROTATION ROTATION_NORMAL
#define TOUCH_MAP_X1 480
#define TOUCH_MAP_X2 0
#define TOUCH_MAP_Y1 480
#define TOUCH_MAP_Y2 0

int touch_last_x = 0, touch_last_y = 0;

TAMC_GT911 ts = TAMC_GT911(I2C_SDA_PIN, I2C_SCL_PIN, TOUCH_INT, TOUCH_RST, max(TOUCH_MAP_X1, TOUCH_MAP_X2), max(TOUCH_MAP_Y1, TOUCH_MAP_Y2));






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
  //if GFX> 1.3.1 
  //gfx->invertDisplay(false);
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

void DisplayCheck(bool invertcheck)
{
  static unsigned long timedInterval;
  static int Color;
  static bool ips;
  if (millis() >= timedInterval) {
    timedInterval = millis()+1000;
    Color = Color+1; 
    if (Color > 5){Color=0; ips=!ips;
     if (invertcheck) {gfx->invertDisplay(ips); gfx->setTextSize(2);
     gfx->fillRect(0, 0, 480, 20,BLACK);gfx->setTextColor(WHITE);Writeat(0,0, "INVERTING DISPLAY Colours?");} }
     gfx->setTextSize(4);
    switch (Color){
      //gfx->fillRect(0, 00, 480, 20,BLACK);gfx->setTextColor(WHITE);Writeat(0,0, "WHITE");
      case 0: gfx->fillRect(0, 20, 480, 460,WHITE);gfx->setTextColor(BLACK);Writeat(180,50,"WHITE");
      break;
      case 1: gfx->fillRect(0, 20, 480, 460,BLACK);;gfx->setTextColor(WHITE);Writeat(180,50,"BLACK");
      break;
      case 2: gfx->fillRect(0, 20, 480, 460,RED);;gfx->setTextColor(BLACK);Writeat(180,50,"RED");
      break;
      case 3: gfx->fillRect(0, 20, 480, 460,GREEN);;gfx->setTextColor(BLACK);Writeat(180,50,"GREEN");
      break;
      case 4: gfx->fillRect(0, 20, 480, 460,BLUE);;gfx->setTextColor(BLACK);Writeat(180,50,"BLUE");
      break;
    }
  }
}


void loop()
{ DisplayCheck(false);

  
}
