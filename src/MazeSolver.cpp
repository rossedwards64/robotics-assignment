#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++11-narrowing"
#include <Zumo32U4.h>
#pragma clang diagnostic pop

#include "MotorHandler.hpp"
#include "ObjectDetectionHandler.hpp"


Zumo32U4LCD display;
Zumo32U4ButtonA button_a;

MotorHandler motor;

ObjectDetectionHandler object_detector;

BorderDetectionHandler border_detector;

const int16_t last_error{ 0 };

Zumo32U4Buzzer buzzer;

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
    border_detector.initialise();
    motor.initialise();
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
    if (border_detector.border_detected_middle()) {
        ledGreen(true);
        direction = MotorHandler::Direction::Left;
        ledGreen(false);
    } else {
        direction = MotorHandler::Direction::Straight;
    }
    motor.do_move(direction, 300);
}

void avoid_object() { object_detector.object_seen(); }


void setup()
{
    Wire.begin();
    randomSeed(analogRead(0));
    intro();
}

void loop()
{
    avoid_border();
    avoid_object();
}
