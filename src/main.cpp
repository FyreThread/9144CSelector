#include "lemlib/chassis/chassis.hpp"
#include "liblvgl/widgets/label/lv_label.h"
#include "pros/abstract_motor.hpp"
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

void wait() { chassis.waitUntilDone(); }

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
int expectedDistRight;

float leftOffset() { return (expectedDistLeft - dLeft.get_distance()) / 25.4; }
float rightOffset() {
  return (expectedDistRight - dRight.get_distance()) / 25.4;
}
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

void skills() {
  chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
  expectedDistBack = 1687;
  expectedDistRight = 456;
  chassis.setPose(0 + rightOffset(), 0 - backOffset(), 0);

  IR.set_value(true);
  intakeIn();
  lW.set_value(true);
  hood.set_value(false);
  wing.set_value(true);
  chassis.moveToPoint(0, 46.25, 1400, {.maxSpeed = 90});
  wait();
  chassis.turnToHeading(87, 650);
  wait();
  chassis.moveToPoint(6, 47.2, 650, {.minSpeed = 35});
  wait();
  chassis.moveToPoint(10, 47.2, 1800, {.maxSpeed = 40});
  wait();
  chassis.moveToPoint(1, 47.2, 500, {.forwards = false});
  wait();
  chassis.turnToHeading(50, 600);
  wait();
  lW.set_value(false);
  chassis.moveToPoint(-12.8, 36.8, 750, {.forwards = false});
  wait();
  chassis.turnToHeading(90, 600);
  wait();
  chassis.moveToPoint(-100, 29.5, 2250, {.forwards = false, .maxSpeed = 90});
  wait();
  chassis.turnToHeading(210, 700);
  wait();
  chassis.moveToPoint(-93.8, 47.4, 725, {.forwards = false});
  wait();
  chassis.turnToHeading(265, 600);
  wait();
  chassis.moveToPoint(-81.4, 48.4, 600, {.forwards = false, .minSpeed = 35});
  wait();
  score();
  chassis.moveToPoint(-70, 48.5, 2750, {.forwards = false, .minSpeed = 127});
  wait();
  lW.set_value(true);
  chassis.moveToPoint(-102.5, 47.5, 750);
  wait();
  chassis.moveToPoint(-107, 47.2, 1000, {.minSpeed = 35});
  chassis.waitUntil(6);
  intakeIn();
  wait();
  chassis.moveToPoint(-112, 47.2, 2750, {.maxSpeed = 37});
  wait();

  chassis.moveToPoint(-81.3, 48.6, 700, {.forwards = false, .minSpeed = 30});
  wait();
  score();
  chassis.moveToPoint(-70, 48.6, 3000, {.forwards = false, .minSpeed = 127});
  wait();
  secondreset();
  chassis.moveToPoint(0, 5, 500);
  wait();
  chassis.turnToHeading(-90, 500);
  wait();
  chassis.moveToPoint(-94.4, 15.8, 2250, {.maxSpeed = 90});
  wait();
  chassis.turnToHeading(4, 550);
  wait();
  lW.set_value(true);
  intakeIn();
  chassis.moveToPoint(-94.3, 29.8, 750, {.minSpeed = 15});
  wait();
  chassis.moveToPoint(-94.2, 38, 3000, {.maxSpeed = 35});
  wait();
  chassis.moveToPoint(-93, 22.7, 500, {.forwards = false});
  wait();
  lW.set_value(false);
  wing.set_value(true);
  chassis.turnToHeading(-24, 600);
  wait();
  chassis.moveToPoint(-82.5, 3.8, 750, {.forwards = false});
  wait();
  chassis.turnToHeading(0, 500);
  wait();
  chassis.moveToPoint(-83, -62.2, 2000, {.forwards = false, .maxSpeed = 90});
  wait();
  chassis.turnToHeading(70, 550);
  wait();
  chassis.moveToPoint(-98.1, -69, 750, {.forwards = false});
  wait();
  chassis.turnToHeading(181, 650);
  wait();
  chassis.moveToPoint(-98.5, -55.6, 650, {.forwards = false, .minSpeed = 35});
  wait();
  score();
  intakeStop();
  score();
  chassis.moveToPoint(-98.5, -35, 2000, {.forwards = false, .minSpeed = 100});
  wait();
  thirdReset();
  lW.set_value(true);
  chassis.moveToPoint(1, -27, 500, {.minSpeed = 35});
  chassis.waitUntil(5);
  intakeIn();
  wait();
  chassis.moveToPoint(1, -30, 2500, {.maxSpeed = 45});
  wait();
  chassis.moveToPoint(-.2, 0, 1250, {.forwards = false, .minSpeed = 35});
  wait();
  score();
  pros::delay(1500);
  lW.set_value(false);

  // No randoms route

  chassis.moveToPoint(0, -6, 500);
  wait();
  intakeIn();
  chassis.moveToPoint(0, 0, 500, {.forwards = false, .minSpeed = 35});
  wait();
  chassis.moveToPoint(0, -13, 750);
  wait();
  chassis.turnToHeading(317.5, 650);
  wait();
  chassis.moveToPoint(17.9, -28.6, 750, {.forwards = false});
  wait();
  chassis.turnToHeading(280, 650);
  wait();
  chassis.moveToPoint(32.9, -32, 500, {.forwards = false});
  wait();
  chassis.turnToHeading(270, 250);
  wait();
  while (dBack.get_distance() > 1800) {
    chassis.moveToPoint(52.5, -32, 150, {.forwards = false, .minSpeed = 87});
  }

  // randoms route
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
  wait();
  chassis.turnToHeading(98, 750, {.minSpeed = 20, .earlyExitRange = .1});
  wait();
  chassis.moveToPoint(4.2, 29.7, 550, {.minSpeed = 45});
  wait();
  pros::delay(350);
  chassis.moveToPoint(-22.2, 34.7, 1250, {.forwards = false, .minSpeed = 35});
  wait();
  score();
  pros::delay(750);
  lW.set_value(false);
  chassis.turnToHeading(195, 750);
  wait();
  chassis.moveToPoint(-34.2, 9.5, 500);
  intakeIn();
  wait();
  chassis.moveToPoint(-42.8, -30.5, 1500, {.maxSpeed = 75});
  chassis.waitUntil(39.5);
  lW.set_value(true);
  wait();
  chassis.turnToPoint(-50.6, -22.1, 500, {.forwards = false});
  wait();
  chassis.moveToPoint(-50.8, -21.2, 850, {.forwards = false});
  wait();
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
  wait();
  chassis.turnToHeading(94, 750);
  wait();
  chassis.moveToPoint(-9.3, -62.9, 1000, {.minSpeed = 35});
  wait();
  chassis.moveToPose(-37.6, -60.8, 97.83, 400,
                     {.forwards = false, .minSpeed = 80});
  wait();
  chassis.moveToPoint(-37.6, -60.8, 900,
                      {.forwards = false, .minSpeed = 40}); // 1270
  wait();
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
  wait();
  chassis.moveToPose(-6.8, 33.2, -30.6, 2000);
  chassis.waitUntil(17.5);
  // lW.set_value(true);
  pros::delay(250);
  wait();
  chassis.turnToHeading(-135, 600);
  wait();
  chassis.moveToPoint(0.4, 43.9, 1000, {.forwards = false});
  wait();
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
  wait();
  hood.set_value(false);
  stage1.move(127);
  stage2.move(127);
  stage3.move(-127);
  chassis.turnToHeading(-178.7, 600);
  wait();
  lW.set_value(true);
  pros::delay(350);
  chassis.moveToPoint(-36.4, -3.7, 845);
  wait();
  chassis.moveToPoint(-36.5, 5.8, 500, {.forwards = false});
  wait();
  chassis.moveToPose(-35.9, 23, -178.9, 1500, {.forwards = false});
  wait();
  chassis.moveToPoint(-35.9, 27.2, 950, {.forwards = false, .minSpeed = 60});
  wait();
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
  wait();
  chassis.moveToPose(-6.8, 33.2, -30.6, 2000);
  chassis.waitUntil(17.5);
  lW.set_value(true);
  wait();
  chassis.turnToHeading(-135, 600);
  wait();
  chassis.moveToPoint(-35, 6.6, 1250, {.maxSpeed = 70});
  wait();
  hood.set_value(false);
  stage1.move(127);
  stage2.move(127);
  stage3.move(-127);
  chassis.turnToHeading(-180, 600);
  wait(); /*
   chassis.moveToPoint(-36.4, -3.5, 825);
   wait();
   chassis.moveToPoint(-36.2, 6.7, 500, {.forwards = false});
   wait();
   chassis.moveToPose(-37.3, 19.5, -179.5, 1250, {.forwards = false});
   wait();
   chassis.moveToPoint(-37, 26.6, 1250, {.forwards = false, .minSpeed = 40});
   wait();
   hood.set_value(true);
   stage1.move(127);
   stage2.move(127);
   stage3.move(-127);
   pros::delay(2000);
   chassis.moveToPoint(-38, 10.2, 500);
   wait();
   chassis.moveToPose(-45.5, 27.9, -178.7, 2000, {.forwards = false});
   wait();
   wing.set_value(false);
   lW.set_value(false);
   chassis.moveToPoint(-43.5, 42, 2500, {.forwards = false, .minSpeed = 50});
   wait();*/
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
  wait();
  chassis.moveToPose(6.4, 29.5, 42.6, 2000);
  chassis.waitUntil(17.5);
  wait();
  chassis.turnToHeading(130, 600);
  wait();
  chassis.moveToPoint(29.5, 8.4, 1250, {.maxSpeed = 75});
  wait();
  chassis.turnToHeading(178.5, 600);
  lW.set_value(true);
  pros::delay(240);
  wait();
  chassis.moveToPoint(31, -3.5, 850, {.minSpeed = 35});
  wait();
  chassis.moveToPoint(31.2, 3, 500, {.forwards = false});
  wait();
  chassis.moveToPose(29.9, 20.6, 178.2, 1500, {.forwards = false});
  wait();
  chassis.moveToPoint(29.8, 26, 750, {.forwards = false, .minSpeed = 40});
  wait();
  hood.set_value(true);
  stage1.move(127);
  stage2.move(127);
  stage3.move(-127);
  pros::delay(1950);
  chassis.moveToPoint(31.8, 8.6, 750);
  wait();
  lW.set_value(false);
  chassis.moveToPose(21.4, 26, 177, 2000, {.forwards = false});
  wait();
  wing.set_value(false);
  // chassis.moveToPoint(20.5, 43.8, 2500, {.forwards = false, .minSpeed = 40});
  // wait();
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
  wait();
  chassis.turnToHeading(45, 500);
  wait();
  chassis.moveToPoint(7.6, 29.8, 800, {.maxSpeed = 100});
  wait();
  chassis.turnToHeading(-43.1, 750);
  wait();
  chassis.moveToPose(-1.1, 42.5, -43.8, 2500, {.horizontalDrift = .75});
  wait();
  hood.set_value(false);
  stage1.move(-100);
  stage2.move(-127);
  stage3.move(127);
  pros::delay(1500);
  chassis.moveToPoint(32.1, 10.6, 2250, {.forwards = false, .maxSpeed = 70});
  lW.set_value(true);
  wait();
  chassis.turnToHeading(-180, 800);
  wait();
  chassis.moveToPoint(28.9, -2.4, 1000, {.minSpeed = 20});
  wait();
  chassis.moveToPose(30.6, 21.6, -178, 1500, {.forwards = false});
  wait();
  chassis.moveToPoint(31.2, 26.1, 1000, {.forwards = false});
  wait();
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
  wait();
  chassis.moveToPose(-13.4, 22.5, -112, 1500);
  wait();
  chassis.moveToPose(-26.1, 16.1, -126, 1000);
  chassis.waitUntil(8);
  lW.set_value(true);
  wait();
  chassis.moveToPose(-36.6, 8.0, -135, 1500);
  lW.set_value(false);
  wait();
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
  wait();
  lW.set_value(true);
  chassis.turnToHeading(-269, 600);
  wait();
  chassis.moveToPoint(9, 38.2, 1100);
  wait();
  chassis.moveToPose(-16, 38.5, -270, 1750, {.forwards = false});
  wait();
  chassis.moveToPoint(-23, 38.5, 1250, {.forwards = false});
  wait();
  hood.set_value(true);
  stage1.move(127);
  stage2.move(127);
  stage3.move(-127);
  pros::delay(2000);
}

