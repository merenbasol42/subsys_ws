#include <Logger.h>
#include <DDCalc.h>

#include <References/Robot.h>

#include "DriveManager.h"
#include "./References.h"

DriveController::DriveController(): motor_ctrl() {}

DriveController::~DriveController() {} 

void DriveController::set_velocity(Velocity velocity) {
    Pair<float> wh_speeds = DDCalc::vel_to_wh(velocity, ROBOT_WH_SEP);

    this->motor_ctrl.set_wh_speeds(wh_speeds);
    Logger::log(
        "::Setted:: Vl = " +
        String(wh_speeds.left) +
        " Vr = " +
        String(wh_speeds.right) 
    );
}

void DriveController::set_enc_stats_pub_f(void (*func) (EncoderStats)){
    this->motor_ctrl.set_enc_stats_pub_f(func);
}

void DriveController::spin_some() {
    this->motor_ctrl.spin_some();
}
