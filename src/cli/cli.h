#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <getopt.h>
#include <algorithm>

using namespace std;

// Cli options holder
struct CliOptions {
    string algorithm = "";
    bool isExact = false;
    bool isApprox = false;
    int editDistance = -1;
    bool count = false;
    vector<string> patterns;
    vector<string> files;
};

// CLI options
static const string shortCliOptions = "ha:e:cp:";
static const vector<option> longCliOptions = {
        {"help",      no_argument,       nullptr, 'h'},
        {"algorithm", required_argument, nullptr, 'a'},
        {"edit",      required_argument, nullptr, 'e'},
        {"count",     no_argument,       nullptr, 'c'},
        {"pattern",   required_argument, nullptr, 'p'},
        {nullptr, 0,                     nullptr, 0}};

// Options arguments
static const vector<string> exactAlgorithmArguments = {"bf", "kmp", "bm", "ac", "so"};
static const vector<string> approxAlgorithmArguments = {"se", "uk", "wu"};

// Help message
static const string help =
        "Usage:\n"
                "ptm [<option>] pattern textfile [textfile...]  ->  search files\n"
                "ptm [<option>] pattern  ->  search input stream\n"
                "ptm <option> <-h | --help>  ->  show global help\n"
                "ptm <-h | --help>  -> show option help\n"
                "Options:\n"
                "-h,--help            Display this information\n"
                "-a,--algorithm <arg> Specify the search algorithm\n"
                "-e,--edit <arg>      Specify the edit distance (required) for approx algorithms\n"
                "-c,--count           Display only the number of found occurrences\n"
                "-p,--pattern <arg>   Specify a file with patterns to search\n"
                "                     If specified, the first global argument (pattern) is treated text file\n"
                "\n"
                "Examples:\n"
                "ptm -algorithm kmp -c abra abracadrabra.txt\n"
                "\n"
                "\n"
                "-a,--algorithm <arg> Specify the search algorithm\n"
                "arg options:\n"
                "\texact: bf, kmp, bm, ac, so\n"
                "\tapprox: se, uk, wu\n"
                "\n"
                "If algorithm is not specified:\n"
                "\tIf nothing (except -c,--count) is specified, \"bm\" is choosen by default\n"
                "\tIf -e,--edit is specified, \"uk\" is choosen by default\n"
                "\tIf -p,--pattern is specified (and not -e,--edit), \"ac\" is choosen by default\n"
                "\tIf -m,--clmode is enabled (and not -e,--edit) \"bf\" is choosen by default\n"
                "\n"
                "Special conditions:\n"
                "\tIf -e,--edit is specified, an exact algorithm can not be choosen\n"
                "\n"
                "\n"
                "-e,--edit <arg>      Specify the edit distance for approx algorithms\n"
                "arg should be positive\n"
                "Special conditions:\n"
                "\tIf -a,--algorithm sets an exact algorithm, -e,--edit can not be specified\n"
                "\tIf -a,--algorithm sets an approx algorithm, -e,--edit should be specified\n"
                "\n"
                "\n"
                "-c,--count           Display only the number of found occurrences\n"
                "\n"
                "\n"
                "-p,--pattern <arg>   Specify a file with patterns to search\n";

bool isInVector(const string &text, vector<string> vector) {
    for (auto &textInVector : vector)
        if (text == textInVector)
            return true;
    return false;
}

CliOptions parseCommand(int argc, char **argv) {
    CliOptions options = {};
    int optionFlag, uselessIndex;
    while ((optionFlag = getopt_long(argc, argv, shortCliOptions.c_str(), &longCliOptions[0], &uselessIndex)) != -1) {
        switch (optionFlag) {
            case 'h':
                cout << help;
                exit(0);
            case 'a':
                options.algorithm = optarg;
                options.isExact = isInVector(optarg, exactAlgorithmArguments);
                options.isApprox = isInVector(optarg, approxAlgorithmArguments);
                if (!options.isExact && !options.isApprox) {
                    cerr << "ERR: illegal algorithm name" << endl;
                    exit(1);
                }
                break;
            case 'e':
                options.editDistance = stoi(optarg);
                if (options.editDistance < 0) {
                    cerr << "ERR: edit distance should be positive" << endl;
                    exit(1);
                }
                break;
            case 'c':
                options.count = true;
                break;
            case 'p': {
                ifstream patternsFileStream(optarg);
                if (!patternsFileStream.is_open()) {
                    cerr << "ERR: can not open patterns file" << endl;
                    exit(1);
                }
                for (string line; getline(patternsFileStream, line);)
                    options.patterns.push_back(line);
                patternsFileStream.close();
            }
                break;
            default:
                cerr << "ERR: option not found" << endl;
                exit(1);
        }
    }

    // Default options
    if (options.algorithm.empty()) {
        if (options.editDistance != -1) {
            options.algorithm = "uk";
            options.isApprox = true;
        } else if (options.patterns.size() > 1) {
            options.algorithm = "ac";
            options.isExact = true;
        } else {
            options.algorithm = "bm";
            options.isExact = true;
        }
    }

    // Check options conflicts
    if ((options.isExact && options.editDistance != -1) || (options.isApprox && options.editDistance == -1))
        cerr << "ERR: conflicting options, -a,--algorithm and -e,--edit, check --help" << endl;

    // Get global arguments
    char **arguments = argv + optind;
    int argumentCount = argc - optind;
    if (argumentCount == 0 && options.patterns.empty()) {
        cerr << "ERR: missing pattern" << endl;
        exit(1);
    }
    for (int i = 0; i < argumentCount; i++)
        if (i == 0 && options.patterns.empty()) options.patterns.emplace_back(arguments[i]);
        else options.files.emplace_back(arguments[i]);

    options.files.resize(unique(options.files.begin(), options.files.end()) - options.files.begin());

    return options;
}
