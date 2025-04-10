#include <Logger.h>
#include <References/Pin.h>
#include <References/Common.h>
#include <References/Robot.h>

#include "Encoder.h"
#include "./References.h"

// Statik üyelerin tanımlamaları
const Pair<uint8_t> Encoder::CLK_PIN = {PIN_ENCODER_L_CLK, PIN_ENCODER_R_CLK};
const Pair<uint8_t> Encoder::DT_PIN = {PIN_ENCODER_L_DT, PIN_ENCODER_R_DT};	
volatile Pair<uint64_t> Encoder::pulses = {0, 0};

Encoder::Encoder():
	RES({RES_L, RES_R}),					// Sabit rezolüsyon değerleri
	RADIUS({ROBOT_WH_R_L, ROBOT_WH_R_R}),				// Sabit yarıçap değerleri (gerektiği şekilde ayarlayın)
	DAP_FACT({ RES.left * 2 * PI, RES.right * 2 * PI }),	// Her teker için tur mesafesi
	last_calc(millis()) 
{
	pinMode(CLK_PIN.left, INPUT);   pinMode(CLK_PIN.right, INPUT);
	pinMode(DT_PIN.left, INPUT);   pinMode(DT_PIN.right, INPUT);

	attachInterrupt(
		digitalPinToInterrupt(CLK_PIN.left),
		pulse_cb_L,
		CHANGE
	);

	attachInterrupt(
		digitalPinToInterrupt(CLK_PIN.right),
		pulse_cb_R,
		CHANGE
	);
}

void Encoder::pulse_cb_L() {
	// CLK pininde kesme tetiklendiğinde DT pininin durumu kontrol edilir.
	int clk_st = digitalRead(CLK_PIN.left);
	int dt_st  = digitalRead(DT_PIN.left);
	
	if(dt_st != clk_st) {
		pulses.left++;
	} else {
		pulses.left--;
	}
}

void Encoder::pulse_cb_R() {
	// CLK pininde kesme tetiklendiğinde DT pininin durumu kontrol edilir.
	int clk_st = digitalRead(CLK_PIN.right);
	int dt_st  = digitalRead(DT_PIN.right);

	if(dt_st != clk_st) {
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

