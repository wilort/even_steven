#include "Person.h"
#include "Problem.h"
#include <iostream>
#include <unistd.h>

int main(int argc, char **argv) {

    int c;
    std::string costsFile;
    std::string numbersFile;
    // "c:n:" indicates that we have flags -c and -n that
    // will be followed by an arguments which can be read as optarg
    while ( (c = getopt (argc, argv, "c:n:")) != -1 ){
      switch (c) {
        case 'c':
          costsFile = optarg;
          break;
        case 'n':
          numbersFile = optarg;
          break;
        default:
          abort ();
      }
    }

    Problem problem;
    problem.readCosts(costsFile);
    problem.readNumbers(numbersFile);
    problem.createTransactions();
    problem.printSolution();
    return 0;
}
