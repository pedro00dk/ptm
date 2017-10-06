#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>
#include "ExactMatcher.h"

#define ISO_ENCODES_SIZE 256

using namespace std;

class BitArray {
    unsigned long long int *bits;
    unsigned long long int lastBitsMask;
    unsigned long long int lastBit = ((unsigned long long int) 1) << 63;
    unsigned long long int positiveThreshold{};
    int arraySize;


public:
    explicit BitArray(int bitCount) {
        auto mod = bitCount % 64;
        positiveThreshold = mod == 0 ? lastBit - 1 : (((unsigned long long int) 1) << (mod - 1)) - 1;
        arraySize = mod == 0 ? bitCount / 64 : bitCount / 64 + 1;
        bits = new unsigned long long int[arraySize];
        lastBitsMask = (positiveThreshold << 1) + 1;
        memset(bits, -1, sizeof(unsigned long long int) * arraySize);
    }

    void setAll() {
        memset(bits, -1, sizeof(int) * arraySize);
        bits[0] &= lastBitsMask;
    }

    void shiftLeft() {
        bool isSetPreviousLastBit = false;
        for (int i = arraySize - 1; i >= 0; i -= 1) {
            bool isSetCurrentLastBit = (bits[i] & lastBit) > 0;
            bits[i] = isSetPreviousLastBit ? (bits[i] << 1) + 1 : bits[i] << 1;
            isSetPreviousLastBit = isSetCurrentLastBit;
        }
        bits[0] &= lastBitsMask;
    }

    void oR(const BitArray &bitArray) {
        for (int i = 0; i < arraySize; ++i) {
            bits[i] |= bitArray.bits[i];
        }
        bits[0] &= lastBitsMask;
    }

    void aND(const BitArray &bitArray) {
        for (int i = 0; i < arraySize; ++i) {
            bits[i] &= bitArray.bits[i];
        }
    }

    void resetFirstBit() {
        bits[arraySize - 1] &= -2;
    }

    void setFirstBit() {
        bits[arraySize - 1] |= 1;
    }

    bool isPositive() {
        return bits[0] <= positiveThreshold;
    }
};

class SO : public ExactMatcher {

private:
    vector<string> patterns;
    vector<BitArray> bitArrays;
    vector<vector<BitArray>> charMasks;

public:
    SO() = default;

    void setPatterns(vector<string> patterns) override {
        this->patterns = patterns;
        bitArrays = vector<BitArray>();
        for (auto &pattern : patterns) {
            bitArrays.push_back(BitArray((int) pattern.length()));
            charMasks.push_back(generateCharMasks(pattern));
        }
    }

    void searchPatterns(const string &textName, istream &text, bool count, bool print) override {
        int occurrences = 0;
        bool lineAlreadyPrint = false;

        vector<BitArray> patternsBits = vector<BitArray>();
        for (int i = 0; i < patterns.size(); ++i) {
            BitArray patternBits = BitArray((int) patterns[i].length());
            patternsBits.push_back(patternBits);
        }
        for (string line; getline(text, line);) {
            auto lineLength = (int) line.length();
            lineAlreadyPrint = false;
            if (line.find("gentlewoman") != string::npos) {
                bool a = false;
            }
            for (int i = 0; i < lineLength; ++i) {
                for (int j = 0; j < patternsBits.size(); j++) {
                    patternsBits[j].shiftLeft();
                    patternsBits[j].oR(charMasks[j][(unsigned char) line[i]]);
                    if (patternsBits[j].isPositive()) {
                        occurrences += 1;
                        if (!lineAlreadyPrint && print) {
                            cout << line << endl;
                            lineAlreadyPrint = true;
                        }
                        if (!count) goto lineItrEnd;
                    }
                }
            }
            lineItrEnd:
            for (auto &patternBits :patternsBits) patternBits.setAll();
        }
        if (count) printf("%s: %d\n", textName.c_str(), occurrences);
    }

private:
    vector<BitArray> generateCharMasks(const string &pattern) {
        vector<BitArray> charMasks = vector<BitArray>();
        auto patternLength = (int) pattern.length();
        for (int i = 0; i < ISO_ENCODES_SIZE; ++i) {
            charMasks.push_back(BitArray(patternLength));
        }
        BitArray stamp = BitArray(patternLength);
        stamp.resetFirstBit(); //stamp.shiftLeft(); // stamp[0] = 0;
        for (int j = 0; j < patternLength; ++j) {
            charMasks[pattern[j]].aND(stamp);
            stamp.shiftLeft();
            stamp.setFirstBit();
        }
        return charMasks;
    }
};
