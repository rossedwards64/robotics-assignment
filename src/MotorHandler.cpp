#include "MotorHandler.hpp"


void MotorHandler::calibrate_turn(BorderDetectionHandler &border_detector)
{
    for (uint16_t i{ 0 }; i < 120; i++) {
        Direction direction;
        if (i > 30 && i <= 90) {
            direction = Direction::Left;
        } else {
            direction = Direction::Right;
        }

        do_move(direction, 200, true);
        border_detector.calibrate();
    }
    stop();
}

void MotorHandler::do_move(Direction direction)
{
    do_move(direction, move_time_);
}

// to go 90 degrees, set to 90 speed and turn for 1000 milliseconds.
void MotorHandler::do_move(Direction direction,
                           uint16_t duration,
                           bool override_turn)
{
    if (!override_turn) {
        // display.clear();
        moves.push(direction);
        // display.print("add");
    }

    void (*led)(bool);

    switch (direction) {
    case Left:
        led = ledGreen;
        led(true);
        if (!override_turn) {
            setSpeeds(-turn_speed_, turn_speed_);
            delay(turn_time_);
        } else {
            setSpeeds(200, -200);
        }
        // gyro.turn_left();
        break;
    case Right:
        led = ledGreen;
        led(true);
        if (!override_turn) {
            setSpeeds(turn_speed_, -turn_speed_);
            delay(turn_time_);
        } else {
            setSpeeds(-200, 200);
        }
        // gyro.turn_right();
        break;
    case Straight:
        led = ledYellow;
        led(true);
        setSpeeds(max_speed_, max_speed_);
        delay(duration);
        break;
    case Reverse:
        led = ledRed;
        led(true);
        setSpeeds(-max_speed_, -max_speed_);
        delay(duration);
        break;
    case Rotate:
        led = ledYellow;
        led(true);
        setSpeeds(-turn_speed_, turn_speed_);
        delay(static_cast<uint32_t>(turn_time_) * 2);
        break;
    }
    led(false);
}

MotorHandler::Direction MotorHandler::invert(Direction direction)
{
    Direction inverted;
    switch (direction) {
    case Left: inverted = Right; break;
    case Right: inverted = Left; break;
    case Straight: inverted = Reverse; break;
    case Reverse: inverted = Straight; break;
    case Rotate:
    default: break;
    }
    return inverted;
}

void MotorHandler::go_back()
{
    for (Direction direction = moves.pop(); !moves.empty();
         direction = moves.pop()) {
        do_move(invert(direction));
    }
}
