// Your First C++ Program

#include <iostream>
#include "Person.h"
#include "Problem.h"

int main() {
    std::cout << "Hello World!\n";
    std::cout << "Hello World again!\n";

    std::vector<Person> people;
    people.emplace_back("A", "0761042991", 1337);
    people.emplace_back("B", "0761042992", 765);
    people.emplace_back("C", "0761042993", 431);
    people.emplace_back("D", "0761042994", 899);
    people.emplace_back("E", "0761042995", 500);

    Problem problem(people);
    problem.solve();
    problem.print_solution();
    return 0;
}
