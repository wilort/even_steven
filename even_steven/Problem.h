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
      Transaction(Person a, Person b, double c);
      /* Transaction(Person a, Person b, double c){ */
      /*   giver = a; */
      /*   reciever = b; */
      /*   amount = c; */
      /* } */
    };
    std::vector<Person> people;
    std::vector<Transaction> transactions;
    Problem();
    Problem(std::vector<Person> a);
    void readNumbers(std::string fileName);
    void readCosts(std::string fileName);
    void solve();
    void print_solution();
    void myFunction();
};
