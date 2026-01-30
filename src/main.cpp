#include "main.h"
#include "autonomous.h"

#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/apix.h"
#include "pros/adi.h"

#include <map>

// controller
pros::Controller controller(pros::E_CONTROLLER_MASTER);

// motor groups
pros::MotorGroup rightMotors({-1, -2, 3}, //front right motor port 1 (reversed), middle right motor port 2 (reversed), back right motor port 3 
                            pros::MotorGearset::green);
pros::MotorGroup leftMotors({4, 5, -6}, pros::MotorGearset::green); //front right motor port 4, middle right motor port 5, back right motor port 6 (reversed) 

// Inertial Sensor on port 11
pros::Imu imu(7);
// tracking wheels
// vertical tracking wheel encoder. Rotation sensor, port 9
pros::Rotation verticalEnc(8);
// vertical tracking wheel. 2" diameter, 2.5" offset, left of the robot (negative)
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
lemlib::ControllerSettings linearController(10, // proportional gain (kP)
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
lemlib::ControllerSettings angularController(2, // proportional gain (kP)
                                             0, // integral gain (kI)
                                             10, // derivative gain (kD)
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

std::map<std::string, std::map<std::string, ButtonPosition>> GUI_BUTTON_POSITIONS = {
    {
        "top", {
            { "1", ButtonPosition(139, 8, 240, 26) },
            { "2", ButtonPosition(249, 8, 351, 26) },
            { "3", ButtonPosition(358, 8, 461, 26) }
        }
    },
    {
        "bottom", {
            { "1", ButtonPosition(19, 52, 138, 73)},
            { "2", ButtonPosition(19, 85, 138, 107) },
            { "3", ButtonPosition(19, 120, 138, 142) }
        }
    }
};

void TeamChoosing() {

    bool confirmed = false;

    while (true) {
        pros::delay(5);

        // exit
        if (confirmed) {

            if (position.asString() == "blue_1") {
                pros::screen::fill_circle(160, 120, 40);
            } else if (position.asString() == "blue_2") {
                pros::screen::fill_circle(160, 120, 40);
                pros::screen::fill_circle(320, 120, 40);
            }
            else if (position.asString() == "red_1") {
                pros::screen::draw_circle(160, 120, 40);
            }
            else if (position.asString() == "red_2") {
                pros::screen::draw_circle(160, 120, 40);
                pros::screen::draw_circle(320, 120, 40);
            }
            else if (position.asString() == "skill_") {
                pros::screen::fill_circle(160, 120, 40);
                pros::screen::draw_circle(320, 120, 40);
            }

            return;
        }

        // controller
        if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
            position.team = "red";
            position.position = "1";
            confirmed = true;
        }
        else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
            position.team = "red";
            position.position = "2";
            confirmed = true;
        }
        else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
            position.team = "blue";
            position.position = "1";
            confirmed = true;
        }
        else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
            position.team = "blue";
            position.position = "2";
            confirmed = true;
        }
        else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
            position.team = "skill";
            confirmed = true;
        }
    }
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {

    chassis.calibrate(); // calibrate sensors
   
    TeamChoosing();

    //define motor ports
    pros::Motor intakeStage1(9, pros::MotorGearset::green, pros::v5::MotorUnits::degrees); //stage 1 intake motor 11W green
    pros::Motor intakeStage2(10, pros::MotorGearset::green, pros::v5::MotorUnits::degrees); //stage 2 intake motor 5.5W
    pros::Motor intakeStage3(11, pros::MotorGearset::green, pros::v5::MotorUnits::degrees); // stage 3 intake motor 5.5W

    //define pneumatics 
    pros::adi::AnalogOut matchload = pros::adi::AnalogOut('A');
    pros::adi::AnalogOut intakeLift = pros::adi::AnalogOut('B');
    pros::adi::AnalogOut descore = pros::adi::AnalogOut('C');
    pros::adi::AnalogOut holder = pros::adi::AnalogOut('D');
}

/**
 * Runs while the robot is disabled
 */
void disabled() {}

/**
 * runs after initialize if the robot is connected to field control
 */
void competition_initialize() {

    chassis.calibrate(); // calibrate sensors
}

/**
 * Runs during auto
 *
 */

void autonomous() {

	if (position.position == "1") {
        Auto1(&chassis);
    } else if (position.position == "2") {
        Auto2(&chassis);
    }
}

/**
 * Runs in driver control
 */
void opcontrol() {
    // controller
    // loop to continuously update motors
    while (true) {
		//get joystick values
		int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        // move the chassis with curvature drive
        chassis.arcade(leftY, rightX);
        // delay to save resources
        pros::delay(10);
    }
}
