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
        for (string line; getline(text, line);) {
            int lineOccurrences = 0;
            for (string &pattern  : patterns) {
                lineOccurrences += countLineOccurrences(line, pattern, count);
                occurrences += lineOccurrences;
                if (lineOccurrences > 0 && !count) break;
            }
            if (lineOccurrences > 0 && print) cout << line << endl;
        }
        if (count) cout << textName << ": " << occurrences << endl;
    }

private:
    int countLineOccurrences(const string &line, const string &pattern, bool count) {
        int occurrences = 0;
        int lineLength = (int) line.length();
        int patternLength = (int) pattern.length();
        for (int i = 0; i <= lineLength - patternLength; i++) {
            int j = 0;
            while (j < patternLength && line[i + j] == pattern[j]) j++;
            if (j == patternLength) {
                occurrences += 1;
                if (!count) return occurrences;
            }
        }
        return occurrences;
    }
};
