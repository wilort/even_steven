#pragma once
#include <string>

class Person{
private:


public:
    std::string name;
    std::string phone_number;
    double payed;

    Person(std::string a, std::string b, double c);
    /* bool operator < (const Person p); */
    void myFunction();

};
