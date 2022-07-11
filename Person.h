#pragma once
#include <string>

class Person{
private:

public:
    std::string name;
    std::string number;
    // balance represents the negative sum of payments that this person did during a trip
    // desired_balance is the negative sum of payments that this person should have after everyone is even steven
    double balance;
    double desired_balance;

    Person();
    Person(std::string a);
    void myFunction();
};
