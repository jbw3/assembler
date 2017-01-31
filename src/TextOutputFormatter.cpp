#include <iomanip>
#include <ostream>

#include "TextOutputFormatter.h"

using namespace std;

TextOutputFormatter::TextOutputFormatter(ostream& os, unsigned int wordSize) :
    os(os),
    wordSize(wordSize)
{
}

void TextOutputFormatter::generate(const CodeGenerator::InstructionCodeList& instCodeList)
{
    // save original settings
    ios_base::fmtflags origFlags = os.flags();
    char origFill = os.fill();

    os << hex << setfill('0');

    int width = wordSize / 4;
    for (CodeGenerator::InstructionCode code : instCodeList)
    {
        /// @todo support instructions longer than 64-bit
        os << setw(width) << code[0] << "\n";
    }

    // restore original settings
    os.flags(origFlags);
    os.fill(origFill);
}
