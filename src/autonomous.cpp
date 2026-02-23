
#include "autonomous.h"

void SoloAutonomous(lemlib::Chassis& chassis, Devices devices) {

}

void Auto1(lemlib::Chassis& chassis, Devices devices) {
    // set position to x:0, y:0, heading:0
    chassis.setPose(0, 0, 0);
    // turn to face heading 90 with a very long timeout
    chassis.turnToHeading(90, 100000);
}

void Auto2(lemlib::Chassis& chassis, Devices devices) {

}

void Skills(lemlib::Chassis& chassis, Devices devices) {
    
}