#include <iostream>
#include <string>

#define SEND 0
#define FOLLOW 1
#define END 2

class Interface {
    std::string lineRead;
public:
    Interface();
    void setInput();
    void cleanScreen();
    int getQuery();
    std::string getUsername();
    std::string getMessage();
};