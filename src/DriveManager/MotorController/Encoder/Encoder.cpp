#include <Logger.h>
#include <References/Pin.h>
#include <References/Common.h>
#include <References/Robot.h>

#include "Encoder.h"
#include "./References.h"

// Statik üyelerin tanımlamaları
const Pair<uint8_t> Encoder::A_PIN = {PIN_ENCODER_L_A, PIN_ENCODER_R_A};	// Örneğin sol için 2, sağ için 18
const Pair<uint8_t> Encoder::B_PIN = {PIN_ENCODER_L_B, PIN_ENCODER_R_B};	// Örneğin sol için 3, sağ için 19
volatile Pair<uint64_t> Encoder::pulses = {0, 0};

Encoder::Encoder():
	RES({RES_L, RES_R}),					// Sabit rezolüsyon değerleri
	RADIUS({ROBOT_WH_R_L, ROBOT_WH_R_R}),				// Sabit yarıçap değerleri (gerektiği şekilde ayarlayın)
	DAP_FACT({ RES.left * 2 * PI, RES.right * 2 * PI }),	// Her teker için tur mesafesi
	last_calc(millis()) 
{
	pinMode(A_PIN.left, INPUT);   pinMode(A_PIN.right, INPUT);
	pinMode(B_PIN.left, INPUT);   pinMode(B_PIN.right, INPUT);

	attachInterrupt(
		digitalPinToInterrupt(A_PIN.left),
		pulse_A_cb_L,
		CHANGE
	);

	attachInterrupt(
		digitalPinToInterrupt(A_PIN.right),
		pulse_A_cb_R,
		CHANGE
	);
}

void Encoder::pulse_A_cb_L() {
	// Sol enkoder için A pininde kesme tetiklendiğinde B pininin durumu kontrol edilir.
	if(digitalRead(B_PIN.left) == HIGH) {
		pulses.left++;
	} else {
		pulses.left--;
	}
}

void Encoder::pulse_A_cb_R() {
	// Sağ enkoder için A pininde kesme tetiklendiğinde B pininin durumu kontrol edilir.
	if(digitalRead(B_PIN.right) == HIGH) {
		pulses.right++;
	} else {
		pulses.right--;
	}
}

EncoderStats Encoder::calc_stats() {
	EncoderStats stats;	// Varsayılan kurucu çağrılır
	
	noInterrupts();
	
	// dt bilgisi kullanılacaksa, stats yapısına ekleyebilirsiniz.
	unsigned long dt = millis() - last_calc;
	
	// Dönüş (delta angular position) hesaplaması: Dönüş başına düşen mesafe
	stats.dap.left = pulses.left / DAP_FACT.left;
	stats.dap.right = pulses.right / DAP_FACT.right;
	
	// Pulses sıfırlanıyor
	pulses.left = 0;
	pulses.right = 0; 	
	
	interrupts();
	
	// Hız hesaplaması: (örneğin, dönüş hızını yarıçapla çarparak lineer hıza çevirebilirsiniz)
	stats.vel.left = stats.dap.left * RADIUS.left / dt;
	stats.vel.right = stats.dap.right * RADIUS.right / dt;
	
	// Son hesaplama zamanı güncelleniyor
	last_calc = millis();
	
	return stats;
}

