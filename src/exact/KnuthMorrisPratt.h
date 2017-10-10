#pragma once

#include <iostream>
#include <vector>
#include <cstring>
#include "ExactMatcher.h"

using namespace std;

class KnuthMorrisPratt : public ExactMatcher {

private:
    vector<string> patterns;
    vector<vector<int>> patternsStrictBordersLength;

public:
    KnuthMorrisPratt() = default;

    void setPatterns(vector<string> patterns) override {
        this->patterns = patterns;
        patternsStrictBordersLength = vector<vector<int>>();
        for (string &pattern : patterns) patternsStrictBordersLength.push_back(calculateStrictBordersLength(pattern));
    }

    void searchPatterns(const string &textName, istream &text, bool count, bool print) override {
        int occurrences = 0;
        for (int p = 0; p < patterns.size(); p++) {
            string pattern = patterns[p];
            int patternLength = (int) pattern.length();
            for (string line; getline(text, line);) {
                int lineLength = (int) line.length();
                bool alreadyPrint = false;
                int i = 0;
                int j = 0;
                while (i <= lineLength - patternLength) {
                    while (j < patternLength && line[i + j] == pattern[j]) j++;
                    if (j == patternLength) {
                        occurrences += 1;
                        if (!alreadyPrint && print) {
                            cout << line << endl;
                            alreadyPrint = true;
                        }
                        if (!count) break;
                    }
                    i += max(1, j - patternsStrictBordersLength[p][j]);
                    j = max(0, patternsStrictBordersLength[p][j]);
                }
            }
        }
        if (count) cout << textName << ": " << occurrences << endl;
    }

private:
    vector<int> calculateStrictBordersLength(string pattern) {
        int patternLength = (int) pattern.length();
        vector<int> strictBordersLen = vector<int>((unsigned long) patternLength + 1);
        for (int i = 0; i <= patternLength; i += 1) strictBordersLen[i] = -1;
        if (patternLength == 1 || (patternLength > 0 && pattern[0] != pattern[1])) strictBordersLen[1] = 0;
        for (int i = 1, j = 0; i < patternLength;) {
            while (i + j < patternLength && pattern[i + j] == pattern[j]) {
                j += 1;
                if (i + j == patternLength || pattern[i + j] != pattern[j]) strictBordersLen[i + j] = j;
                else strictBordersLen[i + j] = strictBordersLen[j];
            }
            if (j == 0 && (i == patternLength - 1 || pattern[0] != pattern[i + 1])) strictBordersLen[i + 1] = 0;
            i += j - strictBordersLen[j];
            j = max(0, strictBordersLen[j]);
        }
        return strictBordersLen;
    }
};
