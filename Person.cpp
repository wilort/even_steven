#include "Person.h"
#include <string>

// constructor
Person::Person(){}
Person::Person(std::string _name) {
    name = _name;
    number = "";
    initialBalance = 0.0;
    desiredBalance = 0.0;
}
