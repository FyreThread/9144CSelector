#include "./lemlib/api.hpp" // IWYU pragma: keep
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/chassis/trackingWheel.hpp"
#include "pros/adi.hpp"
#include "pros/misc.hpp"

inline pros::Controller master(pros::E_CONTROLLER_MASTER);

inline pros::Motor stage1(10);
inline pros::Motor stage2(9);
inline pros::Motor stage3(7);

inline pros::adi::DigitalOut hood('F');
inline pros::adi::DigitalOut lW('D');
inline pros::adi::DigitalOut intakeStopper('H');
inline pros::adi::DigitalOut intakeStopper2('G');

inline pros::Controller controller(pros::E_CONTROLLER_MASTER);

// Define left and right motor groups for the drivetrain
inline pros::MotorGroup
    left_motors({-1, -2, -3},
                pros::MotorGearset::blue); // Left motors on ports -1, -2, -3
inline pros::MotorGroup
    right_motors({4, 5, 6},
                 pros::MotorGearset::blue); // Right motors on ports 4, 5, 6

// Define the drivetrain
inline lemlib::Drivetrain drivetrain(&left_motors, &right_motors, 12.7,
                                     lemlib::Omniwheel::NEW_325, 450, 1.8);

// Define the inertial sensor
inline pros::Imu imu(18); // Inertial sensor on port 4

// Define the vertical encoder
inline pros::Rotation vertical_encoder(20);    // Optical shaft encoder on port
inline pros::Rotation horizontal_encoder(-19); // Optical shaft encoder on port

// Define the vertical tracking wheel
inline lemlib::TrackingWheel
    vertical_tracking_wheel(&vertical_encoder, lemlib::Omniwheel::NEW_2, -1.75);
// Define the vertical tracking wheel
inline lemlib::TrackingWheel horizontal_tracking_wheel(&horizontal_encoder,
                                                       lemlib::Omniwheel::NEW_2,
                                                       -2.75);

// Setup odometry sensors
inline lemlib::OdomSensors
    sensors(&vertical_tracking_wheel,   // Vertical tracking wheel 1
            nullptr,                    // Vertical tracking wheel 2 (not used)
            &horizontal_tracking_wheel, // Horizontal tracking wheel 1
            nullptr, // Horizontal tracking wheel 2 (not used)
            &imu     // Inertial sensor
    );

// Define PID controllers
inline lemlib::ControllerSettings
    lateral_controller(9.38, // Proportional gain (kP)
                       0,    // Integral gain (kI)
                       .31,  // Derivative gain (kD)
                       0,    // Anti windup
                       0,    // Small error range, in inches
                       0,    // Small error range timeout, in milliseconds
                       0,    // Large error range, in inches
                       0,    // Large error range timeout, in milliseconds
                       120   // Maximum acceleration (slew)
    );

inline lemlib::ControllerSettings
    angular_controller(2.2,  // Proportional gain (kP)
                       0.00, // Integral gain (kI)
                       10.2, // Derivative gain (kD)
                       0,    // Anti windup
                       0,    // Small error range, in degrees
                       0,    // Small error range timeout, in milliseconds
                       0,    // Large error range, in degrees
                       0,    // Large error range timeout, in milliseconds
                       0     // Maximum acceleration (slew)
    );

// Create the chassis
inline lemlib::Chassis chassis(drivetrain,         // Drivetrain settings
                               lateral_controller, // Lateral PID settings
                               angular_controller, // Angular PID settings
                               sensors             // Odometry sensors
);