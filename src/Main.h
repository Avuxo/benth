#include <vector>
#include <string>

std::vector<std::string> tokenize(std::string input);
void compileBenthToBVM(std::vector<std::string> program);
std::string compileInstruction(std::string);
bool isInt(std::string str);
void predefineWords();
