
#include "Keyboard.h"
#include <Arduino_GFX_Library.h> // also includes Arduino etc, so variable names are understood

String top = "qQ1wW2eE3rR4tT5yY6uU7iI8oO9pP0";
String middle = "aA_sS/dD:fF;gG(hH)jJ$kK&lL@";
String bottom = "^^^zZ.xX,cC?vV!bB'nN\"mM-";
char TOP[] = "qQ1wW2eE3rR4tT5yY6uU7iI8oO9pP0";
char MIDDLE[] = "aA_sS/dD:fF;gG(hH)jJ$kK&lL@";
char BOTTOM[] = "^^^zZ.xX,cC?vV!bB'nN\"mM-";


bool change = false;
int sz = 3;  //Modulus of key definitions
int Keyboard_X =0;
int Keyboard_Y = 240;
int Key_Size = 2;


void DrawKey(int x, int rows_down, int width, String text ){
      gfx->drawRoundRect(Keyboard_X+(x*Key_Size), Keyboard_Y + (30*rows_down*Key_Size), width*Key_Size, 25*Key_Size, 2, RED);
      gfx->setCursor(Keyboard_X+(x*Key_Size)+(2*Key_Size), Keyboard_Y + (30*rows_down*Key_Size)+15*Key_Size);
      gfx->setTextColor(WHITE); 
      gfx->setTextSize(Key_Size);
      gfx->print(text);
      gfx->setTextColor(WHITE);//SIMPLE reset colour after function. 
} 



void keyboard(int type) {
  // draw the keyboard
  gfx->setTextColor(WHITE);// reset in case its has been changed!
  gfx->fillRect(Keyboard_X,Keyboard_Y,480-Keyboard_X,480-Keyboard_Y,BLACK);
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

  //gfx->drawRoundRect((55*Key_Size)+ Keyboard_X, Keyboard_Y + (90*Key_Size), 30*Key_Size, 25*Key_Size, 2, BLUE);
  //    gfx->setCursor((55*Key_Size)+ Keyboard_X, Keyboard_Y + (90*Key_Size)+ 15*Key_Size); gfx->print("CLR");
   DrawKey(55, 3,30,"CLR");
   DrawKey(155, 3,30,"DEL");
   DrawKey(190, 3,50,"DO");
  gfx->drawRoundRect((90*Key_Size)+ Keyboard_X, Keyboard_Y + (90*Key_Size), 60*Key_Size, 25*Key_Size, 1, WHITE);
  // gfx->drawRoundRect((155*Key_Size)+ Keyboard_X, Keyboard_Y + (90*Key_Size), 30*Key_Size, 25*Key_Size, 2, RED);
  //     gfx->setCursor((155*Key_Size)+ Keyboard_X, Keyboard_Y + (90*Key_Size)+ 15*Key_Size); gfx->print("DEL");

  gfx->setTextSize(1);
}
// example across, and which row.  drawKey((55*Key_Size)+ Keyboard_X,3rd row down, "H" ,BLUE); 30 == row height
     // drawkey(55, 3,"CLR",WHITE);

bool XYinBox(int x,int y, int h,int v,int width,int height){ //xy position in, xy top left width and height 
  return ((x > h && x < h+width) && (y > v && y < v+height));
}

/*void DrawKey(int x, int rows_down, int width, String text ){
      gfx->drawRoundRect(Keyboard_X+(x*Key_Size), Keyboard_Y + (30*rows_down*Key_Size), width*Key_Size, 25*Key_Size, 2, RED);
 
*/
bool XYinBox(int x, int y ,int Kx, int Krows_down, int Kwidth){ // use DrawKey type key setting
    int h=Keyboard_X+(Kx*Key_Size);
    int width=Kwidth*Key_Size;
    int v=Keyboard_Y + (30*Krows_down*Key_Size);
    int height=25*Key_Size;
  return XYinBox(x,y,h,v,width,height);
}

