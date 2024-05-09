/**
 * @file controllers.cpp
 * @author Jakob Friedl (friedl.jak@gmail.com)
 * @brief Utility function and class definitions for controllers.
 * @version 0.1
 * @date 2023-10-08
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <roboost/utils/controllers.hpp>

using namespace roboost::controllers;
using namespace roboost::timing;
using namespace roboost::filters;

PIDController::PIDController(double kp, double ki, double kd, double max_integral, Filter& derivative_filter, TimingService& timing_service)
    : kp_(kp), ki_(ki), kd_(kd), integral_(0.0), previous_error_(0.0), derivative_filter_(derivative_filter), max_integral_(max_integral), timing_service_(timing_service)
{
}

double PIDController::update(double setpoint, double input)
{
    double dt = MICROS_TO_SECONDS_DOUBLE(timing_service_.getDeltaTime());
    double error = setpoint - input;

    integral_ += error * dt;

    // Enforce the maximum integral limit
    if (integral_ > max_integral_)
    {
        integral_ = max_integral_;
    }
    else if (integral_ < -max_integral_)
    {
        integral_ = -max_integral_;
    }
    double derivative = derivative_filter_.update((error - previous_error_) / dt);
    previous_error_ = error;

    double output = kp_ * error + ki_ * integral_ + kd_ * derivative;

    return output;
}

void PIDController::reset()
{
    integral_ = 0.0;
    previous_error_ = 0.0;
    derivative_filter_.reset();
}

double PIDController::get_kp() const { return kp_; }

double PIDController::get_ki() const { return ki_; }

double PIDController::get_kd() const { return kd_; }

double PIDController::get_max_integral() const { return max_integral_; }

double PIDController::get_integral() const { return integral_; }

double PIDController::get_derivative() const { return derivative_filter_.get_output(); }

double PIDController::get_previous_error() const { return previous_error_; }

void PIDController::set_kp(double kp) { kp_ = kp; }

void PIDController::set_ki(double ki) { ki_ = ki; }

void PIDController::set_kd(double kd) { kd_ = kd; }