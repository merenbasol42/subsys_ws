#include <Logger.h>

#include "Messenger/Messenger.h"
#include "DriveManager/DriveManager.h"

Messenger* messenger;
DriveController* drive_ctrl;

void start_signal(){
    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);
    delay(1000);
    digitalWrite(2, LOW);
    delay(1000);
}

void setup() {
    start_signal();
    messenger = new Messenger();
    // Logger fonksiyonunu ayarla
    Logger::set_logger_f(
        [](String text){
            messenger->pub_output(text);
        }
    );
    
    // Dış iletişimi başlat
    messenger->connect();
    // Nesneleri oluştur
    drive_ctrl = new DriveController();

    // İç iletişim bağlantılarını kur
    messenger->set_cmd_vel_cb_func(
        [](Velocity velocity){
            drive_ctrl->set_velocity(velocity);
        }
    );
    
    drive_ctrl->set_enc_stats_pub_f(
        [](EncoderStats stats) {
            messenger->pub_encoder_stats(stats);
        }
    );
}

void loop() {
    messenger->spin_some();
    drive_ctrl->spin_some();
}