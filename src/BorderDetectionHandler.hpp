#ifndef LINE_HANDLER_HPP_
#define LINE_HANDLER_HPP_

#include <stdint.h>
#include <Zumo32U4LineSensors.h>


class BorderDetectionHandler : public Zumo32U4LineSensors
{
public:
    explicit BorderDetectionHandler(uint16_t border_thresh) noexcept
        : border_thresh_(border_thresh)
    {
        initFiveSensors();
        calibrate();
    }

    explicit BorderDetectionHandler() noexcept : BorderDetectionHandler(450){};

    BorderDetectionHandler(const BorderDetectionHandler &) = delete;
    BorderDetectionHandler(BorderDetectionHandler &&) = delete;
    BorderDetectionHandler &operator=(const BorderDetectionHandler &) = delete;
    BorderDetectionHandler &operator=(BorderDetectionHandler &&) = delete;

    virtual ~BorderDetectionHandler() = default;

    uint16_t border_thresh() const;

    void read_sensors();

    bool border_detected_left() const;
    bool border_detected_middle() const;
    bool border_detected_right() const;

private:
    uint16_t border_thresh_;
    uint16_t sensor_vals_[3]{};

    bool border_detected(uint16_t sensor_val) const;

    uint16_t left_sensor() const;
    uint16_t middle_sensor() const;
    uint16_t right_sensor() const;
};

#endif
