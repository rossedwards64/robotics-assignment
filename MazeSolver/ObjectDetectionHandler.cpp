#include "ObjectDetectionHandler.hpp"


void ObjectDetectionHandler::read_sensors()
{
    read();
    // periph_left_ = countsLeftWithLeftLeds();
    front_left_ = countsFrontWithLeftLeds();
    front_right_ = countsFrontWithRightLeds();
    // periph_right_ = countsRightWithRightLeds();
}

bool ObjectDetectionHandler::object_seen() const
{
    return front_left_ >= front_prox_thresh_
           || front_right_ >= front_prox_thresh_;
}

bool ObjectDetectionHandler::object_seen_peripheral() const
{
    return periph_left_ >= periph_prox_thresh_
           || periph_right_ >= periph_prox_thresh_;
}
