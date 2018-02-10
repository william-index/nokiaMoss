/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#include "bmpGraphics.h"
#include "cloud.h"
#include "monster.h"

// Software SPI (slower updates, more flexible pin options):
// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

// Pin assignments
int led = 14;

// game objects
Cloud *cloud = new Cloud();

const int numMons = 3;
Monster *monster[numMons];


// the setup routine runs once when you press reset:
void setup() {     
  Serial.begin(9600); // why this? I just dont know  
  randomSeed(analogRead(4));

  Serial.print(2);
  display.begin();
  display.setContrast(60);
  display.clearDisplay();
  //  end init

  createInitialMonsters();
  
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT); 
  pinMode(0, INPUT); // Water switch
}

const int waterSwitch = 0;

void loop() {

  //  get water on switch
  if (getButtonState(waterSwitch)) {
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  }

  //  update game objects
  cloud->update();
  for (int i=0; i<numMons; i++) { monster[i]->update(); }

  //  draw display from state
  draw();

  //  delay for frame rate
  delay(150);
}

void draw() {
  display.clearDisplay();

  //  draws the cloud
  display.drawBitmap(cloud->x, cloud->y,  cloud_bmp, cloud->w, cloud->h, BLACK);

  //  draws the monsters
  drawMonsters();
  
  display.display();
}

//-----

void createInitialMonsters() {
  monster[0] = new Monster();
  monster[1] = new Monster();
  monster[2] = new Monster();
}

void drawMonsters() {
   for (int i=0; i<numMons; i++) { 
    // outline
    display.drawBitmap(monster[i]->x, monster[i]->y, mon_body_bmp, monster[i]->w, monster[i]->h, BLACK);
    // fill (to hide overlap between monsters)
    display.drawBitmap(monster[i]->x, monster[i]->y, mon_body_fill_bmp, monster[i]->w, monster[i]->h, WHITE);
   }
}

//-----


bool getButtonState(String buttonKey) {
  return digitalRead(waterSwitch) == 1;
}

