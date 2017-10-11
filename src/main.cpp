#include <iostream>
#include "cli/cli.h"
#include "exact/ExactMatcher.h"
#include "exact/BruteForce.h"
#include "exact/KnuthMorrisPratt.h"
#include "exact/Sellers.h"
#include "exact/ahocorasick.h"
#include "exact/BoyerMoore.h"
#include "exact/ukkonen.h"
#include "exact/ShiftOr.h"

using namespace std;

int main(int argc, char **argv) {

    CliOptions options = parseCommand(argc, argv);

    ExactMatcher *matcher = nullptr;
    if (options.algorithm == "bf") matcher = new BruteForce();
    else if (options.algorithm == "kmp") matcher = new KnuthMorrisPratt();
    else if (options.algorithm == "bm") matcher = new BoyerMoore();
    else if (options.algorithm == "ac") matcher = new Aho();
    else if (options.algorithm == "so") matcher = new ShiftOr();
    else if (options.algorithm == "se") matcher = new Sellers(options.editDistance);
    else if (options.algorithm == "uk") matcher = new Ukkonen(options.editDistance);
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
