#include "BorderDetectionHandler.hpp"

uint16_t BorderDetectionHandler::border_thresh() const
{
    return border_thresh_;
}

void BorderDetectionHandler::read_sensors()
{
    readCalibrated(static_cast<uint16_t *>(sensor_vals_));
}

bool BorderDetectionHandler::border_detected_left() const
{
    return border_detected(left_sensor());
}

bool BorderDetectionHandler::border_detected_middle() const
{
    return border_detected(middle_sensor());
}

bool BorderDetectionHandler::border_detected_right() const
{
    return border_detected(right_sensor());
}

bool BorderDetectionHandler::border_detected(uint16_t sensor_val) const
{
    return sensor_val >= border_thresh_;
}

uint16_t BorderDetectionHandler::left_sensor() const { return sensor_vals_[0]; }

uint16_t BorderDetectionHandler::middle_sensor() const
{
    return sensor_vals_[1];
}

uint16_t BorderDetectionHandler::right_sensor() const
{
    return sensor_vals_[2];
}
