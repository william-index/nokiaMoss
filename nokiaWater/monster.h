#include <SPI.h>

class Monster {
  public:
    int x = 0;
    int targX = 10;
    int y = 30;
    int w = 8;
    int h = 5;

    // 
    const int moveFreq = 50;
    const int maxAllowedRot = 9;

    // plant species indicator
    int type = 0;

    // lifecycle state indicators
    bool isHatched = true;
    bool isGhost = false;
    

  private:
    const int screenMax = 84 - w;

    // stats
    int score = 0;
    int rot = 0;
    int hauntTime = 0;
  
  public:
    Monster ();

    void update() {
      updateMovement();
    }

    /** 
     * Adjusts score and rot depending on weather conditions met
     */
    void updateScoreForWeather(int temp, int hum, bool light) {
      if (isGhost) { return; }
      
      int metConditions = numWeatherConditionsMet(temp, hum, light);

      if (metConditions == 3) {
        if (!isHatched) { isHatched = true; return; }
        if (rot > 0) { rot--; }

        score++;
      }
      if (!isHatched) { return; }
      

      if (metConditions == 1) { rot++; }
      if (metConditions == 0) { rot += 3; }

      updateIsGhost();
    }

    bool isWilted() {
      return false;
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
    /**
     * Updates the status of if it is a ghost/dead or not
     */
    void updateIsGhost() {
      if (rot > maxAllowedRot) {
        isGhost = true;
      }
    }
    
    /**
     * Gets the number of weather conditions met depending on plant type 
     * @return {int} number of weather conditions met 0—3
     */
    int numWeatherConditionsMet(int temp, int hum, bool light) {
      return 3;
    }

    // -- MOVEMENT
    /**
     * Updates the left right postion and calls inc/dec in x pos
     */
    void updateMovement() {
      checkForNewX();
      
      if (targX > x) {
        stepRight();
      } else if(targX < x) {
        stepLeft(); 
      }
    }
    
    /** 
     * checks and updates target x position 
     */
    void checkForNewX() {
      if (random(moveFreq) == 1) {
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
