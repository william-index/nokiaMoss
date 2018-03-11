#include <SPI.h>

class SeedSelectorController {
  public:
    bool isActive = false;
    int previousButtonState = 0;
    const int numSeeds = 4;

    int onSeed = 0;
  
  
  public:
    SeedSelectorController ();

    void updateActiveState() {
      handleButtonPress();
    }

    void update() {
      if (digitalRead(16)) {
        onSeed = (onSeed + 1) % numSeeds;
      }
//      digitalRead(15) // left
//      digitalRead(16) // right
    }

    int getCursorX() {
      return getSeedXPosition(onSeed) + 3;
    }

    int getCursorY() {
      return getSeedYPosition(onSeed) + 2;
    }

    int getSeedXPosition(int seedNum) {
      int x = (84/2 - 8)/2 + (84/2 * (seedNum%2));
      return x;
    }

    int getSeedYPosition(int seedNum) {
      if (seedNum > 1) {
        return 28;
      }
      return 12;
    }

  private:
    void handleButtonPress() {
      bool isPressed = digitalRead(18);
      if (isPressed && isPressed != previousButtonState) {
        isActive = !isActive;
      }

      previousButtonState = isPressed;
    }
};

SeedSelectorController::SeedSelectorController () {
}
