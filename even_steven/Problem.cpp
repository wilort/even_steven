#include "Problem.h"
#include <iostream>
#include <algorithm>
#include <numeric>

// constructor
Problem::Problem(std::vector<Person> a) {
    people = a;
}

bool sort_function(const Person p1, const Person p2){ return p1.payed < p2.payed; }

void Problem::solve() {
    std::cout << "Initiating problem solving2 ..." << "\n";

    int number_of_people = people.size();
    double total_payed = 0;
    for (Person p : people) {
      total_payed += p.payed;
    }
    double desired_pay = total_payed / number_of_people;

    std::cout << "Everyone should pay " << desired_pay << "\n";

    std::sort(people.begin(), people.end(), sort_function);
    auto p1 = people.begin();
    auto p2 = people.end() - 1;
    while (p1 < p2){
      double maximum_give = desired_pay - p1->payed;
      double maximum_recieve = p2->payed - desired_pay;
      double transaction = std::min(maximum_give, maximum_recieve);
      std::cout << " " << p1->name << " pays " << transaction << " to " << p2->name << "\n";
      transactions.emplace_back(*p1, *p2, transaction);
      p1->payed += transaction;
      p2->payed -= transaction;
      p1->payed < desired_pay ? --p2 : ++p1;
    }
};

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
          std::cout << person.name << " recieves " << swish << " from " << reciever.name << "\n";
        }
      }
    }
};

void Problem::myFunction() {
    std::cout << "shiter" << "\n";
};
