#include <iostream>
#include <algorithm>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <string>
#include <fstream>


#define PATH_MAX 100
/*2606:ae80:1430::1
2600:1:b090::1
2a02:dd80::1
2001:56a:7870::1*/

using namespace std;


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

string randomize(string ad){
	char acceptableVals[16] ={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
	int randomInt = rand() % 32;
	int randomInt2 = rand() % 16;
	
	ad[randomInt] = acceptableVals[randomInt2];
	return ad;	

}

int main(){
	/*string ad = "2001:56a:7870::1";
	string newAd = IPify(ad);
	cout << newAd << endl;
	string newAdRandom = randomize(newAd);
	cout << newAdRandom << endl;
	cout << newAd << endl;*/
	
	

	ifstream inFile;
	ofstream outFile;
   string ipAd;
   inFile.open("largePure.txt");
	outFile.open("ipNormalized.txt");
   if (!inFile) {
		cout << "Unable to open file";
		exit(1); // terminate with error
   }
    
    while (inFile >> ipAd) {
		//cout << ipAd << endl;
		string newAd = IPify(ipAd);
		outFile << newAd << endl;
		for(int i = 0; i < 5; i ++){
			string newAdRandom = randomize(newAd);
			outFile << newAdRandom << endl;
		}
    }
    
	inFile.close();
	outFile.close();


}
