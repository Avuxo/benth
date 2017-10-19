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
        std::cerr << "ERROR: No input file provided.\n"; 
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
        fileBuffer += " " + temp; // add the temp varible to the full file buffer
        getline(file, temp); // get the next line
    }
    predefineWords();
    
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
            // if the current char is an integer, return true
            if(*cStringInput == digits[i]){
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
        if(program[pc] == "("){ // comments
            while(program[pc] != ")"){
                pc++;
            }
        } else if(program[pc] == ":") { // mark for compilation
            std::string symbol = program[++pc];
            pc++;
            std::string word; // the full word after compilation for the map

            while(program[pc] != ";"){ // is it the end of the compiler statement?
                if(program[pc] == "("){ // comments
                    while(program[pc] != ")"){ // end of comment
                        pc++;
                    }
                } else {
                    std::string instruction = compileInstruction(program[pc]);
                    word += instruction + ",";
                    pc++;
                }
            }
            // insert the new word into the wordmap
            wordMap.insert(std::pair<std::string, std::string>(symbol, word)); 
        } else {
            // word lookup
            if(wordMap.count(program[pc]) != 0){ // if the word exists
                std::cout << wordMap[program[pc]] << ","; // perform a word lookup
            } else if(isInt(program[pc])) { // check for integer
                std::cout << "16," <<
                    program[pc] << ",";
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

/*
  List of predefined handwritten words in the Benth language
*/
void predefineWords(){
    wordMap["+"] = "32"; // ( num1 num2 -- num3 ) add two numbers
    wordMap["-"] = "33"; // ( num1 num2 -- num3 ) subtract two numbers
    wordMap["*"] = "34"; // ( num1 num2 -- num3 ) multiply two numbers
    wordMap["/"] = "35"; // ( num1 num2 -- num3 ) divide two numbers
    wordMap["EMIT"] = "0x84"; // ( char1 -- ) print an ASCII character
    wordMap["."] = "0x81, 0x11"; // ( -- ) print the top of the stack (as int)
    wordMap["STORE"] = "0x60"; // ( address value -- ) store in memory
    wordMap["LOAD"] = "0x64"; // ( address -- ) take from memory and put on stack
    wordMap["HALT"] = "0x01"; // ( -- ) halt program execution
}
