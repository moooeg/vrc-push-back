#include "main.h"
#include "autonomous.h"
#include "lemlib/asset.hpp"
#include "liblvgl/llemu.hpp"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/rtos.hpp"
#include "selector.h"
#include "global.h"

#include "lemlib/api.hpp" // IWYU pragma: keep
#include <cmath>
#include <string>

/**
 * Runs initialization code. This occurs as soon as the program is started.
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {

    // retract all pneumatics to be in starting size
    descore.retract();
    holder.retract();
    matchload.retract();
   
    // team selector
    selector::init();

    // init lcd for easy writing to screen
    pros::lcd::initialize();
    pros::lcd::set_text(1, selector::auton.asString());

    pros::lcd::set_text(1, std::to_string(chassis.getPose().x) + " " + std::to_string(chassis.getPose().y) + " " + std::to_string(chassis.getPose().theta));

    // if testing the position create a thread (never on in matches so thread doesn't intefere with robot not moving)
    if (testing) {
        pros::Task task{[=] {
            while (true) { 
                // print measurements
                pros::lcd::set_text(2, std::to_string(chassis.getPose().x) + " " + std::to_string(chassis.getPose().y) + " " + std::to_string(chassis.getPose().theta));
                pros::lcd::set_text(3, std::to_string(verticalEnc.get_position()));
                pros::delay(10); 
            }
        }};
    }


    // if tuning pid or testing auto without a board
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

    leftMotors.set_brake_mode_all(pros::E_MOTOR_BRAKE_HOLD);
    rightMotors.set_brake_mode_all(pros::E_MOTOR_BRAKE_HOLD);

    while (true) {
		//get controller values
		int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        int r1 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1);
        int r2 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2);
        int l2 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2);
        int l1 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1);
        
        int y = controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y);

        // move the chassis with curvature drive
        chassis.curvature(leftY, rightX);

        holderDown = true;

        // intake
        if (r1) { 
            intakeStage1.move_velocity(600);
            intakeStage2.move_velocity(600);
            intakeStage3.move_velocity(600);

            if (l1) holderDown = false;
            else holderDown = true;
        }
        else if (r2) { 
            int speed;

            // power or not power matchload
            if (l2) { matchload.extend(); speed = 600; }
            // intakeLift on outtake (reversed)
            else {matchload.retract(); intakeLift.extend(); speed = -600; }

            intakeStage1.move_velocity(speed);
            intakeStage2.move_velocity(speed);
            intakeStage3.move_velocity(speed);
        }
        else {
            intakeStage1.move_velocity(0);
            intakeStage2.move_velocity(0);
            intakeStage3.move_velocity(0);
        }

        if (!l2) {
            matchload.retract();
        }


        if (l2 && !r2) {
            holderDown = false;
        }
        
        // descore
        if (l1 && !r1 && !r2) {
            descore.retract();
        }
        else descore.extend(); 

        // center goal - opposite
        if (y) intakeLift.extend();
        else intakeLift.retract();

        // holder position
        if (holderDown) holder.extend();
        else holder.retract();

        // delay to save resources
        pros::delay(10);
    }
}
