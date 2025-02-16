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

TAMC_GT911 ts = TAMC_GT911(TOUCH_SDA, TOUCH_SCL, TOUCH_INT, TOUCH_RST, TOUCH_WIDTH, TOUCH_HEIGHT);
#include <EEPROM.h>
#include "fonts.h"
//**   structures for my variables (for saving) 
struct MySettings {
  int EpromKEY;  // to allow check for clean EEprom and no data stored change in the default will result in eeprom being reset
  int UDP_PORT;
  int Mode;
  bool UDP_ON;
  bool Serial_on;
  bool ESP_NOW_ON;
  uint8_t ListTextSize;  // fontsize for listed text (2 is readable, 1 gives more lines)// all off
  char ssid[16];
  char password[16];
  int DisplayPage;
};

MySettings Default_Settings = { 129, 2002, 1, false, true, true, 2, "N2K0183-proto", "12345678",0 };
MySettings Saved_Settings;
MySettings Current_Settings;

//*********** for keyboard*************
#include "Keyboard.h"

int caps = 0;

int text_height =16; //default?
int font_offset;
//String text = "";
enum swipe {none,left,right,up,down};
bool SwipeLeft,SwipeRight,SwipeUp,SwipeDown;

/// various screens 
#define maxscreens 3 //screens 0 1 2 3 





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
  // gfx->setTextColor(WHITE);
  // gfx->setTextSize(4); 
  // gfx->setCursor(180, 50);
  // gfx->println(F("START "));
  delay(500); // .5 seconds
  EEPROM_READ();  // setup and read saved variables into Saved_Settings

  Current_Settings = Saved_Settings;
  if (Current_Settings.EpromKEY != Default_Settings.EpromKEY) {
    Current_Settings = Default_Settings;
    EEPROM_WRITE();
  }    
  
  gfx->setTextColor(WHITE, BLACK);
//FreeSansBold10pt7b is 24 pixel?
//FreeMono8pt7b is 16 ?
//FreeSansBold24pt7b is 56?
  gfx->setFont(&FreeMono8pt7b);
  text_height=16;
  font_offset=text_height -2;  // lift slightly? 
   /// can this be set from fonts??
  dataline(1,Current_Settings, "Current");
    //dataline(4,Saved_Settings, "Saved");
  //dataline(6,Default_Settings, "Default");

  //setup keyboard
  keyboard(caps);
}



void DisplayCheck(bool invertcheck) {
  static unsigned long timedInterval;
  static unsigned long updatetiming;
   static unsigned long LoopTime;
  static int Color;
  static bool ips;
  LoopTime=millis()-updatetiming;
  updatetiming=millis();
  if (millis() >= timedInterval) {
    //***** Timed updates *******
    timedInterval = millis()+1000;
    //Serial.printf("Loop Timing %ims",LoopTime);
    ScreenShow(1,Current_Settings, "Current"); 
  

    if (invertcheck) {   
      //Serial.println(" Timed display check"); 
      Color = Color+1; 
      if (Color > 5){Color=0; ips=!ips;
                //gfx->invertDisplay(ips);}
                 }  
      gfx->fillRect(300,text_height, 180, text_height*2,BLACK);
      gfx->setTextColor(WHITE); 
      if (ips) {Writeat(310,text_height, "INVERTED");} 
      else {  Writeat(310,text_height, " Normal ");}
     
    switch (Color){
      //gfx->fillRect(0, 00, 480, 20,BLACK);gfx->setTextColor(WHITE);Writeat(0,0, "WHITE");
      case 0: gfx->fillRect(300, 60, 180, 60,WHITE);gfx->setTextColor(BLACK);Writeat(310,62,"WHITE");
      break;
      case 1: gfx->fillRect(300, 60, 180, 60,BLACK);;gfx->setTextColor(WHITE);Writeat(310,62,"BLACK");
      break;
      case 2: gfx->fillRect(300, 60, 180, 60,RED);;gfx->setTextColor(BLACK);Writeat(310,62,"RED");
      break;
      case 3: gfx->fillRect(300, 60, 180, 60,GREEN);;gfx->setTextColor(BLACK);Writeat(310,62,"GREEN");
      break;
      case 4: gfx->fillRect(300, 60, 180, 60,BLUE);;gfx->setTextColor(BLACK);Writeat(310,62,"BLUE");
      break;
    }
    }
  }
  
}

bool actionrequired = false;
String PressedKey;

void loop() {
  static int swipe;
  DisplayCheck(false);
  Use_Keyboard(Current_Settings.password,sizeof(Current_Settings.password));
  swipe=Touchswipe(Current_Settings.DisplayPage);
  if (swipe) {screenSelect(swipe); }
//Select screens 
}


