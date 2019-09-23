#include <string>
#include <vector>

using namespace std;

#include "hash.h"

int primes[] = {12289, 24593, 49157, 98317, 196613, 393241, 786433, 1572869, 3145739, 6291469, 12582917,
	25165843, 50331653, 100663319, 201326611, 402653189, 805306457, 1610612741};


hashTable::hashTable(int size) {
	filled = 0;
	capacity = getPrime(size*2);
	data.resize(capacity);
}

int hashTable::insert(const string &key, void *b) {
	/*if( filled*2 >= capacity ){
		rehash();
		if( filled*2 >= capacity ) { return 2; }
	}*/

	if( findPos(key) > 0) {
		return 1;
	}else{
		int curPos = hash(key);
		while( data[curPos].isOccupied ) {
			curPos++;
			if(curPos >= capacity){
				curPos = 0;
			}
		}
		data[curPos].key = key;
		data[curPos].isOccupied = true;
		data[curPos].isDeleted = false;
		data[curPos].pv = b;
		return 0;
	}
}

bool hashTable::contains(const string &key) {
	int i = findPos(key);
	if ( i = -1 ) { 
		return false; 
	}else{
		return !data[ findPos(key) ].isDeleted;
	}
}

void *hashTable::getPointer(const string &key, bool *b) {}

int hashTable::setPointer(const string &key, void *pv) {
	int i = findPos(key);
	if( i < 0 ){
		return 1;
	}else{
		data[i].pv = pv;
		return 0;
	}
}

bool hashTable::remove(const string &key) {
	int i = findPos(key);
	if( i < 0 ) {
		return false;
	}else{
		data[i].isDeleted = true;
		return true;
	}

	filled--;
}

int hashTable::hash(const string &key) {
	int hashVal = 0;

	for(int i = 0; i < key.length(); i++){
		hashVal = 37*hashVal + key[i];
	}

	hashVal %= capacity;

	if(hashVal < 0){
		hashVal += capacity;
	}

	return hashVal;
}

int hashTable::findPos(const string &key) {
	int inPos = hash(key);
	int curPos = inPos;

	while( data[ curPos ].isOccupied ){
		if( data[ curPos ].key != key ) {
			curPos++;
			if( curPos = capacity ) { curPos = 0; }
		}else{
			return curPos;
		}
	}

	return -1;
}

bool hashTable::rehash() {
	vector<hashItem> prevData = data;
	data.resize( getPrime( 2*capacity) );
	capacity = data.size();

	int j = 0;
	for( int i = 0; i < prevData.size(); i++){
		if( prevData[i].isOccupied && !prevData[i].isDeleted ) {
			data[j++] = prevData[i];
		}else if( !prevData[i].isOccupied ) {
			j++;
		}
	}

	for( j = prevData.size(); j < capacity; j++){
		data[j].key = "";
		data[j].isOccupied = false;
		data[j].isDeleted = false;
		data[j].pv = nullptr;
	}
}

unsigned int hashTable::getPrime(int size) {
	for(int i = 0; i < 18; i++){
		if( primes[i] > size){
			return primes[i];
		}
	}

	return size*2 + 1;
}
