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
