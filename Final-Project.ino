/***
* Climbing Wall Matrix
* This Code is for an Arduino Mega that is connected to a: 
* 1. Touchscreen Sheild TFT 320x480 3.5"
* 2. WS2811 Addressable light string. 
* 
* Notes. For this project there were not any light strings with long enough spacing betwween lights so for every light on the wall there is a light behind the 
* wall that is not used. 
*
*/

   #if 1

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#ifndef PSTR
 #define PSTR // Make Arduino Due happy
#endif
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <TouchScreen.h>
#define MINPRESSURE 200
#define MAXPRESSURE 1000
using namespace std;

#define PIN 45
// ALL Touch panels and wiring is DIFFERENT
// copy-paste results from TouchScreen_Calibr_native.ino

const int XP=8,XM=A2,YP=A3,YM=9; //320x480 ID=0x9486

const int TS_LEFT=913,TS_RT=140,TS_TOP=91,TS_BOT=957;




TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

Adafruit_GFX_Button on_btn, off_btn, new_btn,btn1 ,btn2 ,btn3 ,btn4 ,btn5 ,btn6 ,btn7 ,btn8 ,btn9 ,btn10 ,btn11 ,btn12 ,btn13 ,btn14 ,btn15 ,btn16 ,btn17 ,btn18 ,btn19 ,btn20 ,btn21 ,btn22 ,btn23 ,btn24 ,btn25 ,btn26 ,btn27 ,btn28 ,btn29 ,btn30 ,btn31 ,btn32 ,btn33 ,btn34 ,btn35 ,btn36 ,btn37 ,btn38 ,btn39 ,btn40 ,btn41 ,btn42 ,btn43 ,btn44 ,btn45 ,btn46 ,btn47 ,btn48 ,btn49 ,btn50;
char  * charNumbers[52] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "50"};
int pixel_x, pixel_y;     //Touch_getXY() updates global vars
bool Touch_getXY(void)
{
    TSPoint p = ts.getPoint();
    pinMode(YP, OUTPUT);      //restore shared pins
    pinMode(XM, OUTPUT);
    digitalWrite(YP, HIGH);   //because TFT control pins
    digitalWrite(XM, HIGH);
    bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
    if (pressed) {
          pixel_x = map(p.x, TS_LEFT, TS_RT, 0, 320);
          pixel_y = map(p.y, TS_TOP, TS_BOT, 0, 480);
    }
    return pressed;
}

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define MY_GREEN   0xFD00
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define HEIGHT 47
#define WIDTH  66
#define START  26
#define BTN_WIDTH 53
#define BTN_HEIGHT 39

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(10, 10, PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
  NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
  NEO_GRB         + NEO_KHZ800);

// bool btn.contains(int x, int y, inutton_number) {
Adafruit_GFX_Button buttons[50]; // buttons
int lightColor[50] = {0};      // set the matrix to off // 0 off, 1 magenta(cyan), 2 red(or whatever looks good 0

void setup(void)
{

  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(255);

    Serial.begin(9600);
    uint16_t ID = tft.readID();
    Serial.print("TFT ID = 0x");
    Serial.println(ID, HEX);
    Serial.println("Calibrate for your Touch Panel");
    Serial.print(tft.width());
    Serial.print(tft.height());
    if (ID == 0xD3D3) ID = 0x9486; // write-only shield
    tft.begin(ID);
    tft.setRotation(2);            //PORTRAIT
    tft.fillScreen(BLACK);
     // ( ? x,y, width, height, border, WORD-COLOR, FILL, words,font_size)
  // for moving columns
  int x = 0;
  // setup the buttons
  for (int i = 0; i <= 49; i++)
  {
     if (i != 0 && (i % 5) == 0) {
       x++;
     }
     buttons[i].initButton(&tft,  START  + WIDTH * (i % 5) , START + (HEIGHT * x), BTN_WIDTH, BTN_HEIGHT, WHITE, WHITE, BLACK, charNumbers[i] , 2);
    
        
  }
  // not sure why we have to set these to false. 
  for (int i = 0; i <= 49; ++i)
  {
     buttons[i].drawButton(true);
  }

}



int color = 0;
void loop(void)
{

  matrix.fillScreen(0);
  matrix.setCursor(0, 0);

    bool down = Touch_getXY();
// Error Checking
//    if(down) {
//      Serial.print("x: ");
//      Serial.print(pixel_x);
//      Serial.print("\n");
//      Serial.print("Y: ");
//      Serial.print(pixel_y);
//      Serial.print("\n");
//
//    }
//    

    int y = 0;
    for (int i = 0; i <= 49; i++)
    {

       buttons[i].press(down && buttons[i].contains(pixel_x, pixel_y)); 
       // How to move to next row.
       if (i != 0 && (i % 5) == 0) {
         y++;
       }
       if (buttons[i].justPressed())  {
        if (lightColor[i] == 0) {
          lightColor[i] = 1;
          buttons[i].initButton(&tft,  START  + WIDTH * (i % 5) , START + (HEIGHT * y), BTN_WIDTH, BTN_HEIGHT, WHITE, BLACK, CYAN, charNumbers[i] , 2);
        }else if ( lightColor[i] == 1) {
          lightColor[i] = 2;
          buttons[i].initButton(&tft,  START  + WIDTH * (i % 5) , START + (HEIGHT * y), BTN_WIDTH, BTN_HEIGHT, WHITE, BLACK, GREEN, charNumbers[i] , 2);
        }
        else {
          buttons[i].initButton(&tft,  START  + WIDTH * (i % 5) , START + (HEIGHT * y), BTN_WIDTH, BTN_HEIGHT, WHITE, WHITE, BLACK, charNumbers[i] , 2);
          lightColor[i] = 0;
        }
        buttons[i].drawButton(true);
       }
       // Code to allow more than one color to be shown on the wall
       if (lightColor[i] == 1) {
        
        if ( y % 2 == 0) {
          matrix.drawPixel((i * 2 % 10) +1,y , 0xF81F); 
        }
        else {
          matrix.drawPixel(i * 2 % 10,y , 0xF81F); 
        }
        
       }
       else if (lightColor[i] == 2) {
          if ( y % 2 == 0) {
            matrix.drawPixel((i * 2 % 10) +1,y , MY_GREEN); 
          }
          else {
            matrix.drawPixel(i * 2 % 10,y , MY_GREEN); 
          }
       }
          
    }


matrix.show();
delay(50);

}
#endif
