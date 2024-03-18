#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++11-narrowing"
#include <Zumo32U4.h>
#pragma clang diagnostic pop

#include "MotorHandler.hpp"
#include "ObjectDetectionHandler.hpp"


Zumo32U4LCD display;
Zumo32U4Buzzer buzzer;
Zumo32U4ButtonA button_a;

MotorHandler motor;
ObjectDetectionHandler object_detector;
BorderDetectionHandler border_detector;

uint8_t deliveries{ 0 };

void display_on_lcd(const char *line1, const char *line2)
{
    display.clear();
    display.gotoXY(0, 0);
    display.print(line1);
    display.gotoXY(0, 1);
    display.print(line2);
}

void calibrate_sensors()
{
    ledYellow(true);
    border_detector.initThreeSensors();
    object_detector.initFrontSensor();
    motor.calibrate(border_detector);
    ledYellow(false);
}

void intro()
{
    buzzer.play("O4 T100 V8 L4 MS g12>c12>e12>G6>E12 ML>G2");
    display.clear();
    display_on_lcd("Press A", "to start");
    button_a.waitForButton();
    display_on_lcd("Delivery", "Service...");
    delay(500);
    calibrate_sensors();
    delay(1000);
    display_on_lcd("Go!!", "");
}

void avoid_border()
{
    border_detector.read_sensors();

    display_on_lcd(String(border_detector.middle_sensor()).c_str(), "");

    // TODO: try left hand on the wall strategy for determining turn
    // direction
    MotorHandler::Direction direction;
    if (border_detector.border_detected_left()) {
        direction = MotorHandler::Right;
    } else if (border_detector.border_detected_right()) {
        direction = MotorHandler::Left;
    } else if (border_detector.border_detected_middle()) {
        if (border_detector.left_sensor() > border_detector.right_sensor()) {
            direction = MotorHandler::FlipRight;
        } else if (border_detector.left_sensor()
                   == border_detector.right_sensor()) {
            direction = MotorHandler::Reverse;
        } else {
            direction = MotorHandler::FlipLeft;
        }
    } else if (border_detector.border_detected_all()) {
        direction = MotorHandler::Reverse;
    } else {
        direction = MotorHandler::Straight;
    }
    motor.move(direction);
}

void attempt_delivery()
{
    if (object_detector.object_seen()) {
        MotorHandler::stop();
        deliveries++;
        display_on_lcd("Completed", "Delivery!!");
        ledRed(false);
        for (uint8_t i{ 0 }; i < 3; i++) {
            ledRed(true);
            ledRed(false);
        }
        delay(1000);
        display_on_lcd("Onto the", "next!");
        motor.go_back();
    }
}

void setup()
{
    Wire.begin();
    randomSeed(static_cast<uint16_t>(millis()));
    intro();
}

void loop()
{
    avoid_border();
    attempt_delivery();
}
