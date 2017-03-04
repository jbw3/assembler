#include <iomanip>
#include <ostream>

#include "FormatSaver.hpp"
#include "TextOutputFormatter.h"

using namespace std;

bool TextOutputFormatter::isBinaryOutput() const
{
    return false;
}

void TextOutputFormatter::generate(ostream& os, unsigned int wordSize, const CodeGenerator::InstructionCodeList& instCodeList)
{
    // Save settings. They will be restored in
    // the object's destructor
    FormatSaver<ostream::char_type> saver(os);

    os << hex << setfill('0');

    int width = wordSize / 4;
    for (CodeGenerator::InstructionCode code : instCodeList)
    {
        /// @todo support instructions longer than 64-bit
        os << setw(width) << code[0] << "\n";
    }
}
