
#include <stdio.h>
#include <stdlib.h>
//#include <ctype.h>
#include <string.h>
#include <string>
#include <fstream>

typedef struct NODE node;

using namespace std;

typedef struct NODE{
	string val;
	int count;
	int nCount;
	node** nextLevelNodes;
	node* nextNode;
}node;

int  main(int argc, char* argv[]){

	
	//char* aNumber = (char*)malloc(sizeof(char*));
	//free(aNumber);

	//node* newNode = (node*)malloc(sizeof(node));
	node* newNode = new node;
	//node* newNodeCopy =  newNode;
	newNode->val = 'a';
	newNode->count = 1;
	//free(newNodeCopy -> val)
	free(newNode);

}
