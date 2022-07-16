#pragma once
#include <string>
#include <vector>

class Person{
private:

public:
    struct Expense{
        Expense(const double _amount, const std::string _description):
            amount(_amount), description(_description){}
        double amount;
        std::string description;
    };

    std::string name;
    std::string number;
    // balance represents the negative sum of payments that this person did during a trip
    // desired_balance is the negative sum of payments that this person should have after everyone is even steven
    std::vector<Expense> expenses;
    double balance;
    double desired_balance;

    Person();
    Person(std::string a);
    void myFunction();
};
