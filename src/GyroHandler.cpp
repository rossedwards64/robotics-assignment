#include "GyroHandler.hpp"


void GyroHandler::calibrate()
{
    int32_t total{ 0 };
    for (uint16_t i{ 0 }; i < 1024; i++) {
        blocking_read();
        total = g.z;
    }
    gyro_offset_ = total / 1024;
    reset_angle();
}

void GyroHandler::calibrate_turn(BorderDetectionHandler &border_detector,
                                 bool reverse, bool centre)
{
    if (centre) {
        while (static_cast<int32_t>(turn_angle_) < 0) {
            border_detector.calibrate();
            turn_adjust();
        }
    } else if (reverse) {
        while (static_cast<int32_t>(turn_angle_) > -angle_90_degrees_) {
            border_detector.calibrate();
            turn_adjust();
        }
    } else {
        while (static_cast<int32_t>(turn_angle_) < angle_90_degrees_) {
            border_detector.calibrate();
            turn_adjust();
        }
    }
}

void GyroHandler::turn_right()
{
    while (static_cast<int32_t>(turn_angle_) > -angle_90_degrees_) {
        turn_adjust();
    }
}

void GyroHandler::turn_left()
{
    while (static_cast<int32_t>(turn_angle_) < angle_90_degrees_) {
        turn_adjust();
    }
}

void GyroHandler::recenter()
{
    while (turn_angle_ < 0) { turn_adjust(); }
}

void GyroHandler::reset_angle()
{
    gyro_last_update_ = micros();
    turn_angle_ = 0;
}

void GyroHandler::blocking_read()
{
    while (!gyroDataReady()) {}
    readGyro();
}

void GyroHandler::turn_adjust()
{
    blocking_read();
    turn_rate_ = g.z - gyro_offset_;
    uint32_t now{ micros() };
    uint32_t delta{ now - gyro_last_update_ };
    gyro_last_update_ = now;
    uint32_t d{ static_cast<int32_t>(turn_rate_) * delta };
    turn_angle_ += gyro_digits_to_degrees(d);
}
