#include "profile.h"

#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <set>

using namespace std;

class Learner {
private:
    set<string> new_words;
public:
	int Learn(const vector<string>& words) {
		for (const auto& item : words) {
			if (new_words.find(item) == new_words.end())
				new_words.insert(item);
		}
		return new_words.size();
	}

    vector<string> KnownWords() const {
        return {begin(new_words), end(new_words)};
    }
};

int main() {
    LOG_DURATION("Total");

    Learner learner;
    string line;
    while (getline(cin, line)) {
        vector<string> words;
        stringstream ss(line);
        string word;
        while (ss >> word) {
            words.push_back(word);
        }
        LOG_DURATION("Learn");
        cout << learner.Learn(words) << "\n";
    }
    cout << "=== known words ===\n";
    for (auto word : learner.KnownWords()) {
        LOG_DURATION("Known words");
        cout << word << "\n";
    }
}