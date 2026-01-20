#include "liblvgl/widgets/label/lv_label.h"
#include <cmath>
#define LEMLIB_USE_SCREEN false
#include "./devices.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "main.h"
#include "pros/adi.h" // IWYU pragma: keep
#include "pros/misc.h"
#include "pros/misc.hpp"
#include "pros/motors.h"
#include "pros/rtos.hpp"

int expectedDistLeft = 460;
int expectedDistBack = 1513;

float leftOffset() { return (expectedDistLeft - dLeft.get_distance()) / 25.4; }
float backOffset() { return (expectedDistBack - dBack.get_distance()) / 25.4; }

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
}

// Qual Match Left
void qLeft() {
  expectedDistBack = 300;
  expectedDistLeft = 1307;
  chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
  chassis.setPose(0, 0 - backOffset(), 0);
  IR.set_value(true);
  hood.set_value(false);
  stage1.move(127);
  stage2.move(127);
  stage3.move(-127);
  chassis.moveToPoint(0, 11, 500);
  chassis.waitUntilDone();
  chassis.moveToPose(-6.8, 33.2, -30.6, 2000);
  chassis.waitUntil(17.5);
  lW.set_value(true);
  chassis.waitUntilDone();
  chassis.turnToHeading(-135, 600);
  chassis.waitUntilDone();
  chassis.moveToPoint(0.4, 43.9, 1000, {.forwards = false});
  chassis.waitUntilDone();
  hood.set_value(false);
  stage1.move(-127);
  stage2.move(-127);
  stage3.move(-127);
  pros::delay(250);
  stage1.move(127);
  stage2.move(127);
  stage3.move(66);
  pros::delay(1500);
  chassis.moveToPoint(-34.2, 8.4, 1750, {.maxSpeed = 70});
  chassis.waitUntilDone();
  hood.set_value(false);
  stage1.move(127);
  stage2.move(127);
  stage3.move(-127);
  chassis.turnToHeading(-178.7, 600);
  chassis.waitUntilDone();
  chassis.moveToPoint(-36.4, -3.5, 850);
  chassis.waitUntilDone();
  chassis.moveToPoint(-36.2, 26.7, 1000, {.forwards = false});
  chassis.waitUntilDone();
  hood.set_value(true);
  stage1.move(127);
  stage2.move(127);
  stage3.move(-127);
  pros::delay(2500);
}

// Elim Match Left
void elimLeft() {
  expectedDistBack = 300;
  expectedDistLeft = 1307;
  chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
  chassis.setPose(0, 0 - backOffset(), 0);
  IR.set_value(true);
  hood.set_value(false);
  stage1.move(127);
  stage2.move(127);
  stage3.move(-127);
  chassis.moveToPoint(0, 11, 500);
  chassis.waitUntilDone();
  chassis.moveToPose(-6.8, 33.2, -30.6, 2000);
  chassis.waitUntil(17.5);
  lW.set_value(true);
  chassis.waitUntilDone();
  chassis.turnToHeading(-135, 600);
  chassis.waitUntilDone();
  chassis.moveToPoint(-34.2, 8.4, 1750, {.maxSpeed = 70});
  chassis.waitUntilDone();
  hood.set_value(false);
  stage1.move(127);
  stage2.move(127);
  stage3.move(-127);
  chassis.turnToHeading(-178.7, 600);
  chassis.waitUntilDone();
  chassis.moveToPoint(-36.4, -3.5, 850);
  chassis.waitUntilDone();
  chassis.moveToPoint(-36.2, 26.7, 1000, {.forwards = false});
  chassis.waitUntilDone();
  hood.set_value(true);
  stage1.move(127);
  stage2.move(127);
  stage3.move(-127);
  pros::delay(2500);
}

