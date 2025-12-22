
#include "lemlib/api.hpp"

class Autonomous {

    lemlib::Chassis &chassis;

public:
    void Auto1();
    void Auto2();

   Autonomous(lemlib::Chassis &chassis) : chassis(chassis) {}
};