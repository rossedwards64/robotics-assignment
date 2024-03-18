#include "MotorHandler.hpp"


void MotorHandler::calibrate(BorderDetectionHandler &border_detector)
{
    for (uint16_t i{ 0 }; i < 120; i++) {
        Direction direction;
        if (i > 30 && i <= 90) {
            direction = Direction::Left;
        } else {
            direction = Direction::Right;
        }

        move(direction, true);
        border_detector.calibrate();
    }
    stop();
}

void MotorHandler::move(Direction direction) { move(direction, move_time_); }

void MotorHandler::move(Direction direction, bool override_turn)
{
    move(direction, 0, override_turn);
}

void MotorHandler::move(Direction direction,
                        uint16_t duration,
                        bool override_turn)
{
    if (!override_turn) { moves.push(direction); }

    // use a function pointer to choose which colour LED to flash.
    // this is so we don't have to call led*(false) at the end of every case.
    void (*led)(bool);

    switch (direction) {
    case Left:
        led = ledGreen;
        led(true);
        if (!override_turn) {
            do_move(-turn_speed_, turn_speed_);
        } else {
            do_move(-calib_speed_, calib_speed_, duration);
        }
        break;
    case Right:
        led = ledGreen;
        led(true);
        if (!override_turn) {
            do_move(turn_speed_, -turn_speed_);
        } else {
            do_move(calib_speed_, -calib_speed_, duration);
        }
        break;
    case Straight:
        led = ledYellow;
        led(true);
        do_move(max_speed_, max_speed_, duration);
        break;
    case Reverse:
        led = ledRed;
        led(true);
        do_move(-max_speed_, -max_speed_, duration);
        break;
    case FlipLeft:
        led = ledYellow;
        led(true);
        do_move(-turn_speed_, turn_speed_, flip_time_);
        break;
    case FlipRight:
        led = ledYellow;
        led(true);
        do_move(turn_speed_, -turn_speed_, flip_time_);
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
    case FlipLeft:// no-op
    default: break;
    }
    return inverted;
}

void MotorHandler::do_move(int16_t speed_l, int16_t speed_r)
{
    do_move(speed_l, speed_r, turn_time_);
}

void MotorHandler::do_move(int16_t speed_l, int16_t speed_r, uint16_t duration)
{
    setSpeeds(speed_l, speed_r);
    delay(duration);
}

void MotorHandler::go_back()
{
    for (Direction direction = moves.pop(); !moves.empty();
         direction = moves.pop()) {
        move(invert(direction));
    }
}
