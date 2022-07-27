#pragma once
#include <vector>
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
    std::vector<Person>::iterator getPersonByName(const std::string fileName);
    void readCosts(const std::string fileName);
    void readNumbers(const std::string fileName);
    void createTransactions();
    void printPersonSummary(const Person person) const;
    void printTransactions(const Person person) const;
    void printSolution() const;
    void printTransactionsSummary() const;
};
