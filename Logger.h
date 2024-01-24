// Logger.h
#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>

// Declare the global log on/off variable
extern bool g_enableLogging;

// Define a macro for logging
#define LOG(message) if (g_enableLogging) std::cout << message << std::endl;

#endif // LOGGER_H
