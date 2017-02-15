#ifndef UTILS_H_
#define UTILS_H_

#include <string>

/**
 * @brief Create a bitmask of the given size.
 * @details For example, a bitmask of size 3 is 0b111.
 *
 * @param size the size in bits
 * @return the bitmask
 */
constexpr uint64_t bitMask(unsigned int size)
{
    uint64_t mask = 1;
    mask <<= size;
    mask -= 1;
    return mask;
}

std::string toUpper(const std::string& str);

constexpr bool isIdentifierChar(char ch, bool first)
{
    if (first)
    {
        return isalpha(ch) || ch == '_';
    }
    else
    {
        return isalnum(ch) || ch == '_';
    }
}

bool isIdentifierString(const std::string& str);

#endif // UTILS_H_
