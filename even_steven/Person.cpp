#include "Person.h"
#include <string>
#include <iostream>

// testing

// constructor
Person::Person(){}
Person::Person(std::string a, std::string b, double c) {
    name = a;
    number = b;
    payed = c;
}

/* bool Person::operator < (const Person& p){ */
/*     return (balance < p.balance); */
/* } */

void Person::myFunction() {
    std::cout << "shiter" << "\n";
};
