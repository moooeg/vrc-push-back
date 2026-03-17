#include "main.h"
#include "autonomous.h"
#include "liblvgl/llemu.hpp"
#include "pros/misc.h"
#include "pros/rtos.hpp"
#include "selector.h"
#include "global.h"

#include "lemlib/api.hpp" // IWYU pragma: keep
#include <string>

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
    chassis.setPose(0, 0, 0);
    pros::lcd::set_text(2, std::to_string(chassis.getPose().x) + " " + std::to_string(chassis.getPose().y) + " " + std::to_string(chassis.getPose().theta));

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

    descore.retract();
    holder.retract();
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

void waitTillReleased(void* param) {
    int button = (int) param;

    while (button) pros::delay(500);

    holderToggle = false;
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
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        int r1 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1);
        int r2 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2);
        int l1 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1);
        int l2 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2);
        
        int y = controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y);

        // move the chassis with curvature drive
        chassis.curvature(leftY, rightX);

        // holder toggle
        if (l2 && !r2) {
            holder.toggle();
            
            if (holderToggle) continue;
            else holderToggle = true;

            if (holder.is_extended()) {
                holderToggleOverwrite = true;
            } else holderToggleOverwrite = false;

            pros::Task wait(waitTillReleased, (void*) l2);
            continue;
        }

        // intake
        if (r1) { 
            intakeStage1.move_velocity(100);
            intakeStage2.move_velocity(100);
            intakeStage3.move_velocity(100);

            if (l1) { holder.retract(); }
        }
        else if (r2) { 
            int speed;

            // power or not power matchload
            if (l2) { matchload.extend(); speed = 100; }
            else matchload.retract(); speed = -100;

            intakeStage1.move_velocity(speed);
            intakeStage2.move_velocity(speed);
            intakeStage3.move_velocity(speed);
        }
        else {
            intakeStage1.move_velocity(0);
            intakeStage2.move_velocity(0);
            intakeStage3.move_velocity(0);
        }

        // undo holder if normal
        if (!l1 && !holderToggleOverwrite) holder.extend();
        
        // descore
        if (l1 && !r1) {
            descore.retract();
        }
        else descore.extend(); 

        // double parking
        if (y) intakeLift.toggle();

        // delay to save resources
        pros::delay(10);
    }
}
