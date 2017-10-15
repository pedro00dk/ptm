#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <queue>
#include "../PatternMatcher.h"

using namespace std;

class Aho : public PatternMatcher {
private:
    vector<string> patterns;
    int textLen, patternLen;
    vector<vector<int>> goTrie;
    vector<int> trieFail;
    vector<vector<int>> trieOccurrences;
public:
    Aho() {}

    void setPatterns(vector<string> patterns) override {
        this->patterns = patterns;
    }

    void searchPatterns(const string &textName, istream &text, bool count, bool print) override {
        buildGoto();
        buildFail();

        int occurrences = 0;
        string line;
        while (getline(text, line)) {
            int lineOccurences = ahoCorasick(line);
            occurrences += lineOccurences;
            if (print && lineOccurences) cout << line << "\n";
        }
        if (count) printf("%d\n", occurrences);
    }

private:
    void buildGoto() {
        int triePos, nextTriePos, patternPos;
        char currentChar;
        string pattern;

        nextTriePos = 0;
        goTrie.push_back(vector<int>(ISO_SIZE));
        trieOccurrences.push_back(vector<int>());

        for (int i = 0; i < patterns.size(); i++) {
            pattern = patterns[i];
            patternLen = pattern.size();
            triePos = patternPos = 0;

            currentChar = pattern[patternPos];

            while (patternPos < patternLen && goTrie[triePos][currentChar]) {
                triePos = goTrie[triePos][currentChar];
                patternPos++;
                currentChar = pattern[patternPos];
            }

            while (patternPos < patternLen) {
                triePos = goTrie[triePos][currentChar] = ++nextTriePos;
                goTrie.push_back(vector<int>(ISO_SIZE));
                trieOccurrences.push_back(vector<int>());
                patternPos++;
                currentChar = pattern[patternPos];
            }


            trieOccurrences[triePos].push_back(i);
        }
    }

    void buildFail() {
        trieFail.resize(goTrie.size());
        queue<int> q;
        int triePos, nextTriePos, border;

        for (int ascii = 0; ascii < ISO_SIZE; ascii++) {
            if (goTrie[0][ascii]) {
                q.push(goTrie[0][ascii]);
                trieFail[goTrie[0][ascii]] = 0;
            }
        }

        while (q.size()) {
            triePos = q.front();
            q.pop();

            for (int ascii = 0; ascii < ISO_SIZE; ascii++) {
                nextTriePos = goTrie[triePos][ascii];
                if (nextTriePos) {
                    q.push(nextTriePos);

                    border = trieFail[triePos];
                    while (border && goTrie[border][ascii] == 0) {
                        border = trieFail[border];
                    }
                    trieFail[nextTriePos] = goTrie[border][ascii];

                    for (int i = 0; i < trieOccurrences[trieFail[nextTriePos]].size(); i++) {
                        trieOccurrences[nextTriePos].push_back(trieOccurrences[trieFail[nextTriePos]][i]);
                    }
                }
            }
        }
    }

    int ahoCorasick(const string &text) {
        int occurrences = 0;
        int triePos = 0;
        unsigned char textChar;
        textLen = text.size();
        for (int textPos = 0; textPos < textLen; textPos++) {
            textChar = text[textPos];
            while (triePos && goTrie[triePos][textChar] == 0) {
                triePos = trieFail[triePos];
            }
            triePos = goTrie[triePos][textChar];

            for (int occ = 0; occ < trieOccurrences[triePos].size(); occ++) {
                int patternNumber = trieOccurrences[triePos][occ];
                occurrences++;
            }
        }
        return occurrences;
    }
};