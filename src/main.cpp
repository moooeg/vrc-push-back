#include "main.h"
#include "autonomous.h"
#include "selector.h"
#include "global.h"

#include "lemlib/api.hpp" // IWYU pragma: keep



/**
 * Runs initialization code. This occurs as soon as the program is started.
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {

    chassis.calibrate(); // calibrate sensors
   
    selector::init();

    Auto1(); // temporary for calibrating the pid
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
        chassis.curvature(leftY, rightX);
        // delay to save resources
        pros::delay(10);
    }
}
