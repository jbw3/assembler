#include <iomanip>
#include <ostream>

#include "TextCodeGenerator.h"

using namespace std;

TextCodeGenerator::TextCodeGenerator(ostream& os) :
    os(os)
{
}

void TextCodeGenerator::generate(const SyntaxAnalyzer::InstructionCodeList& instCodeList)
{
    /// @todo reset os state
    os << hex << setfill('0');

    for (SyntaxAnalyzer::InstructionCode code : instCodeList)
    {
        /// @todo support instructions longer than 64-bit
        /// @todo setw() should be based on instruction size
        os << setw(2) << code[0] << "\n";
    }
}
