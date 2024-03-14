#include "MotorHandler.hpp"


void MotorHandler::calibrate_turn(BorderDetectionHandler &border_detector)
{
    setSpeeds(-max_speed_, max_speed_);
    gyro.calibrate_turn(border_detector);
    setSpeeds(max_speed_, -max_speed_);
    gyro.calibrate_turn(border_detector, true);
    setSpeeds(-max_speed_, max_speed_);
    gyro.calibrate_turn(border_detector, false, true);
}

void MotorHandler::do_move(Direction direction) { do_move(direction, 500); }

void MotorHandler::do_move(Direction direction, uint16_t duration)
{
    moves.push(direction);
    switch (direction) {
    case Left:
        setSpeeds(-max_speed_, max_speed_);
        gyro.turn_left();
        break;
    case Right:
        setSpeeds(max_speed_, -max_speed_);
        gyro.turn_right();
        break;
    case Straight:
        setSpeeds(max_speed_, max_speed_);
        delay(duration);
        break;
    case Reverse:
        setSpeeds(-max_speed_, -max_speed_);
        delay(duration);
        break;
    }
}

void MotorHandler::go_back()
{
    for (Direction direction = moves.pop(); !moves.empty();
         direction = moves.pop()) {
        do_move(direction);
    }
}
