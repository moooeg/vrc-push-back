
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
    // drive to matchload
    chassis.moveToPose(-32.5, 20, 180, 1500, {.lead = 0.1, .maxSpeed = 100});
    pros::delay(100);
    // normal intake with matchload down (go back and forth)
    chassis.moveToPoint(-32.5, 0, 2000, {.maxSpeed=50});
    pros::delay(900);
    IntakeStart(127, true, false, true);
    pros::delay(1500);
    // go to long goal and make sure fully pressed up
    chassis.moveToPoint(-32.5, 45, 2000, {.forwards = false, .maxSpeed=80});
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
    IntakeStart(-127, false, false, false);
    pros::delay(750);
    IntakeStart(127, false, false, false);
    pros::delay(1500);
    holder.extend();
    // outtake then intake incase stuck.
    // put hook down and push to middle after resetting chassis pose again.
    chassis.moveToPoint(0, 10, 2000);
    descore.retract();
    chassis.turnToHeading(-45, 750);
    chassis.moveToPose(-10, -20, 0, 6000, {.forwards = false, .lead = 0.8, .minSpeed=64});
    IntakeStop();
}

void Auto2() {

}

void Skills() {
    
}