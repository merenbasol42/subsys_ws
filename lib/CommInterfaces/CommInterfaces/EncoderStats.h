#ifndef ENCODER_STATS_H
#define ENCODER_STATS_H

#include <Pair.h>

struct EncoderStats
{
    Pair<float> dap;
    Pair<float> vel;

    EncoderStats() = default;

    EncoderStats(const Pair<float>& d, const Pair<float>& v): dap(d), vel(v) {}

    EncoderStats(float dap_l, float dap_r, float vel_l, float vel_r){
        dap.left = dap_l;
        dap.right = dap_r;
        vel.left = vel_l;
        vel.right = vel_r;
    }
};

#endif