#pragma once

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class ExactMatcher {

public:
    virtual void setPatterns(vector<string> patterns) = 0;

    virtual void searchPatternsMemory(const string &textName, char *text, int textLength, bool count, bool print) = 0;

    virtual void searchPatternsStream(const string &textName, istream &text, bool count, bool print) = 0;
};
