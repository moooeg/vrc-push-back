/*
https://github.com/kunwarsahni01/Vex-Autonomous-Selector

Base done by a public library and we have adapted it for our needs.
*/

#pragma once

#include <string>

//selector configuration
#define HUE 360
#define DEFAULT 1
#define AUTONS "Front", "Back", "Do Nothing"

namespace selector{

extern int auton;
inline const char *b[] = {AUTONS, ""};
void init(int hue = HUE, int default_auton = DEFAULT, const char **autons = b);

}