void splitLeft() {
  expectedDistBack = 207;
  expectedDistRight = 1315;
  chassis.setPose(0, 0 - backOffset(), 0);
  IR.set_value(true);
  intakeIn();
  chassis.moveToPoint(0, 24.16, 830, {.earlyExitRange = 0.10});
  wait();
  chassis.swingToPoint(-16.3, 35.7, lemlib::DriveSide::LEFT, 630,
                       {.maxSpeed = 70}); // 1000
  chassis.waitUntil(4.8);
  lW.set_value(true);
  chassis.moveToPoint(-8.5, 35.3, 400, {.earlyExitRange = 0.75});
  wait();
  chassis.turnToHeading(-135.0, 550, {.earlyExitRange = .85});
  wait();
  chassis.moveToPose(-33.3, 12.4, -134.4, 1700,
                     {.minSpeed = 75, .earlyExitRange = .45}); // bad pose
  wait();
  chassis.turnToHeading(-178.1, 470,
                        {.earlyExitRange = .4}); // turn to goal first score
  wait();
  chassis.moveToPose(
      -34.4, 30.0, -177.8, 500,
      {.forwards = false, .minSpeed = 65, .earlyExitRange = 3.35}); // 55
  wait();
  chassis.moveToPoint(-34.4, 32.0, 770, {.forwards = false, .minSpeed = 105});
  wait();
  score(); // score blocks
  pros::delay(890);
  chassis.moveToPoint(-34.2, 11.5, 400, {.earlyExitRange = 1.45});
  intakeIn();
  wait();
  chassis.turnToHeading(-180.4, 170, {.earlyExitRange = .25});
  wait();
  chassis.moveToPoint(-34.8, 1.7, 900, {.maxSpeed = 75, .minSpeed = 40});
  wait();
  pros::delay(1450); // matchloadtime
  chassis.moveToPoint(
      -35.6, 7.3, 300,
      {.forwards = false, .minSpeed = 105, .earlyExitRange = 1.15});

  /*chassis.turnToHeading(-177.4, 500);
  wait();
  chassis.moveToPoint(-32.9, 31.3, 850, {.forwards = false, .minSpeed = 45});
  wait();
  score();*/ /*
  chassis.moveToPoint(-32.8, 35, 850, {.forwards = false, .minSpeed = 90});
  wait();
  intakeIn();
  lW.set_value(true);*/
  /*chassis.setPose(0, 0, 0);
  chassis.moveToPoint(0, 21.1, 500);
  wait();
  chassis.moveToPoint(0, 26.7, 500, {.maxSpeed = 50, .minSpeed = 35});
  wait();
  pros::delay(475);
  chassis.moveToPoint(0, 15.7, 750, {false});
  wait();
  chassis.turnToHeading(45, 750);
  wait();
  chassis.moveToPoint(-31.2, -13.7, 1500, {false});
  wait();
  chassis.turnToHeading(45, 150);
  chassis.moveToPoint(-36, -18.5, 500, {false});
  wait();
  IR.set_value(true);
  hood.set_value(false);
  stage1.move(127);
  stage2.move(127);
  stage3.move(75);
  pros::delay(1000);
  intakeStop();
  lW.set_value(false);
  chassis.moveToPoint(-10.5, 10.6, 1500, {.maxSpeed = 95});
  wait();
  chassis.turnToHeading(175, 1000, {.maxSpeed = 80});
  wait();
  chassis.moveToPoint(-7.4, -21.4, 1800);
  wait();*/
  pros::delay(3000);
}

