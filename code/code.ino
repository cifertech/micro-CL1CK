#include <HID-Project.h>                   
#include <HID-Settings.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "animation.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define REVERSED false
#define NUM_LEDS 11                          
#define LED_PIN 12                            

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int left;
int right;

int val = 0;
int previousval = 0;
int val2 = 0;

const int shortcut1Pin = A5;
const int shortcut2Pin = A4;
const int shortcut3Pin = A3;
const int shortcut4Pin = A2;

bool shortcut4State = false;
bool shortcut1State = false;
bool shortcut2State = false;
bool shortcut3State = false;

void setup() {

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  display.clearDisplay();

  display.setRotation(2);

  pinMode(shortcut1Pin, INPUT_PULLUP);
  pinMode(shortcut2Pin, INPUT_PULLUP);
  pinMode(shortcut3Pin, INPUT_PULLUP);
  pinMode(shortcut4Pin, INPUT_PULLUP);
  
  BootKeyboard.begin();
  Consumer.begin();                         
  strip.begin();                            
  strip.show();                             
  delay(1000);                             

  for (int a = 0; a < 52; a++) {
    Consumer.write(MEDIA_VOLUME_DOWN);      
    delay(2);
  }
}

void loop() {

    bool shortcut1Pressed = (digitalRead(shortcut1Pin) == LOW);
    bool shortcut2Pressed = (digitalRead(shortcut2Pin) == LOW);
    bool shortcut3Pressed = (digitalRead(shortcut3Pin) == LOW);
    bool shortcut4Pressed = (digitalRead(shortcut4Pin) == LOW);


    if (shortcut1Pressed && !shortcut1State) {
      sendShortcut(0);
      shortcut1State = true;
    } else if (!shortcut1Pressed) {
      shortcut1State = false; 
    }

    if (shortcut2Pressed && !shortcut2State) {
      sendShortcut(1);
      shortcut2State = true;
    } else if (!shortcut2Pressed) {
      shortcut2State = false; 
    }

    if (shortcut3Pressed && !shortcut3State) {
      sendShortcut(2);
      shortcut3State = true;
    } else if (!shortcut3Pressed) {
      shortcut3State = false; 
    }

    if (shortcut4Pressed && !shortcut4State) {
      sendShortcut(3);
      shortcut4State = true;
    } else if (!shortcut4Pressed) {
      shortcut4State = false; 
    }
  
    
  val = analogRead(A0);                     
  val = map(val, 0, 1023, 0, 101);           

  if (REVERSED) {
    val = 101 - val;
  }

  if (abs(val - previousval) > 1) {         
    previousval = val;
    val /= 2;                               

    while (val2 < val) {
      Consumer.write(MEDIA_VOLUME_UP);       
      val2++;
      delay(2);
    }

    while (val2 > val) {
      Consumer.write(MEDIA_VOLUME_DOWN);     
      val2--;
      delay(2);
    }

    updateLEDs(val2);
  }

  delay(301); 
                        
}

void updateLEDs(int volumeLevel) {
  int numLedsOn = map(volumeLevel, 0, 50, 0, NUM_LEDS); 

  for (int i = 0; i < NUM_LEDS; i++) {
    if (i < numLedsOn) {
      strip.setPixelColor(NUM_LEDS - 1 - i, strip.Color(0, 5, 5));   
    } else {
      strip.setPixelColor(NUM_LEDS - 1 - i, strip.Color(0, 0, 0));     
    }
  }

  strip.show();  
}

void sendShortcut(int switchIndex) {
  switch (switchIndex) {
    case 0:
      BootKeyboard.press(KEY_LEFT_CTRL);
      BootKeyboard.press(KEY_LEFT_ALT);
      BootKeyboard.press('1');
      display.clearDisplay();
      display.drawBitmap(0,0,_leftpawontable, 128, 40, 1);
      display.display();
      delay(100);
      BootKeyboard.releaseAll();
      display.clearDisplay();
      display.drawBitmap(0,0,_pawsonair , 128, 40, 1);
      display.display();
      break;
      
    case 1:
      BootKeyboard.press(KEY_LEFT_CTRL);
      BootKeyboard.press(KEY_LEFT_ALT);
      BootKeyboard.press('2');
      display.clearDisplay();
      display.drawBitmap(0,0,_rightpawontable, 128, 40, 1);
      display.display();
      delay(100);
      BootKeyboard.releaseAll();
      display.clearDisplay();
      display.drawBitmap(0,0,_pawsonair , 128, 40, 1);
      display.display();
      break;
      
    case 2:
      BootKeyboard.press(KEY_RIGHT_GUI);
      BootKeyboard.press('m');
      display.clearDisplay();
      display.drawBitmap(0,0,_leftpawontable, 128, 40, 1);
      display.display();
      delay(100);
      BootKeyboard.releaseAll();
      display.clearDisplay();
      display.drawBitmap(0,0,_pawsonair , 128, 40, 1);
      display.display();
      break;
      
    case 3:
      BootKeyboard.press(KEY_LEFT_CTRL);
      BootKeyboard.press(KEY_LEFT_SHIFT);
      BootKeyboard.press('m');
      display.clearDisplay();
      display.drawBitmap(0,0,_leftpawontable, 128, 40, 1);
      display.display();
      delay(100);
      BootKeyboard.releaseAll();
      display.clearDisplay();
      display.drawBitmap(0,0,_pawsonair , 128, 40, 1);
      display.display();
      break;

  }
}
