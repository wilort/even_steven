#include "Problem.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <map>

// constructor
Problem::Problem() { }
Problem::Problem(const std::vector<Person> a) { }


Problem::Transaction::Transaction(Person a, Person b, double c) {
  giver = a;
  reciever = b;
  amount = c;
}

void Problem::readNumbers() {
    std::cout << "Reading numbers ..." << std::endl;
    std::ifstream csvFile("numbers");
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
        double payed = 0.0;
        people.emplace_back(name, number, payed);
        }
    }
    else{
        std::cout << "no file found" << std::endl;
    }
    csvFile.close();
}

void Problem:: readCosts(){
    std::cout << "Reading costs ..." << std::endl;
    std::ifstream csvFile("costs");
    if (csvFile.is_open()){


        std::string csvLine;
        std::string name;
        std::map<int, std::string> mapColumnToName;

        // map a column number to a name
        // map a row to number of borrowers
        int row = 0;
        while (std::getline(csvFile, csvLine)){
            // remove white space from input
            csvLine.erase(std::remove_if(csvLine.begin(), csvLine.end(), ::isspace), csvLine.end());

            std::stringstream csvStream(csvLine);
            std::string csvElement;
            int column = 0;
            int numberOfBorrowers = 0;

            // identify column number with a name
            while(std::getline(csvStream, csvElement, ',') ) {
                // column 0,1,2 correspond to name, amount and description respectively
                if (row == 0 && column > 2){
                    mapColumnToName.emplace(column, csvElement);
                    std::cout << " column " << column << " maps to " << csvElement << std::endl;
                } else if(column > 2 && stoi(csvElement) == 1){
                    ++numberOfBorrowers;
                }
                ++column;
            }

            std::cout << "row" << row << " has " << numberOfBorrowers << " borrowers " << std::endl;


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
                        std::cout << payer->name << " payed " << amount << std::endl;
                    } else if (column == 2){
                        // description column. Do nothing
                    } else if (stoi(csvElement) == 1){
                        std::string columnName = mapColumnToName[column];
                        borrower = std::find_if(people.begin(), people.end(), [&columnName](const Person p){return p.name == columnName;});
                        borrower->borrowed += amount / numberOfBorrowers;
                        std::cout << borrower->name << " owes " << amount / numberOfBorrowers << std::endl;
                    }
                    ++column;
                }
            }
            ++row;
        }
        /* for(auto p:people){ */
        /*   std::cout << p.name << std::endl; */
        /*   std::cout << p.number << std::endl; */
        /*   std::cout << "payed: " << p.payed << std::endl; */
        /*   std::cout << "borrowed: " << p.borrowed << std::endl; */
        /* } */
    } else{
        std::cout << "no file found" << std::endl;
    }
    csvFile.close();
}

void Problem::solve() {
    std::cout << "Initiating problem solving ..." << std::endl;

    double total_payed = std::accumulate(people.begin(),
                                         people.end(),
                                         0,
                                         [](double a, Person b) { return a + b.payed; });


    std::sort(people.begin(), people.end(), [](Person a, Person b) { return a.payed < b.payed; });

    auto giver = people.begin();
    auto reciever = people.end() - 1;
    double pay = 0;
    double recieve = 0;
    while (giver < reciever){

        const double maximum_give = giver->owes - giver->payed - pay;
        const double maximum_recieve = reciever->payed + recieve - reciever->owes;
        const double amount = std::min(maximum_give, maximum_recieve);

        transactions.emplace_back(*giver, *reciever, amount);
        pay += amount;
        recieve -= amount;

        if(maximum_give > 0){
          --reciever;
          recieve = 0;
        }

        if(maximum_recieve > 0){
          ++giver;
          pay = 0;
        }
    }
};


/* void Problem::solve() { */
/*     std::cout << "Initiating problem solving ..." << std::endl; */

/*     double total_payed = std::accumulate(people.begin(), */
/*                                          people.end(), */
/*                                          0, */
/*                                          [](double a, Person b) { return a + b.payed; }); */

/*     int number_of_people = people.size(); */
/*     double desired_pay = total_payed / number_of_people; */

/*     std::cout << "Everyone should pay " << desired_pay << std::endl; */

/*     std::sort(people.begin(), people.end(), [](Person a, Person b) { return a.payed < b.payed; }); */

/*     auto giver = people.begin(); */
/*     auto reciever = people.end() - 1; */
/*     double pay = 0; */
/*     double recieve = 0; */
/*     while (giver < reciever){ */

/*         const double maximum_give = desired_pay - giver->payed - pay; */
/*         const double maximum_recieve = reciever->payed + recieve - desired_pay; */
/*         const double amount = std::min(maximum_give, maximum_recieve); */

/*         transactions.emplace_back(*giver, *reciever, amount); */
/*         pay += amount; */
/*         recieve -= amount; */

/*         if(maximum_give > 0){ */
/*           --reciever; */
/*           recieve = 0; */
/*         } */

/*         if(maximum_recieve > 0){ */
/*           ++giver; */
/*           pay = 0; */
/*         } */
/*     } */
/* }; */


// TDDO
// 1. group together transactions prints more so you can view transaction breakdown
// 2. make use of input2 and apply weighted split
// 3. add flags for different things. help menu etc: main --help
void Problem::print_solution() {
    std::cout << "Printing a nice solution ..." << std::endl;
    for (Person person: people) {
        std::cout << person.name
                  << " has payed "
                  << person.payed //this will always be 750 because we use person.payed in solve above.
                  // Need to use a tmp variable in solve or else it will be wrong
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