void splitRight() {
  expectedDistBack = 207;
  expectedDistRight = 1315;
  chassis.setPose(0, 0 - backOffset(), 0);
  IR.set_value(true);

  intakeIn();
  chassis.moveToPoint(.22, 17.77, 500);
  wait();
  chassis.turnToHeading(24, 400);
  wait();
  chassis.moveToPoint(7.3, 37.6, 750);
  chassis.waitUntil(9);
  lW.set_value(true);
  wait();
  chassis.turnToHeading(128.6, 500);
  wait();
  chassis.moveToPoint(32, 17.5, 1000);
  wait();
  chassis.turnToHeading(177.4, 500);
  wait();
  chassis.moveToPoint(32.9, 31.3, 750, {.forwards = false, .minSpeed = 45});
  wait();
  score();
  chassis.moveToPoint(32.8, 35, 1500, {.forwards = false, .minSpeed = 60});
  wait();
  chassis.setPose(0, 0, 0);
  stage1.move(127);
  stage2.move(-10);
  hood.set_value(false);
  chassis.turnToHeading(7, 400);
  wait();
  chassis.moveToPoint(.2, 23.1, 500, {.earlyExitRange = 1});
  wait();
  chassis.moveToPoint(.2, 28.2, 500, {.maxSpeed = 55, .minSpeed = 35});
  wait();
  pros::delay(425);
  chassis.moveToPoint(-.2, 20.5, 500, {false});
  wait();
  chassis.turnToHeading(131, 700);
  wait();
  lW.set_value(false);
  chassis.moveToPose(33.3, -12.8, 135, 1750);
  wait();
  stage1.move(-97);
  stage2.move(-127);
  pros::delay(1250);
  chassis.moveToPoint(10.7, 10.7, 1000, {false});
  wait();
  chassis.turnToHeading(0, 750, {.maxSpeed = 75});
  wait();
  chassis.moveToPoint(10.7, -11.6, 2000, {.forwards = false, .minSpeed = 45});
  wait();
}

struct AutoRoutine {
  const char *name;
  const char *description;
  void (*routine)();
};

AutoRoutine autos[] = {
    {"Split Right", "tuning", splitRight},
    {"Split Right", "Right 4 long 3 mid", splitRight},

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
  stage2.set_brake_mode(pros::MotorBrake::hold);
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
      stage3.move(75);
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
