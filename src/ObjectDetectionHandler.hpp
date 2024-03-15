#ifndef OBJECT_DETECTION_HANDLER_HPP_
#define OBJECT_DETECTION_HANDLER_HPP_

#include <stdint.h>
#include <Zumo32U4ProximitySensors.h>


class ObjectDetectionHandler : public Zumo32U4ProximitySensors
{
public:
    explicit ObjectDetectionHandler(uint8_t prox_thresh) noexcept
        : prox_thresh_(prox_thresh)
    {
    }

    explicit ObjectDetectionHandler() noexcept : ObjectDetectionHandler(4) {}

    uint8_t left() const { return left_; };
    uint8_t right() const { return right_; };
    void read_sensors();
    bool object_seen() const;

private:
    uint8_t prox_thresh_;

    uint8_t left_{};
    uint8_t right_{};
};

#endif
