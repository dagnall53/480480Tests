 
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_
#include <Arduino_GFX_Library.h>

extern Arduino_ST7701_RGBPanel *gfx ;  // declare the gfx structure so I can use GFX commands in Keyboard.cpp


void keyboard(int type);
void DrawKey(int x, int rows_down, int width, String text );
bool KeyOver(int x, int y, char * Key,int type);
bool XYinBox(int x,int y, int h,int v,int width,int height);


String key(int x, int y, int type);



#endif