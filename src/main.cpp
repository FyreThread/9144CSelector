#include "main.h"
#include "devices.h"
#include "lemlib/api.hpp" // IWYU pragma: keep

// ---------- Autonomous routines ----------
void left_rush() { pros::lcd::set_text(1, "Running Left Rush"); }
void right_rush() { pros::lcd::set_text(1, "Running Right Rush"); }
void skills() { pros::lcd::set_text(1, "Running Skills"); }
void awp() { pros::lcd::set_text(1, "Running AWP"); }
void solo_awp() { pros::lcd::set_text(1, "Running Solo AWP"); }
void defense() { pros::lcd::set_text(1, "Running Defense"); }
void far_side() { pros::lcd::set_text(1, "Running Far Side"); }
void test_auto() { pros::lcd::set_text(1, "Running Test Auton"); }

// ---------- Function-pointer array (EZ-style) ----------
using AutonFn = void (*)();
AutonFn autonFunctions[8] = {left_rush, right_rush, skills,   awp,
                             solo_awp,  defense,    far_side, test_auto};
const char *autonNames[8] = {"Left Rush", "Right Rush", "Skills",   "AWP",
                             "Solo AWP",  "Defense",    "Far Side", "Test"};

// ---------- UI globals ----------
int selectedAuton = 0;
lv_obj_t *label_auton;
lv_obj_t *label_pose;

// ---------- Helpers ----------
void updateAutonLabel() {
  lv_label_set_text(label_auton, autonNames[selectedAuton]);
}
void updatePoseTask(void *);

// ---------- LVGL button callbacks ----------
void nextAuton(lv_event_t *e) {
  selectedAuton = (selectedAuton + 1) % 8; // wrap forward
  updateAutonLabel();
}
void prevAuton(lv_event_t *e) {
  selectedAuton = (selectedAuton - 1 + 8) % 8; // wrap backward
  updateAutonLabel();
}

// ---------- LVGL page creation ----------
void createAutonPage() {
  lv_obj_t *screen = lv_screen_active();

  // Title
  lv_obj_t *title = lv_label_create(screen);
  lv_label_set_text(title, "Auton Selector");
  lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

  // Auton name label
  label_auton = lv_label_create(screen);
  lv_label_set_text(label_auton, autonNames[selectedAuton]);
  lv_obj_align(label_auton, LV_ALIGN_CENTER, 0, -20);

  // Left button
  lv_obj_t *btn_left = lv_button_create(screen);
  lv_obj_set_size(btn_left, 60, 40);
  lv_obj_align(btn_left, LV_ALIGN_LEFT_MID, 10, -20);
  lv_obj_add_event_cb(btn_left, prevAuton, LV_EVENT_CLICKED, NULL);
  lv_obj_t *lbl_left = lv_label_create(btn_left);
  lv_label_set_text(lbl_left, "<");

  // Right button
  lv_obj_t *btn_right = lv_button_create(screen);
  lv_obj_set_size(btn_right, 60, 40);
  lv_obj_align(btn_right, LV_ALIGN_RIGHT_MID, -10, -20);
  lv_obj_add_event_cb(btn_right, nextAuton, LV_EVENT_CLICKED, NULL);
  lv_obj_t *lbl_right = lv_label_create(btn_right);
  lv_label_set_text(lbl_right, ">");

  // Pose label (bottom)
  label_pose = lv_label_create(screen);
  lv_label_set_text(label_pose, "X: 0.00  Y: 0.00  θ: 0.00");
  lv_obj_align(label_pose, LV_ALIGN_BOTTOM_MID, 0, -10);

  // Start pose-update background task
  pros::Task poseTask(updatePoseTask, NULL, TASK_PRIORITY_DEFAULT,
                      TASK_STACK_DEPTH_DEFAULT, "Pose Update Task");
}

// ---------- Pose update task ----------
void updatePoseTask(void *) {
  while (true) {
    lemlib::Pose pose = chassis.getPose();
    char buf[64];
    snprintf(buf, sizeof(buf), "X: %.2f  Y: %.2f  θ: %.2f", pose.x, pose.y,
             pose.theta);
    lv_label_set_text(label_pose, buf);
    pros::delay(100); // 10 Hz updates
  }
}

// ---------- Standard PROS callbacks ----------
void initialize() {
  chassis.calibrate();
  createAutonPage();
}

void autonomous() {
  autonFunctions[selectedAuton](); // <-- run selected auton!
}

void opcontrol() {
  while (true) {
    pros::delay(10);
  }
}
