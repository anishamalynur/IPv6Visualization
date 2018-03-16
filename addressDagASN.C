	
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <string>
#include <fstream>
//#include <json/value.h>
#define TOTAL_LEVELS 32
#define POSSIBLE_VALUES 16

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
			//cout<< "character did not match" << endl;
			if(curNode -> nextNode == NULL){
				lastNodeAtLevel = curNode;
			}
			curNode = curNode -> nextNode;		
		}
		// yes - increment count
		else if(aChar.compare(curNode->val) == 0){
			//cout<< "character match!!" << endl;

			flag = false;
			curNode->count += 1;
			//cout << "current count for this node is " << curNode->count << endl;

			// now check if a connection to the previous level node exists
			if(prevNode != NULL){
				int index = charToIndex(aChar);
				// if it exists then done
				if(prevNode->nextLevelNodes[index] != NULL){
					//cout << "connection already existed "<< endl;
					prevNode->nextLevelNodes[index] -> nCount++;
					//cout << "ncount is " << prevNode->nextLevelNodes[index] -> nCount << "for the prev node" << prevNode -> val << "for the node pointer in nextLevelNodes" << prevNode->nextLevelNodes[index] ->val << endl;
					return curNode;	
				}
				
				// add connection 
				//if((prevNode-> nextLevelNodes)[index] ==  NULL){
				 else{
					//cout << "adding connection" << endl;
					//node* copyNode  =  (node*)malloc(sizeof(node));
					node* copyNode = new node();  
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
	
	//node* newNode = (node*)malloc(sizeof(node));
	node* newNode = new node();
	newNode->val = aChar;
	newNode->count = 1;
	//node** nextLNodes = (node**)malloc(sizeof(node*)*POSSIBLE_VALUES);
	node** nextLNodes = new node*[POSSIBLE_VALUES];
	for(int i  = 0; i < POSSIBLE_VALUES; i++){
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
	curNode = newNode; // do I need this??
	//cout << "done creating new node because it did not exist in the level" << endl;
	
	//add connection if not the first level
	if(prevNode !=NULL){
		int index = charToIndex(aChar);
		/*cout << "adding connection because not the first level" << endl;
		(prevNode-> nextLevelNodes)[index] = curNode;*/
		
					//cout << "adding connection2" << endl;
					//node* copyNode =  (node*)malloc(sizeof(node));
				node* copyNode = new node;   
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


/*void stringReplacer(string& address, const string& find, const string& replace){
	int pos = 0;
	while((pos = address.find(find, pos)) != std::string::npos){
		address.replace(pos, find.length(), replace);
		pos += replace.length();
	}

}*/

void addAddress(string ad, aDag* theDag){


	node* previousNode = NULL;
	//make
//string c = "";
	for(int i= 0; i < ad.length(); i++){
		string c(1, ad[i]);
		//char ch = ad.at(i);
		//c.push_back(ch);
		//cout << endl << "the letter is: "<< c << endl;
		previousNode = addNodetoDag(theDag, c, theDag->levels[i], previousNode, i);
	}

}

void dataToCsv(aDag* theDag, string fileName){
   ofstream myfile;
   myfile.open(fileName);
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

			for(int j = 0; j < POSSIBLE_VALUES; j++){
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
	// signifies end of file
	myfile << "9999999999999\n";
	myfile << "9999999999999\n";
	
}



void dataToGraphInfo(aDag* theDag, string fileName){
   ofstream myFile;
   myFile.open(fileName);
	int curCount = 0;
	
   int totCount = 0;
	for(int i = 0; i < TOTAL_LEVELS; i++){
		
		node* curNode = theDag->levels[i];
		node* nextNode = NULL;
		int levelCount = 0;
		int minVal = 999;
		int maxVal = -1;
		int nextLevel[POSSIBLE_VALUES] = {0};

		while(curNode != NULL){
		
			int nodeVal = charToIndex(curNode->val);
			if( nodeVal < minVal){
				minVal = nodeVal;	
			}
			if(nodeVal > maxVal){
				maxVal = nodeVal;
			}

			levelCount++;
			
			for(int j = 0; j < POSSIBLE_VALUES; j++){
	
				if(curNode -> nextLevelNodes[j] !=NULL){
				nextLevel[j] +=1;
				}
			}
		
			nextNode = curNode -> nextNode;
		
			curNode = nextNode;
		}
		int minNextVal = 999;
		int maxNextVal = -1;
		int nextLevelSum = 0;
		myFile << i << ",";
		myFile << levelCount << ",";
		myFile << (maxVal - minVal) << ",";
		for(int j = 0; j < POSSIBLE_VALUES; j++){
			
			nextLevelSum += nextLevel[j]; 
			if(nextLevel[j] < minNextVal){
				minNextVal = nextLevel[j];
			}
			if(nextLevel[j] > maxNextVal){
				maxNextVal = nextLevel[j];
			}
			
		}
		myFile << minNextVal << ",";
		myFile << maxNextVal << ",";
		myFile << nextLevelSum/POSSIBLE_VALUES;
		myFile << "\n";
	
	}	
}


string IPify(string ad){
	int ipLen = ad.length();
	int subCount = 0;
	int colonCount = 0;
	string addSub = "";
	string ans = "";
	char prevCh;
	int i;
	int j;
	for(i = 0; i < ipLen; i++){
		if(ad[i] == ':'){
			colonCount +=1;
		} 
	}
	for( i = 0; i < ipLen; i++){
		if(ad[i] == ':' && prevCh == ':'){
			for(j = 0; j < (8- colonCount); j++){
					ans.append("0000");
			}
			subCount = 0;
			continue;
		}
		else if( ad[i] != ':'){
			addSub.append(&ad[i], 1);
			subCount += 1;
		}
		else if(ad[i] == ':'){
			for(j = 0; j <	 (4- subCount); j++){
				ans.append("0");
			}
			ans.append(addSub);
			subCount = 0;
			addSub = "";
		}
		prevCh = ad[i];
	}
	//cout << subCount << endl;
	for(j = 0; j < (4- subCount); j++){
				ans.append("0");
	}
	ans.append(addSub);
	int curLen = ans.length();
	for(i = 0; i < (32- curLen); i ++){
		ans.append("0");
	}
	return ans;
}
//free memory
void cleanup(aDag* theDag){
	for(int i = 0; i < TOTAL_LEVELS; i++){
		node* curNode = theDag->levels[i];
		node* nextNode = NULL;
		
		while(curNode != NULL){
			nextNode = curNode -> nextNode;
			//free(curNode -> nextLevelNodes);
			//delete [] curNode -> nextLevelNodes;
			for( int j = 0; j < POSSIBLE_VALUES; j ++){
				delete curNode -> nextLevelNodes[j];
			}
			delete [] curNode -> nextLevelNodes;
			//free(curNode);
			delete(curNode);
			curNode = nextNode;
		}
	}
	//free(theDag);
	/*for( int j = 0; j < TOTAL_LEVELS; j ++){
		delete theDag -> levels[j];
	}*/
	delete theDag;	
}

aDag* initializeDag(){
	aDag* theDag = new aDag;

	if(theDag != NULL){
		for(int i =0; i < TOTAL_LEVELS; i++){
		theDag->levels[i] = NULL;
		}
	}
	else{
		printf("error in creating the dag");
	}	

	return theDag;
}




int  main(int argc, char* argv[]){

	//initialize the dag
	//aDag* theDag = (aDag*)malloc(sizeof(aDag));
	aDag* theDag = NULL; 
	string title;

	// open IPdata file and read and add each address	
	ifstream infile;

	infile.open(argv[1]);
	string sLine;
	string ad;
	while (!infile.eof()){
		
		infile >> sLine;
		ad = sLine.data();
		cout << ad << endl;

		if(ad == "*"){
			
			if(theDag != NULL){
				dataToGraphInfo(theDag, title);
				cleanup(theDag);
			}
			theDag = initializeDag();
			infile >> sLine;
			title =  sLine.data();
		}
		
		else{
    		addAddress(IPify(ad), theDag);
		}
	}

	infile.close();
	

	//dataToCsv(theDag, argv[2]); //argv[2] is the name of the output .csv file
	dataToGraphInfo(theDag, title);
	cleanup(theDag);
	
//	cout << "end of program " << endl;

}





