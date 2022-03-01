// Your First C++ Program
#include "Person.h"
#include "Problem.h"

int main() {
    Problem problem;
    /* problem.read_input2(); */
    problem.readNumbers();
    problem.readCosts();
    problem.solve();
    problem.print_solution();
    return 0;
}
