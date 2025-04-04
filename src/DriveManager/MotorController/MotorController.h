#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include <Arduino.h>
#include <Pair.h>
#include <MyPID.h>
#include <CommInterfaces/EncoderStats.h>


#include "./Motor/Motor.h"
#include "./Encoder/Encoder.h"

class MotorController {
public:
	MotorController();
	~MotorController();

	// Encoder istatistiklerini yayınlayacak callback fonksiyonunu ayarlar.
	void set_enc_stats_pub_f(void (*func) (EncoderStats));

	// Motor kontrol döngüsünü çalıştırır.
	void spin_some();
	void set_wh_speeds(Pair<float> wh_speeds);
    

private:
	// Motor ve enkoder nesneleri
	Motor motor;
	Encoder encoder;

	// PID kontrolcüler (örneğin sol ve sağ için)
	Pair<MyPID> pid;

	// Son spin zamanı (millis)
	unsigned long last_spin;

	// Encoder istatistiklerini dışarıya aktarmak için callback fonksiyonu.
	std::function<void(EncoderStats)> enc_stats_pub_f;

	// Spin kontrol algoritması (gerektiğinde PID çıktısını motorlara uygular)
	bool spin_control();
};

#endif  // MOTOR_CONTROLLER_H
