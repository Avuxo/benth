#include <iostream>
#include <sstream>
#include <map>
#include <iterator>
#include <numeric>
#include <fstream>

#include "Main.h"

std::map<std::string, std::string> wordMap; // symbol table for words

int main(int argc, char **argv){
    if(argc < 2){
        std::cerr << "USAGE: benth <filename>";
        exit(1);
    }

    std::ifstream file(argv[1]);
    if(file.fail()){
        std::cerr << "ERROR: File does not exist\n";
        exit(1);
    }
    std::string temp, fileBuffer;

    getline(file, temp);
    while(file){
        fileBuffer += temp;
        getline(file, temp);
    }
    
    wordMap["+"] = "32"; // pre-defined function
    wordMap["-"] = "33"; // pre-defined function
    wordMap["*"] = "34"; // pre-defined function
    std::vector<std::string> tokens = tokenize(fileBuffer);

    compileBenthToBVM(tokens);
    return 0;
}

/*
  Tokenize
  Convert a given string into a vector of tokens
*/
std::vector<std::string> tokenize(std::string input){
    std::stringstream strstream(input);
    std::istream_iterator<std::string> start(strstream);
    std::istream_iterator<std::string> end;
    std::vector<std::string> tokens(start, end); /*return value*/
    
    return tokens;
}

/*check if a given string is an integer*/
bool isInt(std::string str){
    const char *cStringInput = str.c_str();
    const char *digits = "0123456789";
    while(*cStringInput){
        for(int i=0; i<10; i++){
            if(*cStringInput == digits[i]){
                return true;
            }
        }
        cStringInput++;
    }
    return false;
}

void compileBenthToBVM(std::vector<std::string> program){
    for(int pc=0; pc<program.size(); pc++){
        // word creation and compilation
        if(program[pc] == ":") {
            std::string symbol = program[++pc];
            int startOfWord = ++pc;
            std::string word;
            while(program[pc] != ";"){
                std::string instruction = compileInstruction(program[pc]);
                word += instruction + ",";
                pc++;
            }

            wordMap.insert(std::pair<std::string, std::string>(symbol, word));
        } else {
            // word lookup
            if(isInt(program[pc])){
                std::cout << "16," <<
                    program[pc] << ","; // if its an int, just print it.
            }else if(wordMap.count(program[pc]) != 0){ // word exists
                std::cout << wordMap[program[pc]] << ",";
            } else {
            std::cerr << "ERROR: Unknown function " << program[pc] << "\n";
            exit(1);
            }
        }
    }
    
    std::cout << "1\n"; // halt
}

std::string compileInstruction(std::string instruction){
    if(isInt(instruction)){ // if its an int, dont compile it, its already an int.
        return "16," + instruction;
    }
    std::string compiledInstruction;
    compiledInstruction = wordMap[instruction];
    return compiledInstruction;
}
