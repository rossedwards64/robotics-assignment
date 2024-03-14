#ifndef OBJECT_DETECTION_HANDLER_HPP_
#define OBJECT_DETECTION_HANDLER_HPP_

#include <stdint.h>

#include "Zumo32U4ProximitySensors.h"

class ObjectDetectionHandler : public Zumo32U4ProximitySensors
{
public:
    explicit ObjectDetectionHandler(uint8_t prox_thresh) noexcept
        : prox_thresh_(prox_thresh)
    {
        initThreeSensors();
    }

    explicit ObjectDetectionHandler() noexcept : ObjectDetectionHandler(4) {}

    bool object_seen() const;

private:
    uint8_t prox_thresh_;
};

#endif
