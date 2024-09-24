#ifndef CAR_H_
#define CAR_H_
#include "Motor.h"
#include "Servo.h"
#include "UltraSound.h"

class Car {
  private:
    Motor *l;
    Motor *r;
    Servo *s;
    UltraSound *us;

    char data[128] = {0};
    // memset(data, 0, 128);
    int count = 0;

    void parse(char* data, int len);

  public:
    Car(Motor* l, Motor *r, Servo *s, UltraSound *us);
    // Car(int pin1Pos, int pin1Neg, int pin1Pwm, int pin2Pos, int pin2Neg, int pin2Pwm, int pinServo);

    void waitForHubMessages();
    
    void forward();
    void backward();
    void stop();
    // public void left();
    // public void right();
    void speedUp();
    void speedDown();
    void setSpeed(float speed);
      
    void left();
    void right();

    void faceTo(int angle);

    void getDistance();


};

#endif