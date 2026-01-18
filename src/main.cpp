#include "liblvgl/misc/lv_color.h"
#include "liblvgl/widgets/label/lv_label.h"
#include <cmath>
#define LEMLIB_USE_SCREEN false
#include "./devices.h"
#include "lemlib/api.hpp"
#include "main.h"
#include "pros/adi.h"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include "pros/motors.h"
#include "pros/rtos.hpp"

int expectedDistLeft = 460;
int expectedDistBack = 1513;

float leftOffset() { return (expectedDistLeft - dLeft.get_distance()) / 25.4; }
float backOffset() { return (expectedDistBack - dBack.get_distance()) / 25.4; }

bool redBallDetected = false;
bool blueBallDetected = false;

bool redMatch = true;

void colourSort() {
  redBallDetected = false;
  blueBallDetected = false;
  int prox = colour.get_proximity();
  int hue = (int)colour.get_hue();
  if (prox > 100) {
    if (hue <= 25 || hue >= 335)
      redBallDetected = true;
    else if (hue >= 180 && hue <= 230)
      blueBallDetected = true;
  }
}

void route1() {
  chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
  chassis.setPose(0 - leftOffset(), 0 - backOffset(), 0);
  IR.set_value(true);
  hood.set_value(false);
  stage1.move(127);
  stage2.move(127);
  stage3.move(-127);
  chassis.moveToPoint(0, 3, 250);
  chassis.waitUntilDone();
  chassis.moveToPoint(0, -39, 1500, {.forwards = false});
  chassis.waitUntilDone();
  chassis.turnToHeading(-90, 650);
  lW.set_value(true);
  chassis.waitUntilDone();
  chassis.moveToPoint(-10, -34.5, 1250);
  chassis.waitUntilDone();
  chassis.moveToPose(22, -35.6, -90, 1850, {.forwards = false});
  chassis.waitUntilDone();
  hood.set_value(false);
  stage1.move(-127);
  stage2.move(-127);
  stage3.move(127);
  pros::delay(200);
  hood.set_value(true);
  stage1.move(127);
  stage2.move(127);
  stage3.move(-127);
  pros::delay(1700);
  hood.set_value(false);
  stage1.move(127);
  stage2.move(127);
  stage3.move(-127);
  chassis.moveToPoint(11.5, -35.6, 500);
  chassis.waitUntilDone();
  lW.set_value(false);
  chassis.turnToHeading(30, 500);
  chassis.waitUntilDone();
  chassis.moveToPose(25.2, -14.0, 34.66, 1500);
  chassis.waitUntilDone();
  chassis.turnToHeading(-2, 600);
  chassis.waitUntilDone();
  chassis.moveToPose(25, 30.4, -4.8, 2500);
  chassis.waitUntilDone();
  chassis.turnToHeading(-52, 600);
  chassis.waitUntilDone();
  chassis.moveToPose(35.7, 21.9, -50, 2500, {.forwards = false});
  chassis.waitUntilDone();
  hood.set_value(false);
  stage1.move(-127);
  stage2.move(-127);
  stage3.move(127);
  pros::delay(200);
  hood.set_value(false);
  stage1.move(127);
  stage2.move(127);
  stage3.move(127);
  pros::delay(1750);
  hood.set_value(false);
  stage1.move(127);
  stage2.move(127);
  stage3.move(-127);
  lW.set_value(true);
  chassis.moveToPose(5.1, 56.2, -41, 2000);
  chassis.waitUntilDone();
  chassis.turnToHeading(-90, 600);
  chassis.waitUntilDone();
  chassis.moveToPoint(-9.3, 58.1, 2500);
  chassis.waitUntilDone();
}

void route2() {
  chassis.setPose(0, 0, 0);
  IR.set_value(true);
}

void skillsAuto() {}
void doNothing() {}

struct AutoRoutine {
  const char *name;
  const char *description;
  void (*routine)();
};

AutoRoutine autos[] = {{"Left", "Description of scoring here", route1},
                       {"Right Side", "Right", route2},
                       {"Skills", "Full field skills run", skillsAuto},
                       {"Do Nothing", "Literally does nothing", doNothing}};

const int NUM_AUTOS = sizeof(autos) / sizeof(autos[0]);
int currentAutoIndex = 0;

lv_obj_t *labelTitle;
lv_obj_t *labelDesc;
lv_obj_t *labelPose;
lv_obj_t *labelOptical;
lv_obj_t *labelColour;
lv_obj_t *screen;

void updateColourLabel() {
  if (redBallDetected)
    lv_label_set_text(labelColour, "Colour: RED");
  else if (blueBallDetected)
    lv_label_set_text(labelColour, "Colour: BLUE");
  else
    lv_label_set_text(labelColour, "Colour: None");
}

void updateAutoDisplay() {
  lv_label_set_text_fmt(labelTitle, "Auto %d/%d: %s", currentAutoIndex + 1,
                        NUM_AUTOS, autos[currentAutoIndex].name);
  lv_label_set_text(labelDesc, autos[currentAutoIndex].description);
}

