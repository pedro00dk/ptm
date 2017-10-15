#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include "../PatternMatcher.h"

using namespace std;

class Sellers : public PatternMatcher {

private:
    vector<string> patterns;
    int maxEditDistance;

public:
    Sellers(int maxEditDistance) : maxEditDistance(maxEditDistance) {}

    void setPatterns(vector<string> patterns) override {
        this->patterns = patterns;
    }

    void searchPatterns(const string &textName, istream &text, bool count, bool print) override {
        int occurrences = 0;
        vector<vector<int>> patternsColumnDistances = vector<vector<int>>();
        for (string &pattern : patterns) {
            vector<int> patternColumnDistances = vector<int>(pattern.length() + 1);
            for (int row = 0; row <= pattern.length(); row += 1)
                patternColumnDistances[row] = row;
            patternsColumnDistances.push_back(patternColumnDistances);
        }
        for (string line; getline(text, line);) {
            int lineOccurrences = countLineOccurrences(line, patternsColumnDistances, count);
            occurrences += lineOccurrences;
            if (lineOccurrences > 0 && print) cout << line << endl;
            for (vector<int> patternColumnDistances : patternsColumnDistances)
                for (int row = 0; row < patternColumnDistances.size(); row += 1) patternColumnDistances[row] = row;
        }
        if (count) cout << occurrences << endl;
    }

private:
    int countLineOccurrences(const string &line, vector<vector<int>> &patternsColumnDistances, bool count) {
        int occurrences = 0;
        int lineLength = (int) line.length();
        for (int p = 0; p < patterns.size(); p += 1) {
            string pattern = patterns[p];
            int patternLength = (int) pattern.length();
            vector<int> patternColumnDistances = patternsColumnDistances[p];
            for (int i = 0; i < lineLength; i += 1) {
                int previousColumnUp = 0;
                for (int row = 1; row <= patternLength; row += 1) {
                    int minimum = previousColumnUp + int(line[i] != pattern[row - 1]);
                    minimum = min(minimum, patternColumnDistances[row - 1] + 1);
                    minimum = min(minimum, patternColumnDistances[row] + 1);

                    previousColumnUp = patternColumnDistances[row];
                    patternColumnDistances[row] = minimum;
                }
                if (patternColumnDistances[patternLength] <= maxEditDistance) {
                    occurrences += 1;
                    if (!count) return occurrences;
                }
            }
        }
        return occurrences;
    }
};