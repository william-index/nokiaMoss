#include <SPI.h>

class Cloud {
  public:
    int x = 0;
    int y = 0;
    int w = 16;
    int h = 7;
  
  
  public:
    Cloud ();

    void update() {
      x = analogRead(A9) / 1024.0 * (84 - 16);
    }

//    bool waterSwitch() {
//      Serial.print(digitalRead(btn1));
//      return digitalRead(btn1) == 1;
//    }
};

Cloud::Cloud () {
 
}
