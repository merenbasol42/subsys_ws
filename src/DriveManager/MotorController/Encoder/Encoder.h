#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>
#include <Pair.h>
#include <CommInterfaces/EncoderStats.h>

class Encoder {
public:
	// Statik pin tanımlamaları; bu değerleri ihtiyacınıza göre ayarlayabilirsiniz.
	static const Pair<uint8_t> A_PIN;
	static const Pair<uint8_t> B_PIN;
	
	// Sabit özellikler: Rezolüsyon ve yarıçap değerleri, örnek olarak atanmıştır.
	const Pair<uint8_t> RES;
	const Pair<float> RADIUS;
	const Pair<float> DAP_FACT;  // Her teker için bir turda gidilen mesafe

	// Motor enkoderı darbeleri (pulses)
	static volatile Pair<uint64_t> pulses;
	
private:
	unsigned long last_calc; // Son hesaplama zamanı (millis)

public:
	Encoder();

	// ISR callback fonksiyonları
	static void pulse_A_cb_L();
	static void pulse_A_cb_R();

	// İstatistik hesaplama fonksiyonu
	EncoderStats calc_stats();
};

#endif  // ENCODER_H
