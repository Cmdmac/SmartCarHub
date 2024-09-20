#ifndef _COMMAND_H
#include <string>
#include <sstream>
using namespace std;

typedef enum {
    DIRECTION = 100,
} Code;

class CodeBuilder {
    public:
        static string CreateCodeJson(Code cmd, int data) {
            std::stringstream ss;
            ss << "{\"code\":" << cmd << ",\"data\":" << data << "}";
            return ss.str();
        }
};
#endif