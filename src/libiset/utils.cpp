#include "utils.h"

using namespace std;

string toUpper(const string& str)
{
    string upper;
    for (char ch : str)
    {
        upper += toupper(ch);
    }
    return upper;
}

bool isIdentifierString(const std::string& str)
{
    if (str.empty())
    {
        return false;
    }
    else
    {
        for (size_t i = 0; i < str.size(); ++i)
        {
            if (!isIdentifierChar(str[i], i == 0))
            {
                return false;
            }
        }

        return true;
    }
}
