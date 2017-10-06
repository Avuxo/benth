#include <iostream>
#include <sstream>
#include <map>
#include <iterator>
#include <numeric>
#include <fstream>

#include "Main.h"

std::map<std::string, std::string> wordMap; // symbol table for words

int main(int argc, char **argv){
    if(argc < 2){ // make sure a filename is provided
        std::cerr << "ERROR: No input file provided."; 
        exit(1);
    }

    // ./benth <filename>
    std::ifstream file(argv[1]); // read the file from the first argument
    if(file.fail()){ // make sure the given file exists
        std::cerr << "ERROR: File does not exist\n";
        exit(1);
    }
    std::string temp, fileBuffer;

    getline(file, temp);
    while(file){ // loop through the function
        fileBuffer += temp; // add the temp varible to the full file buffer
        getline(file, temp); // get the next line
    }
    predefineFunctions();
    
    // convert the given program to a vector of tokens
    std::vector<std::string> tokens = tokenize(fileBuffer);

    compileBenthToBVM(tokens); // compile
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

/*
  isInt
  return true if the given string is an integer, false if it's not.
*/
bool isInt(std::string str){
    const char *cStringInput = str.c_str();
    const char *digits = "0123456789"; // list of all digits to loopthrough
    while(*cStringInput){ // loop through the whole string
        for(int i=0; i<10; i++){ // loop through every digit
            if(*cStringInput == digits[i]){ // if the current char is an integer, return true
                return true;
            }
        }
        cStringInput++;
    }
    return false; // is not an integer
}

/*
  compileBenToBVM
  takes the given vector of tokens and performs a wordmap lookup to convert it.
  If the given input is a word, add it to the wordmap.
*/
void compileBenthToBVM(std::vector<std::string> program){
    for(int pc=0; pc<program.size(); pc++){
        // word creation and compilation
        if(program[pc] == ":") { // mark for compilation
            std::string symbol = program[++pc];
            pc++;
            std::string word; // the full word after compilation for the map
            while(program[pc] != ";"){ // is it the end of the compiler statement?
                std::string instruction = compileInstruction(program[pc]);
                word += instruction + ",";
                pc++;
            }
            // insert the new word into the wordmap
            wordMap.insert(std::pair<std::string, std::string>(symbol, word)); 
        } else {
            // word lookup
            if(isInt(program[pc])){ // if it's an integer, just push it.
                std::cout << "16," <<
                    program[pc] << ","; 
            }else if(wordMap.count(program[pc]) != 0){ // if the word exists
                std::cout << wordMap[program[pc]] << ","; // perform a word lookup
            } else { // word doesn't exist and it's not an integer
                std::cerr << "ERROR: Unknown function " << program[pc] << "\n";
                exit(1);
            }
        }
    }

    // insert a halt to act as an EOF for the compiled program
    std::cout << "1\n";
}

/*
  compileInstruction
  Take the given instruction and perform a wordmap lookup (if it's not an integer)
*/
std::string compileInstruction(std::string instruction){
    if(isInt(instruction)){ // if its an int, dont compile it, its already an int.
        return "16," + instruction;
    }
    
    // perform a lookup on the given instruction
    std::string compiledInstruction;
    compiledInstruction = wordMap[instruction];
    return compiledInstruction;
}

void predefineFunctions(){
    wordMap["+"] = "32"; // addition
    wordMap["-"] = "33"; // subtraction
    wordMap["*"] = "34"; // multiplication
    wordMap["/"] = "35"; // division
}
