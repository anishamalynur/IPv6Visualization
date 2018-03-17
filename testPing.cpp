//
//  testPing.cpp
//  
//
//  Created by Anisha Malynur on 3/15/18.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <string>
#include <fstream>
#include <cstdlib>
//#include <json/value.h>
#define TOTAL_LEVELS 32
#define POSSIBLE_VALUES 16
using namespace std;

#define PATH_MAX 100

int correctCount =0;
int wrongCount = 0;
int pingAdr(string adr){
    FILE *fp;
    int status;
    char path[PATH_MAX];
    int counter = 0;
    
    
    const char* ipAddress = adr.c_str();
    const char* pingString = "ping6 -c 1 "; //CHANGE BACK TO 3
    string ipString = adr;
    
    char buffer[256]; // <- danger, only storage for 256 characters.
    strncpy(buffer, pingString, sizeof(buffer)/2);
    strncat(buffer, ipAddress, sizeof(buffer)/2);
    //fprintf(stderr,"%s", buffer);
    
    fp = popen(buffer, "r");
    if (fp == NULL)
    /* Handle error */;
    
    
    while (fgets(path, PATH_MAX, fp) != NULL){
        //cout << path;
        counter += 1;
    }
    
    if(counter < 7){
        cout << "BOO";
        wrongCount += 1;
        pclose(fp);
        //delete buffer;
        return 0;
        
    }
    
    else{
        cout << "YAY";
        correctCount +=1;
        pclose(fp);
        //delete buffer;
        return 1;
        
        
    }
    pclose(fp);
    delete[] buffer;
    
}




int  main(int argc, char* argv[]){

    ifstream infile;
    //argv[1] should be the file to be opened
    //string openString = "dataSets/";
    //openString.append(argv[1]); //argv[1] is the name of the raw IPv6 file
    //cout << openString;
    infile.open(argv[1]);
    string sLine;
    
    while (!infile.eof()){
 
        infile >> sLine;
        string ad = sLine.data();
        //stringReplacer(ad, "::", "0000");
        //stringReplacer(ad, ":", "");
        
        cout<<ad;
        pingAdr(ad);
        //pingAdr(IPify(ad));
   
    }
    
    infile.close();
    
    cout << "correct: " << correctCount <<"\n";
    cout << "wront: " << wrongCount <<"\n";

}
