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
    // motor.initialise();
    border_detector.initThreeSensors();
    object_detector.initFrontSensor();
    motor.calibrate_turn(border_detector);
    ledYellow(false);
}

void intro()
{
    buzzer.play("!L16 V8 cdefgab>cbagfedc");
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

    // TODO: try left hand on the wall strategy for determining turn direction
    MotorHandler::Direction direction;
    if (border_detector.border_detected_left()) {
        direction = MotorHandler::Right;
    } else if (border_detector.border_detected_right()) {
        direction = MotorHandler::Left;
    } else if (border_detector.border_detected_middle()) {
        direction = MotorHandler::Rotate;
    } else {
        direction = MotorHandler::Straight;
    }
    motor.do_move(direction);
}

void attempt_delivery()
{
    display_on_lcd(String(object_detector.left()).c_str(),
                   String(object_detector.right()).c_str());
    if (object_detector.object_seen()) {
        MotorHandler::stop();
        deliveries++;
        display_on_lcd("Completed", "Delivery!!");
        delay(1000);
        display_on_lcd("Onto the", "next!");
        motor.go_back();
    }
}

void setup()
{
    Wire.begin();
    randomSeed(analogRead(0));
    intro();
}

void loop()
{
    avoid_border();
    attempt_delivery();
}
