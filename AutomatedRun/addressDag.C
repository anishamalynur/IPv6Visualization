	
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <string>
#include <fstream>
#include <cstdlib>

#define TOTAL_LEVELS 32
#define POSSIBLE_VALUES 16
#define MAX_PATH 100

int totalCount = 0;

using namespace std;

////////////////////////////////////////  Data Structures ////////////////////////////////////////

typedef struct NODE node;

// Directed Acyclic Graph that holds 32 levels for IPv6
typedef struct DAG{
	node* levels[TOTAL_LEVELS];

}aDag;

// a level of the DAG is comprised of nodes
typedef struct NODE{
	string val;
	int count;
	int nCount; // only set if this is a node in nextLevelNode. Num of times this val has been encountered from the previous level.
	node** nextLevelNodes;
	node* nextNode;
}node;


////////////////////////////////////////  Helper Functions ////////////////////////////////////////

// convert a character to it's corresponding index for an array of size 16
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

// Reverse of charToIndex - index to corresponding character
string IndexToChar(int ind){
    string chars[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f"};
    return chars[ind];
}

// function that expands a compressed IP address
string IPify(string ad){
    int ipLen = ad.length(); int subCount = 0; int colonCount = 0; int i; int j;
    string addSub = ""; string ans = "";
    char prevCh;
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
            for(j = 0; j < (4- subCount); j++){
                ans.append("0");
            }
            ans.append(addSub);
            subCount = 0;
            addSub = "";
        }
        prevCh = ad[i];
    }
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


////////////////////////////////////////  DAG Creation ////////////////////////////////////////

//curNode is the node pointed to by a level# in DAG
node* addNodetoDag(aDag* dag, string aChar, node* curNode, node* prevNode, int level){
	
	node* lastNodeAtLevel = NULL;

	//check if aChar lives on the current level
	while(curNode != NULL){
		// no - character did not match
		if(aChar.compare(curNode->val) != 0){
            // get the next node in level if exists
			if(curNode -> nextNode == NULL){
                //reached last node in the level, keep track of it
				lastNodeAtLevel = curNode;
			}
			curNode = curNode -> nextNode;		
		}
		// yes found a match! - increment count
		else if(aChar.compare(curNode->val) == 0){
			curNode->count += 1;
			// now check if a connection to the previous level node exists
            // prevNode will be NULL if it is the first level
			if(prevNode != NULL){
				int index = charToIndex(aChar);
				// if connections exists - increment nCount
				if(prevNode->nextLevelNodes[index] != NULL){
					prevNode->nextLevelNodes[index] -> nCount++;
					return curNode;	
				}
				
				// No previous existing connection to previous level - add one
				 else{
					node* copyNode = new node();
					copyNode -> val = aChar;
					copyNode -> nCount = 0;
					((prevNode-> nextLevelNodes)[index]) = copyNode;
					prevNode->nextLevelNodes[index] -> nCount++;
					return curNode;
				}
            
			}
            // else no prevNode exists (this is the first level)
			else{
				return curNode;
			}
		} 
	}
	// Getting out of the while loop without returning means
    // we must add a new node at level# because it did not exist

	node* newNode = new node();
	newNode->val = aChar;
	newNode->count = 1;

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
    
	// done creating new node because it did not exist in the level
	// add connection to previous level if not the first level
	if(prevNode !=NULL){
		int index = charToIndex(aChar);
        node* copyNode = new node;
        copyNode -> nCount = 0;
        copyNode -> val = aChar;
        ((prevNode-> nextLevelNodes)[index]) = copyNode;
        prevNode->nextLevelNodes[index] -> nCount++;
	}
	return newNode;
}

////////////////////////////////////////  Ping Function ////////////////////////////////////////

