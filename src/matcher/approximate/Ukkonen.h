#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <set>
#include <map>
#include <queue>
#include "../PatternMatcher.h"

using namespace std;

typedef int State;
typedef pair<int, char> Transition;
typedef vector<int> Row;
typedef pair<vector<int>, State> RowState;

class Ukkonen : public PatternMatcher {
private:
    vector<string> patterns;
    int **columnDistances;
    int maxEditDist;
    int textLen, patternLen;
    map<Transition, State> *transitions;
    set<State> finalStates;
    int nextState;

public:
    Ukkonen(int maxEditDist) : maxEditDist(maxEditDist) {
        nextState = 1;
    }

    void setPatterns(vector<string> patterns) override {
        this->patterns = patterns;
    }

    void searchPatterns(const string &textName, istream &text, bool count, bool print) override {
        transitions = new map<Transition, State>[patterns.size()];

        for (int patternNumber = 0; patternNumber < patterns.size(); patternNumber++) {
            buildStates(patternNumber);
        }

        int occurrences = 0;
        string line;
        while (getline(text, line)) {
            int lineOccurrences = 0;

            for (int patternNumber = 0; patternNumber < patterns.size(); patternNumber++) {
                lineOccurrences += ukkonen(patternNumber, line);
                occurrences += lineOccurrences;
            }

            if (print && lineOccurrences) cout << line << "\n";
        }
        if (count) printf("%s: %d\n", textName.c_str(), occurrences);

        delete transitions;
    }

private:
    vector<int> nextRow(const vector<int> &pastRow, const string &pattern, char c) {
        vector<int> row;
        row.resize(pastRow.size());
        row[0] = 0;

        int mini;
        for (int i = 1; i < pastRow.size(); i++) {
            mini = pastRow[i - 1] + int(pattern[i - 1] != c);
            mini = min(mini, row[i - 1] + 1);
            mini = min(mini, pastRow[i] + 1);
            mini = min(mini, maxEditDist + 1);
            row[i] = mini;
        }

        return row;
    }

    void buildStates(int patternNumber) {
        string pattern = patterns[patternNumber];
        patternLen = pattern.size();

        vector<int> row;
        row.resize(patternLen + 1);
        for (int i = 0; i <= patternLen; i++) row[i] = i;

        State state = 0;
        RowState rowState = RowState(row, state);

        map<vector<int>, int> rowStatesMap;
        map<vector<int>, int>::iterator it;
        queue<RowState> rowStatesQueue;

        rowStatesQueue.push(rowState);
        rowStatesMap[row] = state;
        Transition transition;

        if (patternLen <= maxEditDist) {
            finalStates.insert(initialState);
        }

        while (rowStatesQueue.size()) {
            rowState = rowStatesQueue.front();
            rowStatesQueue.pop();
            row = rowState.first;

            for (char c = 0; c < ISO_SIZE; c++) {
                row = rowState.first;
                row = nextRow(row, pattern, c);

                it = rowStatesMap.find(row);

                if (it == rowStatesMap.end()) {
                    state = nextState;
                    rowStatesQueue.push(RowState(row, state));
                    rowStatesMap[row] = state;
                    nextState++;

                    if (row[patternLen] <= maxEditDist) {
                        finalStates.insert(state);
                    }

                } else {
                    state = it->second;
                }

                transition = Transition(rowState.second, c);
                transitions[patternNumber][transition] = state;
            }
        }
    }

    int ukkonen(const int &patternNumber, const string &text) {
        int occurrences = 0;
        string pattern = patterns[patternNumber];
        patternLen = pattern.size();
        textLen = text.size();

        State currentState = 0;
        char textChar;
        Transition transition;

        for (int textPos = 0; textPos < textLen; textPos++) {
            textChar = text[textPos];
            transition = Transition(currentState, textChar);
            currentState = transitions[patternNumber][transition];

            if (finalStates.count(currentState)) {
                occurrences++;
            }
        }

        return occurrences;
    }
};