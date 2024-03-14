#include "ObjectDetectionHandler.hpp"

bool ObjectDetectionHandler::object_seen() const
{
    return countsFrontWithLeftLeds() >= prox_thresh_
           || countsFrontWithRightLeds() >= prox_thresh_;
}
