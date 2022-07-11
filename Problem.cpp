#include "Problem.h"
#include "Person.h"
#include "readcsv.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <numeric>

Problem::Problem() { }

Problem::Transaction::Transaction(Person _giver, Person _reciever, double _amount) {
  giver = _giver;
  reciever = _reciever;
  amount = _amount;
}

void Problem::readNumbers(std::string filename){
    std::vector<std::vector<std::string > > lines = readFile(filename);
    for(std::vector<std::string> line : lines){
      std::string name = line[0];
      std::string number = line[1];
      auto person = findPerson(name);
      person->number = number;
    }
}

int getNumberOfBorrowers(std::vector<std::string> line) {
    int numberOfBorrowers = 0;
    for(auto column = line.begin() + 3; column != line.end(); ++column){
        if (stoi(*column) == 1){
            ++numberOfBorrowers;
        }
    }
    return numberOfBorrowers;
}

void Problem::readCosts(std::string filename){
    std::vector<std::vector<std::string > > lines = readFile(filename);

    // Create a list of people
    std::vector<std::string> headerLine = lines[0];
    for(int column = 3; column < headerLine.size(); ++column){
        std::string name = headerLine[column];
        people.emplace_back(name);
    }

    // readBody();
    // Skip first line because of header
    for(auto line = lines.begin() + 1; line < lines.end(); ++line){
        std::string name;
        double amount;
        int column = 0;
        std::vector<Person>::iterator payer;
        std::vector<Person>::iterator borrower;
        int numberOfBorrowers = getNumberOfBorrowers(*line);
        for(auto element = line->begin(); element < line->end(); ++element){
            if (column == 0){
                name = *element;
                payer = findPerson(name);
            } else if (column == 1){
                amount = std::stod(*element);
                payer->payed += amount;
            } else if (column > 2 && stoi(*element) == 1){
                std::string columnName = headerLine[column];
                borrower = findPerson(columnName);
                borrower->borrowed += amount / numberOfBorrowers;
            }
            ++column;
        }
    }
}

std::vector<Person>::iterator Problem::findPerson(std::string name){
    std::vector<Person>::iterator person =
                          std::find_if(people.begin(),
                          people.end(),
                          [&name](const Person p){return p.name == name;});
    return person;
}

void Problem::solve() {
    std::cout << "Solving problem ..." << std::endl;

    std::sort(people.begin(),
              people.end(),
              [](Person a, Person b) { return a.payed-a.borrowed < b.payed-b.borrowed; });
    
    auto giver = people.begin();
    auto reciever = people.end() - 1;
    double give = 0;
    double recieve = 0;
    while (giver < reciever){
        const double maximum_give = giver->borrowed - giver->payed - give;
        const double maximum_recieve = reciever->payed - reciever->borrowed + recieve;
        std::cout << giver->name << " can give " << maximum_give << std::endl;
        std::cout << reciever->name << " can reciever " << maximum_recieve << std::endl;
        const double amount = std::min(maximum_give, maximum_recieve);

        if(amount > 0){
          transactions.emplace_back(*giver, *reciever, amount);
          give += amount;
          recieve -= amount;
        }

        if(maximum_give < maximum_recieve){
          ++giver;
          give = 0;
        } else {
          --reciever;
          recieve = 0;
        }
    }
};


// TDDO
// 1. modify the program such that you can werite for example
// "evensteven -c costs -n numbers"
// 2. add flags for different things. help menu etc: main --help
// 3. make it possible to weight the inputs unevenly
// 4. create unit-tests
void Problem::printPayedAndBorrowed(const Person person) const {
    std::cout << person.name << std::endl;
    std::cout << "    "
              << person.borrowed
              << " borrowed "
              << std::endl;
    std::cout << "    "
              << -person.payed
              << " payed"
              << std::endl;
}

void Problem::printTransactions(const Person person) const {
    double sum = 0;
    for (Transaction transaction: transactions) {
        if(person.name == transaction.giver.name){
            std::cout << "    "
                      << -transaction.amount
                      << " to "
                      << transaction.reciever.name
                      << "("
                      << transaction.reciever.number
                      << ")"
                      << std::endl;
            sum -= transaction.amount;
        }
        else if(person.name == transaction.reciever.name){
            std::cout << "    "
                      << transaction.amount
                      << " from "
                      << transaction.giver.name
                      << std::endl;
            sum += transaction.amount;
        }
    }
    double outgoing_balance = -person.payed + sum;
    std::cout << "    " << outgoing_balance << " outgoing balance\n" << std::endl;
}

void Problem::printSummary() const {
    std::cout << "number of transactions: " << transactions.size() << std::endl;
    double sum_of_transactions = std::accumulate(transactions.begin(),
                                                 transactions.end(),
                                                 0,
                                                 [](double a, Transaction b) { return a + b.amount; });
    std::cout << "sum of transactions: " << sum_of_transactions << std::endl;
}

void Problem::printSolution() const {
    std::cout << "Printing solution ...\n" << std::endl;
    for (Person person: people) {
        printPayedAndBorrowed(person);
        printTransactions(person);
    }
    printSummary();
};
