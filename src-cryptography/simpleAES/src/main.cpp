/******************************************************************************************
 * Project Name: simpleAES
 * 
 * Honbey, honbey@honbey.com
 * Created On 2019-11-11
 * Copyright (c) 2019. All rights reserved.
 * 
 * Date: 2020-03-29
 * Version: 0.5.0
 * 
 * Change Log: added some functions
 * 
 * Description: For the 5th cryptography experiment.
 * Stability impact on source files: 
 *   the file will have a blank line added.
 *  
 * History: 
 * <author> <time> <version > <desc>
 * Honbey 2019-11-11 0.1.0 Create project and normalize code
 * Honbey 2019-11-13 0.2.0 Debug and optimization
 * Honbey 2019-11-15 0.3.0 Print information when encrypting/decrypting
 * Honbey 2019-12-13 0.4.0 Overload the operation in Galois Field
 ******************************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <ctime>
#include <cmath>
#include <iomanip>

using namespace std;

#include "simple_aes.h"

enum typeInfo {
	txt, png, jpg, jpeg
};

int main(int argc, char const *argv[]) {
	if (argc != 3) {
		cout << "error: Wrong number of parameters! \n Usage: simpleAES <FILE> <[0, 3]>" << endl;
		return	-1;
	} else if (atoi(argv[2]) < 0 || 3 < atoi(argv[2])) {
		cout << "error: Wrong value of parameters! \n Usage: simpleAES <FILE> <[0, 3]>" << endl;
		return	-2;
	}
	simpleAES def;
	ifstream inFile(argv[1]); // open the FILE
	string plainStr = "", temp = "";
	while (getline(inFile, temp)) plainStr += temp + '\n'; // copy text to plainStr and add '\n' to the end of each row
	inFile.close(); // close FILE

	string typeStr = "";
	switch (atoi(argv[2])) {
		case txt: 
			typeStr = ".txt";
			break;
		case png:
			typeStr = ".png";
			break;
		case jpg:
			typeStr = ".jpg";
			break;
		default:
			typeStr = ".jpeg";
			break;
	}

	// some information about Encrypt/Decrypt process
	string cipherStr = def.encryption(plainStr);
	cout << " Encrypting: " << endl << cipherStr.substr(0, 10) << endl;

	ofstream outFile("EncryptedFile"); // open or create a txt FILE 
	outFile << cipherStr; // output the encrypted text to FILE
	outFile.close(); // close FILE
	cout << "Encryption completed! The output file is \"EncryptedFile\"." << endl;

	temp = "";
	inFile.open("EncryptedFile", ios::out); // open the FILE
	cout << "Open \"EncryptedFile\" and start decryption.";
	cipherStr = "";
	while (getline(inFile, temp)) cipherStr += temp + '\n'; // copy text to plainStr and add '\n' to the end of each row
	string decipherStr = def.decryption(cipherStr);
	cout << " Decrypting: " << endl << decipherStr.substr(0, 10) << endl;

	outFile.open("DecryptedFile" + typeStr , ios::out); // open or create a txt FILE 
	outFile << decipherStr; // output the decrypted text to FILE
	outFile.close(); // close FILE
	cout << "Decryption completed! The output file is \"DecryptedFile" + typeStr + "\"." << endl;
    
	return 0;
}