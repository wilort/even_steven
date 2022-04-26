#pragma once
#include <string>

class Person{
private:

public:
    std::string name;
    std::string number;
    double payed;
    double borrowed;

    Person();
    Person(std::string a);
    void myFunction();
};
