#include "main.h"

	pros::Motor back_right_wheels(2);
	pros::Motor front_right_wheels(9);
	pros::Motor back_left_wheels(10);
	pros::Motor front_left_wheels(1);
	pros::Motor four_bar (5);
	pros::Motor clamp(6);
	pros::Motor two_bar(7);
	pros::Motor intake(8);

	pros::Controller master(CONTROLLER_MASTER);



	okapi::MotorGroup leftSet({-10,-1});
	okapi::MotorGroup rightSet({9,2});
	std::shared_ptr<ChassisController> chassis =
	ChassisControllerBuilder()
  	.withMotors(leftSet, rightSet)
  	.withDimensions(AbstractMotor::gearset::green, {{4_in, 11.5_in}, imev5GreenTPR})
  	.build();
/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button()
{
	static bool pressed = false;
	pressed = !pressed;
	if (pressed)
	{
		pros::lcd::set_text(2, "I was pressed!");
	}
	else
	{
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize()
{
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

void left_auton()
{
	chassis->moveDistance(6_ft);
	pros::delay(3);
	clamp.move_velocity(-10);
	pros::delay(3);
	chassis->moveDistance(-6_ft);
	pros::delay(3);
	chassis->turnAngle(-90_deg);
	pros::delay(3);
	chassis->moveDistance(-0.5_ft);
	pros::delay(3);
	intake.move_velocity(75);
}

void right_auton()
{
	chassis->moveDistance(6_ft);
	pros::delay(3);
	clamp.move_velocity(-10);
	chassis->turnAngle(-15_deg);
	pros::delay(3);
	chassis->moveDistance(-2.5_ft);
	pros::delay(3);
	chassis->turnAngle(15_deg);
	pros::delay(3);
	chassis->moveDistance(-2.5_ft);
	intake.move_velocity(75);
}

void skills_auton()
{

}

void autonomous()
{
	//left_auton();
	//right_auton();
	//skills_auton();
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol()
{
	autonomous();
	while(true)
	{
		front_left_wheels.set_brake_mode(MOTOR_BRAKE_COAST);
		back_right_wheels.set_brake_mode(MOTOR_BRAKE_COAST);
		front_right_wheels.set_brake_mode(MOTOR_BRAKE_COAST);
		back_right_wheels.set_brake_mode(MOTOR_BRAKE_COAST);
		four_bar.set_brake_mode(MOTOR_BRAKE_HOLD);
		clamp.set_brake_mode(MOTOR_BRAKE_HOLD);
		two_bar.set_brake_mode(MOTOR_BRAKE_HOLD);
		intake.set_brake_mode(MOTOR_BRAKE_COAST);

		int power = master.get_analog(ANALOG_LEFT_Y);
		int turn = master.get_analog(ANALOG_RIGHT_X);
		int left_move = power + turn;
		int right_move = power - turn;
		front_left_wheels.move(-left_move);
		back_left_wheels.move(-left_move);
		front_right_wheels.move(right_move);
		back_right_wheels.move(right_move);


		//Constant intake
		intake.move_velocity(50);
		//Deadzone for left analog stick
		if(abs(master.get_analog(ANALOG_LEFT_Y))>10)
		{
			power = master.get_analog(ANALOG_LEFT_Y);
		}
		else
		{
			power = 0;
		}
		//Deadzone for right analog stick
		if(abs(master.get_analog(ANALOG_RIGHT_X))>10)
		{
			turn = master.get_analog(ANALOG_RIGHT_X);
		}
		else
		{
			turn = 0;
		}

		//Clamp controlled by X and Y button
		if(master.get_digital(DIGITAL_X))
		{
			clamp.move_velocity(50);
		}
		else if(master.get_digital(DIGITAL_Y))
		{
			clamp.move_velocity(-50);
		}
		else
		{
			clamp.move_velocity(0);
		}
		//Fourbar controlled by R1 and R2
		if(master.get_digital(DIGITAL_R2))
		{
			four_bar.move_velocity(500);
		}
		else if(master.get_digital(DIGITAL_R1))
		{
			four_bar.move_velocity(-500);
		}
		else
		{
			four_bar.move_velocity(0);
		}

		//Twobar controlled by L1 and L2
		if(master.get_digital(DIGITAL_L2))
		{
			two_bar.move_velocity(25);
		}
		else if(master.get_digital(DIGITAL_L1))
		{
			two_bar.move_velocity(-25);
		}
		else
		{
			two_bar.move_velocity(0);
		}

		pros::delay(20);
	}
}