String key(int x, int y, int type) {

  // check which key is pressed
 // Serial.printf(" In KEY  %i %i",x,y);
  if ((y > Keyboard_Y) && (y < (Keyboard_Y+25*Key_Size))) { //a, Keyboard_Y, 20*Key_Size, 25*Key_Size,
    //top row  
   // Serial.printf(" In TOP ROW  Key_Size;(%i) x%i y%i \n",Key_Size,x,y);
    for (int z = 0; z < 10; z++) {
      int a = (Key_Size*((z * 4) + (20 * z) + 2)) + Keyboard_X;
      int b = a + (20*Key_Size);
     // Serial.printf(" Locations   %i to %i \n",a,b);
      if (x > a && x < b) {
        return String(top.charAt((z * sz) + type));
      }
    }
  }
 
  if ((y > (Keyboard_Y + (30*Key_Size))) && (y < (Keyboard_Y + (30*Key_Size)+25*Key_Size))) { //Keyboard_Y + (30*Key_Size), 20*Key_Size, 25*Key_Size, 1, WHITE);
    //middle row
      // Serial.printf(" In MIDDLE ROW   %i %i",x,y);
    for (int z = 0; z < 9; z++) {
      int a = Key_Size*((z * 4) + (20 * z) + 13) + Keyboard_X;
      int b = a + (20*Key_Size);
      if (x > a && x < b) {
        return String(middle.charAt((z * sz) + type));
      }
    }
  }

  if ((y > (Keyboard_Y + (60*Key_Size))) && (y < (Keyboard_Y + (60*Key_Size)+25*Key_Size))) {
    //bottom row
    for (int z = 0; z < 8; z++) {
      int a = Key_Size*((z * 4) + (20 * z) + 25) + Keyboard_X;
      int b = a + (20*Key_Size);

      if (x > a && x < b) {
        return String(bottom.charAt((z * sz) + type));
      }
    }
  }
  // new test for space bar  
  /*  gfx->drawRoundRect((55*Key_Size)+ Keyboard_X, Keyboard_Y + (90*Key_Size), 30*Key_Size, 25*Key_Size, 2, BLUE);
  gfx->drawRoundRect((90*Key_Size)+ Keyboard_X, Keyboard_Y + (90*Key_Size), 60*Key_Size, 25*Key_Size, 1, WHITE);
  gfx->drawRoundRect((155*Key_Size)+ Keyboard_X, Keyboard_Y + (90*Key_Size), 30*Key_Size, 25*Key_Size, 2, RED);
or    DrawKey(55, 3,30,"CLR");
   DrawKey(155, 3,30,"DEL");
   DrawKey(190, 3,50,"DO");


  */
  //if (XYinBox(x,y,(55*Key_Size)+ Keyboard_X, Keyboard_Y + (90*Key_Size), 30*Key_Size, 25*Key_Size ) ){ return "CLR";}
  if (XYinBox(x,y,55,3,30)){ return "CLR";}
  if (XYinBox(x,y,90,3,60)){ return " ";}
  if (XYinBox(x,y,155,3,30)){ return "DEL";}
  
  if (XYinBox(x,y,190,3,50)){ return "DO";}
  //if (XYinBox(x,y,(90*Key_Size)+ Keyboard_X, Keyboard_Y + (90*Key_Size), 60*Key_Size, 25*Key_Size ) ){ return " ";}
  //if (XYinBox(x,y,(155*Key_Size)+ Keyboard_X, Keyboard_Y + (90*Key_Size), 30*Key_Size, 25*Key_Size ) ){ return "DEL";}
  return ("");
}


bool KeyOver(int x, int y, char* Key, int type){ //char array version
  bool Keyfound=false;

   if ((y > Keyboard_Y) && (y < (Keyboard_Y+25*Key_Size))) { //a, Keyboard_Y, 20*Key_Size, 25*Key_Size,
    //top row  
   // Serial.printf(" In TOP ROW  Key_Size;(%i) x%i y%i \n",Key_Size,x,y);
    for (int z = 0; z < 10; z++) {
      int a = (Key_Size*((z * 4) + (20 * z) + 2)) + Keyboard_X;
      int b = a + (20*Key_Size);
      if (x > a && x < b) {strcpy(Key,String(TOP[(z * sz) + type]).c_str());Keyfound=true;}
    }
  }
 
  if ((y > (Keyboard_Y + (30*Key_Size))) && (y < (Keyboard_Y + (30*Key_Size)+25*Key_Size))) { //Keyboard_Y + (30*Key_Size), 20*Key_Size, 25*Key_Size, 1, WHITE);
    //middle row
      // Serial.printf(" In MIDDLE ROW   %i %i",x,y);
    for (int z = 0; z < 9; z++) {
      int a = Key_Size*((z * 4) + (20 * z) + 13) + Keyboard_X;
      int b = a + (20*Key_Size);
      if (x > a && x < b) {strcpy(Key,String(MIDDLE[(z * sz) + type]).c_str());Keyfound=true;}
    }
  }

  if ((y > (Keyboard_Y + (60*Key_Size))) && (y < (Keyboard_Y + (60*Key_Size)+25*Key_Size))) {
    //bottom row
    for (int z = 0; z < 8; z++) {
      int a = Key_Size*((z * 4) + (20 * z) + 25) + Keyboard_X;
      int b = a + (20*Key_Size);
      if (x > a && x < b) {strcpy(Key,String(BOTTOM[(z * sz) + type]).c_str());Keyfound=true;}
    }
  }
  if (XYinBox(x,y,55,3,30)){ strcpy(Key,"CLR");Keyfound=true;}
  if (XYinBox(x,y,90,3,60)){ strcpy(Key," ");Keyfound=true;}
  if (XYinBox(x,y,155,3,30)){strcpy(Key,"DEL");Keyfound=true;}
  
  if (XYinBox(x,y,190,3,50)){strcpy(Key,"ENT");Keyfound=true;}
 return Keyfound;
 }