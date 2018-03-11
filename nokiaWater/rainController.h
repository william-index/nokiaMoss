#include <SPI.h>

class RainController {
  public:
    const int numRaindrops = 12;
    int rainDrops[12] ={0,0,0,0,0,0,0,0,0,0,0,0};
  
  
  public:
    RainController ();

    void update() {
      updateDropPositions();
    }

    
  private:
    void updateDropPositions() {
      for (int i=0; i<numRaindrops; i++) {
        rainDrops[i]++;
        if (rainDrops[i] > 32) {
          rainDrops[i] = 0;
        }
      }
    }

    void setRainDropStarts() {
      for (int i=0; i<numRaindrops; i++) {
        rainDrops[i] = random(33);
      }
    }
};

RainController::RainController () {
 setRainDropStarts();
}
