#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#include "bmpGraphics.h"
#include "monster.h"
#include "atmosphereController.h"
#include "rainController.h"

// Software SPI (slower updates, more flexible pin options):
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

// Monster intialiization
const int numMons = 3;
Monster *monster[numMons];

// State Controllers
AtmosphereController *atmosphere = new AtmosphereController();
RainController *rainMaker = new RainController();

// counter for weather
int weatherCurrentCycle = 0;
const int weatherCycleLength = 1;


// the setup routine runs once when you press reset:
void setup() {     
  Serial.begin(9600); // why this? I just dont know  
  randomSeed(analogRead(4));

  Serial.println(2);
  display.begin();
  display.setContrast(60);
  display.clearDisplay();
  //  end display init

  createInitialMonsters();
  
  // initialize the digital pin as an output.
  pinMode(15, INPUT); // hum down
  pinMode(16, INPUT); // hum down
  pinMode(17, INPUT); // light switch
  pinMode(18, INPUT); // new seed button
}

const int waterSwitch = 0;

void loop() {
  weatherCurrentCycle++;

  checkSeedDrop();

  //  update game objects
  for (int i=0; i<numMons; i++) { 
    if (monster[i]->isActive()) {
      monster[i]->update();

      if (weatherCurrentCycle == weatherCycleLength) {
        monster[i]->updateScoreForWeather(
          atmosphere->getTemp(), 
          atmosphere->getHum(), 
          atmosphere->showSun()
        );    
      }
    }
  }

  //  slower cycles for weather
  if (weatherCurrentCycle == weatherCycleLength) {
    atmosphere->update();
    weatherCurrentCycle = 0;
  }

  //  rain!!!!
  if (atmosphere->shouldRain()) {
    rainMaker->update();  
  }
  

  //  draw display from state
  draw();

  //  delay for frame rate
  delay(200);
}

/**
 * Handles readding a random plant type back when death
 */
int previousSeedBtnState = 0;
void checkSeedDrop() {
  bool hasDroppedSeed = false;
  if (digitalRead(18) && digitalRead(18) != previousSeedBtnState) { 
    for (int i=0; i<numMons; i++) { 
      if (!monster[i]->isActive() && !hasDroppedSeed) {
        monster[i]->setAsActive();
        hasDroppedSeed = true;
      }
    }
  }

  previousSeedBtnState = digitalRead(18);
}

void createInitialMonsters() {
  monster[0] = new Monster();
  monster[1] = new Monster();
  monster[2] = new Monster();
}

// ------------ DRAW

void draw() {
  display.clearDisplay();

  drawAvailableSeeds();

  drawRain();
  
  drawMonsters();

  drawWeatherMetrics();

  display.display();
}

//-----

/**
 * Draws the number of seeds that can be added from the queue
 */
void drawAvailableSeeds() {
  int drawnOpenSeeds = 1;
  
  for (int i=0; i<numMons; i++) { 
    if (!monster[i]->isActive()) {
      display.drawCircle(87 - (drawnOpenSeeds * 6), 41, 2, BLACK);
      drawnOpenSeeds++;
    }
  }
}

/**
 * Draws rain/snow
 */
void drawRain() {
  if (atmosphere->shouldRain()) {
    for (int i = 0; i<rainMaker->numRaindrops; i++) {
      if (atmosphere->getTemp() > atmosphere->freezingPoint) {
        display.drawLine(
          10 + (74/rainMaker->numRaindrops) * i, 
          rainMaker->rainDrops[i], 
          10 + (74/rainMaker->numRaindrops) * i, 
          rainMaker->rainDrops[i] + 2, 
          BLACK
        );
      } else {
        display.drawCircle(
          10 + (74/rainMaker->numRaindrops) * i,
          rainMaker->rainDrops[i], 
          1,
          BLACK
        );
      }
    }
  }
}

void drawWeatherMetrics() {
  //  draw celestial body
  drawSunMoon();
  
  //  draws divider line
  display.drawLine(8, 36, 84, 36, BLACK);
  
  //  draw temperature gauge
  display.drawRoundRect(3, 2, 3, 44, 1, BLACK);
  display.drawLine(4, 45, 4, 45 - ((atmosphere->getTemp() / 100.0) * 42), BLACK);

  //  display humidity
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(8, display.height() - 9);
  display.print(atmosphere->getHum());
  display.print("%");
}

