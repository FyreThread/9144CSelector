#include <cmath>
#define LEMLIB_USE_SCREEN false
#include "./devices.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "main.h"         // IWYU pragma: keep
#include "pros/adi.h"     // IWYU pragma: keep
#include "pros/misc.h"
#include "pros/misc.hpp"
#include "pros/motors.h"
#include "pros/rtos.hpp"

// extern const lv_image_dsc_t team_logo;
// extern const lv_image_dsc_t sparrow;
// extern const lv_image_dsc_t hopper;

// =================== AUTON SELECTOR CODE ===================

void route1() {
  chassis.setPose(0, 0, 0);
  IR.set_value(true);
  chassis.moveToPoint(0, 24, 5000);
  chassis.waitUntilDone();
}

// Left Side
void route2() {
  chassis.setPose(0, 0, 0);
  IR.set_value(true);
}

void skillsAuto() { /* your auton code */ }
void doNothing() {}

// Struct for autos
struct AutoRoutine {
  const char *name;
  const char *description;
  void (*routine)();
};

// Define all autos here
AutoRoutine autos[] = {{"Left", "Description of scoring here", route1},
                       {"Right Side", "Right", route2},
                       {"Skills", "Full field skills run", skillsAuto},
                       {"Do Nothing", "Literally does nothing", doNothing}};

const int NUM_AUTOS = sizeof(autos) / sizeof(autos[0]);
int currentAutoIndex = 0;

// LVGL objects
lv_obj_t *labelTitle;
lv_obj_t *labelDesc;
lv_obj_t *labelPose;

// =================== AUTON DISPLAY FUNCTIONS ===================

// Updates the text for the current auto
void updateAutoDisplay() {
  lv_label_set_text_fmt(labelTitle, "Auto %d/%d: %s", currentAutoIndex + 1,
                        NUM_AUTOS, autos[currentAutoIndex].name);
  lv_label_set_text(labelDesc, autos[currentAutoIndex].description);
}

// LVGL pose display task
void poseDisplayTask() {
  while (true) {
    lemlib::Pose pose = chassis.getPose(); // read once

    // USE THIS COPY TO DETECT NaN
    if (std::isnan(pose.theta) || std::isnan(pose.x) || std::isnan(pose.y)) {
      master.rumble(".......");
    }

    char buffer[64];
    snprintf(buffer, sizeof(buffer), "X: %.1f  Y: %.1f  T: %.1f°  Dist: %i",
             pose.x, pose.y, pose.theta, dTop.get_distance());

    lv_label_set_text(labelPose, buffer);
    pros::delay(50);
  }
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

  /*// ---- Team logo ----
  lv_obj_t *imgLogo = lv_image_create(screen);
  lv_image_set_src(imgLogo, &team_logo);
  lv_obj_align(imgLogo, LV_ALIGN_TOP_RIGHT, 30, -30);
  lv_image_set_scale(imgLogo, 128);


  // ---- Dog image ----
  lv_obj_t *imgDog = lv_image_create(screen);
  lv_image_set_src(imgDog, &sparrow);
  lv_obj_align(imgDog, LV_ALIGN_TOP_LEFT, -30, -30);
  lv_image_set_scale(imgDog, 128);


  // ---- Hopper image ----
  lv_obj_t *imgHopper = lv_image_create(screen);
  lv_image_set_src(imgHopper, &hopper);
  lv_obj_align(imgHopper, LV_ALIGN_TOP_MID, 0, 0);
  lv_image_set_scale(imgHopper, 128);*/

  // ---- Pose label ----
  labelPose = lv_label_create(screen);
  lv_obj_align(labelPose, LV_ALIGN_BOTTOM_MID, 0, -15);
  lv_label_set_text(labelPose, "X: 0.0  Y: 0.0  θ: 0.0°");

  // Show initial info
  updateAutoDisplay();
}

// =================== CORE PROS FUNCTIONS ===================

// Run selected autonomous
void runSelectedAuton() { autos[currentAutoIndex].routine(); }

void displayPoseTask() {
  while (true) {
    // Get the robot's pose from LemLib
    lemlib::Pose pose = chassis.getPose();

    // Print X, Y, and theta (formatted to 2 decimals) on line 2
    // Controller lines are 0-indexed: line 0 = top, line 1 = middle, line 2 =
    // bottom
    master.print(2, 0, "X:%.1f Y:%.1f T:%.1f", pose.x, pose.y, pose.theta);

    // Update every 50 ms
    pros::delay(50);
  }
}

void initialize() {
  chassis.calibrate();
  pros::delay(200);

  autonSelectorInit();                     // Build selector UI
  pros::Task displayTask(poseDisplayTask); // start LVGL pose updater
  pros::Task controllerTask(displayPoseTask);

  // Initial pneumatic setup
  hood.set_value(false);
  lW.set_value(false);
  wing.set_value(false);
  wing.set_value(true);
  IR.set_value(false);
}

// Toggle variables
bool IRToggled = false;
bool lastBState = false;
bool lWToggled = false;
bool lastDownState = false;
bool hoodToggled = false;
bool lastAState = false;
bool yState = false;

void autonomous() { runSelectedAuton(); }

void opcontrol() {
  chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
  IR.set_value(true);

  while (true) {
    bool currentAState = master.get_digital(pros::E_CONTROLLER_DIGITAL_A);
    bool currentBState = master.get_digital(pros::E_CONTROLLER_DIGITAL_B);

    bool currentDownState = master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT);
    bool currentYState = master.get_digital(pros::E_CONTROLLER_DIGITAL_Y);

    // Intake/stage logic
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
      hood.set_value(false);
      stage1.move(127);
      stage2.move(127);
      stage3.move(-127);
    } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
      hood.set_value(false);
      stage1.move(-127);
      stage2.move(-127);
      stage3.move(127);
    } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {

      hood.set_value(true);
      stage1.move(127);
      stage2.move(127);
      stage3.move(-127);
    } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
      hood.set_value(false);
      stage1.move(127);
      stage2.move(127);
      stage3.move(127);
    } else {
      stage1.move(0);
      stage2.move(0);
      stage3.move(0);
    }

    /**/
    if (currentAState && !lastAState) {
      IRToggled = !IRToggled;
      wing.set_value(IRToggled);
    }

    // Toggle intake IR
    if (currentBState && !lastBState) {
      hoodToggled = !hoodToggled;
      hood.set_value(hoodToggled);
    }

    // Toggle lilWill
    if (currentDownState && !lastDownState) {
      lWToggled = !lWToggled;
      lW.set_value(lWToggled);
    }

    // Test for deadzones
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_Y)) {
      stage1.move(20);
      stage2.move(-20);
    }

    lastAState = currentAState;
    lastBState = currentBState;
    lastDownState = currentDownState;

    // Drivetrain control
    int leftY = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int rightX = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
    chassis.arcade(leftY, rightX * 0.9);

    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_UP)) {
      if (!pros::competition::is_connected()) {
        autonomous();
      }
    }
    master.set_text(2, 0, "Test");

    pros::delay(20);
  }
}
