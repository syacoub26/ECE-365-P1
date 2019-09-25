#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>

#include "hash.h"

using namespace std;

void lowerCase(string &s) {
	for(int i = 0; i < s.length(); i++){
		if( s.at(i) >= 'A' && s.at(i) <= 'Z'){
			s.at(i) += 32;
		}
	}
}

bool validChar(char &c) {
	if(c >= 'A' && c <= 'Z') {
		c += 32;
	}

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
	clock_t t1 = clock();
	hashTable dictWords;
	string word;
	getline(dict, word);
	while(!dict.eof()) {
		if(word.length() <= 20) {
			lowerCase(word);
			int i = dictWords.insert(word);
			//cout << i << ": " << word << endl;
		}
		getline(dict, word);
	}
	clock_t t2 = clock();
	double timeDiff = ((double)(t2-t1))/CLOCKS_PER_SEC;
	cout << "Total time (in seconds) to load dictionary: " << timeDiff << "\n";

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
	clock_t t3 = clock();
	string line;
	getline(input, line);
	int curline = 1;

	while(!input.eof()) {
		
		//processing word by word
		bool longword = false;
		string w = "";
		int l = 0;
		for(int i = 0; i < line.length(); i++) {
			char j = line.at(i);
			if(validChar(j)) {
				l++;
				if(l <= 20) {
					longword = false;
					w += j;
				}else{
					longword = true;
				}
			}else{
				l = 0;
				lowerCase(w);
				if(longword) {
					output << "Long word at line " << curline << ", starts: " << w << "\n";
				}else if(!dictWords.contains(w) && w.length() > 0) {
					output << "Unkown word at line " << curline << ": " << w << "\n";
				}
				w = "";
			}
		}

		lowerCase(w);
		if(longword){
			output << "Long word at line "  << curline << ", starts: " << w << "\n";
		}else if(!dictWords.contains(w) && w.length() > 0) {
			output << "Unkown word at line " << curline << ": " << w << "\n";
		}

		getline(input, line);
		curline++;
	}

	output.close();

	clock_t t4 = clock();
	timeDiff = ((double)(t4-t3))/CLOCKS_PER_SEC;
	cout << "Total time (in seconds) to check document: " << timeDiff;


	return 0;
}
