#include "Problem.h"
#include "Person.h"
#include <vector>
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

void removeWhiteSpace(std::string& s){
    s.erase(std::remove_if(s.begin(), s.end(), isspace), s.end());
}

std::vector<std::string> readLine(std::string csvLine){

    removeWhiteSpace(csvLine);
    std::istringstream csvStream(csvLine);
    std::string csvElement;
    std::vector<std::string> line;
    while(std::getline(csvStream, csvElement, ',') ) {
        line.emplace_back(csvElement);
    }
    return line;
}

std::vector<std::vector<std::string > > readLines(std::ifstream& csvFile){
    std::vector<std::vector<std::string > > lines;
    std::string csvLine;
    while (std::getline(csvFile, csvLine)){
        std::vector<std::string> line = readLine(csvLine);
        lines.emplace_back(line);
    }
    return lines;
}

std::vector<std::vector<std::string > > readFile(const std::string fileName){
    std::cout << "Reading " << fileName << std::endl;
    std::ifstream csvFile(fileName);
    std::vector<std::vector<std::string > > lines;
    if (csvFile.is_open()){
        lines = readLines(csvFile);
    }
    else{
        std::cout << "no file found" << std::endl;
    }
    csvFile.close();
    return lines;
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
    int numberOfBorrowers;
    for(auto e = line.begin() + 3; e != line.end(); ++e){
        if (stoi(*e) == 1){
            ++numberOfBorrowers;
        }
    }
    return numberOfBorrowers;
}

void Problem::readCosts(std::string filename){
    std::vector<std::vector<std::string > > lines = readFile(filename);

    // createColumnMapAndpeople?(lines[0]);
    // readHeaderAndCreateColumnMap(lines[0])
    std::map<int, std::string> mapColumnToName;
    std::vector<std::string> line = lines[0];
    for(int column = 0; column < line.size(); ++column){
        if(column > 2){
            std::string name = line[column];
            mapColumnToName.emplace(column, name);
            people.emplace_back(name, "", 0.0);
        }
    }

    // readBody();
    for(auto line = lines.begin() + 1; line < lines.end(); ++line){
        std::string name;
        int amount;
        int column = 0;
        std::vector<Person>::iterator payer;
        std::vector<Person>::iterator borrower;
        int numberOfBorrowers = getNumberOfBorrowers(*line);
        for(auto element = (*line).begin(); element < (*line).end(); ++element){
        /* for(int column = 0; column < line->size(); ++column){ */
            if (column == 0){
                name = *element;
                payer = findPerson(name);
            } else if (column == 1){
                amount = std::stoi(*element);
                payer->payed += amount;
            } else if (column > 2 && stoi(*element) == 1){
                std::string columnName = mapColumnToName[column];
                borrower = findPerson(columnName);
                if (payer != borrower){
                    borrower->borrowed += amount / numberOfBorrowers;
                }
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
