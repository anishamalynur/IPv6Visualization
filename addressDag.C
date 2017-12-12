#include <iostream>
#include <algorithm>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <string>
#include <fstream>
//#include <json/value.h>
#define TOTAL_LEVELS 32

using namespace std;

typedef struct NODE node;

typedef struct DAG{
	node* levels[TOTAL_LEVELS];

}aDag;

typedef struct NODE{
	string val;
	int count;
	int nCount;
	node** nextLevelNodes;
	node* nextNode;
}node;

int charToIndex(string ch){

	char charChar = tolower((char)ch[0]); 
	int intChar;

	if(isdigit(charChar)){
		intChar = charChar - '0';
	}
	else{
	intChar = (int)charChar - 97 + 10;
	}

	return intChar;
}


//curNode is the node pointed to by a level# in DAG
node* addNodetoDag(aDag* dag, string aChar, node* curNode, node* prevNode, int level){
	
	node* lastNodeAtLevel = NULL;
	bool flag = true;

	//check if aChar lives on the current level
	while(flag && curNode != NULL){
		// no
		if(aChar.compare(curNode->val) != 0){
			cout<< "character did not match" << endl;
			if(curNode -> nextNode == NULL){
				lastNodeAtLevel = curNode;
			}
			curNode = curNode -> nextNode;		
		}
		// yes - increment count
		else if(aChar.compare(curNode->val) == 0){
			cout<< "character match!!" << endl;

			flag = false;
			curNode->count += 1;
			cout << "current count for this node is " << curNode->count << endl;

			// now check if a connection to the previous level node exists
			if(prevNode != NULL){
				int index = charToIndex(aChar);
				// if it exists then done
				if(prevNode->nextLevelNodes[index] != NULL){
					cout << "connection already existed "<< endl;
					prevNode->nextLevelNodes[index] -> nCount++;
					//cout << "ncount is " << prevNode->nextLevelNodes[index] -> nCount << "for the prev node" << prevNode -> val << "for the node pointer in nextLevelNodes" << prevNode->nextLevelNodes[index] ->val << endl;
					return curNode;	
				}
				
				// add connection 
				if((prevNode-> nextLevelNodes)[index] ==  NULL){
					cout << "adding connection" << endl;
					node* copyNode  =  (node*)malloc(sizeof(node));  
					//memcpy(&copyNode, &curNode, sizeof(node));
					copyNode -> val = aChar;
					copyNode -> nCount = 0;
					//((prevNode-> nextLevelNodes)[index]) = curNode;
					((prevNode-> nextLevelNodes)[index]) = copyNode;
					prevNode->nextLevelNodes[index] -> nCount++;
	//cout << "ncount is " << prevNode->nextLevelNodes[index] -> nCount << "for the prev node" << prevNode -> val << "for the node pointer in nextLevelNodes" << prevNode->nextLevelNodes[index] ->val << endl;
					return curNode;	
				}

			}
			else{
				return curNode;
			}
		} 
	}
	
	// add a new node at level# because it did not exist
	node* newNode = (node*)malloc(sizeof(node));
	newNode->val = aChar;
	newNode->count = 1;
	node** nextLNodes = (node**)malloc(sizeof(node*)*36);
	for(int i  = 0; i < 36; i++){
		nextLNodes[i] = NULL;
	}
	newNode->nextLevelNodes = nextLNodes;
	newNode->nextNode = NULL;

	// if its the first node at this level	
	if(lastNodeAtLevel == NULL){
		dag->levels[level] = newNode;
	}
	// else add the node to the end of the node chain at that level
	else{
		lastNodeAtLevel->nextNode = newNode;
	}
	curNode = newNode;
	cout << "done creating new node because it did not exist in the level" << endl;
	
	//add connection if not the first level
	if(prevNode !=NULL){
		int index = charToIndex(aChar);
		/*cout << "adding connection because not the first level" << endl;
		(prevNode-> nextLevelNodes)[index] = curNode;*/
		
					cout << "adding connection2" << endl;
					node* copyNode =  (node*)malloc(sizeof(node)); 
					//memcpy(&copyNode, &curNode, sizeof(node));
					copyNode -> nCount = 0;
					copyNode -> val = aChar;
					//copyNode -> val = 
					//((prevNode-> nextLevelNodes)[index]) = curNode;
					((prevNode-> nextLevelNodes)[index]) = copyNode;
					
					prevNode->nextLevelNodes[index] -> nCount++;
					//cout << "ncount is " << prevNode->nextLevelNodes[index] -> nCount << "for the prev node" << prevNode -> val << "for the node pointer in nextLevelNodes" << prevNode->nextLevelNodes[index] ->val << endl;

	}
	return newNode;
}


