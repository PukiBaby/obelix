#include "main.h"

// ====================================
// SETUP - GENERAL
// ====================================


// ACTUATORS AND SENSORS SETUP
 
// We are using "#define" so that we can write the names of objects instead of the port number. 
// This improves readability. It also allows us to change everything here instead of having to go through the whole program.

// (1) Motors

// (1.1) Drivetrain motors
#define MOTOR_RB_PORT 1 // RB = right back
#define MOTOR_RM_PORT 2
#define MOTOR_RF_PORT 3

#define MOTOR_LB_PORT -4 // Negative ports indicate reversal
#define MOTOR_LM_PORT -5
#define MOTOR_LF_PORT -6

// (1.2) Shooting motors

// (2) Odometry pods
#define ODOM_POD_LEFT_PORT 7 //Don't know this yet
#define ODOM_POD_RIGHT_PORT 8 //Don't know this yet
#define ODOM_POD_BACK_PORT 9 //Don't know this yet


// STICK CONFIGURATION SETUP

// Similar to the above setup, we are using an enum so we can write names instead of numbers.

enum stickConfigEnum {
	ARCADE_SINGLE_STICK = 1,
	ARCADE_DOUBLE_STICK = 2,
	TANK = 3
}; // Should have a semicolon

enum stickConfigEnum currentStickConfig = ARCADE_DOUBLE_STICK;

// ====================================
// FORWARD DECLARATIONS
// ====================================

// ====================================
// PROS DEFAULT FUNCTIONS
// ====================================

void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);
}

void disabled() {}

void competition_initialize() {}

// Autonomous and opcontrol

void autonomous() {}

void opcontrol() {

	// (1) Construct all objects

	// (1.1) Construct motors
	pros::Motor motor_rb (MOTOR_RB_PORT);
	pros::Motor motor_rm (MOTOR_RM_PORT);
	pros::Motor motor_rf (MOTOR_RF_PORT);

	// IMPORTANT: reversed the left motors in the "ACTUATORS AND SENSORS SETUP" section
	pros::Motor motor_lb (MOTOR_LB_PORT);
	pros::Motor motor_lm (MOTOR_LM_PORT);
	pros::Motor motor_lf (MOTOR_LF_PORT);

	// (1.2) Construct motor groups
	pros::Motor_Group right_mg ({motor_rb, motor_rm, motor_rf}); // This uses set notation
	pros::Motor_Group left_mg ({motor_lb, motor_lm, motor_lf});

	// (1.3) Construct controller
	pros::Controller master (E_CONTROLLER_MASTER);

	// (2) While loop to execute driving
	while (true) {
		// (2.1) Determine how much to move left and right motor groups

		int dir; // Forward and backward movement
		int turn; // Right and left movement

		int tank_right;
		int tank_left;

		switch (currentStickConfig) {
			case ARCADE_SINGLE_STICK:
				dir = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
				turn = master.get_analog(E_CONTROLLER_ANALOG_LEFT_X);
				break;
			case ARCADE_DOUBLE_STICK:
				dir = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
				turn = master.get_analog(E_CONTROLLER_ANALOG_RIGHT_X);
				break;
			case TANK:
				tank_right = master.get_analog(E_CONTROLLER_ANALOG_RIGHT_Y);
				tank_left = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
				break;
		}

		// (2.2) Move the robot

		switch (currentStickConfig) {
			case ARCADE_SINGLE_STICK:
				right_mg.move(dir - turn); // If turn is positive, this should be moving slower (so left_mg pushes more and makes the bot face right)
				left_mg.move(dir + turn);
				break;
			case ARCADE_DOUBLE_STICK:
				right_mg.move(dir - turn);
				left_mg.move(dir + turn);
				break;
			case TANK:
				right_mg.move(tank_right);
				left_mg.move(tank_left);
		}

		// (2.3) Delay

		pros::delay(20); 
		// 20 ms delay to avoid overwhelming the processor
		// Multiple tasks are actually arranged by the scheduler instead of executed all at once
	}
}

// ====================================
// CUSTOM FUNCTIONS
// ====================================