#include "Person.h"
#include <string>
#include <iostream>

// constructor
Person::Person(){}
Person::Person(std::string _name, std::string _number, double _payed) {
    name = _name;
    number = _number;
    payed = _payed;
}

void Person::myFunction() {
    std::cout << "shiter" << "\n";
};
