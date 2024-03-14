#ifndef MOTOR_HANDLER_HPP_
#define MOTOR_HANDLER_HPP_

#include <Zumo32U4Motors.h>

#include "GyroHandler.hpp"
#include "Stack.hpp"


class MotorHandler : Zumo32U4Motors
{
public:
    enum Direction : uint8_t {
        Left,
        Right,
        Straight,
        Reverse,
    };

    explicit MotorHandler() noexcept : gyro(), moves(), max_speed_(100) {}

    explicit MotorHandler(int16_t max_speed) noexcept
        : gyro(), max_speed_(max_speed)
    {
    }

    static void stop() { setSpeeds(0, 0); }

    void do_move(Direction direction);
    void do_move(Direction direction, uint16_t duration);

    void go_back();
    void retrace_steps();

    void set_speed(int16_t speed) { this->max_speed_ = speed; }

private:
    static constexpr uint16_t max_moves_{ 100 };

    static constexpr int16_t move_time_{ 600 };
    static constexpr int16_t turn_speed_{ 200 };

    GyroHandler gyro;
    Stack<Direction, max_moves_> moves;
    int16_t max_speed_;
};

#endif
