#include <istream>
#include <string>
#include <vector>

class LexicalAnalyzer
{
public:
    static const std::string WHITESPACE_CHARS;

    LexicalAnalyzer();

    void process(std::istream& is, std::vector<std::string>& tokens);

private:
    void parseLine(const std::string& line, std::vector<std::string>& tokens);
};
