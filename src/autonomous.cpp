
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
    chassis.setPose(0, 0, 0);
    // get three center block
    chassis.moveToPoint(-3, 17, 2000, {.minSpeed = 72, .earlyExitRange = 8});
    IntakeStart();
    chassis.moveToPoint(-6, 45, 2000, {.maxSpeed = 32});
    pros::delay(900);
    matchload.extend();

    // go to match load tube
    chassis.turnToHeading(315, 1000, {.maxSpeed = 100});
    pros::delay(500);
    chassis.moveToPose(-33, 20, 180, 1500, {.lead = 0.1, .maxSpeed = 100});
    pros::delay(100);
    chassis.moveToPoint(-33, 0, 300, {.maxSpeed = 64});
    pros::delay(1500);

    // score in long goal
    chassis.moveToPoint(-33, 45, 1500, {.forwards = false}, false);
    holder.retract();
    matchload.retract();
    pros::delay(1500);
    IntakeStop();
    chassis.moveToPoint(-33, 35, 500, {.forwards = false}, false);
    holder.extend();
    chassis.moveToPoint(-33, 45, 500, {.forwards = false}, false);
    pros::delay(500);

    // push to center
    chassis.moveToPoint(-33, 25, 1000, {}, false);
    descore.retract();
    chassis.moveToPose(-23, 45, 0, 1000, {.forwards = false, .lead = 0.8});
    chassis.moveToPoint(-23, 60, 1000, {.forwards = false});
}

void Auto2() {

}

void Skills() {
    
}