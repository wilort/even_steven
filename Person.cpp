#include "Person.h"
#include <string>

// constructor
Person::Person(){}
Person::Person(std::string _name) {
    name = _name;
    number = "";
    balance = 0.0;
    desired_balance = 0.0;
}
