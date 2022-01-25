#pragma once
#include <vector>
#include <tuple>
#include "Person.h"
class Problem{
private:


public:
    std::vector<Person> people;
    std::vector<std::tuple<Person, Person, double>> transactions;

    Problem(std::vector<Person> a);
    void read_input();
    void solve();
    void print_solution();
    void myFunction();
};
