#ifndef MOVE_HPP_
#define MOVE_HPP_

#include "Stack.hpp"
#include "Zumo32U4Motors.h"
#include "Zumo32U4IMU.h"
#include <Arduino.h>
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++11-narrowing"
#include <Zumo32U4.h>
#pragma clang diagnostic pop
#include <stdint.h>

class Move
{
public:
    enum Direction : uint8_t {
        Left,
        Right,
        Straight,
        Reverse,
    };

    constexpr Move(Direction direction, int16_t max_speed, uint32_t turn_angle)
        : direction_(direction), max_speed_(max_speed), turn_angle_(turn_angle)
    {
    }

    constexpr Move(Direction direction, int16_t max_speed)
        : Move(direction, max_speed, 0)
    {
    }

    constexpr explicit Move(Direction direction, uint32_t turn_angle)
        : Move(direction, 100, turn_angle)
    {
    }

    constexpr explicit Move(Direction direction) : Move(direction, 100, 0) {}

    auto do_move() -> Direction;
    auto do_move(uint16_t duration) -> Direction;

    void go_back();
    void retrace_steps();

    static void stop() { motors.setSpeeds(0, 0); }
    void reset_angle();

    static int32_t convert_angle(uint32_t angle)
    {
        return constrain((((static_cast<int32_t>(angle) >> 16) * 360) >> 16),
                         -180, 180);
    }

    static Zumo32U4Motors motors;
    static Zumo32U4IMU imu;

private:
    void turn_left();
    void turn_right();
    int32_t convert_angle();
    void turn_adjust();

    static const uint16_t max_moves_{ 100 };
    static Stack<Direction, max_moves_> moves;

    static int16_t gyro_offset_;
    static uint16_t gyro_last_update_;

    static const int16_t move_time_{ 600 };
    static const int16_t turn_speed_{ 200 };

    // Values obtained from Zumo MazeSolver example.
    static const int32_t half_quarter_turn_{ 0x20000000 };
    static constexpr int32_t quarter_turn_{ half_quarter_turn_ * 2 };
    static constexpr int32_t degree_turn_{ (half_quarter_turn_ + 22) / 45 };
    static constexpr int64_t degrees_formula_{ 14680064 / 17578125 };

    Direction direction_;
    int16_t max_speed_;
    uint32_t turn_angle_;
};

#endif
