#include "console.h"

void Logger::Error(std::string message) {
	logFile.push_back("ERROR: " + message);
};

void Logger::Info(std::string message) {
	logFile.push_back("INFO: " + message);
};
