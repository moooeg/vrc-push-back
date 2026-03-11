#include "main.h"
#include "autonomous.h"
#include "liblvgl/llemu.hpp"
#include "selector.h"
#include "global.h"

#include "lemlib/api.hpp" // IWYU pragma: keep

/**
 * Runs initialization code. This occurs as soon as the program is started.
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
   
    selector::init();

    pros::lcd::initialize();
    pros::lcd::set_text(1, selector::auton.asString());

    chassis.calibrate();

    if (tuning) Auto1(); // pid temporary.
}

/**
 * Runs while the robot is disabled
 */
void disabled() {}

/**
 * runs after initialize if the robot is connected to field control
 */
void competition_initialize() {

    chassis.calibrate(); // recalibrate sensors incase robot has moved between being placed on the field and plugged in.
}

/**
 * Runs during auto
 *
 */

void autonomous() {
    if (selector::auton.position == "1") Auto1();
    else if (selector::auton.team == "solo") SoloAutonomous();
    else if (selector::auton.team == "skills") Skills();
    else Auto2();
}

/**
 * Runs in driver control
 */
void opcontrol() {
    // controller
    // loop to continuously update motors

    if (tuning) return;

    while (true) {
		//get controller values
		int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = -controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        int r1 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1);
        int r2 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2);

        // move the chassis with curvature drive
        chassis.curvature(leftY, rightX);

        // intake
        if (r1) { 
            intakeStage1.move_velocity(100);
            intakeStage2.move_velocity(100);
            intakeStage3.move_velocity(100);
        }
        else if (r2) { 
            intakeStage1.move_velocity(-100);
            intakeStage2.move_velocity(-100);
            intakeStage3.move_velocity(-100);
        }
        else {
            intakeStage1.move_velocity(0);
            intakeStage2.move_velocity(0);
            intakeStage3.move_velocity(0);
        }

        // delay to save resources
        pros::delay(10);
    }
}
