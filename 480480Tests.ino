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

TAMC_GT911 ts = TAMC_GT911(TOUCH_SDA, TOUCH_SCL, TOUCH_INT, TOUCH_RST, TOUCH_WIDTH, TOUCH_HEIGHT);




void setup() {
  Serial.begin(115200);
  ts.begin();
  ts.setRotation(ROTATION_NORMAL);

  Serial.println("Test of 4848 ST7701 drivers ");
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

void DisplayCheck(bool invertcheck) {
  static unsigned long timedInterval;
  static int Color;
  static bool ips;
  if (millis() >= timedInterval) {
    timedInterval = millis()+1000;
    Color = Color+1; 
    if (Color > 5){Color=0; ips=!ips;
     if (invertcheck) {gfx->invertDisplay(ips); gfx->setTextSize(2);
     gfx->fillRect(0, 0, 480, 40,BLACK);gfx->setTextColor(WHITE);Writeat(0,0, "INVERTING DISPLAY Colours?");} }
     gfx->setTextSize(4);
    switch (Color){
      //gfx->fillRect(0, 00, 480, 20,BLACK);gfx->setTextColor(WHITE);Writeat(0,0, "WHITE");
      case 0: gfx->fillRect(0, 40, 480, 440,WHITE);gfx->setTextColor(BLACK);Writeat(180,50,"WHITE");
      break;
      case 1: gfx->fillRect(0, 40, 480, 440,BLACK);;gfx->setTextColor(WHITE);Writeat(180,50,"BLACK");
      break;
      case 2: gfx->fillRect(0, 40, 480, 440,RED);;gfx->setTextColor(BLACK);Writeat(180,50,"RED");
      break;
      case 3: gfx->fillRect(0, 40, 480, 440,GREEN);;gfx->setTextColor(BLACK);Writeat(180,50,"GREEN");
      break;
      case 4: gfx->fillRect(0, 40, 480, 440,BLUE);;gfx->setTextColor(BLACK);Writeat(180,50,"BLUE");
      break;
    }
  }
}

bool actionrequired;

void loop() {
  DisplayCheck(false);
  actionrequired=touch_check(true);
  // ts.read();
  // if (ts.isTouched){
  //   //Serial.printf("Touches seen  [%i] (0)is X:%i Y:%i \n",ts.touches,ts.points[0].x,ts.points[0].y);
  //   for (int i=0; i<ts.touches; i++){
  //     Serial.printf("TOUCH  [%i] X:%i Y:%i size:%i \n",i+1,ts.points[i].x,ts.points[i].y,ts.points[i].size);
  //   }
  // }

}




 void TouchCheck(bool debug) {
  ts.read();
  if (ts.isTouched){
    for (int i=0; i<ts.touches; i++){
      if (debug) {
      Serial.printf("Touch [%i] X:%i Y:%i size:%i \n",i+1,ts.points[i].x,ts.points[i].y,ts.points[i].size);
      gfx->setTextSize(1); //8 is text height 1
      gfx->fillRect(0, i*8, 480,(i+1)*8,BLACK);
      gfx->setTextColor(WHITE);
      gfx->setCursor(0, i*8);
      gfx->printf("Touch [%i] X:%i Y:%i size:%i ",i+1,ts.points[i].x,ts.points[i].y,ts.points[i].size);
      }
    }
  }
}

bool touch_check(bool debug) {
    ts.read();
  if (ts.isTouched){
    for (int i=0; i<ts.touches; i++){
      if (debug) {
      Serial.printf("Touch [%i] X:%i Y:%i size:%i \n",i+1,ts.points[i].x,ts.points[i].y,ts.points[i].size);
      gfx->setTextSize(1); //8 is text height 1
      gfx->fillRect(0, i*8, 480,(i+1)*8,BLACK);
      gfx->setTextColor(WHITE);
      gfx->setCursor(0, i*8);
      gfx->printf("Touch [%i] X:%i Y:%i size:%i ",i+1,ts.points[i].x,ts.points[i].y,ts.points[i].size);
      }
    }
    return true;
  } else {
    return false;
  }
}

