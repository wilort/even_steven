#pragma once
#include <vector>
#include "Person.h"
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
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
    std::map<int, std::string> mapColumnToName;

    Problem();
    std::vector<Person>::iterator getPersonByName(const std::string fileName);
    void readCosts(const std::string fileName);
    void readCsv(std::ifstream& csvFile);
    void readLine(const std::string fileName);
    void readHeaderLine(const std::string fileName);
    void readNumbers(const std::string fileName);
    void solve();
    void printPayedAndBorrowed(const Person person) const;
    void printTransactions(const Person person) const;
    void printSolution() const;
    void printSummary() const;
};
