#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include <Pair.h>

class Motor {
private:
    const Pair<uint8_t> IN1_PINS;
    const Pair<uint8_t> IN2_PINS; 
    const Pair<uint8_t> ENA_PINS; 

    Pair<bool> dirs;

    int8_t cnvrt_sign(int16_t val);
    void set_dir(Pair<bool> new_dirs);

public:
    Motor();
    ~Motor();
    void set_pwm(Pair<int16_t> new_pwms);
    Pair<bool> get_dir();
};

#endif