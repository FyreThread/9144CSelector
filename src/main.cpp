#define LEMLIB_USE_SCREEN false
#include "main.h" // IWYU pragma: keep
#include "devices.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/adi.h"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include "pros/motors.h"
#include "pros/rtos.hpp"

// extern const lv_image_dsc_t team_logo;
// extern const lv_image_dsc_t sparrow;
// extern const lv_image_dsc_t hopper;

// =================== AUTON SELECTOR CODE ===================

// Define autonomous routines
void route1() {
  chassis.setPose(0, 0, 0);
  hood.set_value(LOW);
  chassis.moveToPose(0, -34, 0, 1200, {.forwards = false});
  chassis.waitUntilDone();
  stage3.move_relative(-1500, 120);
  pros::delay(160);
  chassis.moveToPose(19.8, -25.7, 66.2, 800);
  intakeStopper.set_value(false);
  intakeStopper2.set_value(false);
  stage1.move(127);
  stage2.move(-127);
  chassis.waitUntilDone();
  lW.set_value(HIGH);
  chassis.moveToPose(36.4, -10.6, 40, 800);
  chassis.waitUntilDone();
  chassis.moveToPose(45, 15, -1, 2750);
  chassis.waitUntilDone();
  chassis.moveToPose(45, -20, -2, 1500, {.forwards = false});
  chassis.waitUntilDone();
  intakeStopper.set_value(true);
  intakeStopper2.set_value(true);
  stage1.move(-127);
  stage2.move(127);
  stage3.move(127);
  pros::delay(100);
  stage1.move(127);
  stage2.move(-127);
  stage3.move(-127);
  pros::delay(1500);
  chassis.moveToPose(24, -8.6, -112, 500);
  chassis.waitUntilDone();
  lW.set_value(LOW);
  intakeStopper.set_value(false);
  intakeStopper2.set_value(false);
  chassis.moveToPose(-29.7, -25.5, -109, 2500);
  chassis.waitUntilDone();
  chassis.turnToHeading(-225, 500);
  chassis.waitUntilDone();
  chassis.moveToPose(-21.5, -36.2, -225, 1000);
  chassis.waitUntilDone();
  intakeStopper.set_value(false);
  intakeStopper2.set_value(false);
  stage1.move(-127);
  stage2.move(127);
  pros::delay(750);
  stage1.move(-75);
  stage2.move(75);
  pros::delay(2500);

  if (!pros::competition::is_connected()) {
    pros::delay(0);
  }
}

void route2() {
  chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
  chassis.setPose(0, 0, 0);
  hood.set_value(LOW);
  chassis.moveToPose(0, -34, 0, 1200, {.forwards = false});
  chassis.waitUntilDone();
  stage3.move_relative(-1500, 120);
  pros::delay(160);
  chassis.moveToPose(19.8, -25.7, 66.2, 800);
  intakeStopper.set_value(false);
  intakeStopper2.set_value(false);
  stage1.move(127);
  stage2.move(-127);
  chassis.waitUntilDone();
  pros::delay(100);
  lW.set_value(HIGH);
  chassis.turnToHeading(110.2, 500);
  chassis.waitUntilDone();
  lW.set_value(LOW);
  chassis.moveToPose(39.8, -41.2, 97, 2000);
  chassis.waitUntilDone();
  chassis.moveToPose(17.6, -35.7, 107.2, 1250, {.forwards = false});
  chassis.waitUntilDone();
  chassis.turnToHeading(31.7, 500);
  chassis.waitUntilDone();
  lW.set_value(true);
  chassis.moveToPose(36.4, -10.6, 40, 1000);
  chassis.waitUntilDone();
  chassis.moveToPose(45, 15, -1, 2750);
  chassis.waitUntilDone();
  chassis.moveToPose(45, -20, -2, 1750, {.forwards = false});
  chassis.waitUntilDone();
  intakeStopper.set_value(true);
  intakeStopper2.set_value(true);
  stage1.move(-127);
  stage2.move(127);
  stage3.move(127);
  pros::delay(125);
  stage1.move(127);
  stage2.move(-127);
  stage3.move(-127);
  pros::delay(1500);
  chassis.moveToPose(45, -7.4, 0, 1000);
  chassis.waitUntilDone();
  hood.set_value(HIGH);
  chassis.moveToPose(45, -22, -2, 1750, {.forwards = false, .minSpeed = 90});
  chassis.waitUntilDone();
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
AutoRoutine autos[] = {{"Left", "Description of scoring here", route2},
                       {"Left Side", "Lefttttttttt", route2},
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
    lemlib::Pose pose = chassis.getPose();

    char buffer[64];
    snprintf(buffer, sizeof(buffer), "X: %.1f  Y: %.1f  T: %.1f°", pose.x,
             pose.y, pose.theta);

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
  hood.set_value(HIGH);
  lW.set_value(LOW);
  intakeStopper.set_value(LOW);
  intakeStopper2.set_value(LOW);
}

// Toggle variables
bool stopperToggled = false;
bool lastBState = false;
bool lWToggled = false;
bool lastDownState = false;

void autonomous() { runSelectedAuton(); }

void opcontrol() {
  chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);

  while (true) {
    bool currentBState = master.get_digital(pros::E_CONTROLLER_DIGITAL_B);
    bool currentDownState = master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT);

    // Intake/stage logic
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
      intakeStopper.set_value(false);
      intakeStopper2.set_value(false);
      stage1.move(127);
      stage2.move(-127);
    } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
      intakeStopper.set_value(false);
      intakeStopper2.set_value(false);
      stage1.move(-127);
      stage2.move(127);
    } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
      intakeStopper.set_value(true);
      intakeStopper2.set_value(true);
      stage1.move(127);
      stage2.move(-127);
      stage3.move(-127);
    } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
      intakeStopper.set_value(true);
      intakeStopper2.set_value(true);
      stage1.move(127);
      stage2.move(-127);
      stage3.move(100);
    } else {
      stage1.move(0);
      stage2.move(0);
      stage3.move(0);
    }

    // Hood control
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A))
      hood.set_value(HIGH);
    else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_X))
      hood.set_value(LOW);

    // Toggle intake stopper
    if (currentBState && !lastBState) {
      stopperToggled = !stopperToggled;
      intakeStopper.set_value(stopperToggled);
      intakeStopper2.set_value(stopperToggled);
    }

    // Toggle left wing
    if (currentDownState && !lastDownState) {
      lWToggled = !lWToggled;
      lW.set_value(lWToggled);
    }

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