void stringReplacer(string& address, const string& find, const string& replace){
	int pos = 0;
	while((pos = address.find(find, pos)) != std::string::npos){
		address.replace(pos, find.length(), replace);
		pos += replace.length();
	}

}

void addAddress(string ad, aDag* theDag){


	node* previousNode = NULL;

	for(int i= 0; i < ad.length(); i++){
		string c(1, ad[i]);
		cout << endl << "the letter is: "<< c << endl;
		previousNode = addNodetoDag(theDag, c, theDag->levels[i], previousNode, i);
	}

}

void dataToCsv(aDag* theDag){
   ofstream myfile;
   myfile.open ("dagInfo4.csv");
	int curCount = 0;
	
   int totCount = 0;
	for(int i = 0; i < TOTAL_LEVELS; i++){
		
		node* curNode = theDag->levels[i];
		node* nextNode = NULL;
		
		while(curNode != NULL){
			myfile << std::to_string(i) + ",";
			myfile << curNode->val + ",";
			myfile << std::to_string(curNode-> count) + ",";
			myfile << std::to_string(totCount) + ",";
			totCount++;

			for(int j = 0; j < 36; j++){
				if(curNode -> nextLevelNodes[j] !=NULL){
				myfile << curNode -> nextLevelNodes[j] -> val + ",";
				curCount =  curNode -> nextLevelNodes[j] -> nCount;
				myfile << std::to_string(curCount) + ","; }
			}
			myfile << "\n";
			nextNode = curNode -> nextNode;
	
			curNode = nextNode;
		}
	
	}

	myfile << "9999999999999\n";
	myfile << "9999999999999\n";
	
}

void cleanup(aDag* theDag){
	for(int i = 0; i < TOTAL_LEVELS; i++){
		node* curNode = theDag->levels[i];
		node* nextNode = NULL;
		
		while(curNode != NULL){
			nextNode = curNode -> nextNode;
			free(curNode -> nextLevelNodes);
			free(curNode);
			curNode = nextNode;
		}
	}
	free(theDag);	
}




int  main(int argc, char* argv[]){

	//initialize the dag
	aDag* theDag = (aDag*)malloc(sizeof(aDag));

	if(theDag != NULL){
		for(int i =0; i < TOTAL_LEVELS; i++){
		theDag->levels[i] = NULL;
		}
	}
	else{
		printf("error in creating the dag");
	}			

	// open IPdata file and read and add each address	
	ifstream infile;
	//argv[1] should be the file to be opened
	infile.open(argv[1]);
	string sLine;

	while (!infile.eof()){
		infile >> sLine;
		string ad = sLine.data();
		stringReplacer(ad, "::", "0000");
		stringReplacer(ad, ":", "");
		cout << ad << endl;
    		addAddress(ad, theDag);
	}

	infile.close();
	
	//testing simple cases
	/*string ad = "aaa";
	string ab = "abd";
	string aa = "aaa";
	string ac = "efd";*/

	/*string ad = "20";
	string ab = "20";
	string aa = "20";
	string ac = "30";

	addAddress(ad, theDag);
	addAddress(ab, theDag);
	addAddress(aa, theDag);
	addAddress(ac, theDag);
	//addAddress(ad, theDag);*/

	dataToCsv(theDag);

	cleanup(theDag);
	
	cout << "end of program " << endl;
}