// this function is responsible for using scamper's ping function to determine if the address is valid
int pingAdr(string adr){
	FILE *fp;
	int status;
	char path[MAX_PATH];
	int counter = 0;

	const char* ipAddress = adr.c_str();
	const char* pingString = "ping6 -c 3 ";
	string ipString = adr;
    
    // only storage for 256 characters - enough for 32 for ip and 11 for command
	char buffer[256];
	strncpy(buffer, pingString, sizeof(buffer) - 1);
	strncat(buffer, ipAddress,  sizeof(buffer) - strlen(buffer) - 1);

	fp = popen(buffer, "r");
    
    if (fp == NULL){
        cout << "error with popen";
    }
    
	while (fgets(path, MAX_PATH, fp) != NULL){
         counter += 1;
	}
    //  unsuccessful ping
	if(counter < 7){
        //cout << "Not Good!";
         pclose(fp);
        return 0;
	}

	else{
        //cout << "Valid!";
         pclose(fp);
        return 1;
	}

}

////////////////////////////////////////  Polya Urn Sampling ////////////////////////////////////////

string addColons(string adr){
    int count4 = 0;
    //string c;
    string newAdr = "";
    for(int i= 0; i < adr.length(); i++){
        count4++;
        string c(1, adr[i]);
        newAdr.append(c );
        if(count4 == 4 & i!= adr.length() -1){
            newAdr.append(":");
            count4 = 0;
        }
    }
    return newAdr;
}

void increaseDagNodeCount(int (&lVals)[TOTAL_LEVELS][POSSIBLE_VALUES], string adr){
    int charIndex;
    for(int i= 0; i < adr.length(); i++){
        string c(1, adr[i]);
        charIndex = charToIndex(c);
        lVals[i][charIndex] += 1;
    }
}

void polyaUrn(aDag* theDag, int numNewAddresses){
    

    // levelVals is a double array that has 32 sub arrays of size 16.
    //Each subarray's index is the total times the corresponding value has been encountered
    int levelVals[TOTAL_LEVELS][POSSIBLE_VALUES] = {{0}};
	for(int i = 0; i < TOTAL_LEVELS; i++){
		node* curNode = theDag->levels[i];
		node* nextNode = NULL;
		int j = 0;
		while(curNode != NULL){
			int innerInd = charToIndex(curNode->val);
			levelVals[i][innerInd] = curNode -> count;
			j++;
			curNode = curNode -> nextNode;
		}
	}

    //get random num for each level
    // repeat numNewAddresses times
    
    for(int n = 0; n < numNewAddresses; n++){
        int randomNum = 0;
        string newAd = "";

        for(int k = 0; k < TOTAL_LEVELS; k++){
            randomNum = (rand() % totalCount) + 1;
            //cout << randomNum << endl;
            int sumToRandomNum = 0;
            for(int j = 0; j < POSSIBLE_VALUES; j++){
                sumToRandomNum += levelVals[k][j];
                if(sumToRandomNum >= randomNum){
                    //levelVals[k][j] += 1;
                    //cout << IndexToChar(j) + " ";
                    newAd.append(IndexToChar(j));
                    break;

                }
            }
	}
        int success = pingAdr(addColons(newAd));
        if(success){
        cout<< "YAY";
        increaseDagNodeCount(levelVals, IPify(newAd));
        }
        else{
        cout << "Awh:(";
        }
        cout << newAd << endl;
    }
/*cout << '\n';
	cout << totalCount;
	srand(time(NULL));
	cout << rand() % totalCount + 1;
	cout << '\n';
	cout << "I got here";
	for(int a = 0; a < TOTAL_LEVELS; a++){
				        cout << endl;
		for(int b = 0; b < POSSIBLE_VALUES; b++){
				cout << levelVals[a][b];
				cout << " ";	
		}
	}*/
}	



////////////////////////////////////////  DAG info -> file ////////////////////////////////////////

// produces a file (specified by fileName) that contains [level, nodeVal, #of Nodes, node#, [next level nodes val, nCount]]
// example of first line: 0,2,1281,0,0,258,1,3,2,1,3,2,4,146,5,1,6,245,7,1,8,183,9,2,a,404,b,6,c,20,d,3,e,2,f,4,
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


