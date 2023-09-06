#include "DShotRMT.h" // We need this for the dshot modes
#include "boards_config.h" // board pinouts are in this file. you can check and modify which switch is on which pin there.

//Selector settings ON BOOT, locked after booting
uint32_t revRPMset[3][2] = { { 50000, 50000, 50000, 50000 }, { 25000, 25000, 25000, 25000 },  { 14000, 14000, 14000, 14000 } }; // adjust this to change fps, groups are firingMode 1, 2, 3, and elements in group are individual motor RPM
uint32_t idleRPMset[3][2] = { { 25000, 25000, 25000, 25000 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }; // adjust this to change idleRPM, groups are firingMode 1, 2, 3, and elements in group are individual motor RPM
uint32_t idleTimeSet_ms[3] = { 0, 0, 15000 }; // how long to idle the flywheels for after releasing the trigger, in milliseconds, for firingMode 1, 2, 3
uint32_t firingDelaySet_ms[3] = {150, 75, 50}; // delay to allow flywheels to spin up before firing dart for firingMode 1, 2, 3

//Live selector settings, change with switch
uint32_t burstLengthSet[3] = { 300, 3, 1 };
uint32_t BufferModeSet[3] = { 0, 1, 1 };
// 0 = stop firing when trigger is released
// 1 = complete current burst when trigger is released
// 2 = fire as many bursts as trigger pulls
// for full auto, set burstLength high (50+) and bufferMode = 0

// Flywheel Settings
uint32_t motorKv = 3200;
uint32_t batteryADC_mv = 14800 / 11; // battery voltage in mv divided by voltage divider ratio (11)
dshot_mode_t dshotMode = DSHOT300; // Options are DSHOT150, DSHOT300, DSHOT600, or DSHOT_OFF. DSHOT300 is recommended, DSHOT150 does not work with either AM32 ESCs or closed loop control, and DSHOT600 seems less reliable. DSHOT_OFF falls back to servo PWM

// Dettlaff Settings
char wifiSsid[32] = "network name";
char wifiPass[63] = "password";
uint32_t wifiDuration_ms = 10 * 60 * 1000; // how long before wifi turns off to save power. default is 10 min

pins_t pins = pins_v0_7; // select the one that matches your board revision
// Options:
// pins_v0_7
// pins_v0_6
// pins_v0_5
// pins_v0_4_n20
// pins_v0_4_noid
// pins_v0_3_n20
// pins_v0_3_noid
// _noid means use the flywheel output to drive a solenoid pusher
// _n20 for a pusher motor on the pusher output
// pins_v0_2
// pins_v0_1

// Pusher Settings
pusherType_t pusherType = PUSHER_MOTOR_CLOSEDLOOP; // either PUSHER_MOTOR_CLOSEDLOOP or PUSHER_SOLENOID_OPENLOOP
uint16_t solenoidExtendTime_ms = 20;
uint16_t solenoidRetractTime_ms = 35;
bool pusherReverseDirection = false; // make motor spin backwards? v0.5 & v0.6 (hBridgeDriver) need this to be false or the pusher logic is inverted? and the v0.2 - v0.4 at8870 pusher seems to need this to be true for reverse polarity braking to work?
uint8_t pusherReversePolarityDuration_ms = 0; // try increasing this if your pusher doesn't stop at the right position because your pusher motor takes too long to stop. 10ms was good for my FDL with cheap pusher motor
bool pusherReverseOnOverrun = false; // these two settings don't seem to work properly
bool pusherEndReverseBrakingEarly = false;

// Advanced Settings
uint16_t pusherStallTime_ms = 500; // for PUSHER_MOTOR_CLOSEDLOOP, how long do you run the motor without seeing an update on the cycle control switch before you decide the motor is stalled?
uint8_t numMotors = 4; // leave at 4 until we have closed loop control
uint16_t spindownSpeed = 1; // higher number makes the flywheels spin down faster after you release the trigger
bool revSwitchNormallyClosed = false; // invert switch signal?
bool triggerSwitchNormallyClosed = false;
bool cycleSwitchNormallyClosed = false;
uint16_t debounceTime_ms = 25;
char AP_SSID[32] = "Dettlaff";
char AP_PW[32] = "KellyIndu";
uint16_t targetLoopTime_us = 1000; // microseconds
float maxDutyCycle_pct = 98;
uint8_t deadtime = 10;
uint16_t pwmFreq_hz = 20000;
uint16_t servoFreq_hz = 200;
