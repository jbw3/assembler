#include "BinaryOutputFormatter.h"

using namespace std;

bool BinaryOutputFormatter::isBinaryOutput() const
{
    return true;
}

/// @todo support instructions longer than 64-bit
void BinaryOutputFormatter::generate(IOutputFormatter::Config& config)
{
    unsigned int wordSizeBytes = config.wordSize / 8;

    for (CodeGenerator::InstructionCode code : config.instCodeList)
    {
        // the following assumes this program is running on a
        // little endian processor
        const char* ptr = reinterpret_cast<const char*>(&(code[0]));
        if (config.isLittleEndian)
        {
            config.os.write(ptr, wordSizeBytes);
        }
        else
        {
            for (unsigned int i = wordSizeBytes; i > 0; --i)
            {
                config.os.write(&ptr[i - 1], 1);
            }
        }
    }
}
