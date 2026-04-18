
#include "autonomous.h"

#include "global.h"
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/pose.hpp"
#include "pros/rtos.hpp"
#include <cmath>

void SoloAutonomous() {
    chassis.setPose(0, 0, 270);
    chassis.moveToPoint(5, 0, 1000);
}

void Auto1() { 
    // start intaking normally then drive to point. as moving runs on own thread delay will run during the second move to point. after 900ms extend the matchload to catch the blocks while still moving
    IntakeStart();
    chassis.moveToPoint(-3, 17, 2000, {.minSpeed = 72, .earlyExitRange = 8});
    chassis.moveToPoint(-6, 45, 2000, {.maxSpeed = 32});
    pros::delay(900);
    matchload.extend();
    // turn to face the general direction of the matchload then wait for blocks to settle and do a quick outtake then intake to fix any jams
    chassis.turnToHeading(chassis.getPose().theta - 155, 1000, {.maxSpeed = 100});
    pros::delay(500);
    IntakeStart(-127, true, false, true);
    // drive to matchload
    chassis.moveToPose(-33, 20, 180, 1500, {.lead = 0.1, .maxSpeed = 100});
    pros::delay(100);
    // normal intake with matchload down (go back and forth)
    IntakeStart(127, true, false, true);
    chassis.moveToPoint(-33, 0, 300, {.maxSpeed = 64});
    chassis.moveToPoint(-33, chassis.getPose().y - 10, 300, {.forwards = false});
    chassis.moveToPoint(-33, 0, 300, {.maxSpeed = 64});
    pros::delay(1500);
    // go to long goal and make sure fully pressed up
    chassis.moveToPoint(-33, 45, 1500, {.forwards = false});
    while (chassis.isInMotion())
    {
        pros::delay(10);
    }
    // score for 1.5 seconds
    holder.retract();
    pros::delay(1500);
    holder.extend();
    // set the chassis x to 0 in case of drifting
    chassis.setPose(0, chassis.getPose().y, 0); // set x and theta to be 0
    // outtake then intake incase stuck.
    IntakeStart(-127, true, false, true);
    chassis.moveToPoint(0, chassis.getPose().y - 15, 500);
    IntakeStart(127, true, false, true);
    // rescore some blocks
    holder.retract();
    chassis.moveToPoint(0, 45, 500, {.forwards = false});
    while (chassis.isInMotion())
    {
        pros::delay(10);
    }
    holder.extend();
    matchload.retract();
    pros::delay(1500);
    // put hook down and push to middle
    chassis.moveToPoint(0, 25, 2000);
    descore.retract();
    chassis.moveToPose(10, 60, 0, 3000, {.forwards = false, .lead = 0.8});
    IntakeStop();
}

void Auto2() {

}

void Skills() {
    
}