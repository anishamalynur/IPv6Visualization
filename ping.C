#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <fstream>

#define PATH_MAX 100

using namespace std;

int main(){
	
	FILE *fp;
	int status;
	char path[PATH_MAX];
	int counter = 0;
	const char* ipAddress = "192.186.1.1";
	const char* pingString = "ping -c 10 ";
	string ipString = ipAddress;
 
	char buffer[256]; // <- danger, only storage for 256 characters.
	strncpy(buffer, pingString, sizeof(buffer));
	strncat(buffer, ipAddress, sizeof(buffer));
	fprintf(stderr,"%s", buffer);

	fp = popen(buffer, "r");
	if (fp == NULL)
		 /* Handle error */;


	while (fgets(path, PATH_MAX, fp) != NULL){
		 printf("%s", path);
		 counter += 1;
	}

	if(counter < 10){
	printf("ERROR");

	ofstream badFile;
 
	badFile.open("badIP.txt",std::ios_base::app);

	badFile << ipString + "\n";

	badFile.close();

	}

	else{
	printf("GOOD");
	
	ofstream goodFile;
 
	goodFile.open("goodIP.txt",std::ios_base::app);

	goodFile << ipString + "\n";

	goodFile.close();


	
	}

	status = pclose(fp);
	if (status == -1) {
		 /* Error reported by pclose() */
		
	} else {
		 /* Use macros described under wait() to inspect `status' in order
		    to determine success/failure of command executed by popen() */
		
	}
}
