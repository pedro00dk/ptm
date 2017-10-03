#include <vector>
#include <iostream>
#include <fstream> 
#include <queue>

using namespace std;

#define N 1010
#define ASCII 127
int p;
char text[N];
vector<string> patterns;
int textLen, patternLen;

#define dbg(x) cerr << fixed << #x " == " << x << endl

char filepath[1010];
int goTrie[N][ASCII];
int trieFail[N];
int trieOccurrences[N][10];
int trieOccurrencesCnt[N];

class Aho {

public:
    void searchPatternsFstream(const string &textName, ifstream &text, bool count, bool print) {
    	buildGoto();
    	buildFail();
        int occurrences = 0; // TODO: acumula de diferentes patterns?
        string line;
        while (getline(text, line)) {
            bool alreadyPrint = false;
            int lineOccurences = ahoCorasick(line);
            occurrences += lineOccurences;
            if (print && lineOccurences) cout << line << "\n";
        }
        if (count) printf("%s: %d\n", textName.c_str(), occurrences);
    }
private:
    void buildGoto() {
        int triePos, nextTriePos, patternPos;
        char currentChar;
        string pattern;
        nextTriePos = 0;
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
                patternPos++;
                currentChar = pattern[patternPos];
            }
    
            trieOccurrences[triePos][trieOccurrencesCnt[triePos]++] = i;
        }
    }
    
    void buildFail() {
        queue<int> q;
        int triePos, nextTriePos, border;
    
        for (char ascii = 0; ascii < ASCII; ascii++) {
            if (goTrie[0][ascii]) {
                q.push(goTrie[0][ascii]);
                trieFail[goTrie[0][ascii]] = 0;
            }
        }
    
        while (q.size()) {
            triePos = q.front();
            q.pop();
    
            for (char ascii = 0; ascii < ASCII; ascii++) {
                nextTriePos = goTrie[triePos][ascii];
                if (nextTriePos) {
                    q.push(nextTriePos);
                    
                    border = trieFail[triePos];
                    while (border && goTrie[border][ascii] == 0) {
                        border = trieFail[border];
                    }
                    trieFail[nextTriePos] = goTrie[border][ascii];
    
                    for (int i = 0; i < trieOccurrencesCnt[trieFail[nextTriePos]]; i++) {
                        trieOccurrences[nextTriePos][trieOccurrencesCnt[nextTriePos]++] = trieOccurrences[trieFail[nextTriePos]][i];
                    }
                }
            }
        }
    }

    int ahoCorasick(const string &text) {
        int occurrences = 0;
        int triePos = 0;
        char textChar;
        textLen = text.size();
        for (int textPos = 0; textPos < textLen; textPos++) {
            textChar = text[textPos];
            while (triePos && goTrie[triePos][textChar] == 0) {
                triePos = trieFail[triePos];
            }
            triePos = goTrie[triePos][textChar];
    
            for (int occ = 0; occ < trieOccurrencesCnt[triePos]; occ++) {
                int patternNumber = trieOccurrences[triePos][occ];
                //printf("    found %s at pos %d\n", patterns[patternNumber], textPos - (patterns[patternNumber]).size() + 1);
                occurrences++;
            }
        }
        return occurrences;
    }
};

int main() {
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

    Aho aho;
    aho.searchPatternsFstream(filepath, inputFileStream, count, print);

    return 0;
}