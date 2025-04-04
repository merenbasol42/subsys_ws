#include "MotorController.h"
#include "./References.h"

MotorController::MotorController(): 
    motor(),				            // Motor nesnesi varsayılan kurucuyla oluşturuluyor
    encoder(),			                // Encoder nesnesi varsayılan kurucuyla oluşturuluyor
    pid({                               // PID çifti oluşturuluyors
        MyPID(PID_L_KP, PID_L_KI, PID_L_KD),
        MyPID(PID_R_KP, PID_R_KI, PID_R_KD),
    }),	
    last_spin(0)                        
{}

MotorController::~MotorController()
{
    // Gerekirse dinamik olarak oluşturulan kaynakları temizleyin.
}

void MotorController::set_enc_stats_pub_f(void (*func)(EncoderStats)) {
    this->enc_stats_pub_f = func;
}

void MotorController::spin_some() {
	if (!spin_control()) return;
	
	EncoderStats stats = encoder.calc_stats();
	Pair<int16_t> pwms = {
        constrain(
            (int16_t)pid.left.get_cmd(stats.vel.left),
            -255,
            255
        ),
        constrain(
            (int16_t)pid.right.get_cmd(stats.vel.right),
            -255,
            255
        )
	};
	
	this->motor.set_pwm(pwms);
	this->enc_stats_pub_f(stats);	
}

void MotorController::set_wh_speeds(Pair<float> wh_speeds) {
    this->pid.left.set_goal(wh_speeds.left);
    this->pid.right.set_goal(wh_speeds.right);
    // motorlara PWM yazmıyoruz zaten spin some'da bizim yerimize yapcak
}

bool MotorController::spin_control() {
    bool result = (millis() - last_spin) > INTERVAL;
    if (result) this->last_spin = millis();
    return result;
}
