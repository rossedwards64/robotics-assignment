#include "Arduino.h"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++11-narrowing"
#include <Zumo32U4.h>
#include <stdint.h>
#include <stdio.h>
#pragma clang diagnostic pop


const uint16_t border_thresh = 450;
const uint16_t prox_thresh = 4;

Zumo32U4LCD display;
Zumo32U4ButtonA button_a;

Zumo32U4ProximitySensors prox_sensors;

Zumo32U4LineSensors line_sensors;
uint16_t line_sensor_vals[3] = { 0, 0, 0 };
const int16_t last_error = 0;

Zumo32U4Buzzer buzzer;

void calibrate_sensors();
void detect_border();
void detect_object();
void display_on_lcd(const char *line1, const char *line2);

class Move
{
public:
    enum Direction : uint8_t {
        Left,
        Right,
        Straight,
        Reverse,
        SpinLeft,
        SpinRight
    };

    constexpr Move(Direction direction, int16_t max_speed)
        : max_speed(max_speed),
          direction(direction),
          turn_speed(max_speed / 2),
          turning_left(false),
          turning_right(false)
    {
    }

    constexpr explicit Move(Direction direction) : Move(direction, 100) {}

    auto do_move() -> Direction
    {
        switch (direction) {
        case Left: turn_left(); break;
        case Right: turn_right(); break;
        case Straight:
            motors.setSpeeds(turn_speed, turn_speed);
            turning_left = false;
            turning_right = false;
            break;
        case Reverse:
            motors.setSpeeds(-turn_speed, -turn_speed);
            turning_left = false;
            turning_right = false;
            break;
        case SpinLeft:
            set_speed(max_speed);
            for (uint16_t i = 0; i < 60; i++) { turn_left(); }
            break;
        case SpinRight:
            set_speed(max_speed);
            turn_speed = max_speed;
            for (uint16_t i = 0; i < 60; i++) { turn_right(); }
            break;
        }

        return direction;
    }

    void set_speed(int16_t turn_speed)
    {
        this->turn_speed = constrain(turn_speed, 0, max_speed);
    }

    static void stop() { motors.setSpeeds(0, 0); }

    static const Zumo32U4Motors motors;

private:
    void turn_left()
    {
        motors.setSpeeds(-turn_speed, turn_speed);
        turning_left = true;
        turning_right = false;
    }

    void turn_right()
    {
        motors.setSpeeds(turn_speed, -turn_speed);
        turning_left = false;
        turning_right = true;
    }

    static const uint16_t move_time = 600;
    static const uint16_t min_turn_speed = 300;
    static const uint16_t accel = 10;
    static const uint16_t decel = 10;

    int16_t max_speed;
    Direction direction;

    int16_t turn_speed;
    bool turning_left;
    bool turning_right;
};

void setup()
{
    buzzer.play("!L16 V8 cdefgab>cbagfedc");

    display.clear();
    display_on_lcd("Press A", "to start");
    button_a.waitForButton();
    display_on_lcd("This is", "Robotics");

    calibrate_sensors();
    delay(1000);
}

void loop()
{
    detect_border();
    // detect_object();
}

void calibrate_sensors()
{
    ledYellow(true);
    prox_sensors.initThreeSensors();
    line_sensors.initThreeSensors();
    for (uint16_t i = 0; i < 120; i++) {
        Move::Direction direction;
        if (i > 30 && i <= 90) {
            direction = Move::Left;
        } else {
            direction = Move::Right;
        }
        Move(direction, 200).do_move();

        line_sensors.calibrate();
    }
    Move::stop();
    ledYellow(false);
}

void detect_border()
{
    line_sensors.read(static_cast<uint16_t *>(line_sensor_vals));

    uint16_t left = line_sensor_vals[0];
    uint16_t middle = line_sensor_vals[1];
    uint16_t right = line_sensor_vals[2];

    display_on_lcd(String(left).c_str(), String(middle).c_str());

    Move::Direction direction;
    if (left >= border_thresh) {
        direction = Move::Direction::Right;
    } else if (right >= border_thresh) {
        direction = Move::Direction::Left;
    } else if (middle >= border_thresh) {
        direction = Move::Direction::Reverse;
    } else {
        direction = Move::Direction::Straight;
    }

    Move(direction).do_move();
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
