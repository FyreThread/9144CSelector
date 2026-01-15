#include "./lemlib/api.hpp" // IWYU pragma: keep
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/chassis/trackingWheel.hpp"
#include "pros/adi.hpp"
#include "pros/distance.hpp"
#include "pros/misc.hpp"

inline pros::Controller master(pros::E_CONTROLLER_MASTER);

inline pros::Distance dTop(17);

inline pros::Motor stage1(11);
inline pros::Motor stage2(19);
inline pros::Motor stage3(20);

inline pros::adi::DigitalOut hood('A', false);
inline pros::adi::DigitalOut lW('H', false);
inline pros::adi::DigitalOut wing('G', false);
inline pros::adi::DigitalOut IR('F', false);

inline pros::Controller controller(pros::E_CONTROLLER_MASTER);

// Define left and right motor groups for the drivetrain
inline pros::MotorGroup
    left_motors({-8, -9, -10},
                pros::MotorGearset::blue); // Left motors on ports -1, -2, -3
inline pros::MotorGroup
    right_motors({1, 2, 3},
                 pros::MotorGearset::blue); // Right motors on ports 4, 5, 6

// Define the drivetrain
inline lemlib::Drivetrain drivetrain(&left_motors, &right_motors, 9.125,
                                     lemlib::Omniwheel::NEW_325, 450, 2);
// Define the inertial sensor
inline pros::Imu imu(5); // Inertial sensor on port 4

// Define the vertical encoder
inline pros::Rotation vertical_encoder(7); // Optical shaft encoder on port
                                           // Optical shaft encoder on port

// Define the vertical tracking wheel
inline lemlib::TrackingWheel vertical_tracking_wheel(&vertical_encoder,

                                                     lemlib::Omniwheel::NEW_275,
                                                     lemlib::Omniwheel::NEW_325,
                                                     -1.75);
// Define the vertical tracking wheel

inline pros::Rotation horizontal_encoder(18);

inline lemlib::TrackingWheel horizontal_tracking_wheel(&horizontal_encoder,
                                                       lemlib::Omniwheel::NEW_2,
                                                       2.25);

// Setup odometry sensors
inline lemlib::OdomSensors
    sensors(&vertical_tracking_wheel,   // Vertical tracking wheel 1
            nullptr,                    // Vertical tracking wheel 2 (not used)
            &horizontal_tracking_wheel, // Horizontal tracking wheel 1
            nullptr, // Horizontal tracking wheel 2 (not used)
            &imu     // Inertial sensor
    );

// Define PID controllers5
inline lemlib::ControllerSettings
    lateral_controller(1, // Proportional gain (kP)
                       0, // Integral gain (kI)
                       0, // Derivative gain (kD)
                       0, // Anti windup
                       0, // Small error range, in inches
                       0, // Small error range timeout, in milliseconds
                       0, // Large error range, in inches
                       0, // Large error range timeout, in milliseconds
                       0  // Maximum acceleration (slew)
    );
inline lemlib::ControllerSettings
    angular_controller(2.0,  // Proportional gain (kP)
                       0.00, // Integral gain (kI)
                       16.6, // Derivative gain (kD)
                       0,    // Anti windup
                       0,    // Small error range, in degrees
                       0,    // Small error range timeout, in milliseconds
                       0,    // Large error range, in degrees
                       0,    // Large error range timeout, in milliseconds
                       0     // Maximum acceleration (slew)
    );

// Create the chassis
inline lemlib::Chassis chassis(drivetrain,         // Drivetrain settings
                               lateral_controller, // Lateral PID settings¯
                               angular_controller, // Angular PID settings
                               sensors             // Odometry sensors
);
