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
        int occurrences = 0;
        for (string &pattern  : patterns) {
            int patternLength = (int) pattern.length();
            for (string line; getline(text, line);) {
                int lineLength = (int) line.length();
                bool alreadyPrint = false;
                for (int i = 0; i <= lineLength - patternLength; i++) {
                    int j = 0;
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
