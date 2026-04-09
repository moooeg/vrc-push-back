
#include "global.h"
#include "pros/adi.hpp"

void IntakeStart(int speed, bool holding, bool lowerGoal, bool matchloading) {

    // holder
    if (holding) {
        holder.extend();
        holderDown = true;
    } 
    else {
        holder.retract();
        holderDown = false;
    }

    // lower goal - opposite
    if (lowerGoal) {
        intakeLift.extend();
    }
    else {
        intakeLift.retract();
    }

    // matchload
    if (matchloading) {
        matchload.extend();
    } 
    else {
        matchload.retract();
    }

    intakeStage1.move(speed);
    intakeStage2.move(speed);
    intakeStage3.move(speed);
}

void IntakeStop() {
    intakeLift.retract();
    matchload.retract();

    intakeStage1.move(0);
    intakeStage2.move(0);
    intakeStage3.move(0);
}

// controller
pros::Controller controller(pros::E_CONTROLLER_MASTER);

// motor groups
pros::MotorGroup leftMotors({-1, 12, -13}, pros::MotorGearset::blue); // front left motor port 1 (reversed), middle left motor port 12, back left motor port 13 (reversed) 
pros::MotorGroup rightMotors({16, -11, 18}, pros::MotorGearset::blue); // front right motor port 16, middle right motor port 11 (reversed), back right motor port 18

// Inertial Sensor on port 17
pros::Imu imu(17);
// tracking wheels
// vertical tracking wheel encoder. Rotation sensor, port 20
pros::Rotation verticalEnc(20);
// vertical tracking wheel. 2" diameter, 0" offset
lemlib::TrackingWheel vertical(&verticalEnc, lemlib::Omniwheel::NEW_2, 0);
 
// drivetrain settings
lemlib::Drivetrain drivetrain(&leftMotors, // left motor group
                              &rightMotors, // right motor group
                              11.81, // 11.81 inch track width
                              lemlib::Omniwheel::NEW_325, // using new 3.25" omnis
                              450, // drivetrain rpm is 450
                              8 // horizontal drift is 8. 2 for full omni, 8 for traction
);

// lateral motion controller
lemlib::ControllerSettings linearController(
    10, // proportional gain (kP)
    0, // integral gain (kI)
    50, // derivative gain (kD)
    0, // anti windup
    1, // small error range, in inches
    100, // small error range timeout, in milliseconds
    3, // large error range, in inches
    5000, // large error range timeout, in milliseconds
    0 // maximum acceleration (slew)
);

// angular motion controller
lemlib::ControllerSettings angularController(
    2.4, // proportional gain (kP)
    0, // integral gain (kI)
    18.25, // derivative gain (kD)
    3, // anti windup
    1, // small error range, in inches
    100, // small error range timeout, in milliseconds
    3, // large error range, in inches
    500, // large error range timeout, in milliseconds
    0 // maximum acceleration (slew)
);

// sensors for odometry
lemlib::OdomSensors sensors(&vertical, // vertical tracking wheel
                            nullptr, // vertical tracking wheel 2, set to nullptr as we don't have a second one
                            nullptr, // horizontal tracking wheel
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);

// input curve for throttle input during driver control
lemlib::ExpoDriveCurve throttleCurve(3, // joystick deadband out of 127
                                     10, // minimum output where drivetrain will move out of 127
                                     1.019 // expo curve gain
);

// input curve for steer input during driver control
lemlib::ExpoDriveCurve steerCurve(3, // joystick deadband out of 127
                                  10, // minimum output where drivetrain will move out of 127
                                  1.019 // expo curve gain
);

// create the chassis
lemlib::Chassis chassis(drivetrain, linearController, angularController, sensors, &throttleCurve, &steerCurve);

//define motor ports
pros::Motor intakeStage1(10, pros::MotorGearset::green, pros::v5::MotorUnits::degrees); //stage 1 intake motor 11W green
pros::Motor intakeStage2(-14, pros::MotorGearset::green, pros::v5::MotorUnits::degrees); //stage 2 intake motor 5.5W
pros::Motor intakeStage3(15, pros::MotorGearset::green, pros::v5::MotorUnits::degrees); // stage 3 intake motor 5.5W

//define pneumatics 
pros::adi::Pneumatics matchload('E', false);
pros::adi::Pneumatics intakeLift('D', false);
pros::adi::Pneumatics descore('B', true);
pros::adi::Pneumatics holder('A', true);