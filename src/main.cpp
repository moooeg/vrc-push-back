#include "main.h"
#include "autonomous.h"

#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/apix.h"
#include "lvgl.h"

#include <map>

// controller
pros::Controller controller(pros::E_CONTROLLER_MASTER);

// motor groups
pros::MotorGroup rightMotors({-1, -2, 3}, //front right motor port 1 (reversed), middle right motor port 2 (reversed), back right motor port 3 
                            pros::MotorGearset::green);
pros::MotorGroup leftMotors({4, 5, -6}, pros::MotorGearset::green); //front right motor port 4, middle right motor port 5, back right motor port 6 (reversed)



// Inertial Sensor on port 11
pros::Imu imu(7);
// tracking wheels
// vertical tracking wheel encoder. Rotation sensor, port 9
pros::Rotation verticalEnc(8);
// vertical tracking wheel. 2" diameter, 2.5" offset, left of the robot (negative)
lemlib::TrackingWheel vertical(&verticalEnc, lemlib::Omniwheel::NEW_2, 0);

// drivetrain settings
lemlib::Drivetrain drivetrain(&leftMotors, // left motor group
                              &rightMotors, // right motor group
                              11.9, // 11.9 inch track width
                              lemlib::Omniwheel::NEW_325, // using new 3.25" omnis
                              450, // drivetrain rpm is 450
                              8 // horizontal drift is 8. 2 for full omni, 8 for traction
);

// lateral motion controller
lemlib::ControllerSettings linearController(10, // proportional gain (kP)
                                            0, // integral gain (kI)
                                            3, // derivative gain (kD)
                                            3, // anti windup
                                            1, // small error range, in inches
                                            100, // small error range timeout, in milliseconds
                                            3, // large error range, in inches
                                            500, // large error range timeout, in milliseconds
                                            20 // maximum acceleration (slew)
);

// angular motion controller
lemlib::ControllerSettings angularController(2, // proportional gain (kP)
                                             0, // integral gain (kI)
                                             10, // derivative gain (kD)
                                             3, // anti windup
                                             1, // small error range, in degrees
                                             100, // small error range timeout, in milliseconds
                                             3, // large error range, in degrees
                                             500, // large error range timeout, in milliseconds
                                             0 // maximum acceleration (slew)
);

// sensors for odometry
lemlib::OdomSensors sensors(&vertical, // vertical tracking wheel
                            nullptr, // vertical tracking wheel 2, set to nullptr as we don't have a second one
                            nullptr, // horizontal tracking wheel
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);

// input curve for throttle input during driver control
lemlib::ExpoDriveCurve throttleCurve(3, // joystick deadband out of 127
                                     10, // minimum output where drivetrain will move out of 127
                                     1.019 // expo curve gain
);

// input curve for steer input during driver control
lemlib::ExpoDriveCurve steerCurve(3, // joystick deadband out of 127
                                  10, // minimum output where drivetrain will move out of 127
                                  1.019 // expo curve gain
);

// create the chassis
lemlib::Chassis chassis(drivetrain, linearController, angularController, sensors, &throttleCurve, &steerCurve);

// declare all images
LV_IMAGE_DECLARE(begin);

LV_IMAGE_DECLARE(blue_1);
LV_IMAGE_DECLARE(blue_1_confirmed);
LV_IMAGE_DECLARE(blue_2);
LV_IMAGE_DECLARE(blue_2_confirmed);

LV_IMAGE_DECLARE(red_1);
LV_IMAGE_DECLARE(red_1_confirmed);
LV_IMAGE_DECLARE(red_2);
LV_IMAGE_DECLARE(red_2_confirmed);

TeamPosition position = TeamPosition();

class ButtonPosition {
public:
    int x1, x2, y1, y2;

    bool pressing(int x, int y) {
        return (x1 <= x <= x2) && (y1 <= y <= y2);
    }

    ButtonPosition(int x1, int x2, int y1, int y2) {
        this->x1 = x1;
        this->x2 = x2;
        this->y1 = y1;
        this->y2 = y2;
    }
};

class TeamPosition {
public:

    std::string team = "";
    std::string position = "";

    std::string asString() {
        return team + "_" + position;
    }
};

std::map<std::string, std::map<std::string, ButtonPosition>> GUI_BUTTON_POSITIONS = {
    {
        "top", {
            { "1", ButtonPosition(139, 8, 240, 26) },
            { "2", ButtonPosition(249, 8, 351, 26) },
            { "3", ButtonPosition(358, 8, 461, 26) }
        }
    },
    {
        "bottom", {
            { "1", ButtonPosition(19, 52, 138, 73)},
            { "2", ButtonPosition(19, 85, 138, 107) },
            { "3", ButtonPosition(19, 120, 138, 142) }
        }
    }
};

TeamPosition TeamChoosing() {

    lv_obj_t *img = lv_image_create(lv_screen_active());
    
    lv_image_set_src(img, &begin);
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);

    TeamPosition teamPosition = TeamPosition();
    bool confirmed = false;

    while (true) {
        pros::delay(5);

        // exit
        if (confirmed) {

            // probs a faster way then all these if checks. 
            if (teamPosition.asString() == "blue_1") {
                lv_image_set_src(img, &blue_1_confirmed);
            } else if (teamPosition.asString() == "blue_2") {
                lv_image_set_src(img, &blue_2_confirmed);
            }
            else if (teamPosition.asString() == "red_1") {
                lv_image_set_src(img, &red_1_confirmed);
            }
            else if (teamPosition.asString() == "red_2") {
                lv_image_set_src(img, &red_2_confirmed);
            }

            return teamPosition;
        }

        // controller
        if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
            teamPosition.team = "red";
            teamPosition.position = "1";
        }
        else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
            teamPosition.team = "red";
            teamPosition.position = "2";
        }
        else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
            teamPosition.team = "blue";
            teamPosition.position = "1";
        }
        else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
            teamPosition.team = "blue";
            teamPosition.position = "2";
        }
    }
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {

    chassis.calibrate(); // calibrate sensors
   
    position = TeamChoosing();
}

/**
 * Runs while the robot is disabled
 */
void disabled() {}

/**
 * runs after initialize if the robot is connected to field control
 */
void competition_initialize() {}

/**
 * Runs during auto
 *
 * This is an example autonomous routine which demonstrates a lot of the features LemLib has to offer
 */
void autonomous() {
    Autonomous autonomous = Autonomous();

	if (position.position == "_1") {
        autonomous.Auto1();
    } else if (position.position == "_2") {
        autonomous.Auto2();
    }
}

/**
 * Runs in driver control
 */
void opcontrol() {
    // controller
    // loop to continuously update motors
    while (true) {
		//get joystick values
		int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        // move the chassis with curvature drive
        chassis.arcade(leftY, rightX);
        // delay to save resources
        pros::delay(10);
    }
}