#include "main.h" // IWYU pragma: keep
#include "devices.h"
#include "lemlib/api.hpp" // IWYU pragma: keep

extern const lv_image_dsc_t team_logo;
extern const lv_image_dsc_t sparrow;

// =================== AUTON SELECTOR CODE ===================

// Define autonomous routines
void leftSideAuto() { /* your auton code */ }
void rightSideAuto() { /* your auton code */ }
void skillsAuto() { /* your auton code */ }
void doNothing() {}

// Struct for autos
struct AutoRoutine {
  const char *name;
  const char *description;
  void (*routine)();
};

// Define all autos here
AutoRoutine autos[] = {
    {"Left Side", "Scores preload and rushes center stake", leftSideAuto},
    {"Right Side", "Scores preload and grabs match load", rightSideAuto},
    {"Skills", "Full field skills run", skillsAuto},
    {"Do Nothing", "Literally does nothing", doNothing}};

const int NUM_AUTOS = sizeof(autos) / sizeof(autos[0]);
int currentAutoIndex = 0;

// LVGL objects
lv_obj_t *labelTitle;
lv_obj_t *labelDesc;
lv_obj_t *labelPose;
lv_timer_t *poseTimer;

// Updates the text for the current auto
void updateAutoDisplay() {
  lv_label_set_text_fmt(labelTitle, "Auto %d/%d: %s", currentAutoIndex + 1,
                        NUM_AUTOS, autos[currentAutoIndex].name);
  lv_label_set_text(labelDesc, autos[currentAutoIndex].description);
}

// Called every 100ms to update chassis position
void updatePose(lv_timer_t *timer) {
  (void)timer;
  lv_label_set_text_fmt(labelPose, "X: %.2f   Y: %.2f   Theta: %.2f°",
                        chassis.getPose().x, chassis.getPose().y,
                        chassis.getPose().theta);
}

// Move between autos with wrap-around
void nextAuto(lv_event_t *e) {
  currentAutoIndex = (currentAutoIndex + 1) % NUM_AUTOS;
  updateAutoDisplay();
}
void prevAuto(lv_event_t *e) {
  currentAutoIndex = (currentAutoIndex - 1 + NUM_AUTOS) % NUM_AUTOS;
  updateAutoDisplay();
}

// Initialize the selector screen
void autonSelectorInit() {
  lv_obj_t *screen = lv_screen_active();
  lv_obj_clean(screen);

  // Title label
  labelTitle = lv_label_create(screen);
  lv_obj_align(labelTitle, LV_ALIGN_TOP_MID, 0, 20);

  // Description label
  labelDesc = lv_label_create(screen);
  lv_label_set_long_mode(labelDesc, LV_LABEL_LONG_WRAP);
  lv_obj_set_width(labelDesc, 200);
  lv_obj_align(labelDesc, LV_ALIGN_CENTER, 0, -10);

  // Buttons
  lv_obj_t *btnPrev = lv_button_create(screen);
  lv_obj_align(btnPrev, LV_ALIGN_LEFT_MID, 10, 0);
  lv_obj_t *lblPrev = lv_label_create(btnPrev);
  lv_label_set_text(lblPrev, LV_SYMBOL_LEFT);
  lv_obj_center(lblPrev);
  lv_obj_add_event_cb(btnPrev, prevAuto, LV_EVENT_CLICKED, NULL);

  lv_obj_t *btnNext = lv_button_create(screen);
  lv_obj_align(btnNext, LV_ALIGN_RIGHT_MID, -10, 0);
  lv_obj_t *lblNext = lv_label_create(btnNext);
  lv_label_set_text(lblNext, LV_SYMBOL_RIGHT);
  lv_obj_center(lblNext);
  lv_obj_add_event_cb(btnNext, nextAuto, LV_EVENT_CLICKED, NULL);

  // ---- Team logo image ----
  lv_obj_t *imgLogo = lv_image_create(screen);
  lv_image_set_src(imgLogo, &team_logo);
  lv_obj_align(imgLogo, LV_ALIGN_TOP_RIGHT, 30, -30);
  lv_image_set_scale(imgLogo, 128);

  // ---- Dog image ----
  lv_obj_t *imgDog = lv_image_create(screen);
  lv_image_set_src(imgDog, &sparrow);
  lv_obj_align(imgDog, LV_ALIGN_TOP_LEFT, -30, -30);
  lv_image_set_scale(imgDog, 128);

  // Pose display
  labelPose = lv_label_create(screen);
  lv_obj_align(labelPose, LV_ALIGN_BOTTOM_MID, 0, -15);

  // Timer to update pose
  poseTimer = lv_timer_create(updatePose, 100, NULL);

  // Show initial info
  updateAutoDisplay();
}

// Runs selected autonomous
void runSelectedAuton() { autos[currentAutoIndex].routine(); }

// =================== PROS CALLBACKS ===================

// This runs once when the program starts
void initialize() {
  autonSelectorInit(); // Build our selector UI
}

// This runs during autonomous
void autonomous() { runSelectedAuton(); }

// This runs during driver control
void opcontrol() {
  while (true) {
    pros::delay(10);
  }
}
