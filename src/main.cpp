#include <iostream>
#include "cli/cli.h"
#include "exact/ExactMatcher.h"
#include "exact/BF.h"
#include "exact/KMP.h"


using namespace std;

int main(int argc, char **argv) {

    CliOptions options = parseCommand(argc, argv);

    ExactMatcher *matcher = nullptr;
    if (options.algorithm == "bf") matcher = new BF();
    else if (options.algorithm == "kmp") matcher = new KMP();
    else if (options.algorithm == "bm") matcher = new BF();
    else if (options.algorithm == "ac") matcher = new BF();
    else if (options.algorithm == "so") matcher = new BF();
    else if (options.algorithm == "se") matcher = new BF();
    else if (options.algorithm == "uk") matcher = new BF();
    else if (options.algorithm == "wu") matcher = new BF();
    else {
        cerr << "ERR: matcher not implemented" << endl;
        exit(1);
    }
    matcher->setPatterns(options.patterns);

    // Processing input stream or files
    if (options.files.empty()) {
        istream &inputStream = std::cin;
        if (inputStream.eof()) {
            cerr << "ERR: empty stream" << endl;
            exit(1);
        }
        matcher->searchPatterns("input", inputStream, options.count, !options.count);
    } else {
        for (auto &file : options.files) {
            ifstream inputFileStream(file);
            if (!inputFileStream.is_open()) {
                cerr << "ERR: can not open file " << file << endl;
                continue;
            }
            matcher->searchPatterns(file, inputFileStream, options.count, !options.count);
        }
    }
}