uint8_t screenSelect(uint8_t swipe){
  static int ScreentoShow;
  
 switch (swipe) {
  case 2:
    ScreentoShow =ScreentoShow+1;
    if (ScreentoShow >= maxscreens){ScreentoShow=0;}
  break; 
  case 1:
    ScreentoShow =ScreentoShow-1;
    if (ScreentoShow <= 0 ){ScreentoShow=maxscreens;}
  break; 
 }
  Serial.printf(" Screen %i selected",ScreentoShow);
  return ScreentoShow;
}


//*********** Touch stuff ****************
void Use_Keyboard (char * DATA, int maxsize ) {
  static unsigned long lastkeypressed;
  static bool KeyPressUsed;
  static bool KeyCommand;
  static bool VariableChanged = false;
  char KEY[6];
  static char  Local_var[30];
  if  (!VariableChanged){strcpy( Local_var,DATA); WriteinBox(0,200,2,  Local_var);VariableChanged=true; } // get a local copy to modify it 
    ts.read();
    int st;
  //Serial.printf(" Pressure test %i \n",ts.points[0].size);
  if (!KeyPressUsed && (ts.isTouched) && (ts.points[0].size >35) && (KeyOver(ts.points[0].x, ts.points[0].y, KEY,caps))) 
   {
       KeyPressUsed =true; lastkeypressed=millis();
       KeyCommand=false;
       //Serial.printf(" Key test %s \n",KEY);
       if (!strcmp(KEY,"^")){caps=caps+1; if (caps>2) {caps=0;} //NB strcmp returns 0 if NO Difference, else position of non match characters
              keyboard(caps); KeyCommand=true;}
       if (!strcmp(KEY,"DEL")){  Local_var[strlen( Local_var)-1] = '\0'; // NOTE single inverted comma !!!
                                KeyCommand=true;}
       if (!strcmp(KEY,"CLR")){  Local_var[0]='\0';KeyCommand=true;}
      if (!strcmp(KEY,"rst")){ strcpy( Local_var,DATA); WriteinBox(0,200,2,  Local_var);KeyCommand=true;}
       if (!strcmp(KEY,"ENT")){strcpy(DATA, Local_var);
           Serial.printf("Updated was %s is  %s",DATA,  Local_var); 
           strcpy(DATA, Local_var); KeyCommand=true;VariableChanged=false;
           EEPROM_WRITE;   
           }    
       if (!KeyCommand){ //Serial.printf(" adding %s on end of variable<%s>\n",KEY,Local_var);
        strcat( Local_var,KEY);}
        WriteinBox(0,200,2,  Local_var);
    }
  if (!ts.isTouched && KeyPressUsed && (millis()> (250+lastkeypressed))){KeyPressUsed =false;} 

}


