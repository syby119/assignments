/**
 * @file        base/core/gpu_timer.h
 * @brief       gpu timer
 * @author      yy
 * @email       syby119@126.com
 * @date        2021/1/27
 * @copyright   MIT license
 */

#pragma once

#include <iostream>
#include <string>

#include <cuda_runtime.h>

#include "error_handle.h"

class GpuTimer {
public:
    /**
     * @brief default constructor
     * @exception std::runtime_error for cudaEventCreate failure
     */
    GpuTimer() {
        try {
            cudaError_t err = cudaEventCreate(&_start);
            if (err != cudaSuccess) {
                THROW(cudaGetErrorString(err));
            }

            err = cudaEventCreate(&_stop);
            if (err != cudaSuccess) {
                THROW(cudaGetErrorString(&_stop));
            }
        } catch (const std::runtime_error& e) {
            _cleanup();
            throw e;
        }
    }

    /**
     * @brief move constructor
     * @note by define the move constructor, implicit copy constructor, 
     *       and copy/move assignment operator are deleted
     */
    GpuTimer(GpuTimer&& timer) noexcept {
        _start = timer._start;
        timer._start = nullptr;

        _stop = timer._stop;
        timer._stop = nullptr;
    }

    /**
     * @brief default destructor
     */
    ~GpuTimer() {
        _cleanup();
    }

    /**
     * @brief record the start of the event
     * @exception std::runtime_error 
     *            if cudaEventRecord failure or previous error unhandled
     */
    void start() {
        cudaError_t err = cudaEventRecord(_start, 0);
        if (err != cudaSuccess) {
            THROW(cudaGetErrorString(err));
        }
    }

    /**
     * @brief record the stop of the event
     * @exception std::runtime_error 
     *            if cudaEventRecord failure or previous error unhandled
     */
    void stop() {
        cudaError_t err = cudaEventRecord(_stop, 0);
        if (err != cudaSuccess) {
            THROW(cudaGetErrorString(err));
        }
    }

    /**
     * @brief get the duration from the last call of start to stop
     * @exception std::runtime_error 
     *            if cudaEventSynchronize/cudaEventElapsedTime failure 
     *            or error failure unhandled
     */
    float getElapsedTime() {
        cudaEventSynchronize(_stop);
        float elapsedTime = 0.0f;
        cudaEventElapsedTime(&elapsedTime, _start, _stop);
        
        cudaError_t err = cudaGetLastError();
        if (err != cudaSuccess) {
            THROW(cudaGetErrorString(err));
        }

        return elapsedTime;
    }

    /**
     * @brief print the info and start the timer
     * @param info message to present
     * @exception std::runtime_error 
     *            if cudaEventRecord failure or previous failure unhandled
     */
    void start(const std::string& info) {
        std::cout << info << std::endl;
        cudaError_t err = cudaEventRecord(_start, 0);
        if (err != cudaSuccess) {
            THROW(cudaGetErrorString(err));
        }
    }

    /**
     * @brief stop the timer and print the info and duration
     * @param info message to present
     * @exception std::runtime_error 
     *            if cudaEventRecord failure or get elapsed time failure 
     *            or previous error unhandled
     */
    void stop(const std::string& info) {
        cudaError_t err = cudaEventRecord(_stop, 0);
        if (err != cudaSuccess) {
            THROW(cudaGetErrorString(err));
        }

        float elapsedRTime = getElapsedTime();
        std::cout << info << ": " << elapsedRTime << "ms" << std::endl;
    }

private:
    cudaEvent_t _start = nullptr;
    cudaEvent_t _stop = nullptr;

    /**
     * @brief recycle all resources allocated before
     */
    void _cleanup() {
        if (_start) {
            cudaError_t err = cudaEventDestroy(_start);
            if (err != cudaSuccess) {
                WARN(cudaGetErrorString(err));
            }
            _start = nullptr;
        }

        if (_stop) {
            cudaError_t err = cudaEventDestroy(_start);
            if (err != cudaSuccess) {
                WARN(cudaGetErrorString(err));
            }
            _stop = nullptr;
        }
    }
};