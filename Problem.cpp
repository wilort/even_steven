#include "Problem.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <map>

// constructor
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

void Problem::readCosts(const std::string fileName){
    std::cout << "Reading costs ..." << std::endl;
    std::ifstream csvFile(fileName);
    if (csvFile.is_open()){


        std::string csvLine;
        std::string name;
        std::map<int, std::string> mapColumnToName;

        int row = 0;
        while (std::getline(csvFile, csvLine)){
            // remove white space from input
            csvLine.erase(std::remove_if(csvLine.begin(), csvLine.end(), ::isspace), csvLine.end());

            std::stringstream csvStream(csvLine);
            std::string csvElement;
            int column = 0;
            int numberOfBorrowers = 0;

            while(std::getline(csvStream, csvElement, ',') ) {
                // column 0,1,2 correspond to name, amount and description respectively
                if (row == 0 && column > 2){
                    mapColumnToName.emplace(column, csvElement);
                    people.emplace_back(csvElement, "", 0.0);
                } else if(column > 2 && stoi(csvElement) == 1){
                    ++numberOfBorrowers;
                }
                ++column;
            }

            if ( row > 0) {
                csvStream.clear();
                csvStream.seekg(0);
                column = 0;
                std::vector<Person>::iterator payer;
                std::vector<Person>::iterator borrower;
                double amount;
                while(std::getline(csvStream, csvElement, ',') ) {
                    if (column == 0){
                        name = csvElement;
                        payer = std::find_if(people.begin(), people.end(), [&name](const Person p){return p.name == name;});
                    } else if (column == 1){
                        amount = std::stoi(csvElement);
                        payer->payed += amount;
                        /* std::cout << payer->name << " payed " << amount << std::endl; */
                    } else if (column == 2){
                        // description column. Do nothing
                    } else if (stoi(csvElement) == 1){
                        std::string columnName = mapColumnToName[column];
                        borrower = std::find_if(people.begin(), people.end(), [&columnName](const Person p){return p.name == columnName;});
                        borrower->borrowed += amount / numberOfBorrowers;
                        /* std::cout << borrower->name << " borrowed " << amount / numberOfBorrowers << std::endl; */
                    }
                    ++column;
                }
            }
            ++row;
        }
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
void Problem::print_solution() const {
    std::cout << "Printing solution ..." << std::endl;

    for (Person person: people) {
        std::cout << person.name
                  << " has payed "
                  << person.payed
                  << " and borrowed "
                  << person.borrowed
                  << std::endl;
        double sum = 0;
        for (Transaction transaction: transactions) {
            if(person.name == transaction.giver.name){
                std::cout << "    "
                          << transaction.amount
                          << " to "
                          << transaction.reciever.name
                          << "("
                          << transaction.reciever.number
                          << ")"
                          << std::endl;
                sum += transaction.amount;
            }
            else if(person.name == transaction.reciever.name){
                std::cout << "    "
                          << transaction.amount
                          << " from "
                          << transaction.giver.name
                          << std::endl;
                sum -= transaction.amount;
            }
        }
        std::cout << "    will have payed "
                  << person.payed + sum
                  << std::endl;
    }

    std::cout << "number of transactions: " << transactions.size() << std::endl;
    double sum_of_transactions = std::accumulate(transactions.begin(),
                                                 transactions.end(),
                                                 0,
                                                 [](double a, Transaction b) { return a + b.amount; });
    std::cout << "sum of transactions: " << sum_of_transactions << std::endl;
};
