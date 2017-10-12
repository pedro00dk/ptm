#pragma once

#include <iostream>
#include <string>
#include <vector>

using namespace std;

#define ASCII_SIZE = 128
#define ISO_SIZE 256

class PatternMatcher {

public:
    virtual void setPatterns(vector<string> patterns) = 0;

    virtual void searchPatterns(const string &textName, istream &text, bool count, bool print) = 0;
};
