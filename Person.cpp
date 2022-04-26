#include "Person.h"
#include <string>
#include <iostream>

// constructor
Person::Person(){}
Person::Person(std::string _name) {
    name = _name;
    number = "";
    payed = 0.0;
}
