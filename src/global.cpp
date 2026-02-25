
#include "global.h"

// controller
pros::Controller controller(pros::E_CONTROLLER_MASTER);

// motor groups
pros::MotorGroup rightMotors({9, -2, 3}, //front right motor port 1 (reversed), middle right motor port 2 (reversed), back right motor port 3 
                            pros::MotorGearset::green);
pros::MotorGroup leftMotors({-4, 5, -6}, pros::MotorGearset::green); //front right motor port 4, middle right motor port 5, back right motor port 6 (reversed) 

// Inertial Sensor on port 12
pros::Imu imu(12);
// tracking wheels
// vertical tracking wheel encoder. Rotation sensor, port 11
pros::Rotation verticalEnc(11);
// vertical tracking wheel. 2" diameter, 0" offset
lemlib::TrackingWheel vertical(&verticalEnc, lemlib::Omniwheel::NEW_2, 0);
 
// drivetrain settings
lemlib::Drivetrain drivetrain(&leftMotors, // left motor group
                              &rightMotors, // right motor group
                              11.9, // 11.9 inch track width
                              lemlib::Omniwheel::NEW_325, // using new 3.25" omnis
                              450, // drivetrain rpm is 450
                              8 // horizontal drift is 8. 2 for full omni, 8 for traction
);

// lateral motion controller
lemlib::ControllerSettings linearController(
                                            10, // proportional gain (kP)
                                            0, // integral gain (kI)
                                            3, // derivative gain (kD)
                                            3, // anti windup
                                            1, // small error range, in inches
                                            100, // small error range timeout, in milliseconds
                                            3, // large error range, in inches
                                            500, // large error range timeout, in milliseconds
                                            20 // maximum acceleration (slew)
);

// angular motion controller
lemlib::ControllerSettings angularController(
                                            2.53, // proportional gain (kP)
                                            0, // integral gain (kI)
                                            25, // derivative gain (kD)
                                            3, // anti windup
                                            1, // small error range, in degrees
                                            100, // small error range timeout, in milliseconds
                                            3, // large error range, in degrees
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
pros::Motor intakeStage1(13, pros::MotorGearset::green, pros::v5::MotorUnits::degrees); //stage 1 intake motor 11W green
pros::Motor intakeStage2(14, pros::MotorGearset::green, pros::v5::MotorUnits::degrees); //stage 2 intake motor 5.5W
pros::Motor intakeStage3(15, pros::MotorGearset::green, pros::v5::MotorUnits::degrees); // stage 3 intake motor 5.5W

//define pneumatics 
pros::adi::AnalogOut matchload('A');
pros::adi::AnalogOut intakeLift('B');
pros::adi::AnalogOut descore('C');
pros::adi::AnalogOut holder('D');

Devices devices({intakeStage1, intakeStage2, intakeStage3}, {matchload, intakeLift, descore, holder});