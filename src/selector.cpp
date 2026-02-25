
#include "selector.h"
#include "pros/apix.h"
#include <cstdint>

static bool isDone = false;

static lv_style_t createStyle(const uint32_t& color, bool skills = false) {
    lv_style_t style;
    lv_style_init(&style);

    lv_style_set_bg_color(&style, lv_color_hex(color));

    if (skills) {
        lv_style_set_text_color(&style, lv_color_black());
    } else {
        lv_style_set_text_color(&style, lv_color_white());
    }

    return style;
}

// create styles
static lv_style_t redStyleUnpressed;
static lv_style_t blueStyleUnpressed;
static lv_style_t skillsStyleUnpressed;

static lv_style_t redStylePressed;
static lv_style_t blueStylePressed;
static lv_style_t skillsStylePressed;

// static
static lv_obj_t * tabView;

static void red1(lv_event_t * event) {
    selector::auton.team = "red";
    selector::auton.position = "1";
    isDone = true;
}

static void red2(lv_event_t * event) {
    selector::auton.team = "red";
    selector::auton.position = "2";
    isDone = true;
}

static void blue1(lv_event_t * event) {
    selector::auton.team = "blue";
    selector::auton.position = "1";
    isDone = true;
}

static void blue2(lv_event_t * event) {
    selector::auton.team = "blue";
    selector::auton.position = "2";
    isDone = true;
}

static void skills(lv_event_t * event) {
    selector::auton.team = "skills";
    isDone = true;
}

static void solo(lv_event_t * event) {
    selector::auton.team = "solo";
    isDone = true;
}

static void redTabPressed(lv_event_t * event) {
    lv_tabview_set_active(tabView, 0, LV_ANIM_ON);

}

static void blueTabPressed(lv_event_t * event) {
    lv_tabview_set_active(tabView, 1, LV_ANIM_ON);

}

static void skillsTabPressed(lv_event_t * event) {
    lv_tabview_set_active(tabView, 2, LV_ANIM_ON);

}

void selector::init() {

    // create tab view
    tabView = lv_tabview_create(lv_screen_active());

    // create styles
    redStyleUnpressed = createStyle(0x991006);
    blueStyleUnpressed = createStyle(0x082980);
    skillsStyleUnpressed = createStyle(0xFFFFFF, true);

    redStylePressed = createStyle(0xc41508);
    blueStylePressed = createStyle(0x0a3dc9);
    skillsStylePressed = createStyle(0xd9d9d9, true);

    // create tabs
    lv_obj_t * redTab = lv_tabview_add_tab(tabView, "Red");
    lv_obj_add_style(redTab, &redStyleUnpressed, 0);
    lv_obj_t * blueTab = lv_tabview_add_tab(tabView, "Blue");
    lv_obj_add_style(blueTab, &blueStyleUnpressed, 0);
    lv_obj_t * skillsTab = lv_tabview_add_tab(tabView, "Skills");
    lv_obj_add_style(skillsTab, &skillsStyleUnpressed, 0);

    // create buttons in tab
    // red 1
    lv_obj_t * red1Button = lv_button_create(redTab);
    lv_obj_t * red1Label = lv_label_create(red1Button);
    lv_label_set_text(red1Label, "Red Left");
    lv_obj_add_style(red1Button, &redStyleUnpressed, 0);
    lv_obj_set_align(red1Button, LV_ALIGN_LEFT_MID);

    // red 2
    lv_obj_t * red2Button = lv_button_create(redTab);
    lv_obj_t * red2Label = lv_label_create(red2Button);
    lv_label_set_text(red2Label, "Red Right");
    lv_obj_add_style(red2Button, &redStyleUnpressed, 0);
    lv_obj_set_align(red2Button, LV_ALIGN_CENTER);

    // blue 1
    lv_obj_t * blue1Button = lv_button_create(blueTab);
    lv_obj_t * blue1Label = lv_label_create(blue1Button);
    lv_label_set_text(blue1Label, "Blue Left");
    lv_obj_add_style(blue1Button, &blueStyleUnpressed, 0);
    lv_obj_set_align(blue1Button, LV_ALIGN_LEFT_MID);
    
    // blue 2
    lv_obj_t * blue2Button = lv_button_create(blueTab);
    lv_obj_t * blue2Label = lv_label_create(blue2Button);
    lv_label_set_text(blue2Label, "Blue Right");
    lv_obj_add_style(blue2Button, &blueStyleUnpressed, 0);
    lv_obj_set_align(blue2Button, LV_ALIGN_CENTER);

    // skills
    lv_obj_t * skillsButton = lv_button_create(skillsTab);
    lv_obj_t * skillsLabel = lv_label_create(skillsButton);
    lv_label_set_text(skillsLabel, "Skills");
    lv_obj_add_style(skillsButton, &skillsStyleUnpressed, 0);
    lv_obj_set_align(skillsButton, LV_ALIGN_CENTER);

    // solo
    lv_obj_t * redSoloButton = lv_button_create(redTab);
    lv_obj_t * blueSoloButton = lv_button_create(blueTab);
    lv_obj_t * redSoloLabel = lv_label_create(redSoloButton);
    lv_obj_t * blueSoloLabel = lv_label_create(blueSoloButton);
    lv_label_set_text(redSoloLabel, "Solo Left");
    lv_label_set_text(blueSoloLabel, "Solo Left");
    lv_obj_add_style(redSoloButton, &skillsStyleUnpressed, 0);
    lv_obj_add_style(blueSoloButton, &skillsStyleUnpressed, 0);
    lv_obj_set_align(redSoloButton, LV_ALIGN_RIGHT_MID);
    lv_obj_set_align(blueSoloButton, LV_ALIGN_RIGHT_MID);

    lv_obj_add_event_cb(red1Button, red1, LV_EVENT_PRESSED, nullptr);
    lv_obj_add_event_cb(red2Button, red2, LV_EVENT_PRESSED, nullptr);
    lv_obj_add_event_cb(blue1Button, blue1, LV_EVENT_PRESSED, nullptr);
    lv_obj_add_event_cb(blue2Button, blue2, LV_EVENT_PRESSED, nullptr);
    lv_obj_add_event_cb(skillsButton, skills, LV_EVENT_PRESSED, nullptr);
    lv_obj_add_event_cb(redSoloButton, solo, LV_EVENT_PRESSED, nullptr);
    lv_obj_add_event_cb(blueSoloButton, solo, LV_EVENT_PRESSED, nullptr);

    lv_obj_add_event_cb(redTab, redTabPressed, LV_EVENT_PRESSED, nullptr);
    lv_obj_add_event_cb(blueTab, blueTabPressed, LV_EVENT_PRESSED, nullptr);
    lv_obj_add_event_cb(skillsTab, skillsTabPressed, LV_EVENT_PRESSED, nullptr);

    while (!isDone) {
        pros::delay(50);
    }
}