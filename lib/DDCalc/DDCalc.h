#ifndef DDCALC_H
#define DDCALC_H

#include <Pair.h>
#include <CommInterfaces/Velocity.h>

class DDCalc {
public:
    static Pair<float> vel_to_wh(Velocity vel, float l);
    static Velocity wh_to_vel(Pair<float> wh_speeds, float l);
};

#endif