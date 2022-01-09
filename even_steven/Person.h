#pragma once
#include <string>

class Person{
private:


public:
    std::string name;
    std::string phone_number;
    double balance;

    Person(std::string a, std::string b, double c);
    void myFunction();
};