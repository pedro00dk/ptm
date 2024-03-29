#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include "../PatternMatcher.h"

using namespace std;

class BoyerMoore : public PatternMatcher {

private:
    vector<string> patterns;
    vector<vector<int>> patternsBadCharacters;
    vector<vector<int>> patternsGoodSuffixes;

public:
    void setPatterns(vector<string> patterns) override {
        this->patterns = patterns;
        patternsBadCharacters = vector<vector<int>>();
        patternsGoodSuffixes = vector<vector<int>>();
        for (string &pattern : patterns) {
            patternsBadCharacters.push_back(calculateBadCharacters(pattern));
            patternsGoodSuffixes.push_back(calculateGoodSuffix(pattern));
        }
    }

    void searchPatterns(const string &textName, istream &text, bool count, bool print) override {
        int occurrences = 0;
        for (string line; getline(text, line);) {
            int lineOccurrences = 0;
            for (int p = 0; p < patterns.size(); p++) {
                lineOccurrences += countLineOccurrences(
                        line, patterns[p], patternsBadCharacters[p], patternsGoodSuffixes[p], count
                );
                if (lineOccurrences > 0 && !count) break;
            }
            if (lineOccurrences > 0 && print) cout << line << endl;
            occurrences += lineOccurrences;
        }
        if (count) cout << occurrences << endl;
    }

private:
    int countLineOccurrences(const string &line, const string &pattern, const vector<int> &patternBadCharacters,
                             const vector<int> &patternGoodSuffixes, bool count) {

        int occurrences = 0;
        int lineLength = (int) line.length();
        int patternLength = (int) pattern.length();

        int textPos = 0;

        while (textPos <= lineLength - patternLength) {
            int patternPos = patternLength - 1;
            while (patternPos >= 0 && line[textPos + patternPos] == pattern[patternPos]) patternPos -= 1;
            if (patternPos == -1) {
                textPos += patternGoodSuffixes[0];
                occurrences++;
            } else {
                unsigned char textChar = line[textPos + patternPos];
                textPos += max(patternPos - patternBadCharacters[textChar],
                               patternGoodSuffixes[patternPos + 1]);
            }
        }

        return occurrences;

    }

    vector<int> calculateBadCharacters(string pattern) {
        int patternLength = (int) pattern.size();
        vector<int> patternBadCharacters = vector<int>(ISO_SIZE);
        for (int patternCharacterIndex = 0; patternCharacterIndex < patternLength; patternCharacterIndex += 1) {
            unsigned char patternCharacter = pattern[patternCharacterIndex];
            patternBadCharacters[patternCharacter] = patternCharacterIndex;
        }
        return patternBadCharacters;
    }

    vector<int> calculateGoodSuffix(string pattern) {
        string reversePattern = string(pattern);
        reverse(reversePattern.begin(), reversePattern.end());

        vector<int> patternBorderLengths = calculateBorderLengths(pattern);
        vector<int> reversePatternBorderLengths = calculateBorderLengths(reversePattern);

        int patternLength = (int) pattern.size();
        vector<int> patternGoodSuffixes = vector<int>((unsigned long) patternLength + 1);
        fill(patternGoodSuffixes.begin(), patternGoodSuffixes.end(),
             patternLength - patternBorderLengths[patternLength]);

        for (int i = 1; i < patternLength + 1; i += 1) {
            int j = patternLength - reversePatternBorderLengths[i];
            patternGoodSuffixes[j] = min(patternGoodSuffixes[j], i - reversePatternBorderLengths[i]);
        }

        return patternGoodSuffixes;
    }

protected:
    vector<int> calculateBorderLengths(string pattern) {
        int patternLength = (int) pattern.length();
        vector<int> patternBorderLengths = vector<int>((unsigned long) patternLength + 1);
        for (int i = 1, j = 0; i + j < patternLength;) {
            while (i + j < patternLength && pattern[i + j] == pattern[j]) {
                j += 1;
                patternBorderLengths[i + j] = j;
            }
            i += max(1, j - patternBorderLengths[j]);
            j = patternBorderLengths[j];
        }
        return patternBorderLengths;
    }
};
