#ifndef GYRO_HANDLER_HPP_
#define GYRO_HANDLER_HPP_

#include <stdint.h>
#include <Arduino.h>
#include "Zumo32U4IMU.h"

class GyroHandler : public Zumo32U4IMU
{
public:
    GyroHandler()
    {
        init();
        enableDefault();
        configureForTurnSensing();
    };

    // Values obtained from Zumo MazeSolver example.
    static const int32_t half_quarter_turn_{ 0x20000000 };
    static constexpr int32_t quarter_turn_{ half_quarter_turn_ * 2 };
    static constexpr int32_t degree_turn_{ (half_quarter_turn_ + 22) / 45 };
    static constexpr int64_t degrees_formula_{ 14680064 / 17578125 };

    void turn();
    void reset_angle();

    static int32_t convert_angle(uint32_t angle)
    {
        return constrain((((static_cast<int32_t>(angle) >> 16) * 360) >> 16),
                         -180, 180);
    }

private:
    void turn_adjust();

    uint32_t turn_angle_{ 0 };
    int16_t turn_rate_{ 0 };
    int16_t gyro_offset_{ 0 };
    uint16_t gyro_last_update_{ 0 };
};

#endif