// Elim Match Right
void elimRight() {
  expectedDistBack = 300;
  expectedDistLeft = 2065;
  chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
  chassis.setPose(0, 0 - backOffset(), 0);
  IR.set_value(true);
  hood.set_value(false);
  stage1.move(127);
  stage2.move(127);
  stage3.move(-127);
  chassis.moveToPoint(0, 11, 500);
  chassis.waitUntilDone();
  chassis.moveToPose(6.4, 29.5, 42.6, 2000);
  chassis.waitUntil(17.5);
  lW.set_value(true);
  chassis.waitUntilDone();
  chassis.turnToHeading(130, 600);
  chassis.waitUntilDone();
  chassis.moveToPoint(29.5, 8.4, 1600);
  chassis.waitUntilDone();
  chassis.turnToHeading(178.5, 600);
  chassis.waitUntilDone();
  chassis.moveToPoint(31, -3.5, 875);
  chassis.waitUntilDone();
  chassis.moveToPoint(31.2, 3, 500, {.forwards = false});
  chassis.waitUntilDone();
  chassis.moveToPose(31.8, 15.7, 177.8, 1500, {.forwards = false});
  chassis.waitUntilDone();
  chassis.moveToPoint(31.4, 26.3, 1000, {.forwards = false, .minSpeed = 30});
  chassis.waitUntilDone();
  hood.set_value(true);
  stage1.move(127);
  stage2.move(127);
  stage3.move(-127);
  pros::delay(1900);
  chassis.moveToPoint(31.8, 8.6, 750);
  chassis.waitUntilDone();
  lW.set_value(false);
  chassis.moveToPose(21.4, 26, 177, 2000, {.forwards = false});
  chassis.waitUntilDone();
  wing.set_value(false);
  chassis.moveToPoint(22.5, 42.1, 2000, {.forwards = false, .minSpeed = 60});
  chassis.waitUntilDone();
}

void qRight() {
  expectedDistBack = 1920;
  chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
  chassis.setPose(0, 0 - backOffset(), 0);
  IR.set_value(true);
  hood.set_value(false);
  stage1.move(127);
  stage2.move(127);
  stage3.move(-127);
  chassis.moveToPoint(0, 12, 500);
  chassis.waitUntilDone();
  chassis.moveToPose(-13.4, 22.5, -112, 1500);
  chassis.waitUntilDone();
  chassis.moveToPose(-26.1, 16.1, -126, 1000);
  chassis.waitUntil(8);
  lW.set_value(true);
  chassis.waitUntilDone();
  chassis.moveToPose(-36.6, 8.0, -135, 1500);
  lW.set_value(false);
  chassis.waitUntilDone();
  hood.set_value(false);
  stage1.move(-100);
  stage2.move(-127);
  stage3.move(-127);
  pros::delay(1250);
  stage2.move(-127);
  stage3.move(-127);
  chassis.moveToPoint(-5.1, 40.6, 1500, {.forwards = false});
  hood.set_value(false);
  stage1.move(127);
  stage2.move(127);
  stage3.move(-127);
  chassis.waitUntilDone();
  lW.set_value(true);
  chassis.turnToHeading(-269, 600);
  chassis.waitUntilDone();
  chassis.moveToPoint(9, 38.2, 1250);
  chassis.waitUntilDone();
  chassis.moveToPose(-16, 38.5, -270, 1750, {.forwards = false});
  chassis.waitUntilDone();
  chassis.moveToPoint(-23, 38.5, 1250, {.forwards = false});
  chassis.waitUntilDone();
  hood.set_value(true);
  stage1.move(127);
  stage2.move(127);
  stage3.move(-127);
  pros::delay(2000);
}

void skillsAuto() {}
void doNothing() {}

struct AutoRoutine {
  const char *name;
  const char *description;
  void (*routine)();
};

AutoRoutine autos[] = {{"Elims Left", "7 Long, Wing", elimLeft},
                       {"Qualifying Left", "4 Mid, 3 Long", qLeft},
                       {"Qualifying Right", "4 Low, 3 Long", qRight},
                       {"Elims Left", "7 Long, Wing", elimLeft},
                       {"Elims Right", "7 Long, Wing", elimRight}};

const int NUM_AUTOS = sizeof(autos) / sizeof(autos[0]);
int currentAutoIndex = 0;

lv_obj_t *labelTitle;
lv_obj_t *labelDesc;
lv_obj_t *labelPose;
lv_obj_t *screen;

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
    pros::delay(250);
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
