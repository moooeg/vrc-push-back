
#include "autonomous.h"

#include "global.h"
#include <cstdint>
#include <string>
#include <thread>

void Info() {
    uint16_t line = 0;
    while (true) {
        std::string heading = std::to_string(imu.get_heading());
        const char* headingChar = heading.c_str();
        controller.print(line, 0, headingChar);
        line++;
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
    chassis.turnToHeading(90, 1000000);
    std::cout << "/n test";
}

void Auto2() {

}

void Skills() {
    
}