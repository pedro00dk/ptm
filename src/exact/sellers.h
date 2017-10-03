#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include "ExactMatcher.h"

using namespace std;

class Sellers : public ExactMatcher {
private:
    vector<string> patterns;
    int ** columnDistances;
    int maxEditDist;
    int textLen, patternLen;

public:
    Sellers(int maxEditDist): maxEditDist(maxEditDist) {}
    
    void setPatterns(vector<string> patterns) override {
        this->patterns = patterns;
    }

    void searchPatterns(const string &textName, istream &text, bool count, bool print) override {
        columnDistances = new int*[patterns.size()];
        for (int patternNumber = 0; patternNumber < patterns.size(); patternNumber++) {
            columnDistances[patternNumber] = new int[patterns[patternNumber].length() + 1];
        }

        int occurrences = 0;
        string line;
        while (getline(text, line)) {
            int lineOccurences = 0;

            for (int patternNumber = 0; patternNumber < patterns.size(); patternNumber++) {
                lineOccurences += sellers(patternNumber, line);
                occurrences += lineOccurences;
            }
            
            if (print && lineOccurences) cout << line << "\n";
        }
        if (count) printf("%s: %d\n", textName.c_str(), occurrences);

        for (int patternNumber = 0; patternNumber < patterns.size(); patternNumber++) {
            delete columnDistances[patternNumber];
        }
        delete columnDistances;
    }

private:
    int sellers(const int &patternNumber, const string &text){
        int occurrences = 0;
        int textPos = 0;
        string pattern = patterns[patternNumber];
        patternLen = pattern.size();
        textLen = text.size();

        int * patternColumn = columnDistances[patternNumber];
        int * tempColumn = new int[patternLen + 1];

        for (int row = 0; row < patternLen + 1; row++) {
            patternColumn[row] = row;
        }
        if (patternColumn[patternLen] <= maxEditDist) occurrences++;

        for (int column = 0; column < textLen; column++) {
            tempColumn[0] = 0;
            for (int row = 1; row < patternLen + 1; row++) {
                tempColumn[row] = tempColumn[row - 1] + 1;
                tempColumn[row] = min(tempColumn[row], patternColumn[row] + 1);
                tempColumn[row] = min(tempColumn[row], patternColumn[row - 1] + int(pattern[row-1] != text[column]));
            }   
            for (int row = 1; row < patternLen + 1; row++) patternColumn[row] = tempColumn[row];
            
            if (patternColumn[patternLen] <= maxEditDist) occurrences++;
        }

        delete tempColumn;
        return occurrences;
    }
};