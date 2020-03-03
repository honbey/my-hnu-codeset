/******************************************************************************************
 * Project Name: simpleRSA
 * 
 * Honbey, honbey@honbey.com
 * Created On 2019-10-13
 * Copyright (c) 2019. All rights reserved.
 * 
 * Date: 2019-10-23
 * Version: 0.3.0
 * 
 * Change Log: Comprehensive test and normalization
 * 
 * Description: For the 4th cryptography experiment.
 * Stability impact on source files: 
 *   the file will have a blank line added.
 *  
 * History: 
 * <author> <time> <version > <desc>
 * Honbey 2019-10-13 0.1.0 Create project and normalize code
 * Honbey 2019-10-15 0.2.0 Debug and optimization and Print information when generate key
 ******************************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <ctime>

using namespace std;

#include "simple_rsa.h"

int main(int argc, char const *argv[]) {
	srand((int)time(NULL)); // using current time stamp as seed of random number generator.
	simpleRSA test; // default setting
	test.changeKey(17, 189, 3); // reset RSA

	if (argc != 2) {
		cout << "usage: simpleRSA <File>" << endl;
		return -1;
	}

	ifstream inFile(argv[1]); // open the FILE
	string plainText = "", temp = "";
	while (getline(inFile, temp)) plainText += temp + '\n'; // copy text to plainText and add '\n' to the end of each row
	inFile.close(); // close FILE

	// some information about Encrypt/Decrypt process
	string cipherText = test.encryption(plainText);
	cout << " Encrypted Text: " << endl << cipherText.substr(0, 10) << endl;
	string deCipherText = test.decryption(cipherText);
	cout << " Decrypted Text: " << endl << deCipherText.substr(0, 10) << endl;

	ofstream outFile("EncryptedFile.txt"); // open or create a txt FILE 
	outFile << cipherText; // output the encrypted text to FILE
	outFile.close(); // close FILE

	outFile.open("DecryptedFile.txt", ios::out); // open or create a txt FILE 
	outFile << deCipherText; // output the decrypted text to FILE
	outFile.close(); // close FILE

	return 0;
}
