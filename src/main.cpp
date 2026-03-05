#include "lemlib/chassis/chassis.hpp"
#include "liblvgl/widgets/label/lv_label.h"
#include <cmath>
#include <cstdio>
#define LEMLIB_USE_SCREEN false
#include "./devices.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "main.h"
#include "pros/adi.h" // IWYU pragma: keep
#include "pros/misc.h"
#include "pros/misc.hpp"
#include "pros/motors.h"
#include "pros/rtos.hpp"

// Distance Sensor Setup

// Unit Conversion Constant
#define MM_TO_IN 0.0393701

void zeroTheta() {
  lemlib::Pose pose = chassis.getPose();
  chassis.setPose(pose.x, pose.y, 0);
}

void firstreset() {
  lemlib::Pose pose = chassis.getPose();
  chassis.setPose(18.7402, 34.4488, 0);
}

int expecedDistFrontfirstgoal = 876;
int expecedDistRightfirstgoal = 476;

void secondreset() {
  lemlib::Pose pose = chassis.getPose();

  float correctedX = (expecedDistRightfirstgoal - dRight.get_distance()) / 25.4;
  float correctedY = (expecedDistFrontfirstgoal - dFront.get_distance()) / 25.4;

  chassis.setPose(correctedX, correctedY, 0);
}

int expecedDistRightSecondgoal = 480;

void thirdReset() {
  lemlib::Pose pose = chassis.getPose();

  float correctedX =
      (expecedDistRightSecondgoal - dRight.get_distance()) / 25.4;

  chassis.setPose(correctedX, 0, pose.theta);
}

int expectedDistLeft = 460;
int expectedDistBack = 1513;

float leftOffset() { return (expectedDistLeft - dLeft.get_distance()) / 25.4; }
float backOffset() { return (expectedDistBack - dBack.get_distance()) / 25.4; }

void intakeIn() {

  hood.set_value(false);
  stage1.move(127);
  stage2.move(127);
  stage3.move(-127);
}

void intakeReverse() {
  IR.set_value(true);
  hood.set_value(false);
  stage1.move(-127);
  stage2.move(-127);
  stage3.move(127);
}

void score() {
  hood.set_value(true);
  stage1.move(127);
  stage2.move(127);
  stage3.move(-127);
}

void intakeStop() {
  stage1.move(0);
  stage2.move(0);
  stage3.move(0);
}

// For getting center 5ish points

/*  chassis.turnToHeading(-130, 650);
  chassis.waitUntilDone();
  chassis.moveToPoint(-12.9, 3.3, 500);
  chassis.waitUntilDone();
  chassis.moveToPose(-21.8, -3.9, -108.7, 700, {.maxSpeed = 70});
  chassis.waitUntilDone();
  chassis.turnToHeading(-105, 500);
  chassis.waitUntilDone();
  chassis.moveToPoint(-26.5, -3.1, 500);
  chassis.waitUntilDone();
  chassis.turnToHeading(-89, 500);
  chassis.waitUntilDone();
  chassis.moveToPoint(-61.9, -3.9, 1000);
  chassis.waitUntilDone();
  lW.set_value(true);
  chassis.moveToPoint(-74.6, -2.6, 750, {.maxSpeed = 50});
  chassis.waitUntilDone();
  lW.set_value(false);
  chassis.turnToHeading(-223, 600);
  chassis.waitUntilDone();
  chassis.moveToPose(-67.7, -12.2, -220, 1500, {.lead = .25});
  chassis.waitUntilDone();
  chassis.moveToPoint(-63.9, -16.3, 400);
  chassis.waitUntilDone();
  stage1.move(-100);
  stage2.move(-127);
  stage3.move(0);
  pros::delay(2000);
  chassis.moveToPoint(-94, 4.9, 1500, {.forwards = false});
  chassis.waitUntilDone();
  chassis.turnToHeading(5, 500);
  chassis.waitUntilDone();*/

