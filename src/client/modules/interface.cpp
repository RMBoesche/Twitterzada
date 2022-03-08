#include "./interface.h"
#include "validation.h"

Interface::Interface() {
    cleanScreen();
}

void Interface::setInput() {
    std::getline(std::cin, lineRead);
}

void Interface::cleanScreen() {
    std::cout << "\nPut your command: ";
}

int Interface::getQuery() {
    // get the first word
    std::string query = lineRead.substr(0, lineRead.find(' '));

    std::cout << query << std::endl;

    // determine the command
    if (query == std::string("SEND")) {
        return SEND;
    }
    else if (query == std::string("FOLLOW")) {
        return FOLLOW;
    }
    else {
        // indicating error
        return -1;
    }

}

std::string Interface::getUsername() {
    // get the username
    std::string username = lineRead.substr(lineRead.find(' ') + 1);
    
    try {
        // check its validity
        Validation::validateUsername(username);
    }
    catch(...) {
        std::cout << "\nThe username put in is not valid" << std::endl;
    }

    return username;
}

std::string Interface::getMessage() {
    std::string message = lineRead.substr(lineRead.find(' ') + 1);

    Validation::validateMessage(message);

    return message;
}