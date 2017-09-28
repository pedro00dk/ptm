#pragma once

#include <iostream>
#include <vector>
#include "ExactMatcher.h"

using namespace std;

class BF : public ExactMatcher {

private:
    vector<string> patterns;

public:
    BF() = default;

    void setPatterns(vector<string> patterns) override {
        this->patterns = patterns;
    }

    void searchPatternsMemory(const string &textName, char *text, int textLength, bool count, bool print) override {
        int occurrences = 0;
        for (auto &pattern : patterns) {
            auto patternLength = (int) pattern.length();
            int currentLineStart = 0;
            bool printCurrentLine = false;
            for (int i = 0; i <= textLength - patternLength; i++) {
                int j = 0;
                while (j < patternLength && text[i + j] == pattern[j]) j += 1;
                if (j == patternLength) {
                    occurrences += 1;
                    if (print) printCurrentLine = true;
                }
                if (text[i + j] == '\n' || text[i + j] == '\r') {
                    if (printCurrentLine) {
                        printf("%.*s\n", i + j - currentLineStart, text + currentLineStart);
                        printCurrentLine = false;
                    }
                    i += j;
                    currentLineStart = i + 1;
                }
            }
        }
        if (count) printf("%s: %d\n", textName.c_str(), occurrences);
    }

    void searchPatternsStream(const string &textName, istream &text, bool count, bool print) override {
        int occurrences = 0;
        for (auto &pattern  : patterns) {
            auto patternLength = (int) pattern.length();
            for (string line; getline(text, line);) {
                bool alreadyPrint = false;
                auto lineLength = static_cast<int>(line.length());
                for (int i = 0; i <= lineLength - patternLength; i++) {
                    int j = 0;
                    while (j < patternLength && line[i + j] == pattern[j]) j++;
                    if (j == patternLength) {
                        occurrences += 1;
                        if (!alreadyPrint && print) {
                            printf("%s\n", line.c_str());
                            alreadyPrint = true;
                        }
                        if (!count) continue;
                    }
                }
            }
        }
        if (count) printf("%s: %d\n", textName.c_str(), occurrences);
    }
};
