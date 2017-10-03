#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include "ExactMatcher.h"

using namespace std;

#define ASCII 127

class BoyerMoore : public ExactMatcher {
private:
    int ** badChar;
    int ** goodSuffix;
    vector<string> patterns;
    int textLen, patternLen;

public:
    void setPatterns(vector<string> patterns) override {
        this->patterns = patterns;
    }

    void searchPatterns(const string &textName, istream &text, bool count, bool print) override {
        badChar = new int*[patterns.size()];
        goodSuffix = new int*[patterns.size()];
        for (int patternNumber = 0; patternNumber < patterns.size(); patternNumber++) {
            buildBadChar(patternNumber);
            buildGoodSuffix(patternNumber);
        }

        int occurrences = 0; // TODO: acumula de diferentes patterns?
        string line;
        while (getline(text, line)) {
            bool alreadyPrint = false;
            int lineOccurences = 0;

            for (int patternNumber = 0; patternNumber < patterns.size(); patternNumber++) {
                lineOccurences += boyermoore(patternNumber, line);
                occurrences += lineOccurences;
            }
            
            if (print && lineOccurences) cout << line << "\n";
        }
        if (count) printf("%s: %d\n", textName.c_str(), occurrences);

        delete badChar;
        delete goodSuffix;
    }

private:

    void buildBadChar(const int &patternNumber){
        string pattern = patterns[patternNumber];
        patternLen = pattern.size();

        char patternChar;

        badChar[patternNumber] = new int[ASCII];
        memset(badChar[patternNumber], -1, sizeof(int) * ASCII);

        for (int patternPos = 0; patternPos < patternLen; patternPos++) {
            patternChar = pattern[patternPos];
            badChar[patternNumber][patternChar] = patternPos;
        }
    }

    void buildGoodSuffix(const int &patternNumber){
        string pattern = patterns[patternNumber];
        patternLen = pattern.size();

        goodSuffix[patternNumber] = new int[patternLen];

        for (int i = 0; i < patternLen; i++) goodSuffix[patternNumber][i] = 1;
    }

    int boyermoore(const int &patternNumber, const string &text){
        int occurrences = 0;
        int textPos = 0;
        string pattern = patterns[patternNumber];
        patternLen = pattern.size();
        textLen = text.size();

        while (textPos <= textLen - patternLen){
            int patternPos = patternLen - 1;
            while (patternPos >= 0 && text[textPos+patternPos] == pattern[patternPos]) patternPos--;
            
            if (patternPos == -1) {
                textPos += goodSuffix[patternNumber][0];
                occurrences++;
            } else {
                char textChar = text[textPos + patternPos];
                textPos += max(patternPos - badChar[patternNumber][textChar], goodSuffix[patternNumber][patternPos]);
            }
        }

        return occurrences;
    }
};