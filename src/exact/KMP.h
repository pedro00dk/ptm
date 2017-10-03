#pragma once

#include <iostream>
#include <vector>
#include "ExactMatcher.h"

using namespace std;

class KMP : public ExactMatcher {

private:
    vector<string> patterns;
    vector<vector<int>> patternsStrictBordersLength;

public:
    KMP() = default;

    void setPatterns(vector<string> patterns) override {
        this->patterns = patterns;
        for (auto &pattern : patterns) patternsStrictBordersLength.push_back(calculateStrictBordersLen(pattern));
    }

    void searchPatterns(const string &textName, istream &text, bool count, bool print) override {
        int occurrences = 0;
        for (int p = 0; p < patterns.size(); p++) {
            string pattern = patterns[p];
            auto patternLength = (int) pattern.length();
            for (string line; getline(text, line);) {
                bool alreadyPrint = false;
                auto lineLength = static_cast<int>(line.length());
                for (int i = 0, j = 0; i <= lineLength - patternLength; i++) {
                    while (j < patternLength && line[i + j] == pattern[j]) j++;
                    if (j == patternLength) {
                        occurrences += 1;
                        if (!alreadyPrint && print) {
                            printf("%s\n", line.c_str());
                            alreadyPrint = true;
                        }
                        if (!count) continue;
                    }
                    i += max(1, j - patternsStrictBordersLength[p][j]);
                    j = max(0, patternsStrictBordersLength[p][j]);
                }
            }
        }
        if (count) printf("%s: %d\n", textName.c_str(), occurrences);
    }

private:
    vector<int> calculateStrictBordersLen(string pattern) {
        auto patternLen = (int) pattern.length();
        vector<int> strictBordersLen((unsigned long long int) patternLen + 1);
        for (int &i : strictBordersLen) i = -1;
        if (patternLen == -1 || (patternLen > 0 && pattern[0] != pattern[1]))strictBordersLen[1] = 0;
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
