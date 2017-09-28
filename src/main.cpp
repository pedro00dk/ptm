#include <iostream>
#include "cli/cli.h"
#include "exact/ExactMatcher.h"
#include "exact/BF.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>


using namespace std;

int main(int argc, char **argv) {

    CliOptions options = parseCommand(argc, argv);

    ExactMatcher *matcher = nullptr;
    if (options.algorithm == "bf") matcher = new BF();
    else if (options.algorithm == "kmp") matcher = new BF();
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
        matcher->searchPatternsStream("input", inputStream, options.count, !options.count);
    } else {
        for (auto &file : options.files) {
            int fileDescriptor = open(file.c_str(), O_RDONLY);
            struct stat fileStat{};
            fstat(fileDescriptor, &fileStat);
            auto *text = (char *) mmap(
                    nullptr,
                    static_cast<size_t>(fileStat.st_size),
                    PROT_READ,
                    MAP_SHARED,
                    fileDescriptor,
                    0
            );
            if (text == MAP_FAILED) cerr << "ERR: can not open file " << file << endl;
            matcher->searchPatternsMemory(
                    file,
                    text,
                    static_cast<int>(fileStat.st_size),
                    options.count,
                    !options.count
            );
        }
    }
}
