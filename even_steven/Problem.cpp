#include "Problem.h"
#include <iostream>
#include <algorithm>

// constructor
Problem::Problem(std::vector<Person> a) {
    people = a;
}

void Problem::solve() {
    std::cout << "Initiating problem solving ..." << "\n";
    for (Person p : people) {
      std::cout << p.name << ", ";
      std::cout << p.phone_number << ", ";
      std::cout << p.balance << "\n";
    }
};
void Problem::print_solution() {
    std::cout << "Printing a nice solution ..." << "\n";
    /* std::vector<Person> tmp = std::sort(people.start, people.end) */
    for (Person p : people) {
      std::cout << p.balance << "\n";
    }
};
void Problem::myFunction() {
    std::cout << "shiter" << "\n";
};
