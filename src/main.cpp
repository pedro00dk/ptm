#include <iostream>
#include "cli/cli.h"

using namespace std;

int main(int argc, char **argv)
{
    // Reading command file options and global arguments
    CliOptions options = parseCommand(argc, argv);

    // Getting patterns and text files
    vector<string> textFiles;
    vector<string> patterns;
    if (options.patternFile.empty())
        textFiles = vector<string>(options.arguments.begin() + 1, options.arguments.end());
    else
        textFiles = options.arguments;

    if (options.patternFile.empty())
        patterns.push_back(options.arguments[0]);
    else
    {
        ifstream patternsStream(options.patternFile, std::ifstream::in);
        if (!patternsStream.is_open())
        {
            cerr << "Can not open patterns file" << endl;
            exit(1);
        }
        for (string line; getline(patternsStream, line);)
            patterns.push_back(line);
        patternsStream.close();
    }

    //
}