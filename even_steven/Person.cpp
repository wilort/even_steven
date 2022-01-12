#include "Person.h"
#include <string>
#include <iostream>

// testing

// constructor
Person::Person(std::string a, std::string b, double c) {
    name = a;
    phone_number = b;
    balance = c;
}

void Person::myFunction() {
    std::cout << "shiter" << "\n";
};
