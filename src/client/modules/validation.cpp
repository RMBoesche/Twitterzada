#include "validation.h"

void Validation::validateUsername(std::string username) {
    if (username.length() < 4 || username.length() > 20) 
        throw std::invalid_argument("The username must be between 4 and 20 digits long.");
}

void Validation::validatePort(std::string port) {
    int numPort;

    try {
        numPort = stoi(port);
    }
    catch(...) { 
        throw std::invalid_argument("Fail to convert string to int.");
    }

    if (numPort < 0 || numPort > 65535)
        throw std::invalid_argument("The PORT must be a value between 0 and 65535.");
    
}

void Validation::validateHostname(std::string hostname) {
    char buffer[sizeof(struct in6_addr)];

    if (inet_pton(AF_INET, hostname.c_str(), buffer) != 1)
        std::invalid_argument("Invalid hostname.");
}

bool Validation::validateMessage(std::string message) {
    if (message.length() > 256) 
        return false;   
    return true;
    
}
