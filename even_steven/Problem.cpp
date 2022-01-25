#include "Problem.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <numeric>

// constructor
Problem::Problem(std::vector<Person> a) {
    people = a;
}

void Problem::read_input() {
  std::cout << "Reading input ..." << "\n";
  std::string line;
  std::ifstream myfile("input");
  if (myfile.is_open()){
    while (std::getline(myfile,line)){
      std::cout << line << '\n';
    }
  }
  else{
    std::cout << line << "\n";
  }
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