void skills() {
  chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
  chassis.setPose(0, 0, 0);
  IR.set_value(true);
  intakeIn();
  lW.set_value(true);
  hood.set_value(false);
  wing.set_value(true);
  chassis.moveToPoint(-.1, 31, 1200, {.maxSpeed = 90});
  chassis.waitUntilDone();
  chassis.turnToHeading(97.8, 800, {.minSpeed = 16});
  chassis.waitUntilDone();
  chassis.moveToPoint(3.65, 29.2, 650, {.minSpeed = 30});
  chassis.waitUntilDone();
  pros::delay(1750);
  chassis.waitUntilDone();
  chassis.moveToPoint(-1.1, 29.9, 700, {.forwards = false, .minSpeed = 40});
  chassis.waitUntilDone();
  pros::delay(100);
  chassis.turnToHeading(324, 950);
  lW.set_value(false);
  chassis.waitUntilDone();
  chassis.moveToPoint(-16.8, 46.4, 2000, {.maxSpeed = 50, .minSpeed = 5});
  chassis.waitUntilDone();
  pros::delay(150);
  chassis.turnToHeading(-79.9, 850);
  chassis.waitUntilDone();
  pros::delay(100);
  intakeStop();
  chassis.moveToPoint(-93.3, 61.2, 2500, {.maxSpeed = 85, .minSpeed = 0});
  chassis.waitUntilDone();
  pros::delay(200);
  chassis.turnToHeading(8.2, 650);
  chassis.waitUntilDone();
  chassis.moveToPoint(-96.5, 47.8, 750, {.forwards = false});
  chassis.waitUntilDone();
  chassis.turnToHeading(-80, 650);
  chassis.waitUntilDone();
  chassis.moveToPoint(-80.8, 47.5, 750, {.forwards = false, .minSpeed = 35});
  chassis.waitUntilDone();
  pros::delay(150);
  chassis.setPose(0, 0, 0);
  chassis.moveToPoint(0, -4, 500, {.maxSpeed = 75});
  chassis.waitUntilDone();
  firstreset();
  pros::delay(100);
  score();
  pros::delay(1500); // score time
  lW.set_value(true);
  chassis.moveToPose(17.9, 56.8, -2, 1500, {.lead = .25});
  chassis.waitUntil(5);
  intakeIn();
  chassis.waitUntilDone();
  chassis.moveToPoint(17.6, 60.9, 500, {.minSpeed = 35});
  chassis.waitUntilDone();
  pros::delay(1750);
  chassis.moveToPoint(18.9, 34.4, 750, {.forwards = false, .minSpeed = 35});
  chassis.waitUntilDone();
  score();
  chassis.moveToPoint(18.9, 25, 1500, {.forwards = false, .maxSpeed = 40});
  chassis.waitUntilDone();
  lW.set_value(false);
  pros::delay(100);
  secondreset();
  chassis.moveToPoint(0, 5, 500);
  chassis.waitUntilDone();
  chassis.turnToHeading(-90, 500);
  chassis.waitUntilDone();
  chassis.moveToPoint(-94.4, 15.8, 2250, {.maxSpeed = 90});
  chassis.waitUntilDone();
  chassis.turnToHeading(4, 550);
  chassis.waitUntilDone();
  lW.set_value(true);
  intakeIn();
  chassis.moveToPoint(-94.3, 29.8, 750, {.minSpeed = 15});
  chassis.waitUntilDone();
  chassis.moveToPoint(-94.2, 38, 2000, {.maxSpeed = 40});
  chassis.waitUntilDone();
  chassis.moveToPoint(-93, 22.7, 500, {.forwards = false});
  chassis.waitUntilDone();
  lW.set_value(false);
  wing.set_value(true);
  chassis.turnToHeading(-24, 600);
  chassis.waitUntilDone();
  chassis.moveToPoint(-82.5, 3.8, 500, {.forwards = false});
  chassis.waitUntilDone();
  chassis.turnToHeading(0, 500);
  chassis.waitUntilDone();
  chassis.moveToPoint(-83, -62.2, 2000, {.forwards = false, .maxSpeed = 90});
  chassis.waitUntilDone();
  chassis.turnToHeading(70, 550);
  chassis.waitUntilDone();
  chassis.moveToPoint(-99.8, -66.8, 750, {.forwards = false});
  chassis.waitUntilDone();
  chassis.turnToHeading(178, 650);
  chassis.waitUntilDone();
  chassis.moveToPoint(-100.2, -55.9, 650, {.forwards = false, .minSpeed = 35});
  chassis.waitUntilDone();
  score();
  intakeStop();
  score();
  thirdReset();
  pros::delay(1500);
  lW.set_value(true);
  chassis.moveToPoint(.7, -26.6, 500, {.minSpeed = 35});
  chassis.waitUntil(5);
  intakeIn();
  chassis.waitUntilDone();
  chassis.moveToPoint(.8, -29, 1750, {.maxSpeed = 45});
  chassis.waitUntilDone();
  chassis.moveToPoint(-.4, 0, 1250, {.forwards = false, .minSpeed = 35});
  chassis.waitUntilDone();
  score();
  pros::delay(1500);
  lW.set_value(false);
  chassis.moveToPoint(0, -6, 500);
  chassis.waitUntilDone();
  intakeIn();
  chassis.moveToPoint(0, 0, 500, {.forwards = false, .minSpeed = 35});
  chassis.waitUntilDone();
  chassis.moveToPoint(0, -13, 750);
  chassis.waitUntilDone();
  chassis.turnToHeading(317.5, 650);
  chassis.waitUntilDone();
  chassis.moveToPoint(17.9, -28.6, 750, {.forwards = false});
  chassis.waitUntilDone();
  chassis.turnToHeading(280, 650);
  chassis.waitUntilDone();
  chassis.moveToPoint(32.9, -32, 500, {.forwards = false});
  chassis.waitUntilDone();
  chassis.turnToHeading(270, 250);
  chassis.waitUntilDone();
  while (dBack.get_distance() > 1750) {
    chassis.moveToPoint(52.5, -32, 150, {.forwards = false, .minSpeed = 90});
  }
}

