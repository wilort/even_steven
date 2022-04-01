#include "Problem.h"
#include "Person.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <map>

Problem::Problem() { }

Problem::Transaction::Transaction(Person _giver, Person _reciever, double _amount) {
  giver = _giver;
  reciever = _reciever;
  amount = _amount;
}

void Problem::readNumbers(const std::string fileName) {
    std::cout << "Reading numbers ..." << std::endl;
    std::ifstream csvFile(fileName);
    if (csvFile.is_open()){

        std::string csvLine;
        std::string name, number;

        while (std::getline(csvFile, csvLine)){
            std::istringstream csvStream(csvLine);
            std::string csvElement;

            int column = 0;
            while(std::getline(csvStream, csvElement, ',') ) {
                if (column == 0){
                    name = csvElement;
                }
                else if (column == 1){
                    number = csvElement;
                }
                column++;
            }
        auto person = std::find_if(people.begin(), people.end(), [&name](const Person p){return p.name == name;});
        person->number = number;
        }
    }
    else{
        std::cout << "no file found" << std::endl;
    }
    csvFile.close();
}

void Problem::readHeaderLine(std::string csvLine){

    std::stringstream csvStream(csvLine);
    std::string csvElement;
    int column = 0;
    while(std::getline(csvStream, csvElement, ',') ) {
        // column 0,1,2 are the column headers name, amount and description respectively
        if (column > 2){
            std::string name = csvElement;
            mapColumnToName.emplace(column, name);
            people.emplace_back(name, "", 0.0);
        }
        ++column;
    }
}

int getNumberOfBorrowers(std::string csvLine){
    std::stringstream csvStream(csvLine);
    std::string csvElement;
    int column = 0;
    int numberOfBorrowers;
    while(std::getline(csvStream, csvElement, ',') ) {
        // column 0,1,2 correspond to name, amount and description respectively
        if(column > 2 && stoi(csvElement) == 1){
            ++numberOfBorrowers;
        }
        ++column;
    }
    return numberOfBorrowers;
}

void Problem::readLine(std::string csvLine){
    int numberOfBorrowers = getNumberOfBorrowers(csvLine);

    std::stringstream csvStream(csvLine);
    std::string csvElement;
    std::string name;
    int column = 0;
    std::vector<Person>::iterator payer;
    std::vector<Person>::iterator borrower;
    double amount;
    while(std::getline(csvStream, csvElement, ',') ) {
        if (column == 0){
            name = csvElement;
            payer = findPerson(name);
        } else if (column == 1){
            amount = std::stoi(csvElement);
            payer->payed += amount;
        } else if (column == 2){
            // description column. Do nothing
        } else if (stoi(csvElement) == 1){
            std::string columnName = mapColumnToName[column];
            borrower = findPerson(columnName);
            if (payer != borrower){
                borrower->borrowed += amount / numberOfBorrowers;
            }
        }
        ++column;
    }
}

std::vector<Person>::iterator Problem::findPerson(std::string name){
    std::vector<Person>::iterator person = std::find_if(people.begin(),
                          people.end(),
                          [&name](const Person p){return p.name == name;});
    return person;
}

void Problem::readCsv(std::ifstream& csvFile){
    std::string csvLine;
    std::map<int, std::string> mapColumnToName;

    int row = 0;
    while (std::getline(csvFile, csvLine)){
        // remove white space from line
        csvLine.erase(std::remove_if(csvLine.begin(), csvLine.end(), ::isspace), csvLine.end());
        if (row == 0){
            readHeaderLine(csvLine);
        } else {
            readLine(csvLine);
        }
        ++row;
    }
}

void Problem::readCosts(const std::string fileName){
    std::cout << "Reading costs ..." << std::endl;
    std::ifstream csvFile(fileName);
    if (csvFile.is_open()){
        readCsv(csvFile);
    } else{
        std::cout << "no file found" << std::endl;
    }
    csvFile.close();
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
        const double amount = std::min(maximum_give, maximum_recieve);

        transactions.emplace_back(*giver, *reciever, amount);
        give += amount;
        recieve -= amount;

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
void Problem::printPayedAndBorrowed(const Person person) const {
    std::cout << person.name << std::endl;
    std::cout << "    "
              << -person.payed
              << " payed"
              << std::endl;
    std::cout << "    "
              << person.borrowed
              << " borrowed "
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
    std::cout << "    " << -person.payed + sum << " outgoing balance\n" << std::endl;
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
