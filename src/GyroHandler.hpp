#ifndef GYRO_HANDLER_HPP_
#define GYRO_HANDLER_HPP_

#include <Arduino.h>
#include <Wire.h>
#include <stdint.h>
#include <Zumo32U4IMU.h>


/* initialisation routine, turn values and turn adjustment routine obtained from
 * the Zumo MazeSolver example.
 */
class GyroHandler : public Zumo32U4IMU
{
public:
    GyroHandler()
    {
        Wire.begin();
        init();
        enableDefault();
        configureForTurnSensing();
    };

    static const int32_t angle_45_degrees_{ 0x20000000 };
    static constexpr int32_t angle_90_degrees_{ angle_45_degrees_ * 2 };
    static constexpr int32_t angle_1_degree_{ (angle_45_degrees_ + 22) / 45 };

    void calibrate();
    void turn();
    void reset_angle();

    static int64_t gyro_digits_to_degrees(uint32_t gyro_digits)
    {
        return static_cast<int64_t>(gyro_digits) * 14680064 / 17578125;
    }

    static int32_t convert_angle(uint32_t angle)
    {
        return constrain((((static_cast<int32_t>(angle) >> 16) * 360) >> 16),
                         -180, 180);
    }

private:
    void blocking_read();
    void turn_adjust();

    uint32_t turn_angle_{ 0 };
    int16_t turn_rate_{ 0 };
    int16_t gyro_offset_{ 0 };
    uint16_t gyro_last_update_{ 0 };
};

#endif
