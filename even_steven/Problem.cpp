#include "Problem.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <numeric>

// constructor
Problem::Problem(const std::vector<Person> a) {
    /* people = a; */
}

void Problem::read_input() {
    std::cout << "Reading input ..." << "\n";
    std::ifstream csvFile("input");
    if (csvFile.is_open()){

        std::string csvLine;
        std::string name, phone_number;
        double payed;

        while (std::getline(csvFile, csvLine)){
            std::istringstream csvStream(csvLine);
            std::string csvElement;

            int column = 0;
            while(std::getline(csvStream, csvElement, ',') ) {
                if (column == 0){
                    name = csvElement;
                }
                else if (column == 1){
                    phone_number = csvElement;
                }
                else if (column == 2){
                    payed = std::stoi(csvElement);
                }
                    column++;
            }
        people.emplace_back(name, phone_number, payed);
        }
    }
    else{
        std::cout << "no file found" << "\n";
    }
    csvFile.close();
}

void Problem::solve() {
    std::cout << "Initiating problem solving ..." << "\n";

    double total_payed = std::accumulate(people.begin(),
                                         people.end(),
                                         0,
                                         [](double a, Person b) { return a + b.payed; });

    int number_of_people = people.size();
    double desired_pay = total_payed / number_of_people;

    std::cout << "Everyone should pay " << desired_pay << "\n";

    std::sort(people.begin(), people.end(), [](Person a, Person b) { return a.payed < b.payed; });

    auto p1 = people.begin();
    auto p2 = people.end() - 1;
    while (p1 < p2){
        double maximum_give = desired_pay - p1->payed;
        double maximum_recieve = p2->payed - desired_pay;
        double transaction = std::min(maximum_give, maximum_recieve);
        if(transaction > 0){
            transactions.emplace_back(*p1, *p2, transaction);
            p1->payed += transaction;
            p2->payed -= transaction;
        }
        p1->payed < desired_pay ? --p2 : ++p1;
    }
};

// TODO group together transactions prints more
// make a struct out of transaction instead of a tuple
void Problem::print_solution() {
    std::cout << "Printing a nice solution ..." << "\n";
    for (Person person: people) {
        for (std::tuple<Person, Person, double> transaction: transactions) {
            Person giver = std::get<0>(transaction);
            Person reciever = std::get<1>(transaction);

            if(person.name == giver.name ){
                double swish = std::get<2>(transaction);
                std::cout << person.name << " gives " << swish << " to " << reciever.name << "("<< reciever.phone_number << ")"<<"\n";
            }
            else if(person.name == reciever.name){
                double swish = std::get<2>(transaction);
                std::cout << person.name << " recieves " << swish << " from " << giver.name << "\n";
            }
        }
    }
    std::cout << "number of transactions: " << transactions.size() << "\n";
    double sum_of_transactions = std::accumulate(transactions.begin(),
                                                 transactions.end(),
                                                 0,
                                                 [](double a, std::tuple<Person, Person, double> b) { return a + std::get<2>(b); });
    std::cout << "sum of transactions: " << sum_of_transactions << "\n";

};

void Problem::myFunction() {
    std::cout << "shiter" << "\n";
};
