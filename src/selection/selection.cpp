/*
https://github.com/kunwarsahni01/Vex-Autonomous-Selector

Base done by a public library and we have adapted it for our needs.
*/

#include "selection.h"

namespace selector{

int auton;
int autonCount;
const char *btnmMap[] = {"","","","","","","","","","",""}; // up to 10 autons

lv_obj_t *tabview;
lv_obj_t *redBtnm;
lv_obj_t *blueBtnm;

void redBtnmAction(lv_event_t * e)
{
    lv_obj_t * btnm = static_cast<lv_obj_t *>(lv_event_get_target(e));
    uint32_t id = lv_buttonmatrix_get_selected_button(btnm);
    const char * txt = lv_buttonmatrix_get_button_text(btnm, id);

    for(int i = 0; i < autonCount; i++) {
        if(strcmp(txt, btnmMap[i]) == 0) {
            auton = i + 1;
        }
    }
}

void blueBtnmAction(lv_event_t * e)
{
	lv_obj_t * btnm = static_cast<lv_obj_t *>(lv_event_get_target(e));
    uint32_t id = lv_buttonmatrix_get_selected_button(btnm);
    const char * txt = lv_buttonmatrix_get_button_text(btnm, id);

    for(int i = 0; i < autonCount; i++) {
        if(strcmp(txt, btnmMap[i]) == 0) {
            auton = i + 1;
        }
    }
}

void skillsBtnAction(lv_event_t * e) {

	auton = 0;
}

int tabWatcher() {
	int activeTab = lv_tabview_get_tab_active(tabview);
	while(1){
		int currentTab = lv_tabview_get_tab_active(tabview);

		if(currentTab != activeTab){
			activeTab = currentTab;
			if(activeTab == 0){
				if(auton == 0) auton = 1;
				lv_buttonmatrix_set_button_ctrl(redBtnm, std::abs(auton) - 1, LV_BUTTONMATRIX_CTRL_CHECKED);
			}else if(activeTab == 1){
				if(auton == 0) auton = -1;
				lv_buttonmatrix_set_button_ctrl(blueBtnm, std::abs(auton) - 1, LV_BUTTONMATRIX_CTRL_CHECKED);
			}else{
				auton = 0;
			}
		}

		pros::delay(20);
	}
}

void init(int hue, int default_auton, const char **autons){

	int i = 0;
	do{
		memcpy(&btnmMap[i], &autons[i], sizeof(&autons));
		i++;
	}while(strcmp(autons[i], "") != 0);

	autonCount = i;
	auton = default_auton;

	// lvgl theme
	lv_theme_t * th = lv_theme_default_init(lv_display_get_default(), 
                                        lv_palette_main(LV_PALETTE_LIME), // Primary color
                                        lv_palette_main(LV_PALETTE_GREY), // Secondary color
                                        true,                             // Dark mode (true/false)
                                        &lv_font_montserrat_14);          // Default font

	lv_display_set_theme(lv_display_get_default(), th);

	// create a tab view object
	tabview = lv_tabview_create(lv_screen_active());

	// add 3 tabs (the tabs are page (lv_page) and can be scrolled
	lv_obj_t *redTab = lv_tabview_add_tab(tabview, "Red");
	lv_obj_t *blueTab = lv_tabview_add_tab(tabview, "Blue");
	lv_obj_t *skillsTab = lv_tabview_add_tab(tabview, "Skills");

	//set default tab
	if(auton < 0){
		lv_tabview_set_active(tabview, 1, LV_ANIM_OFF);
	}else if(auton == 0){
		lv_tabview_set_active(tabview, 2, LV_ANIM_OFF);
	}

	// add content to the tabs
	// red tab button matrix 
	redBtnm = lv_buttonmatrix_create(redTab); 
	lv_buttonmatrix_set_map(redBtnm, btnmMap);
	lv_obj_add_event_cb(redBtnm, redBtnmAction, LV_EVENT_VALUE_CHANGED, NULL);
	lv_buttonmatrix_set_button_ctrl(redBtnm, abs(auton) - 1, LV_BUTTONMATRIX_CTRL_CHECKED);
	lv_buttonmatrix_set_button_ctrl(redBtnm, abs(auton) - 1, LV_BUTTONMATRIX_CTRL_CHECKED);
	lv_obj_set_size(redBtnm, 450, 50);
	lv_obj_set_pos(redBtnm, 0, 100);
	lv_obj_align(redBtnm, LV_ALIGN_CENTER, 0, 0);

	// blue tab button matrix 
	blueBtnm = lv_buttonmatrix_create(blueTab);
	lv_buttonmatrix_set_map(blueBtnm, btnmMap);
	lv_obj_add_event_cb(blueBtnm, blueBtnmAction, LV_EVENT_VALUE_CHANGED, NULL);
	lv_buttonmatrix_set_button_ctrl(blueBtnm, abs(auton) - 1, LV_BUTTONMATRIX_CTRL_CHECKED);
	lv_buttonmatrix_set_button_ctrl(blueBtnm, abs(auton) - 1, LV_BUTTONMATRIX_CTRL_CHECKED);
	lv_obj_set_size(blueBtnm, 450, 50);
	lv_obj_set_pos(blueBtnm, 0, 100);
	lv_obj_align(blueBtnm, LV_ALIGN_CENTER, 0, 0);

	// skills tab button 
	lv_obj_t *skillsbtn = lv_buttonmatrix_create(skillsTab);
	lv_obj_t *label = lv_label_create(skillsbtn);
	lv_label_set_text(label, "skills");
	lv_obj_add_event_cb(skillsbtn, skillsBtnAction, LV_EVENT_CLICKED, NULL);
	lv_obj_set_size(skillsbtn, 450, 50);
	lv_obj_set_pos(skillsbtn, 0, 100);
	lv_obj_align(skillsbtn, LV_ALIGN_CENTER, 0, 0);

	// start tab watcher
	pros::Task tabWatcher_task(tabWatcher);

}

} // namespace selector