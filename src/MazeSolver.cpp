#include "Move.hpp"
#include <Wire.h>

#include "Stack.hpp"

const uint16_t border_thresh = 450;
const uint16_t prox_thresh = 4;

Zumo32U4LCD display;
Zumo32U4ButtonA button_a;

Zumo32U4ProximitySensors prox_sensors;

Zumo32U4LineSensors line_sensors;
uint16_t line_sensor_vals[3] = { 0, 0, 0 };
const int16_t last_error = 0;

Stack<Move::Direction, 100> steps;

Zumo32U4Buzzer buzzer;

void calibrate_sensors();
void detect_border();
void detect_object();
void display_on_lcd(const char *line1, const char *line2);


void setup()
{
    randomSeed(analogRead(0));
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

void loop()
{
    detect_border();
    // detect_object();
}

void calibrate_sensors()
{
    ledYellow(true);
    Wire.begin();
    prox_sensors.initThreeSensors();
    line_sensors.initThreeSensors();
    Move::imu.init();
    Move::imu.enableDefault();
    Move::imu.configureForTurnSensing();
    for (uint16_t i = 0; i < 120; i++) {
        Move::Direction direction;
        if (i > 30 && i <= 90) {
            direction = Move::Left;
        } else {
            direction = Move::Right;
        }
        Move(direction, 200).do_move(0);

        line_sensors.calibrate();
    }
    Move::stop();
    ledYellow(false);
}

void detect_border()
{
    line_sensors.read(static_cast<uint16_t *>(line_sensor_vals));
    Move::imu.readGyro();

    uint16_t left = line_sensor_vals[0];
    uint16_t middle = line_sensor_vals[1];
    uint16_t right = line_sensor_vals[2];

    Move::Direction direction;
    if (middle >= border_thresh) {
        direction = Move::Direction::Left;
    } else {
        direction = Move::Direction::Straight;
    }

    Move(direction).do_move(300);
}

void detect_object()
{
    prox_sensors.read();
    uint8_t left_value = prox_sensors.countsFrontWithLeftLeds();
    uint8_t right_value = prox_sensors.countsFrontWithRightLeds();

    bool object_seen = left_value >= prox_thresh || right_value >= prox_thresh;
}

void display_on_lcd(const char *line1, const char *line2)
{
    display.clear();
    display.gotoXY(0, 0);
    display.print(line1);
    display.gotoXY(0, 1);
    display.print(line2);
}
