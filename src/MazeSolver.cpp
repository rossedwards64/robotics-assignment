#include <Zumo32U4.h>

#include "MotorHandler.hpp"
#include "ObjectDetectionHandler.hpp"


Zumo32U4LCD display;
Zumo32U4Buzzer buzzer;
Zumo32U4ButtonA button_a;
Zumo32U4ButtonB button_b;
Zumo32U4ButtonC button_c;

MotorHandler motor;
ObjectDetectionHandler object_detector;
BorderDetectionHandler border_detector;

uint8_t deliveries{ 0 };

void avoid_border();
bool attempt_delivery();

void make_one_delivery()
{
    while (!attempt_delivery()) { avoid_border(); }
}

void make_multiple_deliveries()
{
    while (deliveries < 5) {
        avoid_border();
        attempt_delivery();
    }
}

// TODO
void map_maze() {}

void (*scenarios[3])(){ make_one_delivery, make_multiple_deliveries, map_maze };
void (*current_scenario)();

void display_on_lcd(const char *line1, const char *line2)
{
    display.clear();
    display.gotoXY(0, 0);
    display.print(line1);
    display.gotoXY(0, 1);
    display.print(line2);
}

void choose_scenario()
{
    bool button_pressed{ false };

    display.clear();
    do {
        if (button_a.isPressed()) {
            button_pressed = true;
            current_scenario = scenarios[0];
            display_on_lcd("Scenario", "1");
        } else if (button_b.isPressed()) {
            button_pressed = true;
            current_scenario = scenarios[1];
            display_on_lcd("Scenario", "2");
        } else if (button_c.isPressed()) {
            button_pressed = true;
            current_scenario = scenarios[2];
            display_on_lcd("Scenario", "3");
        } else {
            display_on_lcd("Choose a", "Scenario");
        }
        delay(200);
    } while (!button_pressed);
    delay(500);
}

void calibrate_sensors()
{
    ledYellow(true);
    border_detector.initThreeSensors();
    object_detector.initFrontSensor();
    motor.calibrate(border_detector);
    ledYellow(false);
}

void calibration()
{
    buzzer.play("O4 T100 V8 L4 MS g12>c12>e12>G6>E12 ML>G2");
    display.clear();
    display_on_lcd("Press A", "to calibrate");
    button_a.waitForButton();
    display_on_lcd("Calibrating", "Bot...");
    delay(500);
    calibrate_sensors();
    delay(1000);
    display_on_lcd("Done!", "");
}

void start_message()
{
    display.clear();
    display_on_lcd("Delivery", "Bot...");
    delay(500);
    display_on_lcd("Go!!", "");
}

void avoid_border()
{
    border_detector.read_sensors();

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

bool attempt_delivery()
{
    bool delivery_made{ false };
    object_detector.read_sensors();

    if (object_detector.object_seen()) {
        MotorHandler::stop();
        deliveries++;
        display_on_lcd("Completed", "Delivery!!");
        for (uint8_t i{ 0 }; i < 3; i++) {
            ledRed(true);
            ledRed(false);
        }
        delay(1000);
        delivery_made = true;
        display_on_lcd("Returning", "to Base");
        motor.go_back();
        display_on_lcd("Onto the", "next!");
    }
    return delivery_made;
}

void setup()
{
    Wire.begin();
    calibration();
}

void loop()
{
    choose_scenario();
    start_message();
    current_scenario();
}
