#include "Logger.h"
#include <iostream>
#include <ctime>
#include <cstring>

void Logger::log(const std::string& message) {
    time_t now = time(0);
    char dt[26];
    errno_t result = ctime_s(dt, sizeof(dt), &now);
 
    if (result == 0) {
        dt[strcspn(dt, "\n")] = '\0';
        std::cout << "[LOG " << dt << "] " << message << std::endl;
    }
}