#ifndef MOVE_HPP_
#define MOVE_HPP_

#include "GyroHandler.hpp"
#include "Stack.hpp"
#include "Zumo32U4Motors.h"
#include <Arduino.h>
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++11-narrowing"
#include <Zumo32U4.h>
#pragma clang diagnostic pop
#include <stdint.h>

class MotorHandler : Zumo32U4Motors
{
public:
    enum Direction : uint8_t {
        Left,
        Right,
        Straight,
        Reverse,
    };

    explicit MotorHandler() noexcept
        : gyro(), moves(), direction_(Direction::Straight), max_speed_(100)
    {
    }

    explicit MotorHandler(Direction direction) noexcept
        : MotorHandler(direction, 100)
    {
    }

    explicit MotorHandler(Direction direction, int16_t max_speed) noexcept
        : gyro(), direction_(direction), max_speed_(max_speed)
    {
    }

    static void stop() { setSpeeds(0, 0); }

    void do_move();
    void do_move(uint16_t duration);

    void go_back();
    void retrace_steps();

    void set_direction(Direction direction) { this->direction_ = direction; }
    void set_speed(int16_t speed) { this->max_speed_ = speed; }

private:
    static constexpr uint16_t max_moves_{ 100 };

    static constexpr int16_t move_time_{ 600 };
    static constexpr int16_t turn_speed_{ 200 };

    GyroHandler gyro;
    Stack<Direction, max_moves_> moves;
    Direction direction_;
    int16_t max_speed_;
};

#endif