void drawSunMoon() {
  if(atmosphere->showSun()) {
    display.drawCircle(84 - 5, 5, 3, BLACK);

    if (atmosphere->showShineAlt()) {
      display.drawPixel(84- 10, 2, BLACK);
      display.drawPixel(84- 9, 8, BLACK);
      display.drawPixel(84- 2, 9, BLACK);
    } else {
      display.drawPixel(84- 11, 5, BLACK);
      display.drawPixel(84- 5, 10, BLACK); 
    }
  } else {
    display.drawBitmap(84-7, 2, moon_bmp, 6, 7, BLACK);
  }
}

//-----

void drawMonsters() {
    for (int i=0; i<numMons; i++) { 
      drawMonster(i);
    }
}

void drawMonster(int i) {
  //  abort if not active game object
  if (!monster[i]->isActive()) { return; }
  //  draw ghost
  if (monster[i]->isGhost) { 
    drawGhost(i); 
    return; 
  }

  if (!monster[i]->isHatched) {
    drawSeed(i);
    return;
  }

  //  draws a living plant monster
  drawMonsterBody(i);

  if (monster[i]->isWilted()) {
    display.drawBitmap(monster[i]->x - 4, monster[i]->getY() + 2 - 10, mon_wilt_bmp, 15, 10, BLACK);
    return;
  }

  //  draw Stem
  if (monster[i]->showStem()) {
    display.drawBitmap(monster[i]->x + 2, monster[i]->getY() - 4, mon_stem_bmp, 3, 4, BLACK);
  } else { return; }

  //  draw leaves
  if (monster[i]->showLeaf()) {
    drawMonsterLeaf(i);
  } else { return; }

  //  draw segments
  if (monster[i]->stemSegments() > 0) {
    drawMonsterSegments(i);
  } else { return; }

  //  draw flower
  if (monster[i]->showFlower()) {
    drawMonsterFlower(i);
  } else if (monster[i]->showBulb()) { 
    drawMonsterBulb(i);
  }
}

/**
 * Draws the leaf for the monster
 */
void drawMonsterLeaf(int i) {
  if (monster[i]->type == 1) {
    display.drawBitmap(monster[i]->x - 6, monster[i]->getY() - 5, mon_leaf_1_bmp, 24, 5, BLACK);
  } else if (monster[i]->type == 2) {
    display.drawBitmap(monster[i]->x - 4, monster[i]->getY() - 5, mon_leaf_2_bmp, 15, 4, BLACK);
  } else if (monster[i]->type == 3) {
    display.drawBitmap(monster[i]->x, monster[i]->getY() - 2, mon_leaf_3_bmp, 7, 2, BLACK);
  } else if (monster[i]->type == 4) {
    display.drawBitmap(monster[i]->x - 4, monster[i]->getY() - 8, mon_leaf_4_bmp, 15, 13, BLACK);
  }
}

/**
 * Draws the flower for the monster
 */
void drawMonsterFlower(int i) {
  if (monster[i]->type == 1) {
    display.drawBitmap(monster[i]->x - 2, monster[i]->getY() - 20, mon_flower_1_bmp, 11, 10, BLACK);
    display.drawBitmap(monster[i]->x - 2, monster[i]->getY() - 20, mon_flower_fill_1_bmp, 8, 10, WHITE);
  } else if (monster[i]->type == 2) {
    display.drawBitmap(monster[i]->x - 2, monster[i]->getY() - 15, mon_flower_2_bmp, 11, 5, BLACK);
    display.drawPixel(monster[i]->x + 3, monster[i]->getY() -14, WHITE);  
  } else if (monster[i]->type == 3) {
    display.drawBitmap(monster[i]->x - 2, monster[i]->getY() - 16, mon_flower_3_bmp, 11, 6, BLACK);
    display.drawBitmap(monster[i]->x - 2, monster[i]->getY() - 16, mon_flower_fill_3_bmp, 11, 6, WHITE);
  } else if (monster[i]->type == 4) {
    display.drawBitmap(monster[i]->x - 2, monster[i]->getY() - 25, mon_flower_4_bmp, 11, 9, BLACK);
    display.drawBitmap(monster[i]->x - 2, monster[i]->getY() - 25, mon_flower_fill_4_bmp, 11, 9, WHITE);
  }
}