int Touchswipe(int movement){// returns 0 (none) 1=left 2=right 3=up 4=down
  static bool  swipecommandset;
  static unsigned long movinginterval;
  static unsigned long  lastswipesensed;
  static int lastx0,lasty0,lastx1,lasty1;
  uint8_t lastmovement = movement;
  uint8_t temp;
  int speed=20;
  temp=0;
  //Serial.printf("entering touchswipe\n");
  if (!ts.isTouched && !swipecommandset) {//only allow a swipe After finger is removed this when touch REMOVED
   if ((lastx0 - lastx1) >= speed ){temp=1;}
   if ((lastx1 - lastx0) >= speed){temp=2;}
   if ((lasty0 - lasty1) >= (speed/2)){temp=3;}  // up down harder to swipe 
   if ((lasty1 - lasty0) >= (speed/2)){temp=4;}
   if (lastmovement != temp) {
    //Serial.printf("lastx%i lasty%i  currentx%i currenty%i SWIPE %i \n",lastx1,lasty1,lastx0,lasty0,temp);
    movement=temp;
    lastswipesensed=millis();
    swipecommandset=true;
     return temp;}
   //Serial.printf("NOT Touched lastx%i lasty%i  currentx%i currenty%i SWIPE %i \n",lastx1,lasty1,lastx0,lasty0,temp);
  }
  else{    // only update movement memory at intervals whilst keypressed?
    if (millis()> movinginterval+50){
      movinginterval=millis();
      lastx1=lastx0;lasty1=lasty0;// needs deeper memory!
      lastx0=ts.points[0].x;
      lasty0=ts.points[0].y;
      }
    if ( swipecommandset && (millis()> (250+ lastswipesensed))){ swipecommandset =false;} //reset
    //Serial.printf(" Touched lastx%i lasty%i  currentx%i currenty%i SWIPE %i \n",lastx1,lasty1,lastx0,lasty0,temp);
  }
  return false ;
}


 void TouchValueShow(bool debug) {
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



//*********** EEPROM functions *********
void EEPROM_WRITE() {
  // save my current settings
  //dataline(1,Current_Settings, "EEPROM_save");
  Serial.println("SAVING EEPROM");
  EEPROM.put(0, Current_Settings);
  EEPROM.commit();
  delay(50);
}
void EEPROM_READ() {
  EEPROM.begin(512);
  Serial.println("READING EEPROM");
  EEPROM.get(0, Saved_Settings);
  //dataline(1,Saved_Settings, "EEPROM_Read");
}

//************** display housekeeping ************
void ScreenShow(int LINE,MySettings A, String Text) {
  gfx->setTextSize(1); 
  gfx->fillRect(0, (LINE)*text_height, 480,(LINE+1)*text_height,RED);
  gfx->setTextColor(WHITE);
  GFXPrintf(0, (LINE)*text_height,"%s: Mode<%d> Ser<%d> UDP<%d> UDP<%d>  ESP<%d> ", Text, A.Mode, A.Serial_on, A.UDP_PORT, A.UDP_ON, A.ESP_NOW_ON);
  gfx->fillRect(0, (LINE+1)*text_height, 480,(LINE+2)*text_height,RED);
  GFXPrintf(0, (LINE+1)*text_height,"Page[%i] SSID<%s> PWD<%s>",A.DisplayPage, A.ssid, A.password);
}

void dataline(int line, MySettings A, String Text) {
  ScreenShow(line,A,Text);
  Serial.printf("%d Dataline display %s: Mode<%d> Ser<%d> UDPPORT<%d> UDP<%d>  ESP<%d> \n ", A.EpromKEY, Text, A.Mode, A.Serial_on, A.UDP_PORT, A.UDP_ON, A.ESP_NOW_ON);
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
void WriteinBox(int h,int v, int size, const char* TEXT ){ //Write text in filled box at h,v (using fontoffset to use TOP LEFT of text convention)
  gfx->fillRect(h, v, 480,text_height*size,WHITE );
  gfx->setTextColor(BLACK);gfx->setTextSize(size);
  Writeat(h,v,size,TEXT);
  gfx->setTextSize(1);
  }



void Writeat(int h,int v, int size, const char* text){ //Write text at h,v (using fontoffset to use TOP LEFT of text convention)
  gfx->setCursor(h, v+ (size*font_offset)-(3*size));   // offset up/down for GFXFONTS that start at Bottom left. Standard fonts start at TOP LEFT
  gfx->println(text);
}
void Writeat(int h,int v,const char* text){
  Writeat(h,v, 1, text);
}


void GFXPrintf(int h,int v,const char* fmt, ...) {  //complete object type suitable for holding the information needed by the macros va_start, va_copy, va_arg, and va_end.
  static char msg[300] = { '\0' };        // used in message buildup
  va_list args;
  va_start(args, fmt);
  vsnprintf(msg, 128, fmt, args);
  va_end(args);
  int len = strlen(msg);
  Writeat(h,v,msg);
  
}



//********* Send Advice function - useful for messages can be switched on/off here for debugging 
void sendAdvice(String message) {  // just a general purpose advice send that makes sure it sends at 115200

    Serial.print(message);

}
void sendAdvicef(const char* fmt, ...) {  //complete object type suitable for holding the information needed by the macros va_start, va_copy, va_arg, and va_end.
  static char msg[300] = { '\0' };        // used in message buildup
  va_list args;
  va_start(args, fmt);
  vsnprintf(msg, 128, fmt, args);
  va_end(args);
  // add checksum?
  int len = strlen(msg);
  sendAdvice(msg);
  delay(10); // let it send!
}

//************ TIMING FUNCTIONS FOR TESTING PURPOSES ONLY ******************
//Note this is also an example of how useful Function overloading can be!! 
void EventTiming(String input ){
  EventTiming(input,1); // 1 should be ignored as this is start or stop! but will also give immediate print!
}

void EventTiming(String input, int number){// Event timing, Usage START, STOP , 'Descrption text'   Number waits for the Nth call before serial.printing results (Description text + results).  
  static unsigned long Start_time;
  static unsigned long timedInterval;
  static unsigned long _MaxInterval;
  static unsigned long SUMTotal;
  static int calls = 0;  static int reads = 0;
  long NOW=micros();
  if (input == "START") { Start_time = NOW; return ;}
  if (input == "STOP") {timedInterval= NOW- Start_time; SUMTotal=SUMTotal+timedInterval;
                       if (timedInterval >= _MaxInterval){_MaxInterval=timedInterval;}
                       reads++;  
                       return; }
  calls++; 
  if (calls < number){return;}
  if (reads>=2){ 

    if (calls >= 2) {Serial.print("\r\n TIMING ");Serial.print(input); Serial.print(" Using ("); Serial.print(reads);Serial.print(") Samples"); 
        Serial.print(" last: ");Serial.print(timedInterval);
        Serial.print("us Average : ");Serial.print(SUMTotal/reads);
        Serial.print("us  Max : ");Serial.print(_MaxInterval);Serial.println("uS");}
    else {Serial.print("\r\n TIMED ");Serial.print(input); Serial.print(" was :");Serial.print(timedInterval);Serial.println("uS");}
    _MaxInterval=0;SUMTotal=0;reads=0; calls=0;
 }
}