void sawp() {
  chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
  chassis.setPose(0, 0, 0);
  IR.set_value(true);
  hood.set_value(false);
  stage1.move(127);
  stage2.move(127);
  stage3.move(-127);
  lW.set_value(true);
  chassis.moveToPoint(-.1, 31, 1050);
  chassis.waitUntilDone();
  chassis.turnToHeading(98, 750, {.minSpeed = 20, .earlyExitRange = .1});
  chassis.waitUntilDone();
  chassis.moveToPoint(4.2, 29.7, 550, {.minSpeed = 45});
  chassis.waitUntilDone();
  pros::delay(350);
  chassis.moveToPoint(-22.2, 34.7, 1250, {.forwards = false, .minSpeed = 35});
  chassis.waitUntilDone();
  hood.set_value(true);
  stage1.move(127);
  stage2.move(127);
  stage3.move(-127);
  pros::delay(750);
  lW.set_value(false);
  chassis.turnToHeading(195, 750);
  chassis.waitUntilDone();
  chassis.moveToPoint(-34.2, 9.5, 500);
  hood.set_value(false);
  stage1.move(127);
  stage2.move(127);
  stage3.move(-127);
  chassis.waitUntilDone();
  chassis.moveToPoint(-42.8, -30.5, 1500, {.maxSpeed = 75});
  chassis.waitUntil(39.5);
  lW.set_value(true);
  chassis.waitUntilDone();
  chassis.turnToPoint(-50.6, -22.1, 500, {.forwards = false});
  chassis.waitUntilDone();
  chassis.moveToPoint(-50.8, -21.2, 850, {.forwards = false});
  chassis.waitUntilDone();
  hood.set_value(false);
  stage1.move(-127);
  stage2.move(-127);
  stage3.move(127);
  pros::delay(150);
  hood.set_value(false);
  stage1.move(127);
  stage2.move(127);
  stage3.move(70);
  pros::delay(875);
  lW.set_value(true);
  hood.set_value(false);
  stage1.move(127);
  stage2.move(127);
  stage3.move(-127);
  chassis.moveToPoint(-22.4, -59.7, 1500, {.maxSpeed = 95});
  chassis.waitUntilDone();
  chassis.turnToHeading(94, 750);
  chassis.waitUntilDone();
  chassis.moveToPoint(-9.3, -62.9, 1000, {.minSpeed = 35});
  chassis.waitUntilDone();
  chassis.moveToPose(-37.6, -60.8, 97.83, 400,
                     {.forwards = false, .minSpeed = 80});
  chassis.waitUntilDone();
  chassis.moveToPoint(-37.6, -60.8, 900,
                      {.forwards = false, .minSpeed = 40}); // 1270
  chassis.waitUntilDone();
  hood.set_value(true);
  stage1.move(127);
  stage2.move(127);
  stage3.move(-127);
  pros::delay(2500);
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
  // lW.set_value(true);
  pros::delay(250);
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
  stage3.move(60);
  pros::delay(1500);
  chassis.moveToPoint(-34.2, 8.4, 1750, {.maxSpeed = 70});
  chassis.waitUntilDone();
  hood.set_value(false);
  stage1.move(127);
  stage2.move(127);
  stage3.move(-127);
  chassis.turnToHeading(-178.7, 600);
  chassis.waitUntilDone();
  lW.set_value(true);
  pros::delay(350);
  chassis.moveToPoint(-36.4, -3.7, 845);
  chassis.waitUntilDone();
  chassis.moveToPoint(-36.5, 5.8, 500, {.forwards = false});
  chassis.waitUntilDone();
  chassis.moveToPose(-35.9, 23, -178.9, 1500, {.forwards = false});
  chassis.waitUntilDone();
  chassis.moveToPoint(-35.9, 27.2, 950, {.forwards = false, .minSpeed = 60});
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
  chassis.moveToPoint(-34.2, 8.4, 1250, {.maxSpeed = 70});
  chassis.waitUntilDone();
  hood.set_value(false);
  stage1.move(127);
  stage2.move(127);
  stage3.move(-127);
  chassis.turnToHeading(-178.7, 600);
  chassis.waitUntilDone();
  chassis.moveToPoint(-36.4, -3.5, 825);
  chassis.waitUntilDone();
  chassis.moveToPoint(-36.2, 6.7, 500, {.forwards = false});
  chassis.waitUntilDone();
  chassis.moveToPose(-37.3, 19.5, -179.5, 1250, {.forwards = false});
  chassis.waitUntilDone();
  chassis.moveToPoint(-37, 26.6, 1250, {.forwards = false, .minSpeed = 40});
  chassis.waitUntilDone();
  hood.set_value(true);
  stage1.move(127);
  stage2.move(127);
  stage3.move(-127);
  pros::delay(2000);
  chassis.moveToPoint(-38, 10.2, 500);
  chassis.waitUntilDone();
  chassis.moveToPose(-45.5, 27.9, -178.7, 2000, {.forwards = false});
  chassis.waitUntilDone();
  wing.set_value(false);
  lW.set_value(false);
  chassis.moveToPoint(-43.5, 42, 2500, {.forwards = false, .minSpeed = 50});
  chassis.waitUntilDone();
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
  chassis.waitUntilDone();
  chassis.turnToHeading(130, 600);
  chassis.waitUntilDone();
  chassis.moveToPoint(29.5, 8.4, 1250, {.maxSpeed = 75});
  chassis.waitUntilDone();
  chassis.turnToHeading(178.5, 600);
  lW.set_value(true);
  pros::delay(240);
  chassis.waitUntilDone();
  chassis.moveToPoint(31, -3.5, 850, {.minSpeed = 35});
  chassis.waitUntilDone();
  chassis.moveToPoint(31.2, 3, 500, {.forwards = false});
  chassis.waitUntilDone();
  chassis.moveToPose(29.9, 20.6, 178.2, 1500, {.forwards = false});
  chassis.waitUntilDone();
  chassis.moveToPoint(29.8, 26, 750, {.forwards = false, .minSpeed = 40});
  chassis.waitUntilDone();
  hood.set_value(true);
  stage1.move(127);
  stage2.move(127);
  stage3.move(-127);
  pros::delay(1950);
  chassis.moveToPoint(31.8, 8.6, 750);
  chassis.waitUntilDone();
  lW.set_value(false);
  chassis.moveToPose(21.4, 26, 177, 2000, {.forwards = false});
  chassis.waitUntilDone();
  wing.set_value(false);
  chassis.moveToPoint(20.5, 43.8, 2500, {.forwards = false, .minSpeed = 40});
  chassis.waitUntilDone();
}

