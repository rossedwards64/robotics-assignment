#ifndef MOTOR_HANDLER_HPP_
#define MOTOR_HANDLER_HPP_

#include <Zumo32U4.h>
#include <Zumo32U4Motors.h>
#include <Zumo32U4LCD.h>

#include "GyroHandler.hpp"
#include "Stack.hpp"
#include "BorderDetectionHandler.hpp"


class MotorHandler : Zumo32U4Motors
{
public:
    enum Direction : uint8_t {
        Left,
        Right,
        Straight,
        Reverse,
        FlipLeft,
        FlipRight
    };

    explicit MotorHandler(int16_t max_speed) noexcept
        : gyro(), moves(), max_speed_(max_speed)
    {
    }

    explicit MotorHandler() noexcept : MotorHandler(100) {}

    static void stop() { setSpeeds(0, 0); }

    void initialise() { gyro.initialise(); }

    // calibration routine obtained from the LineFollower example.
    void calibrate(BorderDetectionHandler &border_detector);

    // override_turn is a flag to allow the calibration routine to run at a
    // higher speed.
    void move(Direction direction);
    void move(Direction direction, bool override_turn);
    void move(Direction direction, uint16_t duration,
              bool override_turn = false);
    static Direction invert(Direction direction);

    void go_back();
    void retrace_steps();

    void set_speed(int16_t speed) { this->max_speed_ = speed; }

private:
    static void do_move(int16_t speed_l, int16_t speed_r);
    static void do_move(int16_t speed_l, int16_t speed_r, uint16_t duration);

    static constexpr uint8_t max_moves_{ 100 };

    static constexpr int16_t calib_speed_{ 200 };
    static constexpr uint16_t move_time_{ 100 };
    static constexpr int16_t turn_speed_{ 90 };
    static constexpr uint16_t turn_time_{ 1200 };
    static constexpr uint32_t flip_time_{ static_cast<uint32_t>(turn_time_)
                                          * 2 };

    GyroHandler gyro;
    Stack<Direction, max_moves_> moves;
    int16_t max_speed_;
    uint32_t last_error{ 0 };
};

#endif
