#pragma once

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class ExactMatcher {

public:
    virtual void setPatterns(vector<string> patterns) = 0;

    virtual void searchPatterns(const string &textName, istream &text, bool count, bool print) = 0;
};