void uiTask() {
  while (true) {
    lemlib::Pose pose = chassis.getPose();

    char buffer[64];
    snprintf(buffer, sizeof(buffer), "X:%d.%d Y:%d.%d T:%d.%d", (int)pose.x,
             abs((int)(pose.x * 10) % 10), (int)pose.y,
             abs((int)(pose.y * 10) % 10), (int)pose.theta,
             abs((int)(pose.theta * 10) % 10));

    lv_label_set_text(labelPose, buffer);

    colourSort();
    updateColourLabel();

    if (redMatch == true) {
      lv_obj_set_style_bg_color(screen, lv_color_make(139, 0, 0), 0);
    }
    if (redMatch == false) {
      lv_obj_set_style_bg_color(screen, lv_color_make(0, 0, 139), 0);
    }
    pros::delay(50);
  }
}

void nextAuto(lv_event_t *e) {
  currentAutoIndex = (currentAutoIndex + 1) % NUM_AUTOS;
  updateAutoDisplay();
}

void prevAuto(lv_event_t *e) {
  currentAutoIndex = (currentAutoIndex - 1 + NUM_AUTOS) % NUM_AUTOS;
  updateAutoDisplay();
}

void swapRedMatch(lv_event_t *e) { redMatch = !redMatch; }

void autonSelectorInit() {
  colour.set_led_pwm(100);
  screen = lv_screen_active();
  lv_obj_clean(screen);

  labelTitle = lv_label_create(screen);
  lv_obj_align(labelTitle, LV_ALIGN_TOP_MID, 0, 20);

  labelDesc = lv_label_create(screen);
  lv_label_set_long_mode(labelDesc, LV_LABEL_LONG_WRAP);
  lv_obj_set_width(labelDesc, 200);
  lv_obj_align(labelDesc, LV_ALIGN_CENTER, 0, -10);

  labelColour = lv_label_create(screen);
  lv_obj_align(labelColour, LV_ALIGN_TOP_RIGHT, 10, 40);
  lv_label_set_text(labelColour, "Colour: None");

  labelOptical = lv_label_create(screen);
  lv_obj_align(labelOptical, LV_ALIGN_TOP_LEFT, 10, 120);

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

  lv_obj_t *btnSwap = lv_button_create(screen);
  lv_obj_align(btnSwap, LV_ALIGN_TOP_RIGHT, 0, 0);
  lv_obj_t *lblSwap = lv_label_create(btnSwap);
  lv_label_set_text(lblSwap, "Swap Match");
  lv_obj_center(lblSwap);
  lv_obj_add_event_cb(btnSwap, swapRedMatch, LV_EVENT_CLICKED, NULL);

  labelPose = lv_label_create(screen);
  lv_obj_align(labelPose, LV_ALIGN_BOTTOM_MID, 0, -15);
  lv_label_set_text(labelPose, "X:0.0 Y:0.0 T:0.0");

  updateAutoDisplay();
}

void runSelectedAuton() { autos[currentAutoIndex].routine(); }

void controllerDisplayTask() {
  while (true) {
    lemlib::Pose pose = chassis.getPose();
    master.print(2, 0, "X:%.1f Y:%.1f T:%.1f", pose.x, pose.y, pose.theta);
    pros::delay(50);
  }
}

void initialize() {
  chassis.calibrate();
  pros::delay(200);
  autonSelectorInit();
  pros::Task ui(uiTask);
  pros::Task controllerDisplay(controllerDisplayTask);
  hood.set_value(false);
  lW.set_value(false);
  wing.set_value(false);
  wing.set_value(true);
  IR.set_value(false);
}

void autonomous() { runSelectedAuton(); }

bool IRToggled = false;
bool lastBState = false;
bool lWToggled = false;
bool lastDownState = false;
bool hoodToggled = false;
bool lastAState = false;

void opcontrol() {
  chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
  IR.set_value(true);

  while (true) {
    bool currentAState = master.get_digital(pros::E_CONTROLLER_DIGITAL_A);
    bool currentBState = master.get_digital(pros::E_CONTROLLER_DIGITAL_B);
    bool currentDownState = master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT);

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
      if ((redMatch && blueBallDetected) || (!redMatch && redBallDetected)) {
        stage1.move(127);
        stage2.move(127);
        stage3.move(127);
        pros::delay(150);
        stage1.move(0);
        stage2.move(0);
        stage3.move(0);
      } else {
        hood.set_value(true);
        stage1.move(127);
        stage2.move(127);
        stage3.move(-127);
      }
    } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
      if ((redMatch && redBallDetected) || (!redMatch && blueBallDetected)) {
        stage1.move(127);
        stage2.move(127);
        stage3.move(-127);
        pros::delay(150);
        stage1.move(0);
        stage2.move(0);
        stage3.move(0);
      } else {
        hood.set_value(false);
        stage1.move(127);
        stage2.move(127);
        stage3.move(127);
      }
    } else {
      stage1.move(0);
      stage2.move(0);
      stage3.move(0);
    }

    if (currentAState && !lastAState) {
      IRToggled = !IRToggled;
      wing.set_value(IRToggled);
    }

    if (currentBState && !lastBState) {
      hoodToggled = !hoodToggled;
      hood.set_value(hoodToggled);
    }

    if (currentDownState && !lastDownState) {
      lWToggled = !lWToggled;
      lW.set_value(lWToggled);
    }

    lastAState = currentAState;
    lastBState = currentBState;
    lastDownState = currentDownState;

    int leftY = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int rightX = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
    chassis.arcade(leftY, rightX * 0.9);

    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_UP)) {
      if (!pros::competition::is_connected())
        autonomous();
    }

    pros::delay(20);
  }
}
