#ifndef SERVO_H
#define SERVO_H

#include <iostream>

class Servo {
public:
    void attach(int pin) {
        _pin = pin;
        std::cout << "[Servo] attach pin=" << pin << std::endl;
    }
    void write(int angle) {
        _angle = angle;
        std::cout << "[Servo] pin=" << _pin << " angle=" << angle << std::endl;
    }
    int read() const { return _angle; }
private:
    int _pin = 0;
    int _angle = 90;
};

#endif
