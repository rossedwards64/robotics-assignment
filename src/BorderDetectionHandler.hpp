#ifndef LINE_HANDLER_HPP_
#define LINE_HANDLER_HPP_

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++11-narrowing"
#include <stdint.h>
#include <Zumo32U4LineSensors.h>
#include <Zumo32U4LCD.h>
#pragma clang diagnostic pop


class BorderDetectionHandler : public Zumo32U4LineSensors
{
public:
    explicit BorderDetectionHandler(uint16_t mid_border_thresh,
                                    uint16_t side_border_thresh_) noexcept
        : mid_border_thresh_(mid_border_thresh),
          side_border_thresh_(side_border_thresh_)
    {
    }

    explicit BorderDetectionHandler() noexcept
        : BorderDetectionHandler(500, 500){};

    BorderDetectionHandler(const BorderDetectionHandler &) = delete;
    BorderDetectionHandler(BorderDetectionHandler &&) = delete;
    BorderDetectionHandler &operator=(const BorderDetectionHandler &) = delete;
    BorderDetectionHandler &operator=(BorderDetectionHandler &&) = delete;

    virtual ~BorderDetectionHandler() = default;

    void read_sensors();

    bool border_detected_left() const;
    bool border_detected_middle() const;
    bool border_detected_right() const;
    bool border_detected_all() const;

    uint16_t left_sensor() const;
    uint16_t middle_sensor() const;
    uint16_t right_sensor() const;

private:
    uint16_t mid_border_thresh_;
    uint16_t side_border_thresh_;
    uint16_t sensor_vals_[3]{};

    Zumo32U4LCD display{};
};

#endif
