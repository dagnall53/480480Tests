/*******************************************************************************

From scratch build! 



*/
// * Start of Arduino_GFX setting

#include <Arduino_GFX_Library.h>
//select pin definitions for installed GFX version
#include "GUITIONESP32-S3-4848S040_GFX_131.h"

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

void DisplayCheck()
{
  static unsigned long timedInterval;
  static int Color;
  if (millis() >= timedInterval) {
    timedInterval = millis()+1000;
    Color = Color+1; 
    if (Color > 5){Color=0;}
    gfx->setTextSize(4);
    switch (Color){
      case 0: gfx->fillScreen(WHITE);gfx->setTextColor(BLACK);Writeat(180,50,"WHITE");
      break;
      case 1: gfx->fillScreen(BLACK);gfx->setTextColor(WHITE);Writeat(180,50,"BLACK");
      break;
      case 2: gfx->fillScreen(RED);gfx->setTextColor(BLACK);Writeat(180,50,"RED");
      break;
      case 3: gfx->fillScreen(GREEN);gfx->setTextColor(BLACK);Writeat(180,50,"GREEN");
      break;
      case 4: gfx->fillScreen(GREEN);gfx->setTextColor(BLACK);Writeat(180,50,"BLUE");
      break;
    }
  }
}


void loop()
{ DisplayCheck();

  
}
