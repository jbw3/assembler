#include <iostream>

class Preprocessor
{
public:
    static constexpr char COMMENT_CHAR = '#';

    Preprocessor();

    void process(std::istream& is, std::ostream& os);
};
