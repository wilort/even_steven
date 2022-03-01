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
    void readNumbers();
    void readCosts();
    void read_input();
    void read_input2();
    void solve();
    void print_solution();
    void myFunction();
};
