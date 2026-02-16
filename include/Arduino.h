#ifndef ARDUINO_H
#define ARDUINO_H

#include <iostream>
#include <cmath>
#include <string>

inline void delay(int ms) {
    (void)ms;
}

struct SerialMock {
    void begin(int baud) { (void)baud; }
    void print(const char* s) { std::cout << s; }
    void print(float v) { std::cout << v; }
    void print(int v) { std::cout << v; }
    void println(const char* s) { std::cout << s << std::endl; }
    void println(float v) { std::cout << v << std::endl; }
    void println(int v) { std::cout << v << std::endl; }
    void println() { std::cout << std::endl; }
};

extern SerialMock Serial;

#endif
