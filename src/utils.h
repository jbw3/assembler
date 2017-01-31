#ifndef UTILS_H_
#define UTILS_H_

#include <string>

std::string toUpper(const std::string& str);

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

#endif // UTILS_H_
