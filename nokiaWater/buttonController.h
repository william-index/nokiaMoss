#include <SPI.h>

class ButtonController {
  private:
    int btn1;
  
    void setPins(int btn1) {
      btn1 = btn1;
    }
  
  public:
    ButtonController (int);

    bool waterSwitch() {
      Serial.print(digitalRead(btn1));
      return digitalRead(btn1) == 1;
    }
};

ButtonController::ButtonController (int btn1) {
  Serial.print(btn1);
  pinMode(btn1, INPUT);   
}
