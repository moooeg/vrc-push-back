
#include "autonomous.h"

#include "global.h"

void SoloAutonomous() {

}

void Auto1() {
    std::cout << "test";
    // set position to x:0, y:0, heading:0
    chassis.setPose(0, 0, 0);
    // turn to face heading 90 with a very long timeout
    chassis.moveToPose(10, 0, 0, 1000000);
    std::cout << "/n test";
}

void Auto2() {

}

void Skills() {
    
}