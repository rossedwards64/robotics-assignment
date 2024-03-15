#include "ObjectDetectionHandler.hpp"


void ObjectDetectionHandler::read_sensors()
{
    read();
    left_ = countsFrontWithLeftLeds();
    right_ = countsFrontWithRightLeds();
}

bool ObjectDetectionHandler::object_seen() const
{
    return left_ >= prox_thresh_ || right_ >= prox_thresh_;
}
