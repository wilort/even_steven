#pragma once
#include <string>

class Person{
private:


public:
    std::string name;
    std::string number;
    double payed;
    double borrowed;
    double owes;

    Person();
    Person(std::string a, std::string b, double c);
    /* bool operator < (const Person p); */
    void myFunction();

};