// produces a file (specified by fileName) that contains
// [level number, number of unique nodes at a level, difference in nodes (smallest - largest), min next level connections, max next level connection,  average next level connections]
// Example: 4,16,15,1,16,8
void dataToGraphInfo(aDag* theDag, string fileName){
    ofstream myFile;
    myFile.open(fileName);
	for(int i = 0; i < TOTAL_LEVELS; i++){
		node* curNode = theDag->levels[i];
		node* nextNode = NULL;
		int levelCount = 0; // number of unique nodes
		int minVal = 999; // initializing minimum node value seen at a level
		int maxVal = -1; // initializing max node value seen at a level
		int nextLevel[POSSIBLE_VALUES] = {0}; // an array of 16 indicating how many times a particular node is encountered at the next level
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
        //after iterating through a particular level's nodes
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



//////////////////////////////////////// Main() and Cleanup ////////////////////////////////////////

// called to clear memory
void cleanup(aDag* theDag){
	for(int i = 0; i < TOTAL_LEVELS; i++){
		node* curNode = theDag->levels[i];
		node* nextNode = NULL;
		while(curNode != NULL){
			nextNode = curNode -> nextNode;
			for( int j = 0; j < POSSIBLE_VALUES; j ++){
				delete curNode -> nextLevelNodes[j];
			}
			delete [] curNode -> nextLevelNodes;
			delete(curNode);
			curNode = nextNode;
		}
	}
	delete theDag;	
}

// initialize a new DAG
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

// used in main to parse an a single address character by character and add it to the DAG
void addAddress(string ad, aDag* theDag){
    node* previousNode = NULL;
    for(int i= 0; i < ad.length(); i++){
        string c(1, ad[i]);
        previousNode = addNodetoDag(theDag, c, theDag->levels[i], previousNode, i);
    }
}

// this main function can be used when the file inputed is sorted by ASN number (in proper format * \n, ASN # \n,  each of the IP's \n)
/*int  main(int argc, char* argv[]){
    
    //initialize the dag
    aDag* theDag = NULL;
    string title;
    // open IPdata file and read and add each address
    ifstream infile;
    infile.open(argv[1]);
    
    srand(time(0)); // random seed
    string sLine;
    string ad;
    while (!infile.eof()){
        
        infile >> sLine;
        ad = sLine.data();
        cout << ad << endl;
        if(ad == "*"){
            if(theDag != NULL){
                dataToGraphInfo(theDag, title);
                polyaUrn(theDag, 2);
                cleanup(theDag);
                totalCount = 0;
            }
            theDag = initializeDag();
            infile >> sLine;
            cout << "starting with ASN " << sLine << endl;
            title =  sLine.data();
        }
        else{
            //cout << IPify(ad) << endl;
            totalCount ++;
            addAddress(IPify(ad), theDag);
        }
    }
    
    infile.close();
}*/

// use this main function when you have a list of IP addresses not sorted by ASN number

int  main(int argc, char* argv[]){
	//initialize the dag
    srand(time(0));
	aDag* theDag = new aDag;
    
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
	//argv[1] should be the file to be opened - file of IPv6 addresses
	infile.open(argv[1]);
	cout << argv[1];
	string sLine;
	while (!infile.eof()){
		totalCount ++;
		infile >> sLine;
		string ad = sLine.data();
		//cout << IPify(ad) << endl;
       //cout << addColons(IPify(ad)) << endl;
    	addAddress(IPify(ad), theDag);
	}

	infile.close();
	//dataToCsv(theDag, argv[2]); //argv[2] is the name of the output .csv file
	dataToGraphInfo(theDag, "newGraphInfo");
	dataToCsv(theDag, "sankeyGraph");
	polyaUrn(theDag, 10);
	cleanup(theDag);
}


