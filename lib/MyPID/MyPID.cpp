#include "MyPID.h"

MyPID::MyPID(float kp, float ki, float kd) : kp(kp), ki(ki), kd(kd), sum(0.0f), pre(0.0f), goal(0.0f) {
    // Yapıcı: Parametreler atanır, hata toplamı ve önceki hata sıfırlanır.
}

MyPID::~MyPID() {
    // Yıkıcı: Dinamik bellek kullanımı yok.
}

void MyPID::set_goal(float new_goal) {
    goal = new_goal;
    // Hedef değiştirildiğinde toplam hata ve önceki hata sıfırlanabilir.
    sum = 0.0f;
    pre = 0.0f;
}

float MyPID::get_cmd(float feedback) {
    // Hata hesaplanır.
    float error = goal - feedback;
    
    // Hata integrali güncellenir.
    sum += error;
    
    // Hata türevi hesaplanır.
    float derivative = error - pre;
    
    // Önceki hata güncellenir.
    pre = error;
    
    // PID kontrol sinyali hesaplanır.
    float cmd = kp * error + ki * sum + kd * derivative;
    return cmd;
}
