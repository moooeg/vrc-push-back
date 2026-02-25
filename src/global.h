#pragma once

#include "main.h"
#include "lemlib/api.hpp"
#include <string>
#include <vector>

class Devices {
public:

    std::vector<pros::Motor> motors;
    std::vector<pros::adi::AnalogOut> analogOuts;

    Devices(std::vector<pros::Motor> motors, std::vector<pros::adi::AnalogOut> analogOuts): motors(std::move(motors)), analogOuts(std::move(analogOuts)) {

    }
};

class TeamPosition {
public:

    std::string team;
    std::string position;

    std::string asString() {
        return team + "_" + position;
    }
};

// controller
extern pros::Controller controller;

// motor groups
extern pros::MotorGroup rightMotors;
extern pros::MotorGroup leftMotors;

// Inertial Sensor on port 12
extern pros::Imu imu;
// tracking wheels
// vertical tracking wheel encoder. Rotation sensor, port 11
extern pros::Rotation verticalEnc;
// vertical tracking wheel. 2" diameter, 0" offset
extern lemlib::TrackingWheel vertical;
 
// drivetrain settings
extern lemlib::Drivetrain drivetrain;

// lateral motion controller
extern lemlib::ControllerSettings linearController;

// angular motion controller
extern lemlib::ControllerSettings angularController;

// sensors for odometry
extern lemlib::OdomSensors sensors;

// input curve for throttle input during driver control
extern lemlib::ExpoDriveCurve throttleCurve;

// input curve for steer input during driver control
extern lemlib::ExpoDriveCurve steerCurve;

// create the chassis
extern lemlib::Chassis chassis;

//define motor ports
extern pros::Motor intakeStage1;
extern pros::Motor intakeStage2;
extern pros::Motor intakeStage3;

//define pneumatics 
extern pros::adi::AnalogOut matchload;
extern pros::adi::AnalogOut intakeLift;
extern pros::adi::AnalogOut descore;
extern pros::adi::AnalogOut holder;

extern Devices devices;