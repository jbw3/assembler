#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "SyntaxInfo.h"
#include "TextMateSyntaxWriter.h"

using namespace std;

int main()
{
    SyntaxInfo info;
    info.fileTypes = {"asm", "inc", "s"};

    fstream file;
    file.open("out.tmLanguage", ios::out);

    SyntaxFileWriter* writer = new TextMateSyntaxWriter;
    writer->write(file, info);
    delete writer;

    file.close();

    return 0;
}
