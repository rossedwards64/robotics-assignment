#include "BorderDetectionHandler.hpp"


void BorderDetectionHandler::read_sensors()
{
    readCalibrated(static_cast<uint16_t *>(sensor_vals_));
}

bool BorderDetectionHandler::border_detected_left() const
{
    return left_sensor() >= side_border_thresh_;
}

bool BorderDetectionHandler::border_detected_middle() const
{
    return middle_sensor() >= mid_border_thresh_;
}

bool BorderDetectionHandler::border_detected_right() const
{
    return right_sensor() >= side_border_thresh_;
}

bool BorderDetectionHandler::border_detected_all() const
{
    return border_detected_left() && border_detected_middle()
           && border_detected_right();
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
