#include "TextMateSyntaxWriter.h"

using namespace std;

const char TextMateSyntaxWriter::HEADER[] = R"(<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
)";

const char TextMateSyntaxWriter::FOOTER[] = R"(</dict>
</plist>
)";

void TextMateSyntaxWriter::write(ostream& os, const SyntaxInfo& info)
{
    os << HEADER;

    writeName(os, info.name);
    writeFileTypes(os, info.fileTypes);
    writePatterns(os, info.rules);

    os << FOOTER;
}

void TextMateSyntaxWriter::writeName(ostream& os, const string& name)
{
    os << "    <key>name</key>\n    <string>" << name << "</string>\n";
}

void TextMateSyntaxWriter::writeFileTypes(ostream& os, const vector<string>& fileTypes)
{
    os << "    <key>fileTypes</key>\n    <array>\n";

    for (string fileType : fileTypes)
    {
        os << "        <string>" << fileType << "</string>\n";
    }

    os << "    </array>\n";
}

void TextMateSyntaxWriter::writePatterns(ostream& os, const vector<MatchRule>& rules)
{
    os << "    <key>patterns</key>\n    <array>\n";

    for (const MatchRule& rule : rules)
    {
        os << "        <dict>\n"
           << "            <key>match</key>\n"
           << "            <string>" << rule.match << "</string>\n"
           << "            <key>name</key>\n"
           << "            <string>" << rule.scope << "</string>\n"
           << "        </dict>\n";
    }

    os << "    </array>\n";
}
