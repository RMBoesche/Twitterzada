#include <iostream>
#include <string>
#include <arpa/inet.h>

class Validation {
public:
    static void validateUsername(std::string username);
    static void validatePort(std::string port);
    static void validateHostname(std::string hostname);
    static bool validateMessage(std::string message);
};