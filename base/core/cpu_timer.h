/**
 * @file        base/core/cpu_timer.h
 * @brief       cpu timer
 * @author      yy
 * @email       syby119@126.com
 * @date        2021/1/27
 * @copyright   MIT license
 */

#pragma once

#include <chrono>
#include <iostream>
#include <string>

class CpuTimer {
public:
    /**
     * @brief default constructor
     */
    CpuTimer() = default;
    
    /**
     * @brief default destructor
     */
    ~CpuTimer() = default;

    /**
     * @brief start the timer
     */
    void start() noexcept {
        _start = std::chrono::high_resolution_clock::now();
    }

    /**
     * @brief stop the timer
     */
    void stop() noexcept {
        _stop = std::chrono::high_resolution_clock::now();
    }

    /**
     * @brief get the duration from the last call of start to stop
     * @return duration in milliseconds
     */
    float getElapsedTime() noexcept {
        return (std::chrono::duration<float, std::milli>(_stop - _start)).count();
    }

    /**
     * @brief print the info and start the timer
     * @param info message to present
     */
    void start(const std::string& info) noexcept {
        std::cout << info << std::endl;
        _start = std::chrono::high_resolution_clock::now();
    }

    /**
     * @brief stop the timer and print the info and duration
     * @param info message to present
     */
    void stop(const std::string& info) noexcept {
        _stop = std::chrono::high_resolution_clock::now();
        std::cout << info << getElapsedTime() << "ms" << std::endl;
    }

private:
    /* record start time point of the timer after call CpuTimer::start */
    std::chrono::time_point<std::chrono::high_resolution_clock> _start;
    /* record stop time point of the timer after call CpuTimer::stop */
    std::chrono::time_point<std::chrono::high_resolution_clock> _stop;
};