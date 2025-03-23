#ifndef MY_PID_H
#define MY_PID_H

class MyPID{
private:
    /* data */
    float kp;
    float ki;
    float kd;

    float sum = 0.0f;
    float pre = 0.0f;

    float goal = 0.0f;

public:
    void set_goal(float new_goal);
    float get_cmd(float feedback);
    MyPID(float kp, float ki, float kd);
    ~MyPID();
};

#endif