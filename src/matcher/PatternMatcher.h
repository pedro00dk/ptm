#pragma once

#include <iostream>
#include <string>
#include <vector>

using namespace std;

#define ISO_SIZE 255

class PatternMatcher {

public:
    virtual void setPatterns(vector<string> patterns) = 0;

    virtual void searchPatterns(const string &textName, istream &text, bool count, bool print) = 0;
};
