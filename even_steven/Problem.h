#pragma once
#include <vector>
#include "Person.h"

class Problem{
private:


public:
    std::vector<Person> people;

    Problem(std::vector<Person> a);
    void solve();
    void print_solution();
    void myFunction();
};
