#include <SPI.h>

class Monster {
  public:
    int x = 10;
    int targX = 10;
    const int y = 30;
    const int w = 7;
    const int h = 5;
    
    // lifelike animation states
    bool isWalking = false;
    bool leftActiveStep = true;
    bool isBlinking = false;
    const int moveFreq = 50;

    

    // lifecycle state indicators
    bool isHatched = false;
    bool isGhost = false;

    // plant species indicators and definitions
    int type = 0;
    const int prehatch = 0;
    const int totalSeedTypes = 4;
    

  private:
    const int screenMax = 84 - w;

    // stats
    int score = 0;
    int rot = 0;
    int hauntTime = 0;
    const int maxHauntTime = 100;
    const int maxScore = 500;

    // progress towards death
    const int hatchScore = maxScore * 0.05;
    const int wiltToRot = 25;
    const int maxAllowedRot = 200;

  
  public:
    Monster ();

    void update() {
      updateMovement();
      calculateBlink();
    }

    bool drawLeftLeg() {
      return (!isWalking || leftActiveStep) && !isWilted();
    }

    bool drawRightLeg() {
      return (!isWalking || !leftActiveStep) && !isWilted();
    }

    bool drawEyes() {
      return isWalking || !isBlinking;
    }

    /**
     * Returns whether or not the monster is currently an active game object
     */
    bool isActive() {
      return type != prehatch;
    }

    void setAsActive() {
      type = random(totalSeedTypes + 1);
    }

    /**
     * Returns the base Y coordinate for upper left of plant body
     */
    int getY() {
      if (isGhost) {
        return y - 2;
      }
      
      return y + (isWilted() * 2);
    }

    /**
     * Is the monster currently walking right
     * - used for flipping ghosts
     */
    bool isMovingRight() {
      return targX > x;
    }

    /** 
     * Adjusts score and rot depending on weather conditions met
     */
    void updateScoreForWeather(int temp, int hum, bool light) {
      if (isGhost) { updateRotState(); return; }
      
      int metConditions = numWeatherConditionsMet(temp, hum, light);

      if (metConditions == 3) {
        if (!isHatched && score >= hatchScore) { isHatched = true; }
        if (rot > 0) { rot--; }
        if (rot == 0) { score++; }
      }
      if (!isHatched) { return; }

      if (metConditions == 1) { rot++; }
      if (metConditions == 0) { rot += 3; }

      if (score >= maxScore) {
        score = maxScore;
      }
      updateRotState();
    }

    /**
     * Determines is plant is in a wilted state
     */
    bool isWilted() {
      return rot >= wiltToRot;
    }

    bool showStem() {
      return score > maxScore * 0.1;
    }

    bool showLeaf() {
      return score > maxScore * 0.2;
    }

    int stemSegments() {
      if(score > maxScore * 0.5) { return 3; }
      if(score > maxScore * 0.4) { return 2; }
      if(score > maxScore * 0.3) { return 1; }
      return 0;
    }

    bool showFlower() {
      return score == maxScore;
    }

    bool showBulb() {
      return score > maxScore * 0.6;
    }

  private:
    void resetMonster() {
      type = 0;
      isHatched = false; // @TODO temp during dev
      isGhost = false;
      score = 0;
      rot = 0;
      hauntTime = 0;
    }
    
    /**
     * Checks and sets if it should blink now
     */
    void calculateBlink() {
      if (isBlinking) {
        isBlinking = false;
      }

      if (random(20) <= 1) {
        isBlinking = true;
      }
    }
    
    /**
     * Updates the status of if it is a ghost/dead or not
     *  also updates if the score should be reset
     */
    void updateRotState() {
      if (hauntTime >= maxHauntTime) {
        resetMonster();
        return;
      }
      if (rot > maxAllowedRot/2) {
        score = 1;
      }
      
      if (rot > maxAllowedRot) {
        isGhost = true;
        hauntTime++;
      }
    }
    
    /**
     * Gets the number of weather conditions met depending on plant type 
     * @param {int} temp Temperature between <10 and >90 (to account for noise and bad hardware
     * @return {int} number of weather conditions met 0—3
     */
    int numWeatherConditionsMet(int temp, int hum, bool light) {
      if (type == 1) {
        return conditionsMetForDeathStalk(temp, hum, light); 
      } else if (type == 2) {
        return conditionsMetForDesertWheat(temp, hum, light);
      } else if (type == 3) { 
        return conditionsMetForToweringShroom(temp, hum, light);
      } else if (type == 4){
        return conditionsMetForBlizzardySnowshroom(temp, hum, light);
      }

      return 0;
    }

    /**
     * Gets the number of weather conditions met for plant type 1 -death stalk
     * @return {int} number of weather conditions met 0—3
     */
    int conditionsMetForDeathStalk(int temp, int hum, bool light) {
      int metConditions = 0;
      if (temp > 60 && temp < 93) { metConditions++; }
      if (hum > 70 && hum < 95) { metConditions++; }
      if (light == 0) { metConditions++; }

      return metConditions;
    }

    /**
     * Gets the number of weather conditions met for plant type 2 -desert wheat
     * @return {int} number of weather conditions met 0—3
     */
    int conditionsMetForDesertWheat(int temp, int hum, bool light) {
      int metConditions = 0;
      if (temp > 88) { metConditions++; }
      if (hum < 10) { metConditions++; }
      if (light == 1) { metConditions++; }

      return metConditions;
    }


    /**
     * Gets the number of weather conditions met for plant type 3 - Towering Shroom
     * @return {int} number of weather conditions met 0—3
     */
    int conditionsMetForToweringShroom(int temp, int hum, bool light) {
      int metConditions = 0;
      if (temp > 26 && temp < 60) { metConditions++; }
      if (hum >= 85) { metConditions++; }
      if (light == 0) { metConditions++; }

      return metConditions;
    }

    /**
     * Gets the number of weather conditions met for plant type 4 - Blizzardy snowshroom
     * @return {int} number of weather conditions met 0—3
     */
    int conditionsMetForBlizzardySnowshroom(int temp, int hum, bool light) {
      int metConditions = 0;
      if (temp < 25) { metConditions++; }
      if (hum >= 95) { metConditions++; }
      if (light == 1) { metConditions++; }

      return metConditions;
    }

    // -- MOVEMENT
    /**
     * Updates the left right postion and calls inc/dec in x pos
     */
    void updateMovement() {
      if (!isHatched) { return; }
      
      checkForNewX();
      isWalking = false;
      
      if (targX > x) {
        stepRight();
        isWalking = true;
      } else if(targX < x) {
        stepLeft(); 
        isWalking = true;
      }

      leftActiveStep = !leftActiveStep;
    }
    
    /** 
     * Checks and updates target x position 
     *  - Wilted plants do not moved
     *  - Ghosts are always moving
     */
    void checkForNewX() {
      if (isWilted() && !isGhost) {
        targX = x;
      } else if (random(moveFreq) == 1 || (isGhost && targX == x)) {
        targX = random(10, screenMax);
      }
    }

    void stepLeft() { x--; }
    void stepRight() { x++; }

};

Monster::Monster () {
 x = random(screenMax-10) + 10;
 targX = x;
}
