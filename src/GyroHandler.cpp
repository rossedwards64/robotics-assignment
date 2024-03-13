#include "GyroHandler.hpp"

void GyroHandler::turn()
{
    while (static_cast<int32_t>(turn_angle_) < quarter_turn_) { turn_adjust(); }
}

void GyroHandler::reset_angle()
{
    gyro_last_update_ = micros();
    turn_angle_ = 0;
}

void GyroHandler::turn_adjust()
{
    // obtained from Zumo MazeSolver example
    readGyro();
    turn_rate_ = g.z - gyro_offset_;
    uint32_t now{ micros() };
    uint32_t delta{ now - gyro_last_update_ };
    gyro_last_update_ = now;
    uint32_t d{ static_cast<int32_t>(turn_rate_) * delta };
    turn_angle_ += static_cast<int64_t>(d) * degrees_formula_;
}
