
#include "autonomous.h"

#include "global.h"
#include <cstdint>
#include <string>
#include <thread>

void Info() {
    while (true) {
        pros::lcd::set_text(2, std::to_string(chassis.getPose().x) + " " + std::to_string(chassis.getPose().y) + " " + std::to_string(chassis.getPose().theta));
        pros::delay(50);
    }
}

void SoloAutonomous() {

}

void Auto1() {
    std::cout << "test";
    // set position to x:0, y:0, heading:0
    chassis.setPose(0, 0, 0);
    // thread info
    // turn to face heading 90 with a very long timeout
    chassis.moveToPoint(-24, 0, 100000);
    std::cout << "/n test";
}

void Auto2() {

}

void Skills() {
    
}