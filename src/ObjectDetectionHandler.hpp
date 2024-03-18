#ifndef OBJECT_DETECTION_HANDLER_HPP_
#define OBJECT_DETECTION_HANDLER_HPP_

#include <stdint.h>
#include <Zumo32U4ProximitySensors.h>


class ObjectDetectionHandler : public Zumo32U4ProximitySensors
{
public:
    explicit ObjectDetectionHandler(uint8_t front_prox_thresh,
                                    uint8_t periph_prox_thresh) noexcept
        : front_prox_thresh_(front_prox_thresh),
          periph_prox_thresh_(periph_prox_thresh)
    {
    }

    explicit ObjectDetectionHandler() noexcept : ObjectDetectionHandler(6, 6) {}

    uint8_t periph_left() const { return periph_left_; }
    uint8_t left() const { return front_left_; };
    uint8_t right() const { return front_right_; };
    uint8_t periph_right() const { return periph_right_; }

    void read_sensors();

    bool object_seen() const;
    bool object_seen_peripheral() const;

private:
    uint8_t front_prox_thresh_;
    uint8_t periph_prox_thresh_;

    uint8_t periph_left_{};
    uint8_t front_left_{};
    uint8_t front_right_{};
    uint8_t periph_right_{};
};

#endif
