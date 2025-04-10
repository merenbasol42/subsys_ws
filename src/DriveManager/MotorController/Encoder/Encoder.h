#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>
#include <Pair.h>
#include <CommInterfaces/EncoderStats.h>

class Encoder {
public:
	// Statik pin tanımlamaları; bu değerleri ihtiyacınıza göre ayarlayabilirsiniz.
	static const Pair<uint8_t> CLK_PIN; // signal
	static const Pair<uint8_t> DT_PIN; // yön
	
	// Sabit özellikler: Rezolüsyon ve yarıçap değerleri.
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
	static void pulse_cb_L();
	static void pulse_cb_R();

	// İstatistik hesaplama fonksiyonu
	EncoderStats calc_stats();
};

#endif  // ENCODER_H
