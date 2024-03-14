#include "MotorHandler.hpp"


void MotorHandler::do_move(Direction direction) { do_move(direction, 500); }

void MotorHandler::do_move(Direction direction, uint16_t duration)
{
    moves.push(direction);
    switch (direction) {
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
    for (Direction direction = moves.pop(); !moves.empty();
         direction = moves.pop()) {
        do_move(direction);
    }
}
