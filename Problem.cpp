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

void Problem::readCosts(std::string filename){
    std::vector<std::vector<std::string > > lines = readFile(filename);

    // Create a list of people
    std::vector<std::string> headerLine = lines[0];
    for(int column = 3; column < headerLine.size(); ++column){
        std::string name = headerLine[column];
        people.emplace_back(name);
    }

    // readBody();
    // Skip first line because of header
    for(auto line = lines.begin() + 1; line < lines.end(); ++line){
        std::string name;
        double amount;
        std::string description = "";
        int column = 0;
        std::vector<Person>::iterator payer;
        std::vector<Person>::iterator borrower;
        const int totalParts = getTotalParts(*line);
        for(auto element = line->begin(); element < line->end(); ++element){
            std::string columnName = headerLine[column];
            if (columnName == "lender"){
                name = *element;
                payer = getPersonByName(name);
            } else if (columnName == "amount"){
                amount = std::stod(*element);
                payer->balance += amount;
            } else if (columnName == "description") {
                description = *element;
                payer->expenses.emplace_back(amount, description);
            } else if (column >= 3){
                double part = stod(*element);
                double fraction = part / totalParts;
                borrower = getPersonByName(columnName);
                borrower->desired_balance += amount * fraction;
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

void Problem::solve() {
    std::cout << "Solving problem ..." << std::endl;

    // sort people by how much they owe other people in descending order
    std::sort(people.begin(),
              people.end(),
              [](Person a, Person b) { return a.desired_balance-a.balance < b.desired_balance-b.balance; });

    auto giver = people.begin();
    auto reciever = people.end() - 1;
    double give = 0;
    double recieve = 0;
    while (giver < reciever){
        const double maximum_give = giver->balance - giver->desired_balance - give;
        const double maximum_recieve = reciever->desired_balance - reciever->balance + recieve;
        const double amount = std::min(maximum_give, maximum_recieve);

        if(amount > 0){
            transactions.emplace_back(*giver, *reciever, amount);
            give += amount;
            recieve -= amount;
        }

        if(maximum_give < maximum_recieve){
            ++giver;
            give = 0;
        } else if ( maximum_give > maximum_recieve){
            --reciever;
            recieve = 0;
        } else {
            ++giver;
            give = 0;
            --reciever;
            recieve = 0;
        }
    }
};

void Problem::printPersonSummary(const Person person) const {
    std::cout << person.name << std::endl;
    std::cout << "    "
              << person.desired_balance
              << " desired balance "
              << std::endl;
    std::cout << "    --------------------" << std::endl;
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
    double final_balance = person.balance + sum;
    std::cout << "    " << final_balance << " final balance\n" << std::endl;
}

void Problem::printTransactionsSummary() const {
    std::cout << "number of transactions: " << transactions.size() << std::endl;
    double sum_of_transactions = std::accumulate(transactions.begin(),
                                                 transactions.end(),
                                                 0,
                                                 [](double a, Transaction b) { return a + b.amount; });
    std::cout << "sum of transactions: " << sum_of_transactions << std::endl;
}

void Problem::printSolution() const {
    std::cout << "Printing solution ...\n" << std::endl;
    for (Person person: people) {
        printPersonSummary(person);
        printTransactions(person);
    }
    printTransactionsSummary();
};
