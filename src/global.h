#pragma once

#include "main.h"
#include "lemlib/api.hpp"
#include "pros/adi.hpp"
#include <string>
#include <vector>

class Devices {
public:

    std::vector<pros::Motor> motors;
    std::vector<pros::adi::Pneumatics> pneumatics;

    Devices(std::vector<pros::Motor> motors, std::vector<pros::adi::Pneumatics> pneumatics): motors(std::move(motors)), pneumatics(std::move(pneumatics)) {

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

inline bool tuning = false;
inline bool holderDown = false;


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
extern pros::adi::Pneumatics matchload;
extern pros::adi::Pneumatics intakeLift;
extern pros::adi::Pneumatics descore;
extern pros::adi::Pneumatics holder;

extern Devices devices;