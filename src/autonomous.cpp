
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

void Auto1() { //left
    chassis.setPose(0, 0, 0);
    // get three center block
    chassis.moveToPoint(-3, 17, 2000, {.minSpeed = 72, .earlyExitRange = 8});
    IntakeStart(127, false, false, false);
    chassis.moveToPoint(-6, 45, 2000, {.maxSpeed = 32});
    pros::delay(1000);
    matchload.extend();

    // go to match load tube
    chassis.turnToHeading(315, 1000, {.maxSpeed = 100});
    pros::delay(500);
    chassis.moveToPose(-33, 20, 180, 1500, {.lead = 0.1, .maxSpeed = 100});
    pros::delay(100);
    chassis.moveToPoint(-33, -10, 300, {.maxSpeed = 64});
    pros::delay(100);
    chassis.moveToPoint(-33, 10, 300, {.forwards = false, .maxSpeed = 64});
    pros::delay(100);
    chassis.moveToPoint(-33, -10, 300, {.maxSpeed = 64});
    pros::delay(1500);

    // score in long goal
    chassis.moveToPoint(-31, 45, 1500, {.forwards = false}, false);
    holder.retract();
    matchload.retract();
    pros::delay(1500);
    IntakeStop();
    chassis.moveToPoint(-31, 35, 500, {.forwards = false}, false);
    holder.extend();
    chassis.moveToPoint(-31, 45, 500, {.forwards = false}, false);
    pros::delay(500);

    // push to center
    chassis.moveToPoint(-31, 15, 1000, {}, false);
    descore.retract();
    chassis.moveToPose(-15, 40, 0, 1000, {.forwards = false, .lead = 0.8});
    chassis.moveToPoint(-15, 60, 1000, {.forwards = false});
}

void Auto2() {

}

void Skills() {
    
}