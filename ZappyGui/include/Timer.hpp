/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Timer
*/

#include <iostream>
#include <chrono>
#include <string>
#include <unordered_map>

class TimerManager {
  public:
    void startTimer(const std::string &timerName)
    {
        timers[timerName] = std::chrono::high_resolution_clock::now();
    }

    double getElapsedTime(const std::string &timerName)
    {
        auto it = timers.find(timerName);
        if (it != timers.end()) {
            auto duration = std::chrono::duration<double>(
                std::chrono::high_resolution_clock::now() - it->second);
            return duration.count();
        } else {
            std::cerr << "Timer not found: " << timerName << std::endl;
            return -1.0;
        }
    }

    void resetTimer(const std::string &timerName)
    {
        auto it = timers.find(timerName);
        if (it != timers.end()) {
            timers[timerName] = std::chrono::high_resolution_clock::now();
        } else {
            std::cerr << "Timer not found: " << timerName << std::endl;
        }
    }

  private:
    std::unordered_map<std::string,
        std::chrono::high_resolution_clock::time_point>
        timers;
};
