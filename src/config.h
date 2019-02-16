/*
 * Different drivers report different values for the same gamepad.
 * Some of them may additionally report incorrect values for sticks.
 * Use this file to mitigate driver issues.
 */

#define A_BTN 0
#define B_BTN 1
#define X_BTN 2
#define Y_BTN 3
#define L_BTN 4
#define R_BTN 5
#define BACK_BTN 6
#define START_BTN 7
#define XBOX_BTN 8

#define DPAD_X 6
#define DPAD_Y 7

#define DPAD_UP -32767
#define DPAD_DOWN 32767
#define DPAD_RIGHT 32767
#define DPAD_LEFT -32767
#define DPAD_CENTERED 0

#define LSTICK_X 0
#define LSTICK_Y 1
#define RSTICK_X 3
#define RSTICK_Y 2

#define RT 4
#define LT 5

// ignore input data from stick if value is less than STICK_MIN_THRESHOLD
#define STICK_MIN_THRESHOLD 6000

// these values are multiplied to values reported by driver, so user can
// fix mixed up axes in this way
#define LSTICK_X_MULTIPLIER 1
#define LSTICK_Y_MULTIPLIER -1
#define RSTICK_X_MULTIPLIER 1
#define RSTICK_Y_MULTIPLIER -1
#define LT_MULTIPLIER 1
#define RT_MULTIPLIER 1


/*
 * serial communication configuration goes here
 */

#define BAUDRATE B9600
