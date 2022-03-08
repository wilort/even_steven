#pragma once
#include <vector>
#include <tuple>
#include "Person.h"
class Problem{
private:


public:
    struct Transaction{
      Person giver;
      Person reciever;
      double amount;
      Transaction(Person _giver, Person _reciever, double _amount);
    };
    std::vector<Person> people;
    std::vector<Transaction> transactions;
    Problem();
    void readNumbers(const std::string fileName);
    void readCosts(const std::string fileName);
    void solve();
    void print_solution() const;
};
