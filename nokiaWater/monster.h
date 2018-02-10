#include <SPI.h>

class Monster {
  public:
    int x = 0;
    int targX = 0;
    int y = 48 - 6;
    int w = 8;
    int h = 5;
    const int moveFreq = 50;

  private:
    const int screenMax = 84 - w;
  
  
  public:
    Monster ();

    void update() {
      checkForNewX();
      
      if (targX > x) {
        stepRight();
      } else if(targX < x) {
        stepLeft(); 
      }
      
    }

    // checks and updates target x position
    void checkForNewX() {
      if (random(moveFreq) == 1) {
        targX = random(screenMax);
      }
    }

    void stepLeft() { x--; }
    void stepRight() { x++; }

};

Monster::Monster () {
 x = random(screenMax);
 targX = x;
}
