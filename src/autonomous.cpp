
#include "autonomous.h"

#include "global.h"
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/pose.hpp"
#include "pros/rtos.hpp"
#include <cmath>

void SoloAutonomous() {

}

void Auto1() {
    IntakeStart();
    chassis.moveToPoint(-3, 17, 2000, {.minSpeed=72, .earlyExitRange=8});
    chassis.moveToPoint(-6, 45, 2000, {.maxSpeed=32});
    pros::delay(900);
    matchload.extend();
    chassis.turnToHeading(chassis.getPose().theta - 155, 1000, {.maxSpeed=100});
    pros::delay(500);
    IntakeStart(-127, true, false, true);
    chassis.moveToPose(-33, 20, 180, 1500, {.lead=0.1, .maxSpeed=100});
    pros::delay(100);
    IntakeStart(127, true, false, true);
    chassis.moveToPoint(-33, 0, 300, {.maxSpeed=64});
    chassis.moveToPoint(-33, chassis.getPose().y - 10, 300, {.forwards=false});
    chassis.moveToPoint(-33, 0, 300, {.maxSpeed=64});
    pros::delay(1500);
    chassis.setPose(0, chassis.getPose().y, chassis.getPose().theta); // set x to be 0
    chassis.moveToPoint(0, 45, 1500, {.forwards=false});
    while (chassis.isInMotion()) {
        pros::delay(10);
    }
    holder.retract();
    pros::delay(1500);
    holder.extend();
    chassis.setPose(0, chassis.getPose().y, 0); // set x and theta to be 0
    IntakeStart(-127, true, false, true);
    chassis.moveToPoint(0, chassis.getPose().y - 15, 500);
    IntakeStart(127, true, false, true);
    holder.retract();
    chassis.moveToPoint(0, 45, 500, {.forwards=false});
    while (chassis.isInMotion()) {
        pros::delay(10);
    }
    holder.extend();
    matchload.retract();
    pros::delay(1500);
    chassis.moveToPoint(0, 25, 2000);
    descore.retract();
    chassis.moveToPose(10, 60, 0, 3000, {.forwards=false, .lead=0.8});
    IntakeStop();
}

void Auto2() {

    std::cout << "Hello" << std::endl;
}

void Skills() {
    
}