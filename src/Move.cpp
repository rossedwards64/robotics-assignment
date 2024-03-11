#include "Move.hpp"

Zumo32U4IMU Move::imu;
Zumo32U4Motors Move::motors;

int16_t Move::gyro_offset_ = 0;
uint16_t Move::gyro_last_update_ = 0;

auto Move::do_move() -> Direction { return do_move(500); }

auto Move::do_move(uint16_t duration) -> Direction
{
    switch (direction_) {
    case Left: motors.setSpeeds(-turn_speed_, turn_speed_); break;
    case Right: motors.setSpeeds(turn_speed_, -turn_speed_); break;
    case Straight: motors.setSpeeds(max_speed_, max_speed_); break;
    case Reverse: motors.setSpeeds(-max_speed_, -max_speed_); break;
    case SpinLeft:
        for (uint16_t i = 0; i < 4; i++) { turn_left(); }
        break;
    case SpinRight:
        for (uint16_t i = 0; i < 4; i++) { turn_right(); }
        break;
    }

    delay(duration);
    reset_angle();
    return direction_;
}

void Move::reset_angle()
{
    gyro_last_update_ = micros();
    turn_angle_ = 0;
}

void Move::turn_left()
{
    motors.setSpeeds(-max_speed_, max_speed_);
    while (static_cast<int32_t>(turn_angle_) < quarter_turn_) { turn_adjust(); }
}

void Move::turn_right()
{
    motors.setSpeeds(max_speed_, -max_speed_);
    while (static_cast<int32_t>(turn_angle_) < quarter_turn_) { turn_adjust(); }
}

void Move::turn_adjust()
{
    // obtained from Zumo MazeSolver example
    this->imu.readGyro();
    uint16_t now = micros();
    uint16_t delta = now - gyro_last_update_;
    gyro_last_update_ = now;
    int32_t d = static_cast<int32_t>(this->imu.g.z) * delta;
    turn_angle_ += static_cast<int64_t>(d) * degrees_formula_;
}
