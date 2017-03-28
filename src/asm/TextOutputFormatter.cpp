#include <iomanip>
#include <ostream>

#include "FormatSaver.hpp"
#include "TextOutputFormatter.h"

using namespace std;

bool TextOutputFormatter::isBinaryOutput() const
{
    return false;
}

/// @todo support instructions longer than 64-bit
void TextOutputFormatter::generate(IOutputFormatter::Config& config)
{
    // Save settings. They will be restored in
    // the object's destructor
    FormatSaver<ostream::char_type> saver(config.os);

    config.os << hex << setfill('0');

    int width = config.wordSize / 4;
    for (CodeGenerator::InstructionCode code : config.instCodeList)
    {
        uint64_t code0 = code[0];

        // the following assumes this program is running on a
        // little endian processor
        if (config.isLittleEndian)
        {
            code0 = ( (code0 << 56) & 0xFF00000000000000 ) |
                    ( (code0 << 40) & 0x00FF000000000000 ) |
                    ( (code0 << 24) & 0x0000FF0000000000 ) |
                    ( (code0 <<  8) & 0x000000FF00000000 ) |
                    ( (code0 >>  8) & 0x00000000FF000000 ) |
                    ( (code0 >> 24) & 0x0000000000FF0000 ) |
                    ( (code0 >> 40) & 0x000000000000FF00 ) |
                    ( (code0 >> 56) & 0x00000000000000FF );

            code0 >>= 64 - config.wordSize;
        }

        config.os << setw(width) << code0 << "\n";
    }
}
