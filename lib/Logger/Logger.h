#include <Arduino.h>

class Logger
{
private:
    /* data */
    static void (*logger_f) (String);

public:
    static void log(String text);
    static void set_logger_f(void (*func) (String));

};
