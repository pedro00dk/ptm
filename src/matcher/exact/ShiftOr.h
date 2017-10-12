#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>
#include "../PatternMatcher.h"

using namespace std;

class BitArray {
    int arraySize;
    unsigned long long positiveThreshold;
    unsigned long long lastElementMask;
    unsigned long long *bits;

    const unsigned int UNIT_SIZE = sizeof(unsigned long long) * 8;
    const unsigned long long LAST_BIT = (unsigned long long) 1L << (UNIT_SIZE - 1L);


public:
    explicit BitArray(int bitCount) {
        int bitCountMod = bitCount % UNIT_SIZE;
        arraySize = bitCountMod == 0 ? bitCount / UNIT_SIZE : bitCount / UNIT_SIZE + 1L;
        positiveThreshold = bitCountMod == 0 ? LAST_BIT - 1L : ((unsigned long long) 1 << (bitCountMod - 1L)) - 1L;
        lastElementMask = (positiveThreshold << 1L) + 1L;
        bits = new unsigned long long[arraySize];
        memset(bits, -1L, sizeof(unsigned long long) * arraySize);
    }

    void setAll() {
        memset(bits, -1L, sizeof(unsigned long long) * arraySize);
        bits[0] &= lastElementMask;
    }

    void shiftLeft() {
        bool isSetPreviousLastBit = false;
        for (int i = arraySize - 1; i >= 0; i -= 1) {
            bool isSetCurrentLastBit = (bits[i] & LAST_BIT) > 0;
            bits[i] = isSetPreviousLastBit ? (bits[i] << 1L) + 1L : bits[i] << 1L;
            isSetPreviousLastBit = isSetCurrentLastBit;
        }
        bits[0] &= lastElementMask;
    }

    void oR(const BitArray &bitArray) {
        for (int i = 0; i < arraySize; i += 1) bits[i] |= bitArray.bits[i];
        bits[0] &= lastElementMask;
    }

    void aND(const BitArray &bitArray) {
        for (int i = 0; i < arraySize; i += 1) bits[i] &= bitArray.bits[i];
    }

    void resetFirstBit() {
        bits[arraySize - 1] &= (unsigned long long) -2L;
    }

    void setFirstBit() {
        bits[arraySize - 1] |= (unsigned long long) 1L;
    }

    bool isPositive() {
        return bits[0] <= positiveThreshold;
    }
};

class ShiftOr : public PatternMatcher {

private:
    vector<string> patterns;
    vector<BitArray> bitArrays;
    vector<vector<BitArray>> charMasks;

public:
    ShiftOr() = default;

    void setPatterns(vector<string> patterns) override {
        this->patterns = patterns;
        bitArrays = vector<BitArray>();
        for (string &pattern : patterns) {
            bitArrays.push_back(BitArray((int) pattern.length()));
            charMasks.push_back(generateCharMasks(pattern));
        }
    }

    void searchPatterns(const string &textName, istream &text, bool count, bool print) override {
        int occurrences = 0;
        vector<BitArray> patternsBits = vector<BitArray>();
        for (string &pattern : patterns) patternsBits.push_back(BitArray((int) pattern.length()));
        for (string line; getline(text, line);) {
            int lineOccurrences = countLineOccurrences(line, patternsBits, count);
            occurrences += lineOccurrences;
            if (lineOccurrences > 0 && print) cout << line << endl;
            for (BitArray &patternBits : patternsBits) patternBits.setAll();
        }
        if (count) cout << textName << ": " << occurrences << endl;
    }

private:
    int countLineOccurrences(const string &line, const vector<BitArray> &patternsBits, bool count) {
        int occurrences = 0;
        int lineLength = (int) line.length();
        for (int i = 0; i < lineLength; i += 1) {
            for (int j = 0; j < patternsBits.size(); j += 1) {
                BitArray patternBits = patternsBits[j];
                patternBits.shiftLeft();
                patternBits.oR(charMasks[j][(unsigned char) line[i]]);
                if (patternBits.isPositive()) {
                    occurrences += 1;
                    if (!count) return occurrences;
                }
            }
        }
        return occurrences;
    }

    vector<BitArray> generateCharMasks(const string &pattern) {
        vector<BitArray> charMasks = vector<BitArray>();
        int patternLength = (int) pattern.length();
        for (int i = 0; i < ISO_SIZE; i += 1) charMasks.push_back(BitArray(patternLength));
        BitArray stamp = BitArray(patternLength);
        stamp.resetFirstBit();
        for (int j = 0; j < patternLength; j += 1) {
            charMasks[(unsigned char) pattern[j]].aND(stamp);
            stamp.shiftLeft();
            stamp.setFirstBit();
        }
        return charMasks;
    }
};
