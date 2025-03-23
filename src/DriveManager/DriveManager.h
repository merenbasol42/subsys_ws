#ifndef DRIVE_MANAGER_H
#define DRIVE_MANAGER_H

#include <CommInterfaces/Velocity.h>
#include <CommInterfaces/EncoderStats.h>

#include "./MotorController/MotorController.h"

class DriveController {
private:
    MotorController motor_ctrl;
    
public:
    DriveController();
    ~DriveController();

    void set_enc_stats_pub_f(void (*func) (EncoderStats));
    void set_velocity(Velocity velocity);
    void spin_some();
};

#endif