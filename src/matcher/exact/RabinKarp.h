#pragma once

#include <iostream>
#include <vector>
#include "../PatternMatcher.h"

using namespace std;

class RabinKarp : public PatternMatcher {

private:
    vector<string> patterns;
    vector<unsigned long long> patternHashes;
    vector<unsigned long long> patternHashesPotencies;
    unsigned long long HASH_BASE = 31;

public:
    RabinKarp() = default;

    void setPatterns(vector<string> patterns) override {
        this->patterns = patterns;

        patternHashes.reserve(patterns.size());
        patternHashesPotencies.reserve(patterns.size());
        for (int i = 0; i < patterns.size(); i++) {
            patternHashes[i] = calculatePatternHash(patterns[i]);
            patternHashesPotencies[i] = calculatePatternHashPotency(patterns[i]);
        }
    }

    void searchPatterns(const string &textName, istream &text, bool count, bool print) override {
        int occurrences = 0;
        for (string line; getline(text, line);) {
            int lineOccurrences = 0;
            for (int i = 0; i < patterns.size(); i++) {
                lineOccurrences += countLineOccurrences(line, patterns[i], patternHashes[i], patternHashesPotencies[i],
                                                        count);
                occurrences += lineOccurrences;
                if (lineOccurrences > 0 && !count) break;
            }
            if (lineOccurrences > 0 && print) cout << line << endl;
        }
        if (count) cout << textName << ": " << occurrences << endl;
    }

private:
    int countLineOccurrences(const string &line, const string &pattern, unsigned long long patternHash,
                             unsigned long long patternHashPotency, bool count) {
        int occurrences = 0;
        int lineLength = (int) line.length();
        int patternLength = (int) pattern.length();
        unsigned long long hash = 0;

        for (int i = 0; i < patternLength; i++) {
            hash = hash * HASH_BASE + line[i];
        }

        if (hash == patternHash && bruteForceAssertOccurrence(line, pattern, 0)) {
            occurrences += 1;
            if (!count) return occurrences;
        }

        for (int i = patternLength; i < lineLength; i++) {
            hash -= line[i - patternLength] * patternHashPotency;
            hash = hash * HASH_BASE + line[i];

            if (hash == patternHash && bruteForceAssertOccurrence(line, pattern, i - patternLength + 1)) {
                occurrences += 1;
                if (!count) return occurrences;
            }
        }
        return occurrences;
    }

    bool bruteForceAssertOccurrence(const string &line, const string &pattern, int startPos) {
        int patternLength = (int) pattern.length();
        int j = 0;
        while (j < patternLength && line[startPos + j] == pattern[j]) {
            j++;
        }

        return j == patternLength;
    }

    unsigned long long calculatePatternHash(const string &pattern) {
        int patternLength = (int) pattern.length();
        unsigned long long hash = 0;

        for (int i = 0; i < patternLength; i++) {
            hash *= hash * HASH_BASE + pattern[i];
        }

        return hash;
    }

    unsigned long long calculatePatternHashPotency(const string &pattern) {
        int patternLength = (int) pattern.length();
        unsigned long long hash = 1;

        for (int i = 0; i < patternLength - 1; i++) {
            hash *= HASH_BASE;
        }

        return hash;
    }

};