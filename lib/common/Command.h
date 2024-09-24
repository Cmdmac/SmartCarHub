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
#define CMD_DIRECTION 100

class CommandBuilder {
    public:
        static string CreateCodeJson(int cmd, int data) {
            std::stringstream ss;
            ss << "{\"code\":" << cmd << ",\"data\":" << data << "}";
            return ss.str();
        }
};
#endif