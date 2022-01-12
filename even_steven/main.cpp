// Your First C++ Program

#include <iostream>
#include "Person.h"
#include "Problem.h"

int main() {
    std::cout << "Hello World!\n";
    std::cout << "Hello World again!\n";

    std::vector<Person> people;
    people.emplace_back("A", "0761042991", -500);
    people.emplace_back("B", "0761042992", -501);
    people.emplace_back("C", "0761042993", -502);
    people.emplace_back("D", "0761042994", -503);
    people.emplace_back("E", "0761042995", -504);

    Problem problem(people);
    problem.solve();
    problem.print_solution();
    /* for (Person p : people) { */
    /*     std::cout << p.name << ", "; */
    /*     std::cout << p.phone_number << ", "; */
    /*     std::cout << p.balance << "\n"; */
    /*     p.myFunction(); */
    /* } */
    return 0;
}
