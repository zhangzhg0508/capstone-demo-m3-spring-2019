#include "checkengine.h"
#include "Command.h"
#include "Pins.h"

namespace ino {

static constexpr int8_t switch_pin = 2;
static constexpr int8_t led_pin = 10;

void checkengine_interrupt() {
	pin<switch_pin>.set_mode(PinMode::Input);
	pin<led_pin>.set_mode(PinMode::Output);
 	switch(digitalRead(switch_pin)) {
	case LOW:
		pin<led_pin>.digital_write(LogicLevel::Low);
		break;
	case HIGH:
		pin<led_pin>.digital_write(LogicLevel::High);
		break;
	}
}

int cmd_checkengine_status(int argc, char** argv) {
	pin<switch_pin>.set_mode(PinMode::Input);
	if(argc != 1) {
		return command_error(F("Command 'checkengine_status' takes no arguments."));
	} else if(digitalRead(switch_pin) == HIGH) {
		return command_success(1);
	} else {
		return command_success(0);
	}
}

int cmd_checkengine_light(int argc, char** argv) {
	pin<led_pin>.set_mode(PinMode::Output);
	switch(argc) {
	default:
		return command_error(F("Command 'checkengine_light' takes at most 1 argument."));
	case 2:
		if(std::strcmp(argv[1], "ON") == 0 or std::strcmp(argv[1], "on") == 0) {
			(void)pin<led_pin>.digital_write(LogicLevel::High);
			digitalWrite(led_pin, HIGH);
		} else if(std::strcmp(argv[1], "OFF") or std::strcmp(argv[1], "off")) {
			(void)pin<led_pin>.digital_write(LogicLevel::Low);
			digitalWrite(led_pin, LOW);
		} else {
			return command_error(F("Expected one of 'on', 'ON', 'off', or 'OFF'."));
		}
		break;
	case 1:
		(void)0;
	}
	// Echo the current status of the led.
	if(digitalRead(led_pin) == HIGH) {
		return command_success(1);
	} else {
		return command_success(0);
	}
}

} /* namespace ino */

