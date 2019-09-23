#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

#include "hash.h"

void lowerCase(string &s) {
	for(int i = 0; i < s.length(); i++){
		if( s.at(i) >= 'A' && s.at(i) <= 'Z'){
			s.at(i) += 32;
		}
	}
}

bool validChar(const char &c) {
	return (c == 39) ||
		(c == '-') ||
		(c >= '0' && c <= '9') ||
		(c >= 'a' && c <= 'z' );
}


int main() {

	//dictionary file
	string dictFile;
	cout << "Enter name of dictionary: ";
	getline(cin, dictFile);
	ifstream dict(dictFile);

	//loading dictionary
	hashTable dictWords();
	string word;
	getline(dict, word);
	while(!word.empty()) {
		if(word.length() <= 20) {
			lowerCase(word);
			int i = dictWords.insert(word);
		}
		getline(dict, word);
	}

	//input file
	string inFile;
	cout << "Enter name of input file: ";
	getline(cin, inFile);
	ifstream input(inFile);

	//output file
	string outFile;
	cout << "Enter name of output file: ";
	getline(cin, outFile);
	ofstream output(outFile);

	//processing lines
	string line;
	getline(input, line);
	int curline = 1;
	while(!line.empty()) {
		
		//processing word by word
		bool longword = false;
		string w = "";
		int l = 0;
		for(int i = 0; i < line.length(); i++) {
			char j = line.at(i);
			if(validChar(j)) {
				l++;
				if(l <= 20) {
					w += j;
				}else{
					longword = true;
				}
			}else{
				l = 0;
				lowerCase(w);
				if(longword) {
					output << "Long word at line " << curline << ", starts : " << w << "\n";
				}else if(!dictWords.contains(w) && w.length() > 0) {
					output << "Unkown word at line " << curline << ": " << w << "\n";
				}
				w = "";
			}
		}

		getline(input, line);
		curline++;
	}

	output.close();
	return 0;
}
