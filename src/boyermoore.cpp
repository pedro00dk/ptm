#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#define N 10100
#define ASCII 127
vector<string> patterns;
int badChar[ASCII], goodSuffix[N];
int textLen, patternLen;

class BoyerMoore {
private:
    int ** badChar;
    int ** goodSuffix;

public:
    void searchPatternsFstream(const string &textName, ifstream &text, bool count, bool print) {
        badChar = (int**) malloc(patterns.size() * sizeof(int*));
        goodSuffix = (int**) malloc(patterns.size() * sizeof(int*));
        for (int patternNumber = 0; patternNumber < patterns.size(); patternNumber++) {
            buildBadChar(patternNumber);
            buildGoodSuffix(patternNumber);
        }

        int occurrences = 0; // TODO: acumula de diferentes patterns?
        string line;
        while (getline(text, line)) {
            bool alreadyPrint = false;
            int lineOccurences = 0;

            for (int patternNumber = 0; patternNumber < patterns.size(); patternNumber++) {
                lineOccurences += boyermoore(patternNumber, line);
                occurrences += lineOccurences;
            }
            
            if (print && lineOccurences) cout << line << "\n";
        }
        if (count) printf("%s: %d\n", textName.c_str(), occurrences);

        free(badChar);
        free(goodSuffix);
    }

private:

    void buildBadChar(const int &patternNumber){
        string pattern = patterns[patternNumber];
        patternLen = pattern.size();

        char patternChar;

        badChar[patternNumber] = (int*) malloc(ASCII * sizeof(int));
        memset(badChar[patternNumber], -1, sizeof(int) * ASCII);

        for (int patternPos = 0; patternPos < patternLen; patternPos++) {
            patternChar = pattern[patternPos];
            badChar[patternNumber][patternChar] = patternPos;
        }
    }

    void buildGoodSuffix(const int &patternNumber){
        string pattern = patterns[patternNumber];
        patternLen = pattern.size();

        goodSuffix[patternNumber] = (int*) malloc(patternLen * sizeof(int));

        for (int i = 0; i < patternLen; i++) goodSuffix[patternNumber][i] = 1;
    }

    int boyermoore(const int &patternNumber, const string &text){
        int occurrences = 0;
        int textPos = 0;
        string pattern = patterns[patternNumber];
        patternLen = pattern.size();
        textLen = text.size();

        while (textPos <= textLen - patternLen){
            int patternPos = patternLen - 1;
            while (patternPos >= 0 && text[textPos+patternPos] == pattern[patternPos]) patternPos--;
            
            if (patternPos == -1) {
                //printf("Found match at %d\n", textPos);
                textPos += goodSuffix[patternNumber][0];
                occurrences++;
            } else {
                char textChar = text[textPos + patternPos];
                //printf("Missmatch at %d: badchar[%c][%d]=%d\n", textPos + patternPos, textChar, patternPos, badChar[patternNumber][textChar]);
                textPos += max(patternPos - badChar[patternNumber][textChar], goodSuffix[patternNumber][patternPos]);
            }
        }

        return occurrences;
    }
};

int main(){
    int p;
    scanf("%d", &p);
    for (int i = 0; i < p; i++) {
        string pattern;
        cin >> pattern;
        patterns.push_back(pattern);
    }

    string filepath;
    cin >> filepath;

    ifstream inputFileStream (filepath);

    bool count = true, print = true;

    BoyerMoore boyermoore;
    boyermoore.searchPatternsFstream(filepath, inputFileStream, count, print);

    return 0;
}