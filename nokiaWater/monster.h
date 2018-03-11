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
    bool isHatched = true;
    bool isGhost = false;

    // plant species indicators and definitions
    int type = 0;
    const int prehatch = 0;
    

  private:
    const int screenMax = 84 - w;

    // stats
    int score = 0;
    int rot = 0;
    int hauntTime = 0;
    const int maxHauntTime = 20;

    // progress towards death
    const int wiltToRot = 15;
    const int maxAllowedRot = 20;
  
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
        if (!isHatched) { isHatched = true; return; }
        if (rot > 0) { rot--; }

        score++;
      }
      if (!isHatched) { return; }
      

      if (metConditions == 1) { rot++; }
      if (metConditions == 0) { rot += 3; }

      updateRotState();
    }

    /**
     * Determines is plant is in a wilted state
     */
    bool isWilted() {
      return rot >= wiltToRot;
    }

    bool showStem() {
      return true;
    }

    bool showLeaf() {
      return true;
    }

    int stemSegments() {
      return 3;
    }

    bool showFlower() {
      return true; 
    }

    bool showBulb() {
      return false;
    }

  private:
    void resetMonster() {
      type = 0;
      isHatched = false;
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
     * @return {int} number of weather conditions met 0—3
     */
    int numWeatherConditionsMet(int temp, int hum, bool light) {
      return 1;
    }

    // -- MOVEMENT
    /**
     * Updates the left right postion and calls inc/dec in x pos
     */
    void updateMovement() {
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
 x = random(screenMax);
 targX = x;
}
