/*******************************************************************************

From scratch build! 
 for keyboard  trying to use elements of https://github.com/fbiego/esp32-touch-keyboard/tree/main



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
#include <EEPROM.h>
#include "fonts.h"
//**   structures for my variables (for saving) 
struct MySettings {
  int EpromKEY;  // to allow check for clean EEprom and no data stored
  int UDP_PORT;
  int Mode;
  bool UDP_ON;
  bool Serial_on;
  bool ESP_NOW_ON;
  uint8_t ListTextSize;  // fontsize for listed text (2 is readable, 1 gives more lines)// all off
  char ssid[16];
  char password[16];
};

MySettings Default_Settings = { 127, 2002, 1, false, true, true, 2, "N2K0183-proto", "12345678" };
MySettings Saved_Settings;
MySettings Current_Settings;

//*********** for keyboard*************
String top = "qQ1wW2eE3rR4tT5yY6uU7iI8oO9pP0";
String middle = "aA_sS/dD:fF;gG(hH)jJ$kK&lL@";
String bottom = "^^ zZ.xX,cC?vV!bB'nN\"mM-";

int caps = 0;
bool change = false;
int sz = 3;  //NOT size
int Keyboard_X =0;
int Keyboard_Y = 240;
int Key_Size = 2;
int text_height =10; //default?
String text = "";


void setup() {
  Serial.begin(115200);
  ts.begin();
  ts.setRotation(ROTATION_INVERTED);

  Serial.println("Test of 4848 ST7701 drivers ");
 #ifdef GFX_BL
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);
#endif
  // Init Display
  gfx->begin();
  //if GFX> 1.3.1 try and do this as the invert colours write 21h or 20h to 0Dh has been lost from the structure!
  //gfx->invertDisplay(false);
  gfx->fillScreen(BLACK);

#ifdef GFX_BL
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);
#endif
  gfx->setTextColor(WHITE);
  gfx->setTextSize(4); 
  gfx->setCursor(180, 50);
  gfx->println(F("START "));
  delay(500); // .5 seconds
  EEPROM_READ();  // setup and read saved variables into Saved_Settings
  //dataline(Saved_Settings, "Saved");
  //dataline(Default_Settings, "Default");
  Current_Settings = Saved_Settings;
  if (Current_Settings.EpromKEY != 127) {
    Current_Settings = Default_Settings;
    EEPROM_WRITE();
  }
  dataline(Current_Settings, "Current");

  //setup keyboard
    gfx->setTextColor(WHITE, BLACK);
  gfx->setFont(&FreeMono8pt7b);

  gfx->setCursor(30, 80);
  gfx->print(text + "_");

  keyboard(caps);




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
  //DisplayCheck(false);
  actionrequired=touch_check(true);


}



//*********** Touch stuff ****************
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
      gfx->fillRect(0, (i+1)*text_height, 480,(i+2)*text_height,BLACK);
      gfx->setTextColor(WHITE);
      gfx->setCursor(0, (i+1)*text_height);
      gfx->printf("Touch [%i] X:%i Y:%i size:%i ",i+1,ts.points[i].x,ts.points[i].y,ts.points[i].size);
      }
    }
    return true;
  } else {
    return false;
  }
}

//*********** EEPROM functions *********
void EEPROM_WRITE() {
  // save my current settings
  //dataline(Current_Settings, "EEPROM_save");
  Serial.println("SAVING EEPROM");
  EEPROM.put(0, Current_Settings);
  EEPROM.commit();
  delay(50);
}
void EEPROM_READ() {
  EEPROM.begin(512);
  Serial.println("READING EEPROM");
  EEPROM.get(0, Saved_Settings);
  //dataline(Saved_Settings, "EEPROM_Read");
}
void dataline(MySettings A, String Text) {

  int i =0; // line to start print on for now!
  //gfx->fillRect(0, TOP_FIXED_AREA, XMAX, YMAX - TOP_FIXED_AREA, TFT_BLACK);
      gfx->setTextSize(1); //8 is text height 1
      gfx->fillRect(0, (i+1)*text_height, 480,(i+2)*text_height,BLACK);
      gfx->setTextColor(WHITE);
      gfx->setCursor(0, i*text_height);
  gfx->printf("%d, %d SER<%d>", A.EpromKEY, A.Mode, A.Serial_on);
  gfx->setTextColor(BLUE);
  gfx->printf("UDP<%d><%d> ", A.UDP_PORT, A.UDP_ON);
  gfx->setTextColor(GREEN);
  gfx->printf("ESP<%d>", A.ESP_NOW_ON);
  gfx->setTextColor(WHITE);  // reset to initial state

  Serial.printf("%d Dataline display %s: Mode<%d> Ser<%d> UDPPORT<%d> UDP<%d>  ESP<%d> ", A.EpromKEY, Text, A.Mode, A.Serial_on, A.UDP_PORT, A.UDP_ON, A.ESP_NOW_ON);
  Serial.print("SSID <");
  Serial.print(A.ssid);
  Serial.print(">  Password <");
  Serial.print(A.password);
  Serial.println("> ");
}
boolean CompStruct(MySettings A, MySettings B) {  // does not check ssid and password
  bool same = false;
  // have to check each variable individually
  if (A.EpromKEY == B.EpromKEY) { same = true; }
  if (A.UDP_PORT == B.UDP_PORT) { same = true; }
  if (A.UDP_ON == B.UDP_ON) { same = true; }
  if (A.ESP_NOW_ON == B.ESP_NOW_ON) { same = true; }
  if (A.Serial_on == B.Serial_on) { same = true; }
  if (A.Mode == B.Mode) { same = true; }
  if (A.ListTextSize == B.ListTextSize) { same = true; }
  return same;
}


void keyboard(int type) {
  // draw the keyboard
  for (int x = 0; x < 10; x++) {
    int a = Key_Size*((x * 4) + (20 * x) + 2) + Keyboard_X;
    gfx->drawRoundRect(a, Keyboard_Y, 20*Key_Size, 25*Key_Size, 1, WHITE);
    gfx->setCursor(a + 5*Key_Size, Keyboard_Y + 15*Key_Size);
    gfx->setTextSize(Key_Size);
    gfx->print(top.charAt((x * sz) + type));
  }

  for (int x = 0; x < 9; x++) {
    int a = Key_Size*((x * 4) + (20 * x) + 13) + Keyboard_X;
    gfx->drawRoundRect(a, Keyboard_Y + (30*Key_Size), 20*Key_Size, 25*Key_Size, 1, WHITE);
    gfx->setCursor(a + 5*Key_Size, Keyboard_Y + (30*Key_Size) + 15*Key_Size);
    gfx->setTextSize(Key_Size);
    gfx->print(middle.charAt((x * sz) + type));
  }

  for (int x = 0; x < 8; x++) {
    int a = Key_Size*((x * 4) + (20 * x) + 25) + Keyboard_X;
    gfx->drawRoundRect(a, Keyboard_Y + (60*Key_Size), 20*Key_Size, 25*Key_Size, 1, x == 0 ? GREEN : WHITE);
    gfx->setCursor(a + 5*Key_Size, Keyboard_Y + (60*Key_Size) + 15*Key_Size);
    gfx->setTextSize(Key_Size);
    gfx->print(bottom.charAt((x * sz) + type));
  }

  gfx->drawRoundRect((55*Key_Size)+ Keyboard_X, Keyboard_Y + (90*Key_Size), 30*Key_Size, 25*Key_Size, 1, BLUE);
  gfx->drawRoundRect((90*Key_Size)+ Keyboard_X, Keyboard_Y + (90*Key_Size), 60*Key_Size, 25*Key_Size, 1, WHITE);
  gfx->drawRoundRect((155*Key_Size)+ Keyboard_X, Keyboard_Y + (90*Key_Size), 30*Key_Size, 25*Key_Size, 1, RED);
  gfx->setTextSize(1);
}

