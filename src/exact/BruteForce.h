#pragma once

#include <iostream>
#include <vector>
#include "ExactMatcher.h"

using namespace std;

class BruteForce : public ExactMatcher {

private:
    vector<string> patterns;

public:
    BruteForce() = default;

    void setPatterns(vector<string> patterns) override {
        this->patterns = patterns;
    }

    void searchPatterns(const string &textName, istream &text, bool count, bool print) override {
        auto occurrences = 0;
        for (auto &pattern  : patterns) {
            auto patternLength = (int) pattern.length();
            for (string line; getline(text, line);) {
                auto alreadyPrint = false;
                auto lineLength = static_cast<int>(line.length());
                for (auto i = 0; i <= lineLength - patternLength; i++) {
                    auto j = 0;
                    while (j < patternLength && line[i + j] == pattern[j]) j++;
                    if (j == patternLength) {
                        occurrences += 1;
                        if (!alreadyPrint && print) {
                            cout << line << endl;
                            alreadyPrint = true;
                        }
                        if (!count) continue;
                    }
                }
            }
        }
        if (count) cout << textName << ": " << occurrences << endl;
    }
};
