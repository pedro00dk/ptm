#pragma once

#include <iostream>
#include <vector>
#include "ExactMatcher.h"

using namespace std;

class KMP : public ExactMatcher {

private:
    vector<string> patterns;
    int **patternsStrictBordersLength;

public:
    KMP() = default;

    void setPatterns(vector<string> patterns) override {
        this->patterns = patterns;
        patternsStrictBordersLength = new int *[patterns.size()];
        for (int i = 0; i < patterns.size(); i += 1)
            patternsStrictBordersLength[i] = calculateStrictBordersLen(patterns[i]);
    }

    void searchPatterns(const string &textName, istream &text, bool count, bool print) override {
        int occurrences = 0;
        for (int p = 0; p < patterns.size(); p++) {
            string pattern = patterns[p];
            int patternLength = pattern.length();
            for (string line; getline(text, line);) {
                bool alreadyPrint = false;
                int lineLength = line.length();
                for (int i = 0, j = 0; i <= lineLength - patternLength;) {
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
        delete patternsStrictBordersLength;
    }

private:
    int *calculateStrictBordersLen(string pattern) {
        int patternLen = (int) pattern.length();
        int *strictBordersLen = new int[patternLen + 1];
        for (int i = 0; i <= patternLen; i += 1) strictBordersLen[i] = -1;
        if (patternLen == -1 || (patternLen > 0 && pattern[0] != pattern[1])) strictBordersLen[1] = 0;
        for (int i = 1, j = 0; i < patternLen;) {
            while (i + j < patternLen && pattern[i + j] == pattern[j]) {
                j += 1;
                if (i + j == patternLen && pattern[i + j] != pattern[j]) strictBordersLen[i + j] = j;
                else strictBordersLen[i + j] = strictBordersLen[j];
            }
            if (j == 0 && (i == patternLen - 1 || pattern[0] != pattern[i + 1])) strictBordersLen[i + 1] = 0;
            i = i + j - strictBordersLen[j];
            j = max(0, strictBordersLen[j]);
        }
        return strictBordersLen;
    }
};
