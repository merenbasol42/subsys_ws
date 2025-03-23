#include "Logger.h"

void _def_logger(String msg) {
    Serial.println(msg);
}

// Statik üye değişkenin tanımı ve varsayılan fonksiyonun atanması
void (*Logger::logger_f)(String) = _def_logger;


void Logger::log(String text) {
    Logger::logger_f(text);
}

void Logger::set_logger_f(void (*func)(String)) {
    Logger::logger_f = func;
}