void qRightSide() {
  expectedDistBack = 300;
  expectedDistLeft = 2065;
  chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
  chassis.setPose(0, 0 - backOffset(), 0);
  IR.set_value(true);
  hood.set_value(false);
  stage1.move(127);
  stage2.move(0);
  stage3.move(-127);
  chassis.moveToPoint(0, 20.5, 750);
  chassis.waitUntilDone();
  chassis.turnToHeading(45, 500);
  chassis.waitUntilDone();
  chassis.moveToPoint(7.6, 29.8, 800, {.maxSpeed = 100});
  chassis.waitUntilDone();
  chassis.turnToHeading(-43.1, 750);
  chassis.waitUntilDone();
  chassis.moveToPose(-1.1, 42.5, -43.8, 2500, {.horizontalDrift = .75});
  chassis.waitUntilDone();
  hood.set_value(false);
  stage1.move(-100);
  stage2.move(-127);
  stage3.move(127);
  pros::delay(1500);
  chassis.moveToPoint(32.1, 10.6, 2250, {.forwards = false, .maxSpeed = 70});
  lW.set_value(true);
  chassis.waitUntilDone();
  chassis.turnToHeading(-180, 800);
  chassis.waitUntilDone();
  chassis.moveToPoint(28.9, -2.4, 1000, {.minSpeed = 20});
  chassis.waitUntilDone();
  chassis.moveToPose(30.6, 21.6, -178, 1500, {.forwards = false});
  chassis.waitUntilDone();
  chassis.moveToPoint(31.2, 26.1, 1000, {.forwards = false});
  chassis.waitUntilDone();
  hood.set_value(true);
  stage1.move(127);
  stage2.move(127);
  stage3.move(-127);
  pros::delay(2500);
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
  chassis.moveToPoint(-5.1, 40.6, 1500, {.forwards = false, .maxSpeed = 75});
  hood.set_value(false);
  stage1.move(127);
  stage2.move(127);
  stage3.move(-127);
  chassis.waitUntilDone();
  lW.set_value(true);
  chassis.turnToHeading(-269, 600);
  chassis.waitUntilDone();
  chassis.moveToPoint(9, 38.2, 1100);
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

struct AutoRoutine {
  const char *name;
  const char *description;
  void (*routine)();
};

AutoRoutine autos[] = {
    {"skills", "skills", skills},
    {"SAWP", "4 Right, 6 High, 3 Left", sawp},
    {"Qualifying Left", "4 Mid, 3 Long", qLeft},
    {"Qualifying Right", "3 Low, 4 Long", qRight},
    {"Elims Left", "7 Long, Wing", elimLeft},
    {"Elims Right", "7 Long, Wing", elimRight},

};

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

  while (true) {
    bool currentAState = master.get_digital(pros::E_CONTROLLER_DIGITAL_A);
    bool currentBState = master.get_digital(pros::E_CONTROLLER_DIGITAL_B);
    bool currentDownState = master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT);

    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
      IR.set_value(true);
      hood.set_value(false);
      stage1.move(127);
      stage2.move(127);
      stage3.move(-127);
    } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
      IR.set_value(true);
      hood.set_value(false);
      stage1.move(-127);
      stage2.move(-127);
      stage3.move(127);
    } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
      IR.set_value(true);
      hood.set_value(true);
      stage1.move(127);
      stage2.move(127);
      stage3.move(-127);
    } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
      IR.set_value(true);
      hood.set_value(false);
      stage1.move(127);
      stage2.move(127);
      stage3.move(80);
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

    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_Y)) {
      IR.set_value(true);
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
