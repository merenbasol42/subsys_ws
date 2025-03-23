#include "DDCalc.h"

Pair<float> DDCalc::vel_to_wh(Velocity vel, float l) {
    // V_left = V_lin - W * L / 2
    // V_right = V_lin + W * L / 2
    return Pair<float>(
        vel.linear - vel.angular * l / 2.0f,
        vel.linear + vel.angular * l / 2.0f
    );
}

Velocity DDCalc::wh_to_vel(Pair<float> wh_speeds, float l) {
    // V_lin = (V_left + V_right) / 2 
    // W = (V_right - V_left) / L
    return Velocity(
        (wh_speeds.left + wh_speeds.right) / 2.0f,
        (wh_speeds.right - wh_speeds.left) / l
    );
}
