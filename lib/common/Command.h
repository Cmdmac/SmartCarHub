#ifndef COMMAND_H_
#define COMMAND_H_

#include <string>
#include <sstream>
using namespace std;

#define CMD_FORWARD 1
#define CMD_BACKWARD 2
#define CMD_TURN_LEFT 3
#define CMD_TURN_RIGHT 4
#define CMD_BRAKE 5
#define CMD_REPORT_DIRECTION 100
#define CMD_SET_SPEED 101
#define CMD_SET_DIRECTION 102

#define PIN_COMPASS_SDA 11
#define PIN_COMPASS_SCL 10

class CommandBuilder {
    public:
        static string CreateCodeJson(int cmd, int data) {
            std::stringstream ss;
            ss << "{\"code\":" << cmd << ",\"data\":" << data << "}";
            return ss.str();
        }
};
#endif