#include "MotorHandler.hpp"

void MotorHandler::do_move() { do_move(500); }

void MotorHandler::do_move(uint16_t duration)
{
    moves.push(direction_);
    switch (direction_) {
    case Left:
        setSpeeds(-max_speed_, max_speed_);
        gyro.turn();
        break;
    case Right:
        setSpeeds(max_speed_, -max_speed_);
        gyro.turn();
        break;
    case Straight: setSpeeds(max_speed_, max_speed_); break;
    case Reverse: setSpeeds(-max_speed_, -max_speed_); break;
    }

    delay(duration);
}

void MotorHandler::go_back()
{
    for (direction_ = moves.pop(); !moves.empty(); direction_ = moves.pop()) {
        do_move();
    }
}
