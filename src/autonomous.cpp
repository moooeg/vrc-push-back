
#include "autonomous.h"

#include "global.h"
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/pose.hpp"
#include "pros/motors.h"
#include "pros/rtos.hpp"
#include <cmath>

void SoloAutonomous() {
    chassis.moveToPoint(0, 3, 500);
}

void Auto1() {
    leftMotors.set_brake_mode_all(pros::E_MOTOR_BRAKE_COAST);
    rightMotors.set_brake_mode_all(pros::E_MOTOR_BRAKE_COAST);
    chassis.setPose(0, 0, 0);
    // start intaking normally then drive to point. as moving runs on own thread delay will run during the second move to point. after 900ms extend the matchload to catch the blocks while still moving
    IntakeStart();
    chassis.moveToPoint(-3, 17, 2000, {.minSpeed = 72, .earlyExitRange = 8});
    chassis.moveToPoint(-6, 45, 2000, {.maxSpeed = 32});
    pros::delay(900);
    matchload.extend();
    // turn to face the general direction of the matchload then wait for blocks to settle and do a quick outtake then intake to fix any jams
    chassis.turnToHeading(chassis.getPose().theta - 155, 1000, {.maxSpeed = 100});
    pros::delay(500);
    // drive to matchload
    chassis.moveToPose(-32.5, 20, 180, 1500, {.lead = 0.1, .maxSpeed = 100});
    pros::delay(100);
    // normal intake with matchload down (go back and forth)
    chassis.moveToPoint(-32.5, 0, 1500, {.maxSpeed=50});
    pros::delay(1300);
    // go to long goal and make sure fully pressed up
    chassis.moveToPoint(-30.5, 50, 2500, {.forwards = false, .maxSpeed=70});
    pros::delay(750);
    IntakeStart(127, false, false, true);
    while (chassis.isInMotion())
    {
        pros::delay(10);
    }
    // set the chassis x to 0 in case of drifting
    chassis.setPose(0, 0, 0);
    // score for 1.5 seconds
    pros::delay(500);
    // outtake then intake incase stuck
    IntakeStart(-127, false, false, true);
    pros::delay(250);
    IntakeStart(127, false, false, true);
    pros::delay(750);
    IntakeStart(0, true);
    // put hook down and push to middle
    /*
    chassis.moveToPoint(0, 10, 2000);
    descore.retract();
    chassis.moveToPose(-9, -15.5, 0, 6000, {.forwards = false, .lead = 0.8, .maxSpeed=100});
    IntakeStop();
    */
}

void Auto2() {
    leftMotors.set_brake_mode_all(pros::E_MOTOR_BRAKE_COAST);
    rightMotors.set_brake_mode_all(pros::E_MOTOR_BRAKE_COAST);
    chassis.setPose(0, 0, 0);
    // start intaking normally then drive to point. as moving runs on own thread delay will run during the second move to point. after 900ms extend the matchload to catch the blocks while still moving
    IntakeStart();
    chassis.moveToPoint(3, 17, 2000, {.minSpeed = 72, .earlyExitRange = 8});
    chassis.moveToPoint(6, 45, 2000, {.maxSpeed = 32});
    pros::delay(900);
    matchload.extend();
    // turn to face the general direction of the matchload then wait for blocks to settle and do a quick outtake then intake to fix any jams
    chassis.turnToHeading(chassis.getPose().theta + 155, 1000, {.maxSpeed = 100});
    pros::delay(500);
    // drive to matchload
    chassis.moveToPose(32.5, 20, 180, 1500, {.lead = 0.1, .maxSpeed = 100});
    pros::delay(100);
    // normal intake with matchload down (go back and forth)
    chassis.moveToPoint(32.5, 0, 2000, {.maxSpeed=50});
    pros::delay(450);
    IntakeStart(127, true, false, true);
    pros::delay(450);
    // go to long goal and make sure fully pressed up
    chassis.moveToPoint(32.5, 45, 2000, {.forwards = false, .maxSpeed=80});
    pros::delay(500);
    holder.retract();
    while (chassis.isInMotion())
    {
        pros::delay(10);
    }
    // set the chassis x to 0 in case of drifting
    chassis.setPose(0, 0, 0);
    // score for 1.5 seconds
    pros::delay(500);
    // outtake then intake incase stuck
    IntakeStart(-127, false, false, true);
    pros::delay(750);
    IntakeStart(127, false, false, false);
    pros::delay(1500);
    holder.extend();
    // put hook down and push to middle
    chassis.moveToPoint(0, 10, 2000);
    descore.retract();
    chassis.moveToPose(-10, -17, -10, 6000, {.forwards = false, .lead = 0.8});
    IntakeStop();
}

void Skills() {
    
}