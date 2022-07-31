#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

void trimRight( std::string& str, const std::string& trimChars) {
   std::string::size_type pos = str.find_last_not_of( trimChars );
   str.erase( pos + 1 );
}


void trimLeft( std::string& str, const std::string& trimChars) {
   std::string::size_type pos = str.find_first_not_of( trimChars );
   str.erase( 0, pos );
}

void trim( std::string& str, const std::string& trimChars) {
   trimRight( str, trimChars );
   trimLeft( str, trimChars );
}

std::vector<std::string> readLine(std::string csvLine){

    std::istringstream csvStream(csvLine);
    std::string csvElement;
    std::vector<std::string> line;
    while(std::getline(csvStream, csvElement, ';') ) {
        trim(csvElement, " ");
        line.emplace_back(csvElement);
    }
    return line;
}

std::vector<std::vector<std::string> > readLines(std::ifstream& csvFile){
    std::vector<std::vector<std::string > > lines;
    std::string csvLine;
    while (std::getline(csvFile, csvLine)){
        std::vector<std::string> line = readLine(csvLine);
        lines.emplace_back(line);
    }
    return lines;
}

std::vector<std::vector<std::string> > readFile(const std::string fileName){
    std::cout << "Reading " << fileName << std::endl;
    std::ifstream csvFile(fileName);
    std::vector<std::vector<std::string > > lines;
    if (csvFile.is_open()){
        lines = readLines(csvFile);
    }
    else{
        std::cout << "no file found" << std::endl;
    }
    csvFile.close();
    return lines;
}
