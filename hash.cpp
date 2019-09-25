#include <iostream>
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
	if( filled*2 >= capacity ){
		rehash();
		if( filled*2 >= capacity ) { return 2; }
	}

	if( findPos(key) >= 0) {
		return 1;
	}else{
		int pos = hash(key);
		while( data[pos].isOccupied ) {
			pos++;
			if(pos == capacity){
				pos = 0;
			}
		}
		data[pos].key = key;
		data[pos].isOccupied = true;
		data[pos].pv = b;
		filled++;
		return 0;
	}
}

bool hashTable::contains(const string &key) {
	int i = findPos(key);
	if ( i < 0 ) { 
		return false; 
	}else{
		return !data[i].isDeleted;
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
		filled--;
		return true;
	}
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
	int pos = hash(key);

	while( data[ pos ].isOccupied ){
		if( data[ pos ].key == key ) {
			return pos;
		}else{
			pos++;
			if( pos == capacity ) { pos = 0; }
		}
	}

	return -1;
}

bool hashTable::rehash() {
	//cout << "started rehashing" << endl;
	vector<hashItem> newData;
	newData.resize( getPrime(2*capacity) );
	capacity = newData.size();

	for(auto i: data) {
		string newKey = i.key;
		if (i.isOccupied && !i.isDeleted) {
			int pos = hash( newKey );
			while( newData[pos].isOccupied ) {
				pos++;
				if (pos == capacity) {
					pos = 0;
				}
			}
			newData[pos].key = newKey;
			newData[pos].isOccupied = true;
		}
	}

	data = move(newData);

}

unsigned int hashTable::getPrime(int size) {
	for(int i = 0; i < 18; i++){
		if( primes[i] > size){
			return primes[i];
		}
	}

	return size + 1;
}
