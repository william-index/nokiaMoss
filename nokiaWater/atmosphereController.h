#include <SPI.h>

class AtmosphereController {
  public:
    int currentTemp = 45;
    int currentHum = 20;
  
  
  public:
    AtmosphereController ();

    void update() {
      updateTemp();
      updateHum();
    }

    int getTemp() {
      return currentTemp;
    }

    int getHum() {
      return currentHum;
    }

  private:
    void updateHum() {
      if (digitalRead(15) && currentHum > 0) {
        currentHum--;
      } else if (digitalRead(16) && currentHum < 100) {
        currentHum++;
      }
    }

    void updateTemp() {
      int targetTemp = (analogRead(A9) / 1024.0) * 100;
      
      if (currentTemp < targetTemp) {
        currentTemp++;
      } else if (currentTemp > targetTemp) {
        currentTemp--;
      }
    }
};

AtmosphereController::AtmosphereController () {
 
}