/**
 * Draws the bulb
 */
void drawMonsterBulb(int i) {
  int yOffset = 16;
  
  if (monster[i]->type == 4) {
    yOffset = 16 + 6;
  }

  display.drawBitmap(monster[i]->x + 1, monster[i]->getY() - yOffset, mon_bulb_bmp, 5, 6, BLACK);
}

/**
 * Draws the segments for the monster
 */
void drawMonsterSegments(int i) {
  int segHeight = 2;

  for (int j=0; j<monster[i]->stemSegments(); j++) { 
    if (monster[i]->type == 1) {
      display.drawBitmap(monster[i]->x, monster[i]->getY() - (6 + j*segHeight), mon_seg_1_bmp, 7, 2, BLACK);
    } else if (monster[i]->type == 2) {
      display.drawBitmap(monster[i]->x, monster[i]->getY() - (6 + j*segHeight), mon_seg_2_bmp, 7, 2, BLACK);
    } else if (monster[i]->type == 3) {
      display.drawBitmap(monster[i]->x + 2, monster[i]->getY() - (6 + j*segHeight), mon_seg_3_bmp, 7, 2, BLACK);
    } else if (monster[i]->type == 4) {
      segHeight = 4;
      display.drawBitmap(monster[i]->x - 1, monster[i]->getY() - (8 + j*segHeight), mon_seg_4_bmp, 9, 4, BLACK);
    }
  }
  
}

/**
 * Draws the seed for a monster
 */
void drawSeed(int i) {
  if (monster[i]->type == 1) {
    display.drawBitmap(monster[i]->x, monster[i]->getY(), mon_seed_1_bmp, 7, 6, BLACK);
  } else if (monster[i]->type == 2) {
    display.drawBitmap(monster[i]->x, monster[i]->getY(), mon_seed_2_bmp, 7, 6, BLACK);
  } else if (monster[i]->type == 3) {
    display.drawBitmap(monster[i]->x, monster[i]->getY(), mon_seed_3_bmp, 7, 6, BLACK);
  } else if (monster[i]->type == 4) {
    display.drawBitmap(monster[i]->x, monster[i]->getY(), mon_seed_4_bmp, 7, 6, BLACK);
  }
}

/**
 * Draws a monster in the ghost state
 */
void drawGhost(int i) {
  if (monster[i]->isMovingRight()) {
    display.drawBitmap(monster[i]->x, monster[i]->getY(), ghost_right_bmp, 8, 6, BLACK);
    display.drawBitmap(monster[i]->x + 3, monster[i]->getY() - 3, mon_stem_bmp, 3, 4, BLACK);
  } else {
    display.drawBitmap(monster[i]->x, monster[i]->getY(), ghost_left_bmp, 8, 6, BLACK);
    display.drawBitmap(monster[i]->x + 2, monster[i]->getY() - 3, mon_stem_bmp, 3, 4, BLACK);
  }
}

/**
 * Draws the monsters body, with the main part, eyes and legs as well as the 
 * state in the current animation cycle
 */
void drawMonsterBody(int i) {
  //  body outline
  display.drawRoundRect(monster[i]->x, monster[i]->getY(), monster[i]->w, monster[i]->h, 1, BLACK);
  display.fillRect(monster[i]->x+1, monster[i]->getY()+1, monster[i]->w-2, monster[i]->h-2, WHITE);

  //  draws legs
  if (monster[i]->drawLeftLeg()) {
    display.drawPixel(monster[i]->x + 1, monster[i]->getY() + monster[i]->h, BLACK);  
  }
  if (monster[i]->drawRightLeg()) {
    display.drawPixel(monster[i]->x + monster[i]->w - 2, monster[i]->getY() + monster[i]->h, BLACK);
  }

  //  draws eyes
  if (monster[i]->drawEyes()) {
    display.drawPixel(monster[i]->x + 2, monster[i]->getY() + 2, BLACK);
    display.drawPixel(monster[i]->x + 4, monster[i]->getY() + 2, BLACK);
  }
}

//-----




