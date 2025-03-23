#include <References/Pin.h>
#include "Motor.h"

Motor::Motor():
    IN1_PINS({PIN_MOTOR_L_IN1, PIN_MOTOR_R_IN1}),
    IN2_PINS({PIN_MOTOR_L_IN2, PIN_MOTOR_R_IN2}),
    ENA_PINS({PIN_MOTOR_L_ENA, PIN_MOTOR_R_ENA}),
    dirs({true, true})
{
    pinMode(IN1_PINS.left, OUTPUT);   pinMode(IN1_PINS.right, OUTPUT);
    pinMode(IN2_PINS.left, OUTPUT);   pinMode(IN2_PINS.right, OUTPUT);
    pinMode(ENA_PINS.left, OUTPUT);   pinMode(ENA_PINS.right, OUTPUT);
}

Motor::~Motor() {

}

int8_t Motor::cnvrt_sign(int16_t val) {
    // truelar 1, falselar 0 olarak yorumlanır
    return (val > 0) - (val < 0);
}

void Motor::set_dir(Pair<bool> new_dirs) {
    digitalWrite(this->IN1_PINS.left, new_dirs.left);
    digitalWrite(this->IN2_PINS.left, !new_dirs.left);

    digitalWrite(this->IN1_PINS.right, new_dirs.right);
    digitalWrite(this->IN2_PINS.right, !new_dirs.right);

    this->dirs.eq(new_dirs);
}

void Motor::set_pwm(Pair<int16_t> new_pwms) {
    Pair<int8_t> sign = {
        this->cnvrt_sign(new_pwms.left),
        this->cnvrt_sign(new_pwms.right)
    };

    Pair<bool> new_dir = this->get_dir();

    if (sign.left != 0)    new_dir.left = (sign.left > 0);
    if (sign.right != 0)   new_dir.right = (sign.right > 0);
    
    set_dir(new_dir);

    analogWrite(this->ENA_PINS.left, new_pwms.left * sign.left);
    analogWrite(this->ENA_PINS.left, new_pwms.right * sign.right);
}

Pair<bool> Motor::get_dir() {
    return Pair<bool>(this->dirs.left, this->dirs.right);
}