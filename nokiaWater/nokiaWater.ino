/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#include "bmpGraphics.h"
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

const int numMons = 3;
Monster *monster[numMons];

// atmosphere variables
int temp = 50;
int hum = 20;
bool light = true;

// counter for weather
int weatherCurrentCycle = 0;
const int weatherCycleLength = 20;


// the setup routine runs once when you press reset:
void setup() {     
  Serial.begin(9600); // why this? I just dont know  
  randomSeed(analogRead(4));

  Serial.println(2);
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
  weatherCurrentCycle++;

  //  update game objects
  for (int i=0; i<numMons; i++) { 
    if (monster[i]->isHatched) {
      monster[i]->update();

      if (weatherCurrentCycle == weatherCycleLength) {
        monster[i]->updateScoreForWeather(temp, hum, light);    
      }
    }
  }

  if (weatherCurrentCycle == weatherCycleLength) {
    weatherCurrentCycle = 0;
  }

  //  draw display from state
  draw();

  //  delay for frame rate
  delay(250);
}

void draw() {
  display.clearDisplay();

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
      if (monster[i]->isHatched && !monster[i]->isGhost) {
        drawMonster(i);
      }
    }
}

void drawMonster(int i) {
  drawMonsterBody(i);

  //  draw Stem
  if (monster[i]->showStem()) {
    display.drawBitmap(monster[i]->x + 2, monster[i]->y - 4, mon_stem_bmp, 3, 4, BLACK);
  } else { return; }

  //  draw leaves
  if (monster[i]->showLeaf()) {
    display.drawBitmap(monster[i]->x - 6, monster[i]->y - 5, mon_leaf_1_bmp, 24, 5, BLACK);
  } else { return; }

  //  draw segments
  if (monster[i]->stemSegments() > 0) {
    for (int j=0; j<monster[i]->stemSegments(); j++) { 
      display.drawBitmap(monster[i]->x, monster[i]->y - (6 + j*2), mon_seg_1_bmp, 8, 2, BLACK);
    }
  } else { return; }

  //  draw flower
  if (monster[i]->showFlower()) {
    display.drawBitmap(monster[i]->x - 2, monster[i]->y - 20, mon_flower_1_bmp, 11, 10, BLACK);
  } else if (monster[i]->showBulb()) { 
    Serial.println("draw bulb");
  }
}

void drawMonsterBody(int i) {
  // outline
  display.drawBitmap(monster[i]->x, monster[i]->y, mon_body_bmp, monster[i]->w, monster[i]->h, BLACK);
  // fill (to hide overlap between monsters)
  display.drawBitmap(monster[i]->x, monster[i]->y, mon_body_fill_bmp, monster[i]->w, monster[i]->h, WHITE);
}

//-----


bool getButtonState(String buttonKey) {
  return digitalRead(waterSwitch) == 1;
}

