#include "Move.hpp"
#include <avr/common.h>

Stack<Move::Direction, Move::max_moves_> Move::moves;

Zumo32U4IMU Move::imu;
Zumo32U4Motors Move::motors;

int16_t Move::gyro_offset_{ 0 };
uint16_t Move::gyro_last_update_{ 0 };

auto Move::do_move() -> Direction { return do_move(500); }

auto Move::do_move(uint16_t duration) -> Direction
{
    moves.push(direction_);
    switch (direction_) {
    case Left: turn_left(); break;
    case Right: turn_right(); break;
    case Straight: motors.setSpeeds(max_speed_, max_speed_); break;
    case Reverse: motors.setSpeeds(-max_speed_, -max_speed_); break;
    }

    delay(duration);
    reset_angle();
    moves.push(direction_);
    return direction_;
}

void Move::go_back()
{
    for (direction_ = moves.pop(); !moves.empty(); direction_ = moves.pop()) {
        do_move();
    }
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
    uint32_t now{ micros() };
    uint32_t delta{ now - gyro_last_update_ };
    gyro_last_update_ = now;
    uint32_t d{ static_cast<int32_t>(this->imu.g.z) * delta };
    turn_angle_ += static_cast<int64_t>(d) * degrees_formula_;
}
