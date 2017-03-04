#include "BinaryOutputFormatter.h"

using namespace std;

void BinaryOutputFormatter::generate(ostream& os, unsigned int wordSize, const CodeGenerator::InstructionCodeList& instCodeList)
{
    for (CodeGenerator::InstructionCode code : instCodeList)
    {
        /// @todo support instructions longer than 64-bit

        // this only works on little endian processors
        const char* ptr = reinterpret_cast<const char*>(&(code[0]));

        os.write(ptr, wordSize);
    }
}
