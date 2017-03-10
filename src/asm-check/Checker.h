#ifndef CHECKER_H_
#define CHECKER_H_

#include <string>
#include <vector>

class InstructionSet;

class Checker
{
public:
    Checker(const std::string& iSetName);

    void check();

private:
    std::string iSetName;
    const InstructionSet* iSet;
    bool errors;

    void logError(const std::string& errorMsg);
};

#endif // CHECKER_H_
