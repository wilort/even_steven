#include "Problem.h"
#include "Person.h"
#include "readcsv.h"
#include <vector>
#include <iostream>
#include <numeric>

Problem::Problem() { }

Problem::Transaction::Transaction(Person _giver, Person _reciever, double _amount) {
    giver = _giver;
    reciever = _reciever;
    amount = _amount;
}

void Problem::readNumbers(std::string filename){
    std::vector<std::vector<std::string > > lines = readFile(filename);
    for(std::vector<std::string> line : lines){
        std::string name = line[0];
        std::string number = line[1];
        auto person = getPersonByName(name);
        person->number = number;
    }
}

int getTotalParts(std::vector<std::string> line) {
    int totalParts = 0;
    for(auto column = line.begin() + 3; column != line.end(); ++column){
        totalParts += stoi(*column);
    }
    return totalParts;
}

bool isName(std::string str){
    return !(str == "lender"
          or str == "amount"
          or str == "description");
}

void Problem::readCosts(std::string filename){
    std::vector<std::vector<std::string > > lines = readFile(filename);

    // Create a list of people
    std::vector<std::string> headerLine = lines[0];
    for(auto header: headerLine){
        if(isName(header)){
            std::string name = header;
            people.emplace_back(name);
        }
    }

    // readBody();
    // Skip first line because of header
    for(auto line = lines.begin() + 1; line < lines.end(); ++line){
        double amount;
        std::string description = "";
        int column = 0;
        std::vector<Person>::iterator payer;
        std::vector<Person>::iterator borrower;
        const int totalParts = getTotalParts(*line);
        for(auto element = line->begin(); element < line->end(); ++element){
            std::string columnHeader = headerLine[column];
            if (columnHeader == "lender"){
                std::string name = *element;
                payer = getPersonByName(name);
            } else if (columnHeader == "amount"){
                amount = std::stod(*element);
                payer->initialBalance += amount;
            } else if (columnHeader == "description") {
                description = *element;
                payer->expenses.emplace_back(amount, description);
            } else {
                double part = stod(*element);
                double fraction = part / totalParts;
                borrower = getPersonByName(columnHeader);
                borrower->desiredBalance += amount * fraction;
            }
            ++column;
        }
    }
}

std::vector<Person>::iterator Problem::getPersonByName(std::string name){
    std::vector<Person>::iterator person =
                          std::find_if(people.begin(),
                          people.end(),
                          [&name](const Person p){return p.name == name;});
    return person;
}

void Problem::createTransactions() {
    std::cout << "Creating transactions ..." << std::endl;

    // sort people by how much they owe other people in descending order
    // person who owes the most comes first
    std::sort(people.begin(),
              people.end(),
              [](Person a, Person b) { return a.desiredBalance-a.initialBalance
                                            < b.desiredBalance-b.initialBalance; });

    auto giver = people.begin();
    auto reciever = people.end() - 1;
    double giverBalance = giver->initialBalance;
    double recieverBalance = reciever->initialBalance;
    double amount;
    while (giver < reciever){

        double maximum_give = giverBalance - giver->desiredBalance;
        double maximum_recieve = reciever->desiredBalance - recieverBalance;
        amount = std::min(maximum_give, maximum_recieve);

        transactions.emplace_back(*giver, *reciever, amount);
        giverBalance -= amount;
        recieverBalance += amount;

        if(giverBalance == giver->desiredBalance){
            ++giver;
            giverBalance = giver->initialBalance;
        }

        if(recieverBalance == reciever->desiredBalance){
            --reciever;
            recieverBalance = reciever->initialBalance;
        }
    }
};

void Problem::printPersonSummary(const Person person) const {
    std::cout << person.name << std::endl;
    /* std::cout << "    " */
    /*           << person.desiredBalance */
    /*           << " desired balance " */
    /*           << std::endl; */
    /* std::cout << "    --------------------" << std::endl; */
    for (Person::Expense expense: person.expenses) {
        std::cout << "    "
                  << expense.amount
                  << " "
                  << expense.description
                  << std::endl;
    }
}

void Problem::printTransactions(const Person person) const {
    double sum = 0;
    for (Transaction transaction: transactions) {
        if(person.name == transaction.giver.name){
            std::cout << "    "
                      << -transaction.amount
                      << " to "
                      << transaction.reciever.name
                      << "("
                      << transaction.reciever.number
                      << ")"
                      << std::endl;
            sum -= transaction.amount;
        }
        else if(person.name == transaction.reciever.name){
            std::cout << "    +"
                      << transaction.amount
                      << " from "
                      << transaction.giver.name
                      << std::endl;
            sum += transaction.amount;
        }
    }
    std::cout << "    --------------------" << std::endl;
    double finalBalance = person.initialBalance + sum;
    std::cout << "    " << finalBalance << " final balance\n" << std::endl;
}

void Problem::printTransactionsSummary() const {
    std::cout << "number of transactions: " << transactions.size() << std::endl;
    double sumOfTransactions = std::accumulate(transactions.begin(),
                                               transactions.end(),
                                               0.0,
                                               [](double a, Transaction b) { return a + b.amount; });
    std::cout << "sum of transactions: " << sumOfTransactions << std::endl;
}

void Problem::printSolution() const {
    std::cout << "Printing solution ...\n" << std::endl;
    for (Person person: people) {
        printPersonSummary(person);
        printTransactions(person);
    }
    printTransactionsSummary();
};
