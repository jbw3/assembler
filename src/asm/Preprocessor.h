#ifndef PREPROCESSOR_H_
#define PREPROCESSOR_H_

#include <iostream>

class Preprocessor
{
public:
    static constexpr char COMMENT_CHAR = '#';

    Preprocessor();

    void process(std::istream& is, std::ostream& os);
};

#endif // PREPROCESSOR_H_
