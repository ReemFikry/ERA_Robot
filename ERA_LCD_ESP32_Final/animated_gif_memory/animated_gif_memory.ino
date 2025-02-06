// Animated GIF with Round Display
//
// ESP32 40MHz SPI single frame rendering performance



#include <ESP32Servo.h>

#define VRX_PIN      36 // ESP32 pin GPIO36 (ADC0) connected to VRX pin
#define VRY_PIN      39 // ESP32 pin GPIO39 (ADC0) connected to VRY pin
#define SERVO_X_PIN  33 // ESP32 pin GPIO33 connected to Servo motor 1
#define SERVO_Y_PIN  26 // ESP32 pin GPIO26 connected to Servo motor 2

Servo xServo;  // create servo object to control a servo 1
Servo yServo;  // create servo object to control a servo 2

#include <SPI.h>
#include <TFT_eSPI.h>     // Install this library with the Arduino IDE Library Manager
                          // Don't forget to configure the driver for the display!
                          
#include <AnimatedGIF.h>  // Install this library with the Arduino IDE Library Manager
AnimatedGIF gif;

// Examples images
#include "images/hyperspace.h"
#include "images/nostromo.h"
#include "images/hud_1.h"
#include "images/hud_2.h"
#include "images/hud_5.h"
#include "images/hud_6.h"
#include "images/hud_7.h"
#include "images/darthvader.h"
#include "images/x_wing.h"
#include "images/colortest.h"
#include "images/era_black.h"
#include "images/era_white.h"

// Uncomment the image to display
// You could run out of program memory if you load more than one image
// You can also increase the program memory by changing the partition of the ESP32
// See this tutorial to increase program memory: https://youtu.be/NTbiKssK8_8
//
// #define GIF_IMAGE colortest
// #define GIF_IMAGE hyperspace
// #define GIF_IMAGE nostromo
// #define GIF_IMAGE darthvader
// #define GIF_IMAGE hud_1
// #define GIF_IMAGE hud_2
// #define GIF_IMAGE hud_5
// #define GIF_IMAGE hud_6
// #define GIF_IMAGE hud_7
// #define GIF_IMAGE x_wing
#define GIF_IMAGE ERA_Black_Sclera
// #define GIF_IMAGE ERA_White_Sclera

TFT_eSPI tft = TFT_eSPI();
void setup() {
  Serial.begin(115200);
  tft.begin();
  tft.setRotation(1);     // Adjust Rotation of your screen (0-3)
  tft.fillScreen(TFT_BLACK);
  gif.begin(BIG_ENDIAN_PIXELS);
  xServo.attach(SERVO_X_PIN);
  yServo.attach(SERVO_Y_PIN);
  xServo.write(70);

}


unsigned long currentMillis=0;
unsigned long PreviousGifMillis=0;
const unsigned long GifInterval=5000;

unsigned long ServoPreviousMillis=0;
unsigned long ServoInterval;



void loop()
{
  currentMillis=millis();
  PlayGif();
  ServoControl();
}

void PlayGif(){
   if (currentMillis - PreviousGifMillis >= GifInterval){
    PreviousGifMillis=currentMillis;
    if (gif.open((uint8_t *)GIF_IMAGE, sizeof(GIF_IMAGE), GIFDraw))
    {
      Serial.printf("Successfully opened GIF; Canvas size = %d x %d\n", gif.getCanvasWidth(), gif.getCanvasHeight());
      tft.startWrite();
      while (gif.playFrame(true, NULL))
      {
        yield();
      }
      gif.close();
      tft.endWrite(); 
    }
  }
}


void ServoControl(){
  ServoInterval = random(3000,10000);
  if (currentMillis - ServoPreviousMillis >= ServoInterval){
    ServoPreviousMillis=currentMillis;

    int RandNum = random(1,6);
    if (RandNum == 1){
      xServo.write(10);
      Serial.println("Head to side");
    }
    else if (RandNum == 2){
      xServo.write(160);
      Serial.println("Head to side");
    }
    else if (RandNum == 3){
      yServo.write(30);
      Serial.println("Head UP");
      
    }
    // else if (RandNum == 4){
    //   yServo.write(120);
    //   Serial.println("Head up or down");

    // }
    else if (RandNum == 4){
      xServo.write(85);     //Leveled
      Serial.println("Servo X leveled");
    }
    else if (RandNum == 5){
      yServo.write(65);     //Leveled
      Serial.println("Servo Y leveled");
    }

    
  }
}

