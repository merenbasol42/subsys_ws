#ifndef VELOCITY_H
#define VELOCITY_H

struct Velocity{
    float linear;
    float angular;

    Velocity(){
        linear = 0;
        angular = 0;
    }

    Velocity(float linear, float angular){
        this->linear = linear;
        this->angular = angular;
    }
};

#endif