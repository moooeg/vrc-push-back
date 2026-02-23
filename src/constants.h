
#include "pros/adi.hpp"
#include "pros/motors.hpp"
#include <string>
#include <vector>

class Devices {
public:

    std::vector<pros::Motor> motors;
    std::vector<pros::adi::AnalogOut> analogOuts;

    Devices(std::vector<pros::Motor> motors, std::vector<pros::adi::AnalogOut> analogOuts): motors(std::move(motors)), analogOuts(std::move(analogOuts)) {
        
    }
};

class TeamPosition {
public:

    std::string team;
    std::string position;

    std::string asString() {
        return team + "_" + position;
    }
};
