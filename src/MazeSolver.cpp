#include <Wire.h>

#include "MotorHandler.hpp"

const uint16_t border_thresh{ 450 };
const uint16_t prox_thresh{ 4 };

Zumo32U4LCD display;
Zumo32U4ButtonA button_a;

MotorHandler motor;

Zumo32U4ProximitySensors prox_sensors;

Zumo32U4LineSensors line_sensors;
uint16_t line_sensor_vals[3]{ 0, 0, 0 };
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
    Wire.begin();
    prox_sensors.initThreeSensors();
    line_sensors.initThreeSensors();
    for (uint16_t i{ 0 }; i < 120; i++) {
        MotorHandler::Direction direction;
        if (i > 30 && i <= 90) {
            direction = MotorHandler::Left;
        } else {
            direction = MotorHandler::Right;
        }
        MotorHandler(direction, 200).do_move(0);

        line_sensors.calibrate();
    }
    MotorHandler::stop();
    ledYellow(false);
}

void introduction()
{

    buzzer.play("!L16 V8 cdefgab>cbagfedc");
    display.clear();
    display_on_lcd("Press A", "to start");
    button_a.waitForButton();
    display_on_lcd("This is", "Robotics");
    delay(500);
    calibrate_sensors();
    delay(1000);
    display_on_lcd("Go!!", "");
}

void detect_border()
{
    line_sensors.read(static_cast<uint16_t *>(line_sensor_vals));

    uint16_t left{ line_sensor_vals[0] };
    uint16_t middle{ line_sensor_vals[1] };
    uint16_t right{ line_sensor_vals[2] };

    MotorHandler::Direction direction;
    if (middle >= border_thresh) {
        direction = MotorHandler::Direction::Left;
    } else {
        direction = MotorHandler::Direction::Straight;
    }

    MotorHandler(direction).do_move(300);
}

void detect_object()
{
    prox_sensors.read();
    uint8_t left_value{ prox_sensors.countsFrontWithLeftLeds() };
    uint8_t right_value{ prox_sensors.countsFrontWithRightLeds() };

    bool object_seen{ left_value >= prox_thresh || right_value >= prox_thresh };
}


void setup()
{
    randomSeed(analogRead(0));
    introduction();
}

void loop()
{
    detect_border();
    // detect_object();
}
