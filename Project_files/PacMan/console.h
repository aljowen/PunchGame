#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <iostream>
#include <fstream>

namespace Logger
{
	static std::vector<std::string> logFile;

	static void Error(std::string message) {
		logFile.push_back(std::to_string(std::chrono::high_resolution_clock::now().time_since_epoch().count()) + "| ERROR: " + message);
		std::cout << std::to_string(std::chrono::high_resolution_clock::now().time_since_epoch().count()) + "| ERROR: " + message << std::endl;
	};
	static void Info(std::string message) {
		logFile.push_back(std::to_string(std::chrono::high_resolution_clock::now().time_since_epoch().count()) + "| INFO: " + message);
		std::cout << std::to_string(std::chrono::high_resolution_clock::now().time_since_epoch().count()) + "| INFO: " + message << std::endl;
	};
	static void Perf(std::string message) {
		logFile.push_back(std::to_string(std::chrono::high_resolution_clock::now().time_since_epoch().count()) + "| PERFORMANCE: " + message);
		std::cout << std::to_string(std::chrono::high_resolution_clock::now().time_since_epoch().count()) + "| PERFORMANCE: " + message << std::endl;
	};

	static void Out() {
		Logger::Info("Game closed normally");
		Logger::Info("Writing Log to File... (this may take a while)");

		//Saving log to a file
		std::ofstream logOut;

		logOut.open("./logFile.txt");
		if (!logOut.is_open()) {
			Logger::Info("Writing to Log FAILED !!!");
		}
		for each (std::string line in Logger::logFile)
		{
			logOut << line << std::endl;
		}
		logOut.close();
	}

};